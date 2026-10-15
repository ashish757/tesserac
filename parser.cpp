#include "parser.h"
#include <iostream>
#include <cstdlib>

Parser::Parser(std::vector<Token> inputTokens) : tokens(std::move(inputTokens)), currentIndex(0) {}

Token Parser::consumeToken() {
    return tokens[currentIndex++];
}

Token Parser::peekToken() const {
    if (currentIndex < tokens.size()) {
        return tokens[currentIndex];
    }
    return {TokenType::endOfFile, ""};
}

std::unique_ptr<AstNode> Parser::parsePrimary() {
    Token currentToken = consumeToken();

    if (currentToken.type == TokenType::integer) {
        return std::make_unique<IntegerLiteralNode>(currentToken.value);
    }

    if (currentToken.type == TokenType::identifier) {
        return std::make_unique<IdentifierNode>(currentToken.value);
    }

    std::cerr << "Syntax Error: Expected an integer or variable\n";
    exit(1);
}

std::unique_ptr<AstNode> Parser::parseExpression() {
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

std::unique_ptr<AstNode> Parser::parseVarStatement() {
    consumeToken();

    Token identifierToken = consumeToken();
    if (identifierToken.type != TokenType::identifier) {
        std::cerr << "Syntax Error: Expected identifier\n";
        exit(1);
    }

    Token equalsToken = consumeToken();
    if (equalsToken.type != TokenType::equals) {
        std::cerr << "Syntax Error: Expected '='\n";
        exit(1);
    }

    std::unique_ptr<AstNode> expressionNode = parseExpression();

    Token semicolonToken = consumeToken();
    if (semicolonToken.type != TokenType::semicolon) {
        std::cerr << "Syntax Error: Expected ';'\n";
        exit(1);
    }

    return std::make_unique<VarStatementNode>(identifierToken.value, std::move(expressionNode));
}

std::unique_ptr<AstNode> Parser::parseStatement() {
    if (peekToken().type == TokenType::varKeyword) {
        return parseVarStatement();
    }

    if (peekToken().type == TokenType::printKeyword) {
        return parsePrintStatement();
    }

    std::unique_ptr<AstNode> expressionNode = parseExpression();

    Token semicolonToken = consumeToken();
    if (semicolonToken.type != TokenType::semicolon) {
        std::cerr << "Syntax Error: Expected ';'\n";
        exit(1);
    }

    return expressionNode;
}

std::unique_ptr<AstNode> Parser::parseProgram() {
    auto programNode = std::make_unique<ProgramNode>();

    while (peekToken().type != TokenType::endOfFile) {
        programNode->statements.push_back(parseStatement());
    }
    return programNode;
}

std::unique_ptr<AstNode> Parser::parsePrintStatement() {
    consumeToken();
    std::unique_ptr<AstNode> expressionNode = parseExpression();
    Token semicolonToken = consumeToken();
    if (semicolonToken.type != TokenType::semicolon) {
        std::cerr<<"Syntax Error: Expected ';'\n";
        exit(1);
    }
    return std::make_unique<PrintStatementNode>(std::move(expressionNode));
}