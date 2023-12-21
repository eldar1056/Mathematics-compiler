#pragma once

#include "Fundamental.h"
#include <map>

class Memory_registry final
{


public:
	Memory_registry(const Memory_registry&) = delete;
	Memory_registry(Memory_registry&&) = delete;

	static Memory_registry* get_register()
	{
		if (!The_register)
			The_register = new Memory_registry();

		return The_register;
	}

	template <typename... T>
	Finite_set* create_finite_set(std::string name, T... params)
	{
		Finite_set* fs = new Finite_set(params...);

		if (finite_sets.find(fs) != finite_sets.end())
		{
			for (const auto& str : finite_sets[fs])
				if (str == name)
				{
					return fs;
				}
		}

		finite_sets[fs].push_back(name);

		return fs;
	}

	const Finite_set* get_set(std::string name)
	{
		for (const auto& [fin_set, vec] : finite_sets)
		{
			for (const auto& str : vec)
				if (str == name)
					return fin_set;
		}

		return undefined_ptr;
	}

	const std::vector<std::string> find_name(const Finite_set* target)
	{
		if (target->is_undefined())
			return { "undefined" };

		if (finite_sets.find(target) == finite_sets.end())
			return {};
		else
			return finite_sets[target];
	}

	~Memory_registry()
	{
		for (auto [link, vec] : finite_sets)
			delete link;
	}

private:
	Memory_registry() {}

	static Memory_registry* The_register;

	std::map<const Finite_set* , std::vector<std::string>> finite_sets;
};

Memory_registry* Memory_registry::The_register = NULL;

std::ostream& operator<<(std::ostream& stream, const Finite_set* value)
{
	for (const auto& name : Memory_registry::get_register()->find_name(value))
	{
		stream << name << " | ";
	}

	return stream;
}