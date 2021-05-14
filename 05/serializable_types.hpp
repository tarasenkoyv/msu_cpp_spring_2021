#pragma once

#include <iostream>

#include "error.hpp"

struct DataBoolean
{
	bool value_;

	template<class TSerializer>
	Error Serialize(TSerializer& serializer)
	{
		return serializer(value_);
	}

	template<class TDeserializer>
	Error Load(TDeserializer& deserializer)
	{
		return deserializer(value_);
	}
};

struct Data
{
	uint64_t a;
	bool b;
	uint64_t c;

	template<class TSerializer>
	Error Serialize(TSerializer& serializer)
	{
		return serializer(a, b, c);
	}

	template<class TDeserializer>
	Error Load(TDeserializer& deserializer)
	{
		return deserializer(a, b, c);
	}
};

struct DataUINT64
{
	uint64_t value_;

	template<class TSerializer>
	Error Serialize(TSerializer& serializer)
	{
		return serializer(value_);
	}

	template<class TDeserializer>
	Error Load(TDeserializer& deserializer)
	{
		return deserializer(value_);
	}
};