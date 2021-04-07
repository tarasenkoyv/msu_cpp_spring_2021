#include <cassert>
#include <functional>
#include <iostream>

#include "token_parser.hpp"

void CountValueTypeTokensTest();
void TextAfterStartCallbackTest();
void TextAfterEndCallbackTest();
void DigitTokenCallbackTest();
void StrTokenCallbackTest();
void AllCallbackTest();
void ParseDigit_UINT64_MAX_Test();
void ParseEmpyStringTest();
void ParseStringOfSpacesTest();
void ParseSingleCharStringTest();
void ParseSingleDigitStringTest();

int main()
{
    CountValueTypeTokensTest();
    TextAfterStartCallbackTest();
    TextAfterEndCallbackTest();
    DigitTokenCallbackTest();
    StrTokenCallbackTest();
    AllCallbackTest();
    ParseDigit_UINT64_MAX_Test();
    ParseEmpyStringTest();
    ParseStringOfSpacesTest();
    ParseSingleCharStringTest();
    ParseSingleDigitStringTest();

    std::cout << "Success!" << std::endl;

    return 0;
}

void CountValueTypeTokensTest() {
    TokenParser parser;
    std::string text("1\n2\t\t3\na;bc 12ab\n");
    parser.Parse(text, " \t\n");
    assert(parser.get_cnt_tokens() == 5);
    assert(parser.get_token_type(0) == typeid(uint64_t).name());
    assert(parser.get_token(0) == "1");
    assert(parser.get_token_type(1) == typeid(uint64_t).name());
    assert(parser.get_token(1) == "2");
    assert(parser.get_token_type(2) == typeid(uint64_t).name());
    assert(parser.get_token(2) == "3");
    assert(parser.get_token_type(3) == typeid(std::string).name());
    assert(parser.get_token(3) == "a;bc");
    assert(parser.get_token_type(4) == typeid(std::string).name());
    assert(parser.get_token(4) == "12ab");

    text = "1\n2\t\t3\na;bc 12ab\n";
    parser.Parse(text, "\t\n");
    assert(parser.get_cnt_tokens() == 4);
    assert(parser.get_token_type(0) == typeid(uint64_t).name());
    assert(parser.get_token(0) == "1");
    assert(parser.get_token_type(1) == typeid(uint64_t).name());
    assert(parser.get_token(1) == "2");
    assert(parser.get_token_type(2) == typeid(uint64_t).name());
    assert(parser.get_token(2) == "3");
    assert(parser.get_token_type(3) == typeid(std::string).name());
    assert(parser.get_token(3) == "a;bc 12ab");

}

void TextAfterStartCallbackTest() {
    TokenParser parser;
    
    auto start_callback = [](const std::string& text, std::string& out_text) -> void {
        out_text = "{ " + text + " }";
    };
    parser.SetStartCallback(start_callback);

    std::string text("1\n2\t\t3\n jjl 0 jl 12ab\n");
    parser.Parse(text, " \t\n");

    assert(parser.get_cnt_tokens() == 9);
    assert(parser.get_text_after_start_callback() == "{ 1\n2\t\t3\n jjl 0 jl 12ab\n }");
}

void TextAfterEndCallbackTest() {
    TokenParser parser;

    auto end_callback = [](const std::string& text, std::string& out_text) -> void {
        out_text = "{ " + text + " }";
    };
    parser.SetEndCallback(end_callback);

    std::string text("1\n2\t\t3\n jjl 0 jl 12ab\n");
    parser.Parse(text, " \t\n");

    assert(parser.get_cnt_tokens() == 7);
    assert(parser.get_text_after_end_callback() == "{ 1 2 3 jjl 0 jl 12ab }");
}

void DigitTokenCallbackTest() {
    TokenParser parser;

    auto digit_token_callback = [](uint64_t d, std::string& out_str) -> void {
        if (d % 5 == 0) {
            out_str = std::to_string(d / 5);
        }
    };
    parser.SetDigitTokenCallback(digit_token_callback);

    std::string text("10\n2\t\t3\n jjl 0 jl 25ab\n");
    parser.Parse(text, " \t\n");

    assert(parser.get_cnt_tokens() == 7);
    assert(parser.get_text_after_parse() == "2 2 3 jjl 0 jl 25ab");
}

void StrTokenCallbackTest() {
    TokenParser parser;

    auto str_token_callback = [](const std::string& str, std::string& out_str) -> void {
        out_str = "{ " + str + " }";
    };
    parser.SetStrTokenCallback(str_token_callback);

    std::string text("10\n2\t\t3\n jjl 0 jl 25ab\n");
    parser.Parse(text, " \t\n");

    assert(parser.get_cnt_tokens() == 7);
    assert(parser.get_text_after_parse() == "10 2 3 { jjl } 0 { jl } { 25ab }");
}

