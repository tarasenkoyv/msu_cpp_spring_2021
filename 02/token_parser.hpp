#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <vector>

using DigitTokenCallback = std::function<void(unsigned int d, std::string& out_str)>;
using StrTokenCallback = std::function<void(const std::string& str, std::string& out_str)>;
using StartCallback = std::function<void(const std::string& text, std::string& out_text)>;
using EndCallback = std::function<void(const std::string& text, std::string& out_text)>;

class TokenParser
{
public:
    TokenParser();

    void Parse(const std::string& text, const std::string& delimiters = " ");

    // Setting a callback function before starting parsing.
    void SetStartCallback(StartCallback callback);

    // Setting a callback function after the end of parsing.
    void SetEndCallback(EndCallback callback);

    // Setting a callback function to handle numbers.
    void SetDigitTokenCallback(DigitTokenCallback callback);

    // Setting a callback function to handle strings.
    void SetStrTokenCallback(StrTokenCallback callback);

    std::string get_text_after_start_callback() const;
    std::string get_text_after_end_callback() const;
    std::string get_text_after_parse() const;

    size_t get_cnt_tokens() const;
private:
    StartCallback start_callback_ = nullptr;
    EndCallback end_callback_ = nullptr;
    StrTokenCallback str_token_callback_ = nullptr;
    DigitTokenCallback digit_token_callback_ = nullptr;
    std::vector<std::string> tokens_;
    std::vector<std::string> out_tokens_;
    std::string text_after_start_callback_;
    std::string text_after_parse_;
    std::string text_after_end_callback_;

    void Tokenize(const std::string& str, const std::string& delimiters = " ");

    void DigitTokenHandler(unsigned int digit, std::string& out_str);

    void StrTokenHandler(const std::string& str, std::string& out_str);

    void StartHandler(const std::string& text, std::string& out_text);

    void EndHandler(const std::string& text, std::string& out_text);

    bool IsDigit(const std::string& token);
};
