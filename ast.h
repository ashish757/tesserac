#pragma once
#include <string>
#include <memory>
#include <utility>
#include <vector>


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

class ProgramNode : public AstNode {
public:
    std::vector<std::unique_ptr<AstNode>> statements;
};

class IdentifierNode : public AstNode {
public:
    std::string name;
    explicit IdentifierNode(std::string varName) : name(std::move(varName)) {}
};

class PrintStatementNode : public AstNode {
public:
    std::unique_ptr<AstNode> expressionNode;

    explicit PrintStatementNode(std::unique_ptr<AstNode> expr)
        : expressionNode(std::move(expr)) {}
};

class AssignmentNode : public AstNode {
public:
    std::string variableName;
    std::unique_ptr<AstNode> expressionNode;

    AssignmentNode(std::string name, std::unique_ptr<AstNode> expr)
        : variableName(std::move(name)), expressionNode(std::move(expr)) {}
};

class BlockNode : public AstNode {
public:
    std::vector<std::unique_ptr<AstNode>> statements;
};


class IfStatementNode : public AstNode {
public:
    std::unique_ptr<AstNode> conditionNode;
    std::unique_ptr<AstNode> trueBlock;
    std::unique_ptr<AstNode> falseBlock;

    IfStatementNode(
        std::unique_ptr<AstNode> cond,
        std::unique_ptr<AstNode> trueB,
        std::unique_ptr<AstNode> falseB)
            : conditionNode(std::move(cond)), trueBlock(std::move(trueB)), falseBlock(std::move(falseB)) {}
};