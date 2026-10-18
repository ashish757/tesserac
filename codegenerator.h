#pragma once
#include "ast.h"
#include <string>
#include <unordered_map>
#include <vector>

struct VariableContext {
    int offset;
    std::string dataType;
};

class CodeGenerator {
private:
    std::string assemblyCode;
    std::vector<std::unordered_map<std::string, VariableContext>> environmentStack;
    int currentStackOffset;
    int labelCounter;
    std::string dataSection;
    int floatLiteralCounter;


    VariableContext resolveVariable(const std::string& variableName);
    std::string determineDataType(const AstNode* node);


public:
    CodeGenerator();
    void generateNode(const AstNode* node);
    std::string generateAssembly(const AstNode* rootNode);
};