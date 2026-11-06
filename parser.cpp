#include "parser.h"
#include <iostream>
#include <cstdlib>

Parser::Parser(std::vector<Token> inputTokens, std::string source) : tokens(std::move(inputTokens)), currentIndex(0), sourceCode(std::move(source)) {}

Token Parser::consumeToken() {
    return tokens[currentIndex++];
}

Token Parser::peekToken() const {
    if (currentIndex < tokens.size()) {
        return tokens[currentIndex];
    }
    return {TokenType::endOfFile, ""};
}

std::unique_ptr<AstNode> Parser::parsePrimary() {
    if (peekToken().type == TokenType::integer) {
        return std::make_unique<IntegerLiteralNode>(consumeToken().value);
    }

    if (peekToken().type == TokenType::floatLiteral) {
        return std::make_unique<FloatLiteralNode>(consumeToken().value);
    }

    if (peekToken().type == TokenType::stringLiteral) {
        return std::make_unique<StringLiteralNode>(consumeToken().value);
    }

    if (peekToken().type == TokenType::nullKeyword) {
        consumeToken();
        return std::make_unique<NullNode>();
    }

    if (peekToken().type == TokenType::identifier) {
        std::string identifierName = consumeToken().value;

        if (peekToken().type == TokenType::leftParen) {
            return parseFunctionCall(identifierName);
        }


        if (peekToken().type == TokenType::leftBracket) {
            consumeToken();
            std::unique_ptr<AstNode> indexExpr = parseExpression();
            consumeToken();
            return std::make_unique<ArrayAccessNode>(identifierName, std::move(indexExpr));
        }

        return std::make_unique<IdentifierNode>(identifierName);
    }

    std::cerr << "Syntax Error: Expected an integer or variable\n";
    exit(1);
}
std::unique_ptr<AstNode> Parser::parseStatement() {
    if (peekToken().type == TokenType::intKeyword || peekToken().type == TokenType::floatKeyword || peekToken().type == TokenType::stringKeyword ) {
        return parseVarStatement();
    }

    if (peekToken().type == TokenType::printKeyword) {
        return parsePrintStatement();
    }

    if (peekToken().type == TokenType::ifKeyword) {
        return parseIfStatement();
    }

    if (peekToken().type == TokenType::whileKeyword) {
        return parseWhileStatement();
    }

    if (peekToken().type == TokenType::fnKeyword) {
        return parseFunctionDeclaration();
    }

    if (peekToken().type == TokenType::returnKeyword) {
        return parseReturnStatement();
    }

    if (peekToken().type == TokenType::leftBrace) {
        return parseBlock();
    }

    if (peekToken().type == TokenType::breakKeyword) {
       consumeToken();
        Token semiColonToken = consumeToken();
        if (semiColonToken.type != TokenType::semicolon) {
            std::cerr<<"Syntax Error: Expected ';' \n";
            exit(1);
        }
        return std::make_unique<BreakNode>();

    }


    if (peekToken().type == TokenType::continueKeyword) {
        consumeToken();

        Token semiColonToken = consumeToken();
        if (semiColonToken.type != TokenType::semicolon) {
           throwError("Expected ';' at the end of the statement", semiColonToken);

        }

        return std::make_unique<ContinueNode>();
    }

    if (peekToken().type == TokenType::identifier) {
        if (currentIndex + 1 < tokens.size() && tokens[currentIndex + 1].type == TokenType::equals) {
            return parseAssignmentStatement();
        }
        if (currentIndex + 1 < tokens.size() && tokens[currentIndex + 1].type == TokenType::leftBracket) {
            return parseArrayAssignmentStatement();
        }
    }

    std::unique_ptr<AstNode> expressionNode = parseExpression();

    Token semicolonToken = consumeToken();
    if (semicolonToken.type != TokenType::semicolon) {
        std::cerr<<"Syntax Error: Expected ';' \n";
        exit(1);
    }

    return expressionNode;
}

