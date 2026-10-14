#pragma once

#include "token.h"
#include <string>
#include <vector>

class Lexer {
private:
    std::string sourceCode;
    size_t currentIndex;

public:
    explicit Lexer(std::string source);
    std::vector<Token> tokenize();
};