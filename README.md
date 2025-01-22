Final Grade: 98

This is the Final project (Mmn 14) in Laboratory C 20465- 2024B course at the Open University.
It's a comprehensive assembler program for a specific assembly language, written in C. 
The purpose is to simulate the operation of common system programs, and thus experience writing a large-scale software project.

The project consists of multiple C and header files, each serving a specific purpose in the program. 
The assembler works in three main stages:

1.Pre-Processing - The pre-assembler reads the source file line by line and identifies macro definitions. If a macro is identified, the lines of the macro are added to a macro list and replaced in the source file.

2.First Pass - The first pass identifies labels and assigns them numerical values representing their corresponding memory addresses.

3.Second Pass - In the second pass, the assembler generates the final machine code, replacing operations with their binary equivalents.

Input - files with the assembly lines with .as ending
Output - .ob file with the binary equivalents, .ent/.ext if there are, and .am file that is the file after the macro execute.

