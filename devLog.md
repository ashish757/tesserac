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
