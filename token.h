#pragma once
#include <string>

enum class TokenType {
    integer,
    floatLiteral,
    stringLiteral,
    nullKeyword,

    identifier,

    varKeyword,
    printKeyword,
    ifKeyword,
    elseKeyword,
    whileKeyword,
    fnKeyword,
    returnKeyword,

    leftBrace,
    rightBrace,
    leftParen,
    rightParen,

    plus,
    minus,
    equals,
    doubleEquals,
    lessThan,
    greaterThan,

    comma,
    semicolon,
    endOfFile
};

struct Token {
    TokenType type;
    std::string value;
};