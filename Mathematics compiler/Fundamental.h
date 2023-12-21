#pragma once

#include "Numbers.h"

// класс, от которого наследуются все объекты в математике
class Fundamental_set
{
public:
	Fundamental_set() {}

	virtual bool is_equal(const Fundamental_set* other) const
	{
		return is_subset(other) && other->is_subset(this);
	}

	virtual bool is_subset(const Fundamental_set* other) const { return true; }
	virtual bool contains(const Fundamental_set* other) const { return false; }

	Integer_number get_cardinality() const
	{
		return m_cardinality;
	}

	virtual bool is_undefined() const
	{
		return m_undefined;
	}

protected:
	Integer_number m_cardinality; // мощность множества, отрицательные значения - более чем конечная мощность
	std::vector<const Fundamental_set *> m_elements;
	bool m_undefined = false;
};

template <typename T>
concept is_set = std::is_convertible<T, Fundamental_set>::value;


class Finite_set : public Fundamental_set
{
public:
	Finite_set() { }

	Finite_set(const std::vector<const Fundamental_set *>& elements)
	{
		for (const auto& e : elements)
		{
			if (!contains(e))
			{
				m_elements.push_back(e);
				m_cardinality++;
			}
		}
	}

	template<typename... types>
	Finite_set(const Fundamental_set* first, types... elements) : Finite_set(elements...)
	{
		if (!contains(first))
		{
			m_elements.push_back(first);
			m_cardinality++;
		}
	}

	Finite_set(const Fundamental_set* first)
	{
		m_elements.push_back(first);
		m_cardinality = 1;
	}

	virtual bool contains(const Fundamental_set* other) const override
	{
		for (const auto e : m_elements)
		{
			if (e->is_equal(other))
				return true;
		}

		return false;
	}

	const Finite_set& merge(const Finite_set* other)
	{
		for (const auto e : other->get_elements())
		{
			if (!contains(e))
			{
				m_elements.push_back(e);
				++m_cardinality;
			}
		}

		return *this;
	}

	void add(const Fundamental_set* other)
	{
		if (!contains(other))
		{
			m_elements.push_back(other);
			m_cardinality++;
		}
	}

	const Finite_set& intersect(const Finite_set* other)
	{
		std::vector<const Fundamental_set*> new_elements;

		for (const auto e : m_elements)
		{
			if (other->contains(e))
				new_elements.push_back(e);
		}

		m_elements = new_elements;
		m_cardinality = m_elements.size();

		return *this;
	}

	bool is_subset(const Fundamental_set* other) const override
	{
		for (const auto& e : m_elements)
			if (!other->contains(e))
				return false;

		return true;
	}

	const std::vector<const Fundamental_set *>& get_elements() const
	{
		return m_elements;
	}
};

class Undefined_set : public Finite_set
{
public:
	Undefined_set() { m_undefined = true; }
};

const Undefined_set undefined_set;
const Undefined_set* undefined_ptr = &undefined_set;

bool operator==(const Fundamental_set& lhs, const Fundamental_set& rhs)
{
	return lhs.is_equal(&rhs);
}

Finite_set wrap(const Fundamental_set* input)
{
	Finite_set output;
	output.add(input);

	return output;
}

Finite_set unite(const Finite_set* lhs, const Finite_set* rhs)
{
	Finite_set result = *lhs;
	result.merge(rhs);

	return result;
}

Finite_set intersect(const Finite_set* lhs, const Finite_set* rhs)
{
	Finite_set result = *lhs;
	result.intersect(rhs);

	return result;
}

class Nothing : public Finite_set
{
public:
	Nothing() {}
};

class Infinite_set : public Fundamental_set
{
	bool is_subset(const Fundamental_set* other) const override
	{
		if (other->get_cardinality() >= 0)
			return false;

		return false;
	}
};