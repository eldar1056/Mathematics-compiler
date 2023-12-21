// означает соответствие между множеством m_domain и множеством m_codomain

#include "Fundamental.h"

class Abstract_function
{
public:
	Abstract_function(const Finite_set* domain, const Finite_set* codomain) : m_domain(domain), m_codomain(codomain) {}

	const Finite_set* get_domain() const
	{
		return m_domain;
	}

	const Finite_set* get_codomain() const
	{
		return m_codomain;
	}

private:
	const Finite_set* m_domain;
	const Finite_set* m_codomain;
};

class Function_values
{
public:
	Function_values() {}

	template<typename... types>
	Function_values(types... abstract_functions)
	{
		m_values = { abstract_functions... };
	}

	long long find(const Finite_set* input) const
	{
		for (size_t i = 0; i < m_values.size(); i++)
			if (m_values[i].get_domain()->is_equal(input))
				return i;

		return -1;
	}

	void add_value(const Finite_set* domain, const Finite_set* codomain)
	{
		if (domain->is_undefined() || codomain->is_undefined())
			return;

		auto location = find(domain);
		if (location > 0)
			m_values[location] = Abstract_function(domain, codomain);
		else
			m_values.push_back(Abstract_function(domain, codomain));
	}

	void add_value(std::string domain, std::string codomain)
	{
		auto Memory = Memory_registry::get_register();
		add_value(Memory->get_set(domain), Memory->get_set(codomain));
	}

	const Finite_set* get(const Finite_set* input)
	{
		auto loc = find(input);

		if (loc < 0)
			return undefined_ptr;

		return m_values[find(input)].get_codomain();
	}

	const Finite_set* get(std::string input)
	{
		return get(Memory_registry::get_register()->get_set(input));
	}

private:
	std::vector<Abstract_function> m_values;
};

class Function : public Abstract_function
{
public:
	Function(const Finite_set* domain, const Finite_set* codomain, Function_values values) : Abstract_function(domain, codomain), m_values(values) {}

	template<typename T>
	const Finite_set* apply(T input)
	{
		return m_values.get(input);
	}

private:
	Function_values m_values;
};


#pragma once
