#pragma once

#include <iostream>
#include <string>

struct President
{
    std::string name;
    std::string country;
    int year;

    President() : name(""), country(""), year(0)
    {
        //std::cout << "I am being constructed.\n";
    }

    President(std::string p_name, std::string p_country, int p_year)
        : name(std::move(p_name)), country(std::move(p_country)), year(p_year)
    {
        //std::cout << "I am being constructed.\n";
    }
    
    President(const President& other) noexcept
        : name(other.name), country(other.country), year(other.year)
    {
        //std::cout << "I am being moved.\n";
    }

    President(President&& other) noexcept
        : name(std::move(other.name)), country(std::move(other.country)), year(other.year)
    {
        //std::cout << "I am being moved.\n";
    }

    President& operator=(const President& other) = default;
};