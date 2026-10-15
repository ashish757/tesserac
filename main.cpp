#include "lexer.h"
#include "parser.h"
#include "codegenerator.h"
#include <iostream>
#include <fstream>

int main() {
    std::string code = "var first = 5 + 10; var second = first - 5;";

    Lexer lexer(code);
    std::vector<Token> lexedTokens = lexer.tokenize();

    Parser parser(lexedTokens);
    std::unique_ptr<AstNode> abstractSyntaxTree = parser.parseProgram();

    CodeGenerator myGenerator;
    std::string generatedCode = myGenerator.generateAssembly(abstractSyntaxTree.get());

    std::ofstream outputFile("../output.s");
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