#pragma once
#include "ast.h"
#include <string>

class CodeGenerator {
private:
    std::string assemblyCode;
    void generateNode(const AstNode* node);

public:
    std::string generateAssembly(const AstNode* rootNode);
};