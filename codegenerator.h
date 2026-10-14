#pragma once
#include "ast.h"
#include <string>
#include <map>

class CodeGenerator {
private:
    std::string assemblyCode;
    std::map<std::string, int> symbolTable;
    int currentStackOffset;

    void generateNode(const AstNode* node);

public:
    CodeGenerator();
    std::string generateAssembly(const AstNode* rootNode);
};