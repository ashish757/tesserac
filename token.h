#pragma once
#include <string>

enum class TokenType {
    integer,
    plus,
    minus,
    identifier,
    varKeyword,
    printKeyword,
    equals,
    semicolon,
    endOfFile,

    ifKeyword,
    elseKeyword,
    leftBrace,
    rightBrace,
    doubleEquals,
};

struct Token {
    TokenType type;
    std::string value;
};