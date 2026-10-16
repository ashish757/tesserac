#pragma once
#include "token.h"
#include "ast.h"
#include <vector>
#include <memory>

class Parser {
private:
    std::vector<Token> tokens;
    size_t currentIndex;

    Token consumeToken();
    Token peekToken() const;
    std::unique_ptr<AstNode> parsePrimary();
    std::unique_ptr<AstNode> parseExpression();
    std::unique_ptr<AstNode> parseVarStatement();
    std::unique_ptr<AstNode> parseStatement();
    std::unique_ptr<AstNode> parsePrintStatement();
    std::unique_ptr<AstNode> parseAssignmentStatement();
    std::unique_ptr<AstNode> parseIfStatement();
    std::unique_ptr<BlockNode> parseBlock();

public:
    explicit Parser(std::vector<Token> inputTokens);
    std::unique_ptr<AstNode> parseProgram();
};