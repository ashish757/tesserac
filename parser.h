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
    std::unique_ptr<AstNode> parseWhileStatement();
    std::unique_ptr<AstNode> parseFunctionCall(const std::string&  functionName);
    std::unique_ptr<AstNode> parseFunctionDeclaration();
    std::unique_ptr<AstNode> parseReturnStatement();

    std::unique_ptr<AstNode> parseLogicalOr();
    std::unique_ptr<AstNode> parseLogicalAnd();
    std::unique_ptr<AstNode> parseEquality();
    std::unique_ptr<AstNode> parseRelational();
    std::unique_ptr<AstNode> parseAdditive();
    std::unique_ptr<AstNode> parseMultiplicative();
    std::unique_ptr<AstNode> parsePostfix();

    std::unique_ptr<AstNode> parseArrayAssignmentStatement();


public:
    explicit Parser(std::vector<Token> inputTokens);
    std::unique_ptr<AstNode> parseProgram();
};