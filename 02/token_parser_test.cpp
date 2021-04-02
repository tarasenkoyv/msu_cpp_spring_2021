#include <cassert>
#include <functional>
#include <iostream>

#include "token_parser.hpp"

void CountTokensTest();
void TextAfterStartCallbackTest();
void TextAfterEndCallbackTest();
void DigitTokenCallbackTest();
void StrTokenCallbackTest();
void AllCallbackTest();

int main()
{
    CountTokensTest();
    TextAfterStartCallbackTest();
    TextAfterEndCallbackTest();
    DigitTokenCallbackTest();
    StrTokenCallbackTest();
    AllCallbackTest();

    std::cout << "Success!" << std::endl;

    return 0;
}

void CountTokensTest() {
    TokenParser parser;
    std::string text("1\n2\t\t3\na;bc 12ab\n");
    parser.Parse(text, " \t\n");
    assert(parser.get_cnt_tokens() == 5);

    text = "1\n2\t\t3\na;bc 12ab\n";
    parser.Parse(text, "\t\n");
    assert(parser.get_cnt_tokens() == 4);
}

void TextAfterStartCallbackTest() {
    TokenParser parser;
    
    auto start_callback = [](const std::string& text, std::string& out_text) -> void {
        out_text = "{ ";
        out_text.append(text);
        out_text.append(" }");
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
        out_text = "{ ";
        out_text.append(text);
        out_text.append(" }");
    };
    parser.SetEndCallback(end_callback);

    std::string text("1\n2\t\t3\n jjl 0 jl 12ab\n");
    parser.Parse(text, " \t\n");

    assert(parser.get_cnt_tokens() == 7);
    assert(parser.get_text_after_end_callback() == "{ 1 2 3 jjl 0 jl 12ab }");
}

void DigitTokenCallbackTest() {
    TokenParser parser;

    auto digit_token_callback = [](unsigned int d, std::string& out_str) -> void {
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
        out_str = "{ ";
        out_str.append(str);
        out_str.append(" }");
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
        out_str = "<";
        out_str.append(str);
        out_str.append(">");
    };
    auto digit_token_callback = [](unsigned int d, std::string& out_str) -> void {
        if (d % 5 == 0) {
            out_str = std::to_string(d / 5);
        }
    };
    auto end_callback = [](const std::string& text, std::string& out_text) -> void {
        out_text = "[ ";
        out_text.append(text);
        out_text.append(" ]");
    };
    auto start_callback = [](const std::string& text, std::string& out_text) -> void {
        out_text = "{ ";
        out_text.append(text);
        out_text.append(" }");
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