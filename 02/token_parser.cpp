#include "token_parser.hpp"

#include <iostream>
#include <regex>

TokenParser::TokenParser() {
    digit_token_callback_ = nullptr;
    end_callback_ = nullptr;
    start_callback_ = nullptr;
    str_token_callback_ = nullptr;
}

void TokenParser::Parse(const std::string &text, const std::string &delimiters) {
    StartHandler(text, text_after_start_callback_);

    Tokenize(text_after_start_callback_, delimiters);

    ProcessTokens();

    ReCreateTextAfterParsing();

    EndHandler(text_after_parse_, text_after_end_callback_);
}

void TokenParser::ProcessTokens() {
    out_tokens_.clear();
    for (size_t i = 0; i < tokens_.size(); i++) {
        std::string token = tokens_[i];

        unsigned int out_digit;
        if (IsUnsignedInt(token, out_digit)) {
            std::string out_str = token;
            DigitTokenHandler(out_digit, out_str);
            out_tokens_.push_back(out_str);
        }
        else {
            std::string out_str = token;
            StrTokenHandler(token, out_str);
            out_tokens_.push_back(out_str);
        }
    }
}

/*
* When re-creating, all separators are replaced by spaces.
*/
void TokenParser::ReCreateTextAfterParsing() {
    text_after_parse_ = "";
    for (size_t i = 0; i < out_tokens_.size(); i++) {
        text_after_parse_ += out_tokens_[i];
        if (i != out_tokens_.size() - 1) {
            text_after_parse_ += " ";
        }
    }
}

inline bool TokenParser::IsUnsignedInt(const std::string &token, unsigned int &out_digit) {
    std::regex isDigit{ R"(\d+)" };
    if (std::regex_match(token, isDigit)) {
        try {
            out_digit = std::stoul(token);
            return true;
        }
        catch (const std::exception &e) {
            return false;
        }
    }
    return false;
}

void TokenParser::Tokenize(const std::string &str, const std::string &delimiters)
{
    tokens_.clear();
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens_.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void TokenParser::SetStartCallback(StartCallback callback) {
    start_callback_ = callback;
}

void TokenParser::SetEndCallback(EndCallback callback) {
    end_callback_ = callback;
}

void TokenParser::SetDigitTokenCallback(DigitTokenCallback callback) {
    digit_token_callback_ = callback;
}

void TokenParser::SetStrTokenCallback(StrTokenCallback callback) {
    str_token_callback_ = callback;
}

void TokenParser::DigitTokenHandler(unsigned int digit, std::string &out_str) {
    if (digit_token_callback_ == nullptr) return;

    digit_token_callback_(digit, out_str);
}

void TokenParser::StrTokenHandler(const std::string &str, std::string &out_str) {
    if (str_token_callback_ == nullptr) return;
    
    str_token_callback_(str, out_str);
}

void TokenParser::StartHandler(const std::string &text, std::string &out_text) {
    out_text = std::string(text);

    if (start_callback_ == nullptr) return;
     
    start_callback_(text, out_text);
}

void TokenParser::EndHandler(const std::string &text, std::string &out_text) {
    out_text = std::string(text);

    if (end_callback_ == nullptr) return;

    end_callback_(text, out_text);
}

std::string TokenParser::get_text_after_start_callback() const {
    return std::string(text_after_start_callback_);
}

std::string TokenParser::get_text_after_end_callback() const {
    return std::string(text_after_end_callback_);
}

size_t TokenParser::get_cnt_tokens() const {
    return tokens_.size();
}

std::string TokenParser::get_text_after_parse() const {
    return std::string(text_after_parse_);
}