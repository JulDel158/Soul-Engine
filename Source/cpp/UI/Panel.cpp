#include "UI/Panel.hpp"

Panel::Panel()
{
}

Panel::~Panel()
{
	widgets_.clear();
}

void Panel::AddWidget(const unsigned int layer, Widget& widget)
{
	widgets_[layer].push_back(&widget);
}

void Panel::Start() const
{
	for (const auto& layer : widgets_) //NOLINT
	{
		for (const auto& widget : layer.second)
		{
			widget->Start();
		}
	}
}

void Panel::Update(const float deltaTime) const
{
	for (const auto& layer : widgets_) //NOLINT
	{
		for (const auto& widget : layer.second)
		{
			if (widget->IsActive() && widget->CanUpdate())
			{
				widget->Update(deltaTime);
			}
		}
	}
}

void Panel::End() const
{
	for (const auto& layer : widgets_) // NOLINT
	{
		for (const auto& widget : layer.second)
		{
			widget->End();
		}
	}
}
