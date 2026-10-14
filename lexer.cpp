#include "lexer.h"
#include <iostream>
#include <cctype>
#include <cstdlib>


Lexer::Lexer(std::string source) : sourceCode(std::move(source)), currentIndex(0) {}


std::vector<Token> Lexer::tokenize() {
    std::vector<Token> generatedTokens;

    while (currentIndex < sourceCode.length()) {
        char currentChar = sourceCode[currentIndex];

        if (std::isspace(currentChar)) {
            currentIndex++;
            continue;
        }

        if (std::isalpha(currentChar)) {
            std::string alphaString = "";

            while (currentIndex < sourceCode.length() && std::isalnum(sourceCode[currentIndex])) {
                alphaString += sourceCode[currentIndex];
                currentIndex++;
            }

            if (alphaString == "var") {
                generatedTokens.push_back({TokenType::varKeyword, alphaString});
            } else {
                generatedTokens.push_back({TokenType::identifier, alphaString});
            }
            continue;
        }

        if (std::isdigit(currentChar)) {
            std::string numberLiteral = "";
            while (currentIndex < sourceCode.length() && std::isdigit(sourceCode[currentIndex])) {
                numberLiteral += sourceCode[currentIndex];
                currentIndex++;
            }
            generatedTokens.push_back({TokenType::integer, numberLiteral});
            continue;
        }

        if (currentChar == '+') {
            generatedTokens.push_back({TokenType::plus, "+"});
            currentIndex++;
            continue;
        }

        if (currentChar == '-') {
            generatedTokens.push_back({TokenType::minus, "-"});
            currentIndex++;

            continue;
        }

        if (currentChar == '=') {
            generatedTokens.push_back({TokenType::equals, "="});
            currentIndex++;
            continue;
        }
        if (currentChar == ';') {
            generatedTokens.push_back({TokenType::semicolon, ";"});
            currentIndex++;
            continue;
        }

        std::cerr<<"Synatax Error: Unexpected Caharacter "<<currentChar<<"\n";
        exit(1);
    }
    generatedTokens.push_back({TokenType::endOfFile, ""});
    return generatedTokens;
}