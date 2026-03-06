#include "UI/BaseWidget.hpp"

#include "Utils/Logger.hpp"

namespace
{
	constexpr auto ZERO_VECTOR_2_F = glm::vec2(0.0f);
	constexpr auto HUNDRED_VECTOR_2_F = glm::vec2(100.0f);
}

BaseWidget::BaseWidget() :
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

BaseWidget::~BaseWidget()
{
	parent_ = nullptr;
	children_.clear();
	for (auto& child : neighbors_)
	{
		child = nullptr;
	}
}

void BaseWidget::Start()
{
}

void BaseWidget::Update(const float deltaTime)
{
	if (can_refresh_)
	{
		render_list_.clear();
	}
}

void BaseWidget::End()
{
}

void BaseWidget::OnMouseEnter()
{
	is_highlighted_ = true;
}

void BaseWidget::OnMouseLeave()
{
	is_highlighted_ = false;
}

void BaseWidget::OnFocused()
{
	is_highlighted_ = true;
}

void BaseWidget::OnUnfocused()
{
	is_highlighted_ = false;
}

void BaseWidget::OnClick()
{
}

void BaseWidget::OnSelected()
{
}

void BaseWidget::SetParent(BaseWidget& parent)
{
	parent_ = &parent;
}

void BaseWidget::AddChild(BaseWidget& child)
{
	int a = 1;
	int* b = &a;
	children_.push_back(&child);
	child.SetParent(*this);
}

void BaseWidget::AddNeighbor(BaseWidget& neighbor, EWidgetNeighbor direction)
{
	if (direction == EWidgetNeighbor::Count)
	{
		auto logger = Logger();
		logger.Log(ELogLevel::Warning, "BaseWidget::AddNeighbor: Passed EWidgetNeighbor::Count as direction!!");
		return;
	}
	
	neighbors_[static_cast<unsigned int>(direction)] = &neighbor;
}
