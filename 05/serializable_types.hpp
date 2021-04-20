#pragma once

#include <iostream>

#include "error.hpp"

struct Data
{
	uint64_t a;
	bool b;
	uint64_t c;

	template<class Serializer>
	Error Serialize(Serializer& serializer)
	{
		return serializer(a, b, c);
	}

	template<class Deserializer>
	Error Load(Deserializer& deserializer)
	{
		return deserializer(a, b, c);
	}
};