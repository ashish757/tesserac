#pragma once
#include "ast.h"
#include <string>
#include <unordered_map>
#include <vector>

class CodeGenerator {
private:
    std::string assemblyCode;
    std::vector<std::unordered_map<std::string, int>> environmentStack;
    int currentStackOffset;
    int labelCounter;

    int resolveVariable(const std::string& variableName);


public:
    CodeGenerator();
    void generateNode(const AstNode* node);
    std::string generateAssembly(const AstNode* rootNode);
};