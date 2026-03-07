#include "UI/Widget.hpp"

#include "Utils/Logger.hpp"

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
	can_update_(true),
	can_refresh_(true)
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
}

void Widget::Start()
{
}

void Widget::Update(const float deltaTime)
{
	if (can_refresh_)
	{
		render_list_.clear();
	}
}

void Widget::End()
{
}

void Widget::OnMouseEnter()
{
	is_highlighted_ = true;
}

void Widget::OnMouseLeave()
{
	is_highlighted_ = false;
}

void Widget::OnFocused()
{
	is_highlighted_ = true;
}

void Widget::OnUnfocused()
{
	is_highlighted_ = false;
}

void Widget::OnClick(const bool leftClick)
{
}

void Widget::OnSelected()
{
}

void Widget::SetParent(Widget& parent)
{
	parent_ = &parent;
}

void Widget::AddChild(Widget& child)
{
	int a = 1;
	int* b = &a;
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
