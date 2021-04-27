#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class bad_format_string : public std::exception {
private:
    std::string error_;
public:
    bad_format_string(size_t pos) {
        error_ = "Incorrect format: pos = " + std::to_string(pos);
    }

    bad_format_string() {
        error_ = "Incorrect format";
    }

    const char* what() const noexcept { return error_.c_str(); }
};

class too_few_args : public std::exception {
private:
    std::string error_;
public:
    too_few_args(size_t num) {
        error_ = "Fewer arguments than specified in the formatted string: number = " + std::to_string(num);
    }

    too_few_args() {
        error_ = "Fewer arguments than specified in the formatted string";
    }

    const char* what() const noexcept { return error_.c_str(); }
};

template<class T>
std::string to_str(const T& val) {
	std::stringstream stream;
	stream << val;
	std::string str_out;
	stream >> str_out;

	return str_out;
}

inline bool is_number(const std::string& token, size_t& out_digit) {
    try {
        uint64_t digit = std::stoull(token);
        if (digit <= std::numeric_limits<size_t>::max()) {
            out_digit = (size_t)digit;
            return true;
        }
        else {
            return false;
        }
    }
    catch (...) {
        return false;
    }
}

template<class T>
void process_args(std::vector<std::string>& v_args, const T& val) {
	v_args.push_back(to_str(val));
}

template<class T, class... ArgsT>
void process_args(std::vector<std::string>& v_args, const T& val, const ArgsT&... args) {
	v_args.push_back(to_str(val));

	process_args(v_args, args...);
}

template<class... ArgsT>
std::string format(const char* text, const ArgsT&...args) {
	std::vector<std::string> v_args;
	process_args(v_args, args...);

	std::string result = "";

    int i = 0;
    while(text[i] != '\0') {
        if (text[i] == '}') {
            throw bad_format_string(i);
        }
        if (text[i] == '{') {
            std::string str_number = "";
            int j = i + 1;
            while (text[j] != '}' && text[j] != '\0')
            {
                if (text[j] < '0'  || text[j] > '9') {
                    throw bad_format_string(j);
                }
                str_number += text[j];
                j++;
            }
            if (text[j] != '}') {
                throw bad_format_string(j);
            }
            if (str_number == "") {
                throw bad_format_string(j);
            }

            size_t number;
            if (is_number(str_number, number)) {
                if (number >= v_args.size()) {
                    throw too_few_args(number);
                }
                else {
                    result += v_args[number];
                }
            }
            else {
                throw bad_format_string(j);
            }

            i = j;
        }
        else {
            result += text[i];
        };

        i++;
    }

	return result;
}
