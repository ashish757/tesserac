#include "codegenerator.h"
#include <iostream>
#include <cstdlib>

CodeGenerator::CodeGenerator() : currentStackOffset(0), labelCounter(0) {}

void CodeGenerator::generateNode(const AstNode* node) {
    if (const auto&  programNode = dynamic_cast<const ProgramNode*>(node)) {
      for (const auto& statement : programNode->statements) {
          generateNode(statement.get());
      }
    } else if (const auto* integerNode = dynamic_cast<const IntegerLiteralNode*>(node)) {
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
        } else if (binaryNode->operatorSymbol == "==") {
            assemblyCode += "    cmp w0, w1\n";
            assemblyCode += "    cset w0, eq\n";
        }
    } else if (const auto* varNode = dynamic_cast<const VarStatementNode*>(node)) {
        generateNode(varNode->expressionNode.get());

        currentStackOffset += 16;
        symbolTable[varNode->variableName] = currentStackOffset;

        assemblyCode += "    str w0, [x29, #-" + std::to_string(currentStackOffset) + "]\n";

    } else if (const auto* identifierNode = dynamic_cast<const IdentifierNode*>(node)) {
        if (symbolTable.find(identifierNode->name) == symbolTable.end()) {
            std::cerr<<"Compilation Error: Undefined Variable\n";
            exit(1);
        }
        int offset = symbolTable[identifierNode->name];
        assemblyCode += "    ldr w0, [x29, #-" + std::to_string(offset) +  "]\n";
    } else if (const auto* printNode = dynamic_cast<const PrintStatementNode*>(node)) {
        generateNode(printNode->expressionNode.get());

        assemblyCode += "    str w0, [sp]\n";
        assemblyCode += "    adrp x0, formatString@PAGE\n";
        assemblyCode += "    add x0, x0, formatString@PAGEOFF\n";
        assemblyCode += "    bl _printf\n";

    } else if (const auto* assignNode = dynamic_cast<const AssignmentNode*>(node)) {
        generateNode(assignNode->expressionNode.get());

        if (symbolTable.find(assignNode->variableName) == symbolTable.end()) {
            std::cerr<<"Compilation Error: Undefined Variable\n";
            exit(1);
        }

        int offset = symbolTable[assignNode->variableName];
        assemblyCode += "   str w0, [x29, #-" + std::to_string(offset) + "]\n";

    } else if (const auto* blockNode = dynamic_cast<const BlockNode*>(node)) {
        for (const auto& statement : blockNode->statements) {
            generateNode(statement.get());
        }
    } else if (const auto* ifNode = dynamic_cast<const IfStatementNode*>(node)) {
        int currentLabel = labelCounter++;

        std::string elseLabel = ".L_else_" + std::to_string(currentLabel);
        std::string endLabel = ".L_end_" + std::to_string(currentLabel);

        generateNode(ifNode->conditionNode.get());
        assemblyCode += "   cmp w0, #0\n";
        assemblyCode += "   b.eq " + elseLabel + "\n";

        generateNode(ifNode->trueBlock.get());
        assemblyCode += "   b " + endLabel + "\n";

        assemblyCode += elseLabel + ":\n";

        if (ifNode->falseBlock) {
            generateNode(ifNode->falseBlock.get());
        }

        assemblyCode += endLabel + ":\n";
    }
    else {
        std::cerr << "Compilation Error: Unknown AST Node\n";
        exit(1);
    }
}

std::string CodeGenerator::generateAssembly(const AstNode* rootNode) {
    assemblyCode = ".data\n";
    assemblyCode += "formatString: .asciz \"%d\\n\"\n";

    assemblyCode += ".text\n";
    assemblyCode += ".global _main\n.align 2\n_main:\n";

    assemblyCode += "    stp x29, x30, [sp, #-16]!\n";
    assemblyCode += "    mov x29, sp\n";
    assemblyCode += "    sub sp, sp, #256\n";

    generateNode(rootNode);

    assemblyCode += "    mov sp, x29\n";
    assemblyCode += "    ldp x29, x30, [sp], #16\n";
    assemblyCode += "    ret\n";
    return assemblyCode;
}