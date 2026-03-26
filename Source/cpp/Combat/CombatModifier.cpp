#include "Combat/CombatModifier.hpp"

CombatModifier::CombatModifier()
{
}

CombatModifier::CombatModifier(ECombatModifierType type, ECombatModifierExecutionTime executionTime, int count) :
	type_(type),
	execution_time_(executionTime),
	stacks_(count),
	data_int_(0),
	data_uint_(0),
	data_float_(0.0f),
	data_bool_(false)
{
}
