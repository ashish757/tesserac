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
    endOfFile,

    star,
    slash,
    modulo,
    logicalAnd,
    logicalOr,
    increment,
    decrement,
    
    breakKeyword,
    continueKeyword,
};

struct Token {
    TokenType type;
    std::string value;
};