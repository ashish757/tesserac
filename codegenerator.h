#pragma once
#include "ast.h"
#include <string>
#include <unordered_map>

class CodeGenerator {
private:
    std::string assemblyCode;
    std::unordered_map<std::string, int> symbolTable;
    int currentStackOffset;
    int labelCounter;


public:
    CodeGenerator();
    void generateNode(const AstNode* node);
    std::string generateAssembly(const AstNode* rootNode);
};