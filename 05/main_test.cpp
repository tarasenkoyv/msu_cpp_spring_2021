#include <cassert>
#include <iostream>
#include <sstream>

#include "error.hpp"
#include "deserializer.hpp"
#include "serializer.hpp"
#include "serializable_types.hpp"

void SaveLoadTest();
void CorruptedArchive_UINT64_MAX_Test();
void CorruptedArchiveBoolTest();
void CorruptedArchiveIntTest();

int main() {
	SaveLoadTest();
	CorruptedArchive_UINT64_MAX_Test();
	CorruptedArchiveBoolTest();
	CorruptedArchiveIntTest();

	std::cout << "Success!" << std::endl;

	return 0;
}

void SaveLoadTest() {
	Data x{ 1, true, 2 };
	std::stringstream stream;

	Serializer serializer(stream);

	serializer.Save(x);

	Data y{ 0, false, 0 };

	Deserializer deserializer(stream);
	const Error err = deserializer.Load(y);

	assert(err == Error::NoError);
	assert(x.a == y.a);
	assert(x.b == y.b);
	assert(x.c == y.c);

	Data x1{ 1, false, UINT64_MAX };
	std::stringstream stream1;

	Serializer serializer1(stream1);

	serializer1.Save(x1);

	Data y1{ 0, false, 0 };

	Deserializer deserializer1(stream1);
	const Error err1 = deserializer1.Load(y1);

	assert(err1 == Error::NoError);
	assert(x1.a == y1.a);
	assert(x1.b == y1.b);
	assert(x1.c == y1.c);
}

void CorruptedArchive_UINT64_MAX_Test() {
	std::stringstream stream;
	stream << std::to_string(UINT64_MAX) + "134" << " ";
	stream << "true" << " ";
	stream << "1" << " ";

	Data data_out{ 0, false, 0 };

	Deserializer deserializer(stream);

	const Error err = deserializer.Load(data_out);
	assert(err == Error::CorruptedArchive);
}

void CorruptedArchiveBoolTest() {
	std::stringstream stream;
	stream << "134" << " ";
	stream << "strue" << " ";
	stream << "1" << " ";

	Data data_out{ 0, false, 0 };

	Deserializer deserializer(stream);

	const Error err = deserializer.Load(data_out);
	assert(err == Error::CorruptedArchive);
}

void CorruptedArchiveIntTest() {
	std::stringstream stream;
	stream << "sd134" << " ";
	stream << "true" << " ";
	stream << "1" << " ";

	Data data_out{ 0, false, 0 };

	Deserializer deserializer(stream);

	const Error err = deserializer.Load(data_out);
	assert(err == Error::CorruptedArchive);
}