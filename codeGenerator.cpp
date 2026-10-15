#include "codegenerator.h"
#include <iostream>
#include <cstdlib>

CodeGenerator::CodeGenerator() : currentStackOffset(0) {}

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
        }
    } else if (const auto* varNode = dynamic_cast<const VarStatementNode*>(node)) {
        generateNode(varNode->expressionNode.get());

        currentStackOffset += 16;
        symbolTable[varNode->variableName] = currentStackOffset;

        assemblyCode += "    str w0, [x29, #-" + std::to_string(currentStackOffset) + "]\n";

    }
    else {
        std::cerr << "Compilation Error: Unknown AST Node\n";
        exit(1);
    }
}

std::string CodeGenerator::generateAssembly(const AstNode* rootNode) {
    assemblyCode = ".global _main\n.align 2\n_main:\n";

    assemblyCode += "    stp x29, x30, [sp, #-16]!\n";
    assemblyCode += "    mov x29, sp\n";
    assemblyCode += "    sub sp, sp, #256\n";

    generateNode(rootNode);
    assemblyCode += "    mov sp, x29\n";
    assemblyCode += "    ldp x29, x30, [sp], #16\n";
    assemblyCode += "    ret\n";
    return assemblyCode;
}