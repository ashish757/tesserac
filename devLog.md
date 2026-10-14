# TesseraC
- my attempt on compiler design

# Devlog 1, 12 oct
- created a C++ executable project in Clion and setup basic project
- a very simple lex analyzer for numbers and end of file detection

# Devlog 2, 14 oct
-  i have create a AST for arithmatic expression 
- so basically the operator is root node and it has two child node on which the operator is to be applied
- for example "5 + 10 - 3"
            -
         /    \
        +     3 
      /   \
    10    5
- its called postorder traversal, same technique is used with stack or queue as well i dont remember which one
- some registry shit is going on in assembly, i dont understand is completely, there is access to two register stack, which is used to execute all the arithmatic operations


# devlog 14 oct
### refactored into three parts
- lexer
- parser
- code generation
### so the pipeline so far
- lexer now analyses a lot more, it looks for
- arithmatic operators (+, - for now)
- parser can now recognize the variable declaration by var keyword and and = sign
- code generator module outputs raw assembly for arm, accessing stack (sp) and registers
- it uses a table to mapoo variable names to permenent memeory slots (x29)

### now you can do like this
- var myVar = 34+23-3;
the expression will be evaluated and stored into the myVar varibale

> whats next, well right now i can only execute only one line, will make it multiline and also the varibales are just beign stored for now and cant be accessed as wished