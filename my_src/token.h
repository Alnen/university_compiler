#pragma once
#include <string>

namespace Grammar
{

class Token
{
public:
	std::string value;

public:
	Token() : value(""){};
	Token(std::string v) : value(v){};

	bool operator ==(const Token& other) const
	{
		return (value.compare(other.value) == 0);
	}

	bool operator <(const Token& other) const
	{
		return (value.compare(other.value) < 0);
	}

	bool isEpsilon()
	{
		return (value.compare("epsilon") == 0);
	}

	bool isFinalStateSymbol()
	{
		return (value.compare("$") == 0);
	}
};

} // namespace Grammar