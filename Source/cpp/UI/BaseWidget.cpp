#include "UI/BaseWidget.hpp"

#include "Utils/Logger.hpp"

BaseWidget::BaseWidget() :
parent_(nullptr),
neighbors_{nullptr}
{
}

BaseWidget::~BaseWidget()
{
	parent_ = nullptr;
	children_.clear();
	for (BaseWidget*& child : neighbors_)
	{
		child = nullptr;
	}
}

void BaseWidget::AddParent(BaseWidget& parent)
{
	parent_ = &parent;
}

void BaseWidget::AddChild(BaseWidget& child)
{
	int a = 1;
	int* b = &a;
	children_.push_back(&child);
	child.AddParent(*this);
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
