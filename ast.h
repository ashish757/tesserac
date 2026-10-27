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

class LogicalExpressionNode : public AstNode {
public:
    std::string operatorSymbol;
    std::unique_ptr<AstNode> leftNode;
    std::unique_ptr<AstNode> rightNode;

    LogicalExpressionNode(std::string opSymbol, std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right)
    : operatorSymbol(std::move(opSymbol)), leftNode(std::move(left)), rightNode(std::move(right)) {}
};

class PostfixNode : public AstNode {
    public:
    std::string variableName;
    std::string operatorSymbol;

    PostfixNode(std::string name, std::string opSymbol)
        : variableName(std::move(name)), operatorSymbol(std::move(opSymbol)) {}
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
        std::unique_ptr<AstNode> trueB,        std::unique_ptr<AstNode> falseB)
            : conditionNode(std::move(cond)), trueBlock(std::move(trueB)), falseBlock(std::move(falseB)) {}
};

class WhileStatementNode : public AstNode {
public:
    std::unique_ptr<AstNode> conditionNode;
    std::unique_ptr<AstNode> loopBlock;

    WhileStatementNode(
        std::unique_ptr<AstNode> cond,
        std::unique_ptr<AstNode> loopBlock)
            : conditionNode(std::move(cond)), loopBlock(std::move(loopBlock)) {}
};

class FunctionDeclarationNode: public AstNode {
public:
    std::string functionName;
    std::vector<std::string> parameters;
    std::unique_ptr<BlockNode> body;

    FunctionDeclarationNode(
        std::string name,
        std::vector<std::string> params,
        std::unique_ptr<BlockNode> block)
    : functionName(std::move(name)), parameters(std::move(params)), body(std::move(block)) {}

};

class FunctionCallNode : public AstNode {
public:
    std::string functionName;
    std::vector<std::unique_ptr<AstNode>> arguments;

    FunctionCallNode(
        std::string name,
        std::vector<std::unique_ptr<AstNode>> params) : functionName(std::move(name)), arguments(std::move(params)) {}
};


class ReturnNode : public AstNode {
public:
    std::unique_ptr<AstNode> expressionNode;
    explicit ReturnNode(std::unique_ptr<AstNode> expr)
        : expressionNode(std::move(expr)) {}
};

class NullNode  : public AstNode {};

class FloatLiteralNode : public AstNode {
public:
    std::string value;

    explicit FloatLiteralNode(std::string val) : value(std::move(val)) {}
};

class StringLiteralNode : public AstNode {
public:
    std::string value;
    explicit StringLiteralNode(std::string val) : value(std::move(val)) {}
};