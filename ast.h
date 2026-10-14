#pragma once
#include <string>
#include <memory>
#include <utility>

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

class VarStatementNode : public AstNode {
public:
    std::string variableName;
    std::unique_ptr<AstNode> expressionNode;

    VarStatementNode(std::string name, std::unique_ptr<AstNode> expr)
        : variableName(std::move(name)), expressionNode(std::move(expr)) {}
};