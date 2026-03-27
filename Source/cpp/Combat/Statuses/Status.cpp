#include "Combat/Statuses/Status.hpp"

Status::Status() :
stack_(0),
cost_per_cycle_(1),
max_(999),
name_("Status"),
description_("Default description...")
{
}

void Status::OnTurnCycleEnd()
{
	stack_ -= cost_per_cycle_;
}
