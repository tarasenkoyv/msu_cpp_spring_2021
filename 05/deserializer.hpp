#pragma once

#include <iostream>
#include <string>

#include "error.hpp"
#include "serializable_types.hpp"

class Deserializer {
    static const char kSeparator = ' ';
public:
    explicit Deserializer(std::istream& in_);

    template <class T>
    Error Load(T& object);

    template <class... ArgsT>
    Error operator()(ArgsT&... args);
private:
    std::istream& in_;

    bool IsEmptyStream();
    Error process(bool& arg);
    Error process(uint64_t& arg);

    template <class T, class... ArgsT>
    Error process(T& val, ArgsT&... args);
};

#include "deserializer.tpp"