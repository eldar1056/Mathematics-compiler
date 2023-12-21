#pragma once

#include "Fundamental.h"
#include "memory_management.h"

Finite_set* Natural(const Integer_number& num)
{
	auto Memory = Memory_registry::get_register();

	Finite_set* result = Memory->create_finite_set("Natural " + num.get_value());

	for (Integer_number i = 0; i < num; i++)
	{
		Finite_set* current = Memory->create_finite_set("Natural " + i.get_value(), result->get_elements());

		result->add(current);
	}

	return result;
}
