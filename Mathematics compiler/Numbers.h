// определяет обычные числа для использования в компиляторе
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <set>

template <typename T>
concept is_int = std::is_integral<T>::value;

class Integer_number
{
public:
	Integer_number(std::string s = "")
	{
		if (s == "")
			return;

		if (s[0] == '-')
		{
			m_positive = false;
			s.erase(0, 1);
		}

		for (const auto& ch : s)
		{
			if (!std::isdigit(ch))
			{
				m_value = "";
				throw std::invalid_argument("Integer constructor received non-digit string");
			}

			m_value = ch + m_value;
		}

		remove_leading_zero();
	}
	
	void remove_leading_zero()
	{
		while (m_value.length() > 0 && m_value[m_value.length() - 1] == '0')
			m_value.erase(m_value.length() - 1, 1);
	}

	void reverse()
	{
		if (m_value != "")
			m_positive = !m_positive;
	}

	static Integer_number abs(const Integer_number& input)
	{
		Integer_number output(input);
		output.m_positive = true;

		return output;
	}

	static Integer_number reverse_number(const Integer_number& input)
	{
		Integer_number reversed(input);
		reversed.reverse();

		return reversed;
	}

	template <is_int T>
	Integer_number(T a) : Integer_number(std::to_string(a)) {}

	Integer_number& operator+= (const Integer_number& other)
	{
		size_t original_length = m_value.length();
		size_t other_length = other.m_value.length();

		bool carry_over = false;
		if (m_positive == other.m_positive)
		{
			for (size_t i = 0; i < std::min(other_length, original_length); i++)
			{
				int sum = m_value[i] + other.m_value[i] + carry_over - 96;

				if (sum >= 10)
				{
					sum -= 10;
					carry_over = true;
				}
				else
					carry_over = false;

				m_value[i] = sum + 48;
			}

			if (other_length > original_length)
				m_value += other.m_value.substr(m_value.length());

			size_t index = original_length;
			while (carry_over)
			{
				if (index >= other_length)
				{
					m_value += '1';
					carry_over = false;
				}
				else
				{
					char sum = 1 + m_value[index];

					if (sum >= 58)
					{
						sum -= 10;
						carry_over = true;
					}
					else
						carry_over = false;

					m_value[index++] = sum;
				}
			}
		}
		else
		{
			m_positive = (abs(*this) > abs(other));

			auto smaller = m_positive ? other.m_value : m_value;
			
			if (!m_positive)
				m_value = other.m_value;

			for (size_t i = 0; i < smaller.length(); i++)
			{
				int value = m_value[i] - smaller[i] - carry_over;

				if (value < 0)
				{
					value += 10;
					carry_over = true;
				}
				else
					carry_over = false;
				
				m_value[i] = value + 48;
			}

			if (carry_over)
			{
				m_value[smaller.length()] = char(m_value[smaller.length()] - 1);
			}

		}

		remove_leading_zero();

		return *this;
	}

	Integer_number& operator-= (const Integer_number& other)
	{
		return *this += reverse_number(other);;
	}

	Integer_number& operator++()
	{
		return *this += 1;
	}

	Integer_number operator++(int)
	{
		Integer_number temp = *this;
		++*this;

		return temp;
	}

	Integer_number& operator--()
	{
		return *this -= 1;
	}

	Integer_number operator--(int)
	{
		Integer_number temp = *this;
		--*this;

		return temp;
	}


	bool get_positive() const
	{
		return m_positive;
	}

	std::string get_value() const
	{
		return m_value;
	}

	friend bool operator<(const Integer_number& lhs, const Integer_number& rhs)
	{
		if (!lhs.m_positive && rhs.m_positive)
			return true;
		if (lhs.m_positive && !rhs.m_positive)
			return false;
		if (lhs == rhs)
			return false;

		if (lhs.m_value.length() != rhs.m_value.length())
			return lhs.m_positive == lhs.m_value.length() < rhs.m_value.length();

		for (long long i = lhs.m_value.length() - 1; i >= 0; i--)
		{
			if (lhs.m_positive == lhs.m_value[i] < rhs.m_value[i])
				return true;
		}

		return false;
	}

	friend bool operator==(const Integer_number& lhs, const Integer_number& rhs)
	{
		return lhs.get_positive() == rhs.get_positive() && lhs.get_value() == rhs.get_value();
	}

	friend bool operator!=(const Integer_number& lhs, const Integer_number& rhs)
	{
		return !(lhs == rhs);
	}

	friend bool operator>(const Integer_number& lhs, const Integer_number& rhs)
	{
		return !(lhs < rhs) && lhs != rhs;
	}

	friend bool operator>=(const Integer_number& lhs, const Integer_number& rhs)
	{
		return !(lhs < rhs);
	}

	friend bool operator<=(const Integer_number& lhs, const Integer_number& rhs)
	{
		return !(lhs > rhs);
	}


private:
	bool m_positive = true;
	std::string m_value;
};

Integer_number operator+(const Integer_number& lhs, const Integer_number& rhs)
{
	Integer_number result = { lhs };

	return result += rhs;
}

Integer_number operator-(const Integer_number& lhs, const Integer_number& rhs)
{
	Integer_number result = { lhs };

	return result -= rhs;
}

std::ostream& operator<<(std::ostream& stream, const Integer_number& number)
{
	if (!number.get_positive())
		stream << '-';

	std::string value = number.get_value();
	reverse(value.begin(), value.end());
	stream << value;

	if (number.get_value() == "")
		stream << '0';

	return stream;
}


class Rational_number
{

};