std::unique_ptr<AstNode> Parser::parseExpression() {
    return parseLogicalOr();
}

std::unique_ptr<AstNode> Parser::parseLogicalOr() {
        std::unique_ptr<AstNode> leftNode = parseLogicalAnd();

    while (peekToken().type == TokenType::logicalOr) {
        Token operatorToken = consumeToken();

        std::unique_ptr<AstNode> rightNode = parseLogicalAnd();
        leftNode = std::make_unique<LogicalExpressionNode>(operatorToken.value, std::move(leftNode), std::move(rightNode));
    }

    return leftNode;
}


std::unique_ptr<AstNode> Parser::parseLogicalAnd() {
    std::unique_ptr<AstNode> leftNode = parseEquality();

    while (peekToken().type == TokenType::logicalAnd) {
        Token operatorToken = consumeToken();
        std::unique_ptr<AstNode> rightNode = parseEquality();
        leftNode = std::make_unique<LogicalExpressionNode>(operatorToken.value, std::move(rightNode), std::move(leftNode));
    }

    return leftNode;
}


std::unique_ptr<AstNode> Parser::parseEquality() {
    std::unique_ptr<AstNode> leftNode = parseRelational();

    while (peekToken().type == TokenType::doubleEquals) {
        Token operatorToken = consumeToken();
        std::unique_ptr<AstNode> rightNode = parseRelational();
        leftNode = std::make_unique<BinaryExpressionNode>(operatorToken.value, std::move(leftNode), std::move(rightNode));

    }
    return leftNode;
}

std::unique_ptr<AstNode> Parser::parseRelational() {
    std::unique_ptr<AstNode> leftNode = parseAdditive();
    while (peekToken().type == TokenType::lessThan || peekToken().type == TokenType::greaterThan) {
        Token operatorToken = consumeToken();
        std::unique_ptr<AstNode> rightNode = parseAdditive();
        leftNode = std::make_unique<BinaryExpressionNode>(operatorToken.value, std::move(leftNode), std::move(rightNode));

    }
    return leftNode;

}

std::unique_ptr<AstNode> Parser::parseAdditive() {
    std::unique_ptr<AstNode> leftNode = parseMultiplicative();

    while (peekToken().type == TokenType::plus || peekToken().type == TokenType::minus) {
        Token operatorToken = consumeToken();
       std::unique_ptr<AstNode> rightNode = parseMultiplicative();
        leftNode = std::make_unique<BinaryExpressionNode>(operatorToken.value, std::move(leftNode), std::move(rightNode));

    }
    return leftNode;
}


std::unique_ptr<AstNode> Parser::parseMultiplicative() {
    std::unique_ptr<AstNode> leftNode = parsePostfix();

    while (peekToken().type == TokenType::star || peekToken().type == TokenType::slash || peekToken().type == TokenType::modulo) {
        Token operatorToken = consumeToken();
        std::unique_ptr<AstNode> rightNode = parsePostfix();
        leftNode = std::make_unique<BinaryExpressionNode>(operatorToken.value, std::move(leftNode), std::move(rightNode));
    }
    return leftNode;
}

std::unique_ptr<AstNode> Parser::parsePostfix() {
    std::unique_ptr<AstNode> leftNode = parsePrimary();

    if (peekToken().type == TokenType::increment || peekToken().type == TokenType::decrement) {
        Token operatorToken = consumeToken();
        if (auto* idNode = dynamic_cast<IdentifierNode*>(leftNode.get())) {
            return std::make_unique<PostfixNode>(idNode->name, operatorToken.value);
        }
        std::cerr << "Syntax Error: Increment and decrement require a variable\n";
        exit(1);

    }
    return leftNode;
}


std::unique_ptr<AstNode> Parser::parseVarStatement() {
    Token typeToken = consumeToken();

    Token identifierToken = consumeToken();
    if (identifierToken.type != TokenType::identifier) {
        std::cerr << "Syntax Error: Expected identifier\n";
        exit(1);
    }

    if (peekToken().type == TokenType::leftBracket) {
        consumeToken();
        Token sizeToken = consumeToken();
        if (sizeToken.type != TokenType::integer) {
            std::cerr<<"Syntax Error: Array size must be an integer\n";
            exit(1);
        }

        int size = std::stoi(sizeToken.value);

        consumeToken();
        consumeToken();
        return std::make_unique<ArrayDeclarationNode>(typeToken.value, identifierToken.value, size);

    }



    Token equalsToken = consumeToken();

    if (equalsToken.type != TokenType::equals) {
        std::cerr << "Syntax Error: Expected '=' "<<"\n";
        exit(1);
    }

    std::unique_ptr<AstNode> expressionNode = parseExpression();

    Token semicolonToken = consumeToken();
    if (semicolonToken.type != TokenType::semicolon) {
        std::cerr<<"Syntax Error: Expected ';' \n";
        exit(1);
    }

    return std::make_unique<VarStatementNode>(typeToken.value, identifierToken.value, std::move(expressionNode));
}



std::unique_ptr<AstNode> Parser::parseProgram() {
    auto programNode = std::make_unique<ProgramNode>();

    while (peekToken().type != TokenType::endOfFile) {
        programNode->statements.push_back(parseStatement());
    }
    return programNode;
}

std::unique_ptr<AstNode> Parser::parsePrintStatement() {
    consumeToken();
    std::unique_ptr<AstNode> expressionNode = parseExpression();
    Token semicolonToken = consumeToken();
    if (semicolonToken.type != TokenType::semicolon) {
        std::cerr<<"Syntax Error: Expected ';' \n";
        exit(1);
    }
    return std::make_unique<PrintStatementNode>(std::move(expressionNode));
}


std::unique_ptr<AstNode> Parser::parseAssignmentStatement() {
    Token identifierToken = consumeToken();

    Token equalsToken = consumeToken();

    if (equalsToken.type != TokenType::equals) {
        std::cerr<<"Syntax Error: Expected '='\n";
        exit(1);
    }

    std::unique_ptr<AstNode> expressionNode = parseExpression();

    Token semicolonToken = consumeToken();
    if (semicolonToken.type != TokenType::semicolon) {
        std::cerr<<"Syntax Error: Expected ';' \n";
        exit(1);
    }
    return std::make_unique<AssignmentNode>(identifierToken.value, std::move(expressionNode));
}

std::unique_ptr<BlockNode> Parser::parseBlock() {
    Token leftBraceToken = consumeToken();
    if (leftBraceToken.type != TokenType::leftBrace) {
        std::cerr<<"Syntax Error: Expected '{'\n";
        exit(1);
    }

    auto blockNode = std::make_unique<BlockNode>();

    while (peekToken().type != TokenType::rightBrace && peekToken().type != TokenType::endOfFile) {
        blockNode->statements.push_back(parseStatement());
    }

    Token rightBraceToken = consumeToken();
    if (rightBraceToken.type != TokenType::rightBrace) {
        std::cerr<<"Syntax Error: Expected '}'\n";
        exit(1);
    }

    return blockNode;
}

std::unique_ptr<AstNode> Parser::parseIfStatement() {
    consumeToken();

    std::unique_ptr<AstNode> conditionNode = parseExpression();
    std::unique_ptr<AstNode> trueBlock = parseBlock();
    std::unique_ptr<AstNode> falseBlock = nullptr;

    if (peekToken().type == TokenType::elseKeyword) {
        consumeToken();

        if (peekToken().type == TokenType::ifKeyword) {
            falseBlock = parseIfStatement();
        } else {
            falseBlock = parseBlock();
        }
    }

    return std::make_unique<IfStatementNode>(
        std::move(conditionNode),
        std::move(trueBlock),
        std::move(falseBlock));
}


