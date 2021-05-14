#include <cassert>
#include <iostream>
#include <sstream>

#include "serializable_types.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"

void SaveLoadTest();
void CorruptedArchive_UINT64_MAX_Test();
void CorruptedArchiveBoolTest();
void CorruptedArchiveUINT64Test();
void SaveLoad_StructWithSingleFieldBoolTest();
void CorruptedArchive_SingleFieldBoolTest();
void CorruptedArchive_SingleFieldUINT64Test();
void SaveLoad_StructWithSingleFieldUINT64Test();
void CorruptedArchiveLessTest();
void CorruptedArchiveGreaterTest();

int main() {
	SaveLoadTest();
	CorruptedArchive_UINT64_MAX_Test();
	CorruptedArchiveBoolTest();
	CorruptedArchiveUINT64Test();

	CorruptedArchive_SingleFieldUINT64Test();
	SaveLoad_StructWithSingleFieldUINT64Test();
	SaveLoad_StructWithSingleFieldBoolTest();
	CorruptedArchive_SingleFieldBoolTest();

	CorruptedArchiveLessTest();
	CorruptedArchiveGreaterTest();

	std::cout << "Success!" << std::endl;

	return 0;
}

void SaveLoadTest() {
	{
		Data data1{ 1, true, 2 };
		std::stringstream stream;
		Serializer serializer(stream);
		assert(serializer.Save(data1) == Error::NoError);
		assert((stream.str()) == "1 true 2");
		Data data2{ 0, false, 0 };
		Deserializer deserializer(stream);
		assert(stream.str() == "1 true 2");
		assert(deserializer.Load(data2) == Error::NoError);
		assert(data1.a == data2.a);
		assert(data1.b == data2.b);
		assert(data1.c == data2.c);
	}

	{
		Data data1{ 1, false, UINT64_MAX };
		std::string str_test = "1 false " + std::to_string(UINT64_MAX);
		std::stringstream stream;
		Serializer serializer(stream);
		assert(serializer.Save(data1) == Error::NoError);
		assert(stream.str() == str_test);
		Data data2{ 0, false, 0 };
		Deserializer deserializer(stream);
		assert(stream.str() == str_test);
		assert(deserializer.Load(data2) == Error::NoError);
		assert(data1.a == data2.a);
		assert(data1.b == data2.b);
		assert(data1.c == data2.c);
	}
}

void CorruptedArchive_UINT64_MAX_Test() {
	std::stringstream stream;
	std::string str_test = std::to_string(UINT64_MAX) + "134 true ghvk";
	stream << str_test;
	Data data_out{ 0, false, 0 };
	Deserializer deserializer(stream);
	assert(stream.str() == str_test);
	assert(deserializer.Load(data_out) == Error::CorruptedArchive);
}

void CorruptedArchiveBoolTest() {
	{
		std::stringstream stream;
		std::string str_test = "134 strue 1";
		stream << str_test;
		Data data_out{ 0, false, 0 };
		assert(stream.str() == str_test);
		Deserializer deserializer(stream);
		assert(deserializer.Load(data_out) == Error::CorruptedArchive);
	}
	{
		std::stringstream stream;
		std::string str_test = "134 0 1";
		stream << str_test;
		Data data_out{ 0, false, 0 };
		assert(stream.str() == str_test);
		Deserializer deserializer(stream);
		assert(deserializer.Load(data_out) == Error::CorruptedArchive);
	}
	{
		std::stringstream stream;
		std::string str_test = "134 1 1";
		stream << str_test;
		Data data_out{ 0, false, 0 };
		assert(stream.str() == str_test);
		Deserializer deserializer(stream);
		assert(deserializer.Load(data_out) == Error::CorruptedArchive);

		const Error err = deserializer.Load(data_out);
		assert(err == Error::CorruptedArchive);
	}
}

void CorruptedArchiveUINT64Test() {
	std::stringstream stream;
	std::string str_test = "sd134 1 1";
	stream << str_test;
	Data data_out{ 0, false, 0 };
	Deserializer deserializer(stream);
	assert(stream.str() == str_test);
	assert(deserializer.Load(data_out) == Error::CorruptedArchive);
}

