#include "codeGenerator.h"
#include <iostream>
#include <cstdlib>

void CodeGenerator::generateNode(const AstNode* node) {
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

std::string CodeGenerator::generateAssembly(const AstNode* rootNode) {
    assemblyCode = ".global _main\n.align 2\n_main:\n";
    generateNode(rootNode);
    assemblyCode += "    ret\n";
    return assemblyCode;
}