std::unique_ptr<AstNode> Parser::parseWhileStatement() {
    consumeToken();

    std::unique_ptr<AstNode> conditionNode = parseExpression();
    std::unique_ptr<BlockNode> loopBlock = parseBlock();

    return std::make_unique<WhileStatementNode>(std::move(conditionNode), std::move(loopBlock));
}

std::unique_ptr<AstNode> Parser::parseFunctionDeclaration() {
    consumeToken();
    Token nameToken = consumeToken();
    std::string functionName = nameToken.value;

    consumeToken();

    std::vector<std::string> parameters;

    if (peekToken().type != TokenType::rightParen) {
        parameters.push_back(consumeToken().value);
        while (peekToken().type == TokenType::comma) {
            consumeToken();
            parameters.push_back(consumeToken().value);
        }
    }
     consumeToken();

    std::unique_ptr<BlockNode> body = parseBlock();
    return std::make_unique<FunctionDeclarationNode>(functionName, std::move(parameters), std::move(body));
}


std::unique_ptr<AstNode> Parser::parseReturnStatement() {
    consumeToken();

    std::unique_ptr<AstNode> expressionNode = parseExpression();

    Token semicolonToken = consumeToken();
    if (semicolonToken.type != TokenType::semicolon) {
        std::cerr<<"Syntax Error: Expected ';' \n";
        exit(1);
    }

    return std::make_unique<ReturnNode>(std::move(expressionNode));
}

std::unique_ptr<AstNode> Parser::parseFunctionCall(const std::string& functionName) {
    consumeToken();

    std::vector<std::unique_ptr<AstNode>> arguments;

    if (peekToken().type != TokenType::rightParen) {
        arguments.push_back(parseExpression());
        while (peekToken().type == TokenType::comma) {
            consumeToken();
            arguments.push_back(parseExpression());
        }
    }

    consumeToken();
    return std::make_unique<FunctionCallNode>(functionName, std::move(arguments));
}


std::unique_ptr<AstNode> Parser::parseArrayAssignmentStatement() {
    Token identifierToken = consumeToken();
    consumeToken();


    std::unique_ptr<AstNode> indexExpr = parseExpression();

    Token rightBracket = consumeToken();
    if (rightBracket.type != TokenType::rightBracket) {
        std::cerr<<"Syntax Error: Expected ']'\n";
        exit(1);
    }

    Token equalsToken = consumeToken();
    if (equalsToken.type != TokenType::equals) {
        std::cerr<<"Syntax Error: Expected '='\n";
        exit(1);
    }

    std::unique_ptr<AstNode> valueExpr = parseExpression();

    Token semicolonToken = consumeToken();
    if (semicolonToken.type != TokenType::semicolon) {
        // Add the semicolonToken.value to the output
        std::cerr << "Syntax Error: Expected ';', but got '" << semicolonToken.value << "'\n";
        exit(1);
    }



    return std::make_unique<ArrayAssignmentNode>(identifierToken.value, std::move(indexExpr), std::move(valueExpr));





}








void Parser::throwError(const std::string& message, Token token) {
    std::cerr<<"Syntax Error:\033[0m "<<message<<"\n\n";
    std::cerr<<"  --> program.ash:"<<token.value<<":"<<token.column<<"\n\n";


    int currentLine = 1;
    size_t  lineStart = 0;
    size_t lineEnd = sourceCode.find('\n');

    while (currentLine != token.line && lineEnd != std::string::npos) {
        lineStart = lineEnd + 1;
        lineEnd = sourceCode.find("\n", lineStart);
        currentLine++;
    }

    if (lineEnd == std::string::npos) lineEnd = sourceCode.length();

    std::string errorLine = sourceCode.substr(lineStart, lineEnd - lineStart);

    std::cerr<<" "<<token.line<<" | "<<errorLine<<"\n";
    std::cerr<<"   | ";
    for (int i =1; i < token.column; i++) std::cerr<<" ";
    std::cerr<<"\033[1;31m^\033[0m\n\n";

    exit(1);
}


/*






*/
