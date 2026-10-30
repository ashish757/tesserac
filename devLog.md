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



# Devlog 7, 18 oct

### A lot happened
- functions, data types, arguments, return values, null, string, int, float


- since last time i am so down to write a dev log, i have taken another step towards the control flows and data
- i mean data is all there is , everything in a language revolves around it

## Functions
so this has a lot of technical details
- the syntax
```
fire funName(a, b) {
    return a + b;
}
```
### some bullshit
- fire is used to create the function, why fire? when i started my aim was just a basic compiler and i named is TesseraC, is has that 'C' in last which indicated the c lang family of compilers
- and later i just sort of made a mini programign language, i named the language ASH (after my name Ashish)
- and fire and ash are related so, and also i am thinking of wood to be keyword for maybe creating classes or a dictionary like data type called log
- basically wood, fire and ash themed, idk why but its gonna happen, its my language i can do anything

#### Technicalities
- so in theory implementing function required two things
    - a local scoped stack to manage the local scope of function and clear it when function returns
    - a direction to where the control of execution is supposed to go and where it has to come back to after execution

`stp x29, x30, [sp, #-16]!`
- this is assembly code for arm65, its is called prologue (i dont  know why)
- `stp x29, x30` - this saves the registers data into the memory, x29 and x30 are the two being saved
- [sp, #-16]! -> this basically calculates the memory address and updates the stack pointer

After this as the current state of program is saved and we can safely run the function ans when the function is completed we load this state back into stack and registors

`ldp x29, x30, [sp], 16)`
- now this is referred as the epilogue (now i understand why they are called what they aree called)
- ldp x29 x30 loads the both old data back from stack into the registers
- and sp 16 handles memory address calculation and points to the previous location for continuing executing after function call

# devlog 7 continued - 19 oct
## Data Type
- i also implemented support for data types, null, string, float, int
- not in full action right now, as currently its neither stoically typed not dynamic, i dont knwo what it is i need to read about it.
- if you test i know it will feel like dynamic typed but believe me its not even that, anyways my final goal is to make is static typed anyways

### Technicalities
- in here i basically has zero idea of how to do it
- the float thing was done by i dont know some different hardware level thing by assembly
- but i understood the string and null, string just uses a 64 bit pointer and stores the data into .dat section instead of stack


- In case you are interested, the major problems i faced are the apple hardware bugs for implementing float and ignorance in string department




## Devlog 8 - 30 Oct
took a long break i guess since last coding, i was attending a week long cybersecurity workshop

anyways i am back and i completed some thigs and also implemented the static typing thing


- ### break and continue
- simple just check that these keywords are within the a while loop only
- and then put up labels accordingly `.L_while_start.` , `.L_while_end_`

- ### else if ladder
- i added the support for else if ladder, that is now you can chain the `if else-if else-if else` 
- it was easier to do then it sounds, i was initially confused on how to even start implementing this, i toook some ai help and got to know that a little improvement in my existing logic will support it


- ### Static type and schemantic check -  this is most interesting one
- so earlier i used a `var` keyword to declare and initialize the variables, but it is childish, like a dynamically typed language in 2026, ???
- to change i first defined datatypes and then keyword like `int` `float` `string` 
- then did schemantic checking to ensure a int has only int value if not then throw a error.
- schema checking was the most difficulty part i could not recall everything about it i had to hit the textbook and AI.

## Note
- i saw i hackatime has like tracked a differnet project time into this project time, 
- ran a agent on a different project and then i started working on this while agenet was actively coding in the backend, the agent was working on  a personal project the repo is private to hackatime could not detect it
- as a result i now see about 3hrs of ai code on this project and also typescript in languages, but i never had a typescript file in this project
- just wanted to mention the bug, i dont know if there is some official portal to report this bug, i will look into