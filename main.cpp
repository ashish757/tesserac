#include "lexer.h"
#include "parser.h"
#include "codeGenerator.h"
#include <iostream>
#include <fstream>

int main() {
    std::string codeToCompile = "5 + 10 - 3";

    Lexer myLexer(codeToCompile);
    std::vector<Token> lexedTokens = myLexer.tokenize();

    Parser myParser(lexedTokens);
    std::unique_ptr<AstNode> abstractSyntaxTree = myParser.parseProgram();

    CodeGenerator myGenerator;
    std::string generatedCode = myGenerator.generateAssembly(abstractSyntaxTree.get());

    std::ofstream outputFile("output.s");
    if (outputFile.is_open()) {
        outputFile << generatedCode;
        outputFile.close();
        std::cout << "Successfully compiled to output.s\n";
    } else {
        std::cerr << "Failed to open output.s for writing\n";
        return 1;
    }

    return 0;
}