void CorruptedArchiveGreaterTest() {
	{
		std::stringstream stream;
		std::string str_test = "134 false 1 2";
		stream << str_test;
		Data data{ 0, false, 0 };
		Deserializer deserializer(stream);
		assert(stream.str() == str_test);
		assert(deserializer.Load(data) == Error::CorruptedArchive);
	}
	{
		std::stringstream stream;
		std::string str_test = "true 1";
		stream << str_test;
		DataBoolean data{ false };
		Deserializer deserializer(stream);
		assert(stream.str() == str_test);
		assert(deserializer.Load(data) == Error::CorruptedArchive);
	}
	{
		std::stringstream stream;
		std::string str_test = "12 12";
		stream << str_test;
		DataUINT64 data{ false };
		Deserializer deserializer(stream);
		assert(stream.str() == str_test);
		assert(deserializer.Load(data) == Error::CorruptedArchive);
	}
}

void CorruptedArchiveLessTest() {
	{
		std::stringstream stream;
		std::string str_test = "134";
		stream << str_test;
		Data data{ 0, false, 0 };
		Deserializer deserializer(stream);
		assert(stream.str() == str_test);
		assert(deserializer.Load(data) == Error::CorruptedArchive);
	}
	{
		std::stringstream stream;
		std::string str_test = "";
		stream << str_test;
		DataBoolean data{ false };
		Deserializer deserializer(stream);
		assert(stream.str() == str_test);
		assert(deserializer.Load(data) == Error::CorruptedArchive);
	}
	{
		std::stringstream stream;
		std::string str_test = "";
		stream << str_test;
		DataUINT64 data{ false };
		Deserializer deserializer(stream);
		assert(stream.str() == str_test);
		assert(deserializer.Load(data) == Error::CorruptedArchive);
	}
}

void SaveLoad_StructWithSingleFieldBoolTest() {
	{
		std::stringstream stream;
		stream << "false";
		DataBoolean data{ false };
		Deserializer deserializer(stream);
		assert(stream.str() == "false");
		assert(deserializer.Load(data) == Error::NoError);
		assert(!data.value_);
	}

	{
		std::stringstream stream;
		stream << "true";
		DataBoolean data{ false };
		Deserializer deserializer(stream);
		assert(stream.str() == "true");
		assert(deserializer.Load(data) == Error::NoError);
		assert(data.value_);
	}


	{
		DataBoolean data_save{ false };
		DataBoolean data_load{ true };
		std::stringstream stream;
		Serializer serializer(stream);
		assert(serializer.Save(data_save) == Error::NoError);
		assert(stream.str() == "false");
		Deserializer deserializer(stream);
		assert(deserializer.Load(data_load) == Error::NoError);
		assert(data_save.value_ == data_load.value_);
	}
}

void CorruptedArchive_SingleFieldBoolTest() {
	{
		std::stringstream stream;
		stream << "1";
		DataBoolean data_load{ false };
		Deserializer deserializer(stream);
		assert(stream.str() == "1");
		assert(deserializer.Load(data_load) == Error::CorruptedArchive);
		assert(!data_load.value_);
	}
	{
		std::stringstream stream;
		stream << "0";
		DataBoolean data_load{ true };
		Deserializer deserializer(stream);
		assert(stream.str() == "0");
		assert(deserializer.Load(data_load) == Error::CorruptedArchive);
		assert(data_load.value_);
	}
}

void CorruptedArchive_SingleFieldUINT64Test() {
	{
		std::stringstream stream;
		const std::string str_test = std::to_string(UINT64_MAX) + "134";
		stream << str_test;
		
		DataUINT64 data{ 3 };
		Deserializer deserializer(stream);
		assert(stream.str() == str_test);
		assert(deserializer.Load(data) == Error::CorruptedArchive);
	}
}

void SaveLoad_StructWithSingleFieldUINT64Test() {
	{
		std::stringstream stream;
		const std::string str_test = std::to_string(UINT64_MAX);
		stream << str_test;
		DataUINT64 data{ 2 };
		Deserializer deserializer(stream);
		assert(stream.str() == str_test);
		assert(deserializer.Load(data) == Error::NoError);
		assert(data.value_ == UINT64_MAX);
	}

	{
		std::stringstream stream;
		const std::string str_test = "1273979";
		stream << str_test;
		DataUINT64 data{ 12 };
		Deserializer deserializer(stream);
		assert(stream.str() == str_test);
		assert(deserializer.Load(data) == Error::NoError);
		assert(data.value_ == std::stoul(str_test));
	}
}