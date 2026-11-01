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

        if (currentChar == '"') {
            std::string stringLiteral = "";
            currentIndex++;

            while (currentIndex < sourceCode.length() && sourceCode[currentIndex] != '"') {
                stringLiteral += sourceCode[currentIndex];
                currentIndex++;
            }

            if (currentIndex < sourceCode.length() && sourceCode[currentIndex] == '"') {
                currentIndex++;
            } else {
                std::cerr<<"Syntax Error: looks like quote is missing in string initialization";
                exit(1);
            }

            generatedTokens.push_back({TokenType::stringLiteral, stringLiteral});
            continue;
        }

        if (std::isalpha(currentChar)) {
            std::string alphaString = "";

            while (currentIndex < sourceCode.length() && std::isalnum(sourceCode[currentIndex])) {
                alphaString += sourceCode[currentIndex];
                currentIndex++;
            }


            if (alphaString == "print") {
                generatedTokens.push_back({TokenType::printKeyword, alphaString});
            } else if (alphaString == "if") {
                generatedTokens.push_back({TokenType::ifKeyword, alphaString});
            } else if (alphaString == "else") {
                generatedTokens.push_back({TokenType::elseKeyword, alphaString});
            } else if (alphaString == "while") {
                generatedTokens.push_back({TokenType::whileKeyword, alphaString});
            } else if (alphaString == "fn") {
                generatedTokens.push_back({TokenType::fnKeyword, alphaString});
            } else if (alphaString == "return") {
                generatedTokens.push_back({TokenType::returnKeyword, alphaString});
            } else if (alphaString == "break") {
                generatedTokens.push_back({TokenType::breakKeyword, alphaString});
            } else if (alphaString == "continue") {
                generatedTokens.push_back({TokenType::continueKeyword, alphaString});
            } else if (alphaString == "null") {
                generatedTokens.push_back({TokenType::nullKeyword, alphaString});
            } else if (alphaString == "int") {
                generatedTokens.push_back({TokenType::intKeyword, alphaString});
            } else if (alphaString == "float") {
                generatedTokens.push_back({TokenType::floatKeyword, alphaString});
            } else if (alphaString == "string") {
                generatedTokens.push_back({TokenType::stringKeyword, alphaString});
            } else {
                generatedTokens.push_back({TokenType::identifier, alphaString});
            }
            continue;
        }


        if (std::isdigit(currentChar)) {
            std::string numberLiteral = "";
            bool hasDecimal = false;

            while (currentIndex < sourceCode.length() && (std::isdigit(sourceCode[currentIndex]) || sourceCode[currentIndex] == '.')) {
                if (sourceCode[currentIndex] == '.') {
                    if (hasDecimal) {
                        break;
                    }
                    hasDecimal = true;
                }
                numberLiteral += sourceCode[currentIndex];
                currentIndex++;
            }
            if (hasDecimal) {
                generatedTokens.push_back({TokenType::floatLiteral, numberLiteral});
            } else {
                generatedTokens.push_back({TokenType::integer, numberLiteral});
            }
            continue;
        }

        if (currentChar == '+') {
            if (currentIndex + 1 < sourceCode.length() && sourceCode[currentIndex + 1] == '+') {
                generatedTokens.push_back({TokenType::increment, "++"});
                currentIndex += 2;
                continue;
            }
            generatedTokens.push_back({TokenType::plus, "+"});
            currentIndex++;
            continue;
        }

        if (currentChar == '-') {
            if (currentIndex + 1 < sourceCode.length() && sourceCode[currentIndex + 1] == '-') {
                generatedTokens.push_back({TokenType::decrement, "--"});
                currentIndex += 2;
                continue;
            }
            generatedTokens.push_back({TokenType::minus, "-"});
            currentIndex++;
            continue;
        }
        if (currentChar == '*') {
            generatedTokens.push_back({TokenType::star, "*"});\
            currentIndex++;\
            continue;


        }
        if (currentChar == '/') {
            generatedTokens.push_back({TokenType::slash, "/"});

            currentIndex++;
            continue;
        }
        if (currentChar == '%') {
            generatedTokens.push_back({TokenType::modulo, "%"});
            currentChar++;
            continue;
        }
        if (currentChar == '<') {
            generatedTokens.push_back({TokenType::lessThan, "<"});
            currentIndex++;
            continue;
        }
        if (currentChar == '>') {
            generatedTokens.push_back({TokenType::greaterThan, ">"});
            currentIndex++;
            continue;
        }

        if (currentChar == '&') {
            if (currentIndex + 1 < sourceCode.length() && sourceCode[currentIndex + 1] == '&') {
                generatedTokens.push_back({TokenType::logicalAnd, "&"});
                currentIndex += 2;
                continue;
            }
        }

        if (currentChar == '|') {
            if (currentIndex + 1 < sourceCode.length() && sourceCode[currentIndex + 1] == '|') {
                generatedTokens.push_back({TokenType::logicalOr, "|"});
                currentIndex += 2;
                continue;
            }
        }

        if (currentChar == '=') {
            if (currentIndex + 1 < sourceCode.length() && sourceCode[currentIndex + 1] == '=') {
                generatedTokens.push_back({TokenType::doubleEquals, "=="});
                currentIndex += 2;
                continue;
            }
            generatedTokens.push_back({TokenType::equals, "="});
            currentIndex++;
            continue;
        }
        if (currentChar == ';') {
            generatedTokens.push_back({TokenType::semicolon, ";"});
            currentIndex++;
            continue;
        }

        if (currentChar == '{') {
            generatedTokens.push_back({TokenType::leftBrace, "{"});
            currentIndex++;
            continue;
        }

        if (currentChar == '}') {
            generatedTokens.push_back({TokenType::rightBrace, "}"});
            currentIndex++;
            continue;
        }
        if (currentChar  == '(') {
            generatedTokens.push_back({TokenType::leftParen, "("});
            currentIndex++;
            continue;
        }
        if (currentChar  == ')') {
            generatedTokens.push_back({TokenType::rightParen, "_"});
            currentIndex++;
            continue;
        }
        if (currentChar == ',') {
            generatedTokens.push_back({TokenType::comma, ","});
            currentIndex++;
            continue;
        }

        if (currentChar == '[') {
            generatedTokens.push_back({TokenType::leftBracket, "["});
            currentIndex++;
            continue;
        }

        if (currentChar == ']') {
            generatedTokens.push_back({TokenType::rightBracket, "]"});
            currentIndex++;
            continue;
        }

        std::cerr<<"Synatax Error: Unexpected Character "<<currentChar<<"\n";
        exit(1);
    }
    generatedTokens.push_back({TokenType::endOfFile, ""});
    return generatedTokens;
}