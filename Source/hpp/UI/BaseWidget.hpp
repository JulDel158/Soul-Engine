#ifndef BASE_WIDGET_H
#define BASE_WIDGET_H

#include "EngineDataStructures.hpp"

#include <vector>

class BaseWidget
{
protected:
	BaseWidget* parent_;
	std::vector<BaseWidget*> children_;
	BaseWidget* neighbors_[EWidgetNeighbor::Count];
	
public:
	BaseWidget();
	virtual ~BaseWidget();
	
	void AddParent(BaseWidget& parent);
	void AddChild(BaseWidget& child);
	void AddNeighbor(BaseWidget& neighbor, EWidgetNeighbor direction);
};

#endif
