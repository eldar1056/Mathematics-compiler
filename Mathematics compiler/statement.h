#pragma once

#include "Fundamental.h"
#include "function.h"

enum Quantor
{
	forall = 0,
	exists = 1
};

class Quantor_sequence
{
public:
	Quantor_sequence(std::vector<std::pair<Quantor, Integer_number>> sequence) : m_sequence(sequence) {};
private:
	std::vector<std::pair<Quantor, Integer_number>> m_sequence;
};

class Statement
{
	Statement(Function function, Finite_set target_set, Quantor_sequence sequence) : m_function(function), m_set(target_set), m_sequence(sequence) {};

	template<typename... T>
	bool Solve(T... sets)
	{

	}

private:
	Function m_function;
	Finite_set m_set;
	Quantor_sequence m_sequence;
};

