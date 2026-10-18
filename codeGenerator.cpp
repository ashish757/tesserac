#include "codegenerator.h"
#include <iostream>
#include <cstdlib>

CodeGenerator::CodeGenerator() : currentStackOffset(0), labelCounter(0) {
    environmentStack.push_back(std::unordered_map<std::string, int>());
}

int CodeGenerator::resolveVariable(const std::string& variableName) {
    for (auto iterator = environmentStack.rbegin(); iterator != environmentStack.rend(); ++iterator) {
        if (iterator->contains(variableName)) {
            return iterator->at(variableName);
        }
    }
    return -1;
}

void CodeGenerator::generateNode(const AstNode* node) {
    if (const auto&  programNode = dynamic_cast<const ProgramNode*>(node)) {
      for (const auto& statement : programNode->statements) {
          generateNode(statement.get());
      }
    } else if (const auto* integerNode = dynamic_cast<const IntegerLiteralNode*>(node)) {
        assemblyCode += "    mov w0, #" + integerNode->value + "\n";
    } else if (const auto* binaryNode = dynamic_cast<const BinaryExpressionNode*>(node)) {
    } else if (const auto* nullNode = dynamic_cast<const NullNode*>(node)) {
        assemblyCode += "    mov w0, #0\n";
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
        } else if (binaryNode->operatorSymbol == "<") {
            assemblyCode += "    cmp w0, w1\n";
            assemblyCode += "    cset w0, lt\n";
        } else if (binaryNode->operatorSymbol == ">") {
            assemblyCode += "    cmp w0, w1\n";
            assemblyCode += "    cset w0, gt\n";
        }
    } else if (const auto* varNode = dynamic_cast<const VarStatementNode*>(node)) {
        generateNode(varNode->expressionNode.get());

        currentStackOffset += 16;
        environmentStack.back()[varNode->variableName] = currentStackOffset;

        assemblyCode += "    str w0, [x29, #-" + std::to_string(currentStackOffset) + "]\n";

    } else if (const auto* identifierNode = dynamic_cast<const IdentifierNode*>(node)) {
        int offset = resolveVariable(identifierNode->name);

        if (offset == -1) {
            std::cerr<<"Compilation Error: Undefined Variable\n";
            exit(1);
        }
        assemblyCode += "    ldr w0, [x29, #-" + std::to_string(offset) +  "]\n";
    } else if (const auto* printNode = dynamic_cast<const PrintStatementNode*>(node)) {
        generateNode(printNode->expressionNode.get());

        assemblyCode += "    str w0, [sp]\n";
        assemblyCode += "    adrp x0, formatString@PAGE\n";
        assemblyCode += "    add x0, x0, formatString@PAGEOFF\n";
        assemblyCode += "    bl _printf\n";

    } else if (const auto* assignNode = dynamic_cast<const AssignmentNode*>(node)) {
        generateNode(assignNode->expressionNode.get());
        int offset = resolveVariable(assignNode->variableName);

        if (offset == -1) {
            std::cerr<<"Compilation Error: Undefined Variable\n";
            exit(1);
        }

        assemblyCode += "   str w0, [x29, #-" + std::to_string(offset) + "]\n";

    } else if (const auto* blockNode = dynamic_cast<const BlockNode*>(node)) {
        environmentStack.push_back(std::unordered_map<std::string,  int>());
        for (const auto& statement : blockNode->statements) {
            generateNode(statement.get());
        }
        environmentStack.pop_back();
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
    } else if (const auto* whileNode = dynamic_cast<const WhileStatementNode*>(node)) {
        int currentLabel = labelCounter++;
        std::string startLabel = ".L_while_start_" + std::to_string(currentLabel);
        std::string endLabel = ".L_while_end_" + std::to_string(currentLabel);

        assemblyCode += startLabel + ":\n";

        generateNode(whileNode->conditionNode.get());

        assemblyCode += "    cmp w0, #0\n";
        assemblyCode += "    b.eq " + endLabel + "\n";

        generateNode(whileNode->loopBlock.get());

        assemblyCode += "    b " + startLabel + "\n";

        assemblyCode += endLabel + ":\n";
    } else if (const auto* fnNode = dynamic_cast<const FunctionDeclarationNode*>(node)) {
        int currentLabel = labelCounter++;
        std::string skipLabel = ".L_skip_fn_" + std::to_string(currentLabel);

        assemblyCode += "    b " + skipLabel + "\n";
        assemblyCode += "_" + fnNode->functionName + ":\n";
        assemblyCode += "    stp x29, x30, [sp, #-16]!\n";
        assemblyCode += "    mov x29, sp\n";

        environmentStack.push_back(std::unordered_map<std::string, int>());
        int previousOffset = currentStackOffset;
        currentStackOffset = 0;

        for (size_t i =0; i  < fnNode->parameters.size(); i++) {
            currentStackOffset += 16;
            environmentStack.back()[fnNode->parameters[i]] = currentStackOffset;
            assemblyCode += "    str w" + std::to_string(i) + ", [x29, #-" + std::to_string(currentStackOffset) + "]\n";
        }
        generateNode(fnNode->body.get());

        assemblyCode += "    ldp x29, x30, [sp], 16\n";
        assemblyCode += "    ret\n";

        environmentStack.pop_back();
        currentStackOffset = previousOffset;
        assemblyCode += skipLabel + ":\n";
    } else if (const auto* callNode = dynamic_cast<const FunctionCallNode*>(node)) {
        for (size_t i =0; i< callNode->arguments.size(); i++) {
            generateNode(callNode->arguments[i].get());
            assemblyCode += "    str w0, [sp, #-16]!\n";
        }
        for (int i = callNode->arguments.size()-1; i>=0; i--) {
            assemblyCode += "    ldr w" + std::to_string(i) + ", [sp], 16\n";
        }

        assemblyCode += "    bl _" + callNode->functionName + "\n";
    } else if (const auto* returnNode = dynamic_cast<const ReturnNode*>(node)) {
        generateNode(returnNode->expressionNode.get());
        assemblyCode += "    ldp x29, x30, [sp], 16\n";
        assemblyCode += "    ret\n";
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
