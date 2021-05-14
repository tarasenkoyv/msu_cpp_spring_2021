#pragma once

#include <iostream>

#include "error.hpp"
#include "serializable_types.hpp"

class Serializer {
    static const char kSeparator = ' ';
public:
    explicit Serializer(std::ostream& out_);

    template <class T>
    Error Save(T& object);

    template <class... ArgsT>
    Error operator()(ArgsT... args);
private:
    std::ostream& out_;

    Error process(bool arg);
    Error process(uint64_t arg);

    template <class T, class... ArgsT>
    Error process(T&& val, ArgsT&&... args);
};

#include "serializer.tpp"