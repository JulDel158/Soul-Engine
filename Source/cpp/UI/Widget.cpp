#include "UI/Widget.hpp"

#include "Utils/Logger.hpp"
#include "Components/BaseComponent.hpp"

namespace
{
	constexpr auto ZERO_VECTOR_2_F = glm::vec2(0.0f);
	constexpr auto HUNDRED_VECTOR_2_F = glm::vec2(100.0f);
}

Widget::Widget() :
	parent_(nullptr),
	neighbors_{nullptr},
	position_(ZERO_VECTOR_2_F),
	size_(HUNDRED_VECTOR_2_F),
	rotation_(0.0f),
	layer_(0),
	is_active_(true),
	is_visible_(true),
	is_highlighted_(false),
	is_overlapping_(false),
	can_update_(true),
	can_refresh_(true),
	can_listen_to_input_(false),
	can_be_focused_(false),
	fixed_render_list_(false),
	render_list_initialized_(false), 
	click_pressed_l_(false), 
	click_pressed_r_(false)
{
}

Widget::~Widget()
{
	parent_ = nullptr;
	children_.clear();
	for (auto& child : neighbors_)
	{
		child = nullptr;
	}
	
	render_list_.clear();
	text_list_.clear();
	components_.clear();
}

void Widget::Start()
{
	for (auto& component : components_)
	{
		component->Start();
	}
}

void Widget::Update(const float deltaTime)
{
	// we will flush the render list before updating components, that way components may repopulate it if necessary
	if (!fixed_render_list_)
	{
		render_list_.clear();
		render_list_.reserve(components_.size());
	}
	
	for (const auto& component : components_)
	{
		if (component->IsEnabled())
		{
			component->Update(deltaTime);
		}
		component->SetPosition(position_);
		component->SetSize(size_);
		component->SetRotation(rotation_);
		
		if (fixed_render_list_ && render_list_initialized_)
		{
			continue;
		}
		
		if (auto renderData = component->GetRenderData(); component->IsVisible() && renderData != std::nullopt)
		{
			render_list_.emplace_back(renderData.value());
		}
	}
	render_list_initialized_ = !render_list_.empty();
}

void Widget::End()
{
}

void Widget::OnMouseEnter()
{
	is_highlighted_ = true;
	is_overlapping_ = true;
}

void Widget::OnMouseLeave()
{
	is_highlighted_ = false;
	is_overlapping_ = false;
}

void Widget::OnFocused()
{
	is_highlighted_ = true;
}

void Widget::OnUnfocused()
{
	is_highlighted_ = false;
}

void Widget::OnClick(const glm::vec2 mousePosition, const bool leftClick)
{
	if (leftClick)
	{
		click_pressed_l_ = true;
	}
	else
	{
		click_pressed_r_ = true;
	}
}

void Widget::OnClickRelease(bool leftClick)
{
	if (leftClick)
	{
		click_pressed_l_ = false;
	}
	else
	{
		click_pressed_r_ = false;
	}
}

void Widget::OnSelected()
{
}

void Widget::OnReturnPressed()
{
}

void Widget::SetParent(Widget& parent)
{
	parent_ = &parent;
}

void Widget::AddChild(Widget& child)
{
	children_.push_back(&child);
	child.SetParent(*this);
}

void Widget::AddNeighbor(Widget& neighbor, EWidgetNeighbor direction)
{
	if (direction == EWidgetNeighbor::Count)
	{
		auto logger = Logger();
		logger.Log(ELogLevel::Warning, "BaseWidget::AddNeighbor: Passed EWidgetNeighbor::Count as direction!!");
		return;
	}
	
	neighbors_[static_cast<unsigned int>(direction)] = &neighbor;
}

void Widget::RegisterComponent(BaseComponent* component)
{
	components_.push_back(component);
	component->owner_ = nullptr;
}