void AllCallbackTest() {
    TokenParser parser;

    auto str_token_callback = [](const std::string& str, std::string& out_str) -> void {
        out_str = "<" + str + ">";
    };
    auto digit_token_callback = [](uint64_t d, std::string& out_str) -> void {
        if (d % 5 == 0) {
            out_str = std::to_string(d / 5);
        }
    };
    auto end_callback = [](const std::string& text, std::string& out_text) -> void {
        out_text = "[ " + text + " ]";
    };
    auto start_callback = [](const std::string& text, std::string& out_text) -> void {
        out_text = "{ " + text + " }";
    };

    parser.SetStartCallback(start_callback);
    parser.SetEndCallback(end_callback);
    parser.SetStrTokenCallback(str_token_callback);
    parser.SetDigitTokenCallback(digit_token_callback);

    std::string text("10\n2\t\t3\n jjl 0 jl 25ab\n");
    parser.Parse(text, " \t\n");

    assert(parser.get_cnt_tokens() == 9);
    assert(parser.get_text_after_start_callback() == "{ 10\n2\t\t3\n jjl 0 jl 25ab\n }");
    assert(parser.get_text_after_parse() == "<{> 2 2 3 <jjl> 0 <jl> <25ab> <}>");
    assert(parser.get_text_after_end_callback() == "[ <{> 2 2 3 <jjl> 0 <jl> <25ab> <}> ]");
}

void ParseDigit_UINT64_MAX_Test() {
    TokenParser parser;

    auto digit_token_callback = [](uint64_t d, std::string& out_str) -> void {
        out_str = "{" + std::to_string(d) + "}";
    };
    parser.SetDigitTokenCallback(digit_token_callback);

    // Checking UINT64_MAX
    std::string text = "000" + std::to_string(UINT64_MAX) + " t 0042295";
    parser.Parse(text, " \t\n");

    assert(parser.get_cnt_tokens() == 3);
    assert(parser.get_token_type(0) == typeid(std::uint64_t).name());
    assert(parser.get_token(0) == "000" + std::to_string(UINT64_MAX));
    assert(parser.get_token_type(1) == typeid(std::string).name());
    assert(parser.get_token(1) == "t");
    assert(parser.get_token_type(2) == typeid(std::uint64_t).name());
    assert(parser.get_token(2) == "0042295");

    assert(parser.get_text_after_parse() == "{" + std::to_string(UINT64_MAX) + "} t {42295}");

    // Checking the number following UINT64_MAX
    text = "18446744073709551616 t 42295";
    
    parser.Parse(text, " \t\n");

    assert(parser.get_cnt_tokens() == 3);
    assert(parser.get_token_type(0) == typeid(std::string).name());
    assert(parser.get_token(0) == "18446744073709551616");
    assert(parser.get_token_type(1) == typeid(std::string).name());
    assert(parser.get_token(1) == "t");
    assert(parser.get_token_type(2) == typeid(std::uint64_t).name());
    assert(parser.get_token(2) == "42295");
    assert(parser.get_text_after_parse() == "18446744073709551616 t {42295}");
}

void ParseEmpyStringTest() {
    TokenParser parser;

    auto digit_token_callback = [](uint64_t d, std::string& out_str) -> void {
        out_str = "{" + std::to_string(d) + "}";
    };
    parser.SetDigitTokenCallback(digit_token_callback);

    std::string text = "";

    parser.Parse(text, " \t\n");
    assert(parser.get_cnt_tokens() == 0);
    assert(parser.get_text_after_parse() == "");
}

void ParseStringOfSpacesTest() {
    TokenParser parser;

    auto digit_token_callback = [](uint64_t d, std::string& out_str) -> void {
        out_str = "{" + std::to_string(d) + "}";
    };
    parser.SetDigitTokenCallback(digit_token_callback);

    std::string text = "     ";

    parser.Parse(text, " \t\n");
    assert(parser.get_cnt_tokens() == 0);
    assert(parser.get_text_after_parse() == "");
}

void ParseSingleCharStringTest() {
    TokenParser parser;

    auto str_token_callback = [](const std::string& str, std::string& out_str) -> void {
        out_str = "{" + str + "}";
    };
    parser.SetStrTokenCallback(str_token_callback);

    std::string text = "x";

    parser.Parse(text, " \t\n");
    assert(parser.get_cnt_tokens() == 1);
    assert(parser.get_token(0) == "x");
    assert(parser.get_token_type(0) == typeid(std::string).name());
    assert(parser.get_text_after_parse() == "{x}");
}

void ParseSingleDigitStringTest() {
    TokenParser parser;

    auto digit_token_callback = [](uint64_t d, std::string& out_str) -> void {
        out_str = "{" + std::to_string(d) + "}";
    };
    parser.SetDigitTokenCallback(digit_token_callback);

    std::string text = "1";

    parser.Parse(text, " \t\n");
    assert(parser.get_cnt_tokens() == 1);
    assert(parser.get_token(0) == "1");
    assert(parser.get_token_type(0) == typeid(std::uint64_t).name());
    assert(parser.get_text_after_parse() == "{1}");
}
