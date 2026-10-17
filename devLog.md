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


# devlog  3, 14 oct
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



# devlog 4, 16 oct
- well as promised in last devlog now compiler executes multiple lines, very easy implementation (😭) jsut added a bunch of loops (you believe that).
- well the actual implementation incldued some messing with register and assembly 
- now you can also use a print keyword to print anything (just numbers for now, but everything because text is just bunch of numbers).


# devlog 5, 16 oct
- i think i am  a bit locked in today, 2nd dev log already
- so if else implemented, 
- scopes implemented (basically detected opening and closing curly brackets after if and else keyword)
- the actual scope detections of local or global is my next target
- while loop works now but run only once why? well beacuse i am a fool, i have not implemented the relational operator like > and <, only the equality, it looks like somthign this
```
var counter = 3;
while counter == 3 {
    print counter;
    counter = counter - 1;
}
```
- also the variables created inside a scope exists even in global scope i need to fix this because right now i have a big table which hold all the vars i need to find what is the standard complier way to create local and global scopes
- and i forget one thing the file support for source code, earlier i used to write the source code as string in the main.cpp file directly
- but now the compiler opens the source code file and reads it and give the output just like any other programing language (Hold  your breath Rust, ash is coming, {ash i name of my language})


# devlog 6, 17 oct
Well well well, i have a crazy feature done
- ## SCOPESS
- here goes nothing
```
var a = 10;
{
    print a;
}

print a;
```

> 10

> 10

- updating the var in a scope
```
var a = 10;
{
    a = 20;
    print a;
}

print a;
```

> 20

> 20

- creating a shadow var in local scope
```
var a = 10;
{
    var a = 20;
    print a;
}

print a;
```

> 20

> 10

- And this one throws error
```
{
    var a = 1;

}

print a;
```
>ashish@Mac tesserac % ./tesserac program.ash

>Compilation Error: Undefined Variable


- So how the hell it works?
- so when ever a local var is created with a same identifier as a global one, what we do is we push back a new unorderd map in the table, which is bascially a stack (vector)
- and then we iterate that vector in reverse order so that the newest created comes first
- so technically both the global and local vars exists but the local one is guaranteed to be present after the global one and hence in reverser iteration we find it and return
- and as soon as the block ends we just remove that whole unordered map from the stack clearing out the memary
