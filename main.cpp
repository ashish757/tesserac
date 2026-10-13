#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <fstream>
#include <cctype>
#include <cstdlib>

enum class TokenType {
    integer,
    plus,
    minus,
    endOfFile
};

struct Token {
    TokenType type;
    std::string value;
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

class BinaryExpressionNode : public AstNode {
public:
    std::string operatorSymbol;
    std::unique_ptr<AstNode> leftNode;
    std::unique_ptr<AstNode> rightNode;

    BinaryExpressionNode(std::string opSymbol, std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right)
        : operatorSymbol(std::move(opSymbol)), leftNode(std::move(left)), rightNode(std::move(right)) {}
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

            std::cerr << "Syntax Error: Unexpected character " << currentChar << "\n";
            exit(1);
        }

        generatedTokens.push_back({TokenType::endOfFile, ""});
        return generatedTokens;
    }
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t currentIndex;

    Token consumeToken() {
        return tokens[currentIndex++];
    }

    Token peekToken() const {
        if (currentIndex < tokens.size()) {
            return tokens[currentIndex];
        }
        return {TokenType::endOfFile, ""};
    }

    std::unique_ptr<AstNode> parsePrimary() {
        Token currentToken = consumeToken();
        if (currentToken.type == TokenType::integer) {
            return std::make_unique<IntegerLiteralNode>(currentToken.value);
        }
        std::cerr << "Syntax Error: Expected an integer\n";
        exit(1);
    }

public:
    explicit Parser(std::vector<Token> inputTokens) : tokens(std::move(inputTokens)), currentIndex(0) {}

    std::unique_ptr<AstNode> parseExpression() {
        std::unique_ptr<AstNode> leftNode = parsePrimary();

        while (peekToken().type == TokenType::plus || peekToken().type == TokenType::minus) {
            Token operatorToken = consumeToken();
            std::unique_ptr<AstNode> rightNode = parsePrimary();

            leftNode = std::make_unique<BinaryExpressionNode>(
                operatorToken.value,
                std::move(leftNode),
                std::move(rightNode)
            );
        }

        return leftNode;
    }

    std::unique_ptr<AstNode> parseProgram() {
        std::unique_ptr<AstNode> abstractSyntaxTree = parseExpression();

        if (peekToken().type != TokenType::endOfFile) {
            std::cerr << "Syntax Error: Expected end of file\n";
            exit(1);
        }

        return abstractSyntaxTree;
    }
};

class CodeGenerator {
private:
    std::string assemblyCode;

    void generateNode(const AstNode* node) {
        if (const auto* integerNode = dynamic_cast<const IntegerLiteralNode*>(node)) {
            assemblyCode += "    mov w0, #" + integerNode->value + "\n";
        } else if (const auto* binaryNode = dynamic_cast<const BinaryExpressionNode*>(node)) {
            generateNode(binaryNode->leftNode.get());

            assemblyCode += "    str w0, [sp, #-16]!\n";

            generateNode(binaryNode->rightNode.get());

            assemblyCode += "    mov w1, w0\n";
            assemblyCode += "    ldr w0, [sp], #16\n";

            if (binaryNode->operatorSymbol == "+") {
                assemblyCode += "    add w0, w0, w1\n";
            } else if (binaryNode->operatorSymbol == "-") {
                assemblyCode += "    sub w0, w0, w1\n";
            }
        } else {
            std::cerr << "Compilation Error: Unknown AST Node\n";
            exit(1);
        }
    }

public:
    std::string generateAssembly(const AstNode* rootNode) {
        assemblyCode = ".global _main\n.align 2\n_main:\n";
        generateNode(rootNode);
        assemblyCode += "    ret\n";
        return assemblyCode;
    }
};

int main() {
    std::string codeToCompile = "5 + 10 - 3";

    Lexer myLexer(codeToCompile);
    std::vector<Token> lexedTokens = myLexer.tokenize();

    Parser myParser(lexedTokens);
    std::unique_ptr<AstNode> abstractSyntaxTree = myParser.parseProgram();

    CodeGenerator myGenerator;
    std::string generatedCode = myGenerator.generateAssembly(abstractSyntaxTree.get());

    std::ofstream outputFile("../output.s");
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



