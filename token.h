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
    lessThan,
    greaterThan,
    semicolon,
    endOfFile,

    ifKeyword,
    elseKeyword,
    leftBrace,
    rightBrace,
    doubleEquals,

    whileKeyword,
};

struct Token {
    TokenType type;
    std::string value;
};