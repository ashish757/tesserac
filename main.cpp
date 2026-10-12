#include <iostream>
#include <string>
#include <vector>
#include <cctype>

enum class TokenType {
    integer,
    endOfFile
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
private:
    std::string sourceCode;
    size_t currentIndex;

public:
    Lexer(std::string sourceCode) : sourceCode(std::move(sourceCode)), currentIndex(0) {}

   std::vector<Token> tokenize(){
        std::vector<Token> generatedTokens;

        while (currentIndex < sourceCode.length()) {
            char currentChar = sourceCode[currentIndex];

            if (std::isspace(currentChar)) {
                 currentIndex++;
                continue;
            }

            if (std::isdigit(currentChar)) {
                std::string numberLiteral = "";
                while (currentIndex < sourceCode.length() && std::isdigit(sourceCode[currentIndex])) {
                    numberLiteral +=  sourceCode[currentIndex];
                    currentIndex++;
                }
                generatedTokens.push_back(Token(TokenType::integer, numberLiteral));
                continue;
            }

            std::cerr << "Unexpected character: "<<currentChar<<std::endl;
            exit(1);
        }

        generatedTokens.push_back(Token(TokenType::endOfFile, ""));
        return generatedTokens;
   }
};


int main () {
    std::string  codeToCompile = "       435   ";
    Lexer myLexer(codeToCompile);
    std::vector<Token> lexedTokens = myLexer.tokenize();
    for (const auto& currentToken : lexedTokens) {
        std::cout<<"type: "<<static_cast<int>(currentToken.type)<<" | Value: "<<currentToken.value<<std::endl;
    }
    return 0;
}