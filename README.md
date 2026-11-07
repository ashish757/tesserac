# TesseraC Compiler
A compiler for simple programing language Ash (i created it), written in C++ for ARM64 (will support x86_64 in future version)

It tokenizes, parses and compiles  syntax directly into native Apple Silicon (ARM64) assembly



# Motive
- i built a CHIP-8 Emulator in C++, and i liked it very much it was very different from the Web Development that i have been doing all the time
- so this time i decided to go more into low level, my senior (3rd year CS), gave me his spare book and guess the title, Compiler Design and Operating System
- i gave it a read and i went all in, hail low level



# Features 
### Implementation of Compiler Design
- Lexical Analyzer : This basically converts the raw text into separate tokens/chunks for further processing
- Syntax Analyzer : takes the tokens from Lexical Analyzer and builds a Abstract Syntax Tree following the rules of the programing languages
- Semantic Analyzer : Its like grammar check, it checks if the AST is valid or not
- Code Generator : finally assembly code is generated here
- Symbol Table : it stores all the symbol name and map them to corresponding memory address
- Error Handler : This section is not separate but this is about correct and descriptive error display

## Programing Language: Ash
- Static Typed
- Procedural
- operators : Arithmetic, Relational, Increment & decrement, Logical
- Control Flow : scoped if-else ladder and while loop with break and continue keywords
- Data Types: int , float, string, boolean
- local and global Variables scopes
- Functions, return values and arguments
- print keyword for printing to console





## Syntax
`
int a = 5;
print a

int b = a + 10;
print b;
`



`
fn sum(int a, int b) {
    return a+b;
}

print fn(10, 20);
`

## builtin keywords
- int, float, string  --> to create variable of same data type
- fn -> to create a function
```
fn sum(int a, int b) {
  return a +b;
}
```
- print -> for output on console
- if-else, while and break & continue for proper control flow
- other symbols like brackets, semicolon, comma, period etc.



# For Technical Deatils and my journey while building this 
please read devLog.md,   [Link](devLog.md)



# How to Run/Test
# 1. Download the executable from the release section

# 2. Build locally
- clone the repo
- cd to root directory
- then run `clang++ *.cpp -o tesserac -std=c++20` to build the code
- then run `./tesserac <file.ash>`



# Future Versions
- Taking input form user
- Complex Data structures like arrays and maps
- more complex functional programing support
- better error messages display

## Resources
https://en.wikipedia.org/wiki/Tail_call
https://developer.apple.com/documentation/xcode/writing-arm64-code-for-apple-platforms
https://student.cs.uwaterloo.ca/~cs452/docs/rpi4b/aapcs64.pdf
https://en.wikipedia.org/wiki/Application_binary_interface#C++
https://en.wikipedia.org/wiki/Calling_convention#ARM_(A64)
https://www.codermusings.com/resources/calling-conventions-cheat-sheet/


# AI Use
- used to learn and simplify the articles on developer.apple.com & wikipedia
- used some code snippets here and there in code, mainly for print feature's assembly code and function and stack  management
- more details in devLog.md

# Some Example Programs

```
string greeting = "Hello from TesseraC!";
print greeting;

int version = 1;
print "Compiler Version:";
print version;

float pi = 3.14159;
print "Float support active:";
print pi;
```


# While loop (FizzBuzz)
```
int limit = 15;
int i = 1;

print "Starting FizzBuzz:";

while (i < limit + 1) {
    if (i % 15 == 0) {
        print "FizzBuzz";
    } else if (i % 3 == 0) {
        print "Fizz";
    } else if (i % 5 == 0) {
        print "Buzz";
    } else {
        print i;
    }
    i++;
}

```

# Recursion (factorial)

```
fn factorial(int n) {
    if (n == 1 || n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}

print "Factorial of 6 is:";
print factorial(6);
```


# Array

```
int sequence[5];
sequence[0] = 10;
sequence[1] = 20;
sequence[2] = 30;
sequence[3] = 40;
sequence[4] = 50;

int sum = 0;
int idx = 0;

while (idx < 5) {
    sum = sum + sequence[idx];
    idx++;
}

print "The sum of the array is:";
print sum;
```



# Function
```
fn formatUser(int id, int age, string role) {
    print "User Role:";
    print role;
    
    print "Combined ID and Age Metric:";
    return id + age;
}

int result = formatUser(404, 25, "Administrator");
print result;
```