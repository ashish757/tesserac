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

    while (peekToken().type == TokenType::plus || peekToken().type == TokenType::minus || peekToken().type == TokenType::doubleEquals) {
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

    if (peekToken().type == TokenType::identifier) {
        return  parseAssignmentStatement();
    }

    if (peekToken().type == TokenType::ifKeyword) {
        return parseIfStatement();
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


std::unique_ptr<AstNode> Parser::parseAssignmentStatement() {
    Token identifierToken = consumeToken();

    Token equalsToken = consumeToken();

    if (equalsToken.type != TokenType::equals) {
        std::cerr<<"Syntax Error: Expected '='\n";
        exit(1);
    }

    std::unique_ptr<AstNode> expressionNode = parseExpression();

    Token semicolonToken = consumeToken();
    if (semicolonToken.type != TokenType::semicolon) {
        std::cerr<<"Syntax Error: Expected ';'\n";
        exit(1);
    }
    return std::make_unique<AssignmentNode>(identifierToken.value, std::move(expressionNode));
}

std::unique_ptr<BlockNode> Parser::parseBlock() {
    Token leftBraceToken = consumeToken();
    if (leftBraceToken.type != TokenType::leftBrace) {
        std::cerr<<"Syntax Error: Expected '{'\n";
        exit(1);
    }

    auto blockNode = std::make_unique<BlockNode>();

    while (peekToken().type != TokenType::rightBrace && peekToken().type != TokenType::endOfFile) {
        blockNode->statements.push_back(parseStatement());
    }

    Token rightBraceToken = consumeToken();
    if (rightBraceToken.type != TokenType::rightBrace) {
        std::cerr<<"Syntax Error: Expected '}'\n";
        exit(1);
    }

    return blockNode;
}

std::unique_ptr<AstNode> Parser::parseIfStatement() {
    consumeToken();

    std::unique_ptr<AstNode> conditionNode = parseExpression();
    std::unique_ptr<BlockNode> trueBlock = parseBlock();
    std::unique_ptr<BlockNode> falseBlock = nullptr;

    if (peekToken().type == TokenType::elseKeyword) {
        consumeToken();

        falseBlock = parseBlock();
    }

    return std::make_unique<IfStatementNode>(
        std::move(conditionNode),
        std::move(trueBlock),
        std::move(falseBlock));
}
