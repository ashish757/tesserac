#include "lexer.h"
#include "parser.h"
#include "codegenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cerrno>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: tesserac <file.ash>\n";
        return 1;
    }

    std::string fileName = argv[1];
    std::ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        std::cerr << "Error: could not open file " << fileName << " - " << std::strerror(errno) << "\n";
        return 1;
    }

    std::stringstream fileBuffer;
    fileBuffer << inputFile.rdbuf();
    std::string sourceCode = fileBuffer.str();
    inputFile.close();

    Lexer myLexer(sourceCode);
    std::vector<Token> lexedTokens = myLexer.tokenize();

    Parser myParser(lexedTokens);
    std::unique_ptr<AstNode> abstractSyntaxTree = myParser.parseProgram();

    CodeGenerator myGenerator;
    std::string generatedCode = myGenerator.generateAssembly(abstractSyntaxTree.get());

    std::ofstream outputFile("output.s");
    if (outputFile.is_open()) {
        outputFile << generatedCode;
        outputFile.close();
    } else {
        std::cerr << "Error: Failed to open output.s for writing\n";
        return 1;
    }

    int compileResult = std::system("clang output.s -o tesserac_out");
    if (compileResult != 0) {
        std::cerr << "Error: Assembly compilation failed\n";
        return 1;
    }

    std::system("./tesserac_out");

    return 0;
}