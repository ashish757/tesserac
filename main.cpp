#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <fstream>
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
    explicit Lexer(std::string source) : sourceCode(std::move(source)), currentIndex(0) {}

    std::vector<Token> tokenize() {
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
                    numberLiteral += sourceCode[currentIndex];
                    currentIndex++;
                }
                generatedTokens.push_back({TokenType::integer, numberLiteral});
                continue;
            }

            std::cerr << "Syntax Error: Unexpected character " << currentChar << "\n";
            exit(1);
        }

        generatedTokens.push_back({TokenType::endOfFile, ""});
        return generatedTokens;
    }
};

class AstNode {
public:
    virtual ~AstNode() = default;
};

class IntegerLiteralNode : public AstNode {
public:
    std::string value;
    explicit IntegerLiteralNode(std::string val) : value(std::move(val)) {}
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t currentIndex;

    Token consumeToken() {
        return tokens[currentIndex++];
    }

public:
    explicit Parser(std::vector<Token> inputTokens) : tokens(std::move(inputTokens)), currentIndex(0) {}

    std::unique_ptr<AstNode> parseProgram() {
        Token currentToken = consumeToken();

        if (currentToken.type == TokenType::integer) {
            Token eofToken = consumeToken();
            if (eofToken.type != TokenType::endOfFile) {
                std::cerr << "Syntax Error: Expected end of file\n";
                exit(1);
            }
            return std::make_unique<IntegerLiteralNode>(currentToken.value);
        }

        std::cerr << "Syntax Error: Expected an integer\n";
        exit(1);
    }
};

class CodeGenerator {
public:
    std::string generateAssembly(const AstNode* rootNode) {
        std::string assemblyCode = ".global _main\n.align 2\n_main:\n";

        if (const auto* integerNode = dynamic_cast<const IntegerLiteralNode*>(rootNode)) {
            assemblyCode += "    mov w0, #" + integerNode->value + "\n";
            assemblyCode += "    ret\n";
            return assemblyCode;
        }

        std::cerr << "Compilation Error: Invalid AST Node\n";
        exit(1);
    }
};

int main() {
    std::string codeToCompile = "    42   ";

    Lexer myLexer(codeToCompile);
    std::vector<Token> lexedTokens = myLexer.tokenize();

    Parser myParser(lexedTokens);
    std::unique_ptr<AstNode> abstractSyntaxTree = myParser.parseProgram();

    CodeGenerator myGenerator;
    std::string generatedCode = myGenerator.generateAssembly(abstractSyntaxTree.get());

    std::ofstream outputFile("output.s");
    if (outputFile.is_open()) {
        outputFile << generatedCode;
        outputFile.close();
        std::cout << "Successfully compiled to output.s\n";
    } else {
        std::cerr << "Failed to open output.s for writing\n";
        return 1;
    }

    return 0;
}