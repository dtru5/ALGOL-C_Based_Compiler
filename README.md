Overview

This project implements a MIPS code generator for ALGOL-like language constructs using an Abstract Syntax Tree (AST). The compiler is developed in stages to gradually build up complexity and provide a strong learning foundation in compiler construction and code generation.

The core of the project is housed in the lab9 folder, which contains the complete compiler pipeline—from lexical analysis and parsing (using LEX and YACC) to AST construction and MIPS code emission.
Purpose

The main goal is to generate MIPS assembly code for ALGOL constructs while reinforcing compiler design principles and gaining hands-on experience with:

    Abstract Syntax Trees (ASTs)

    Activation records

    Code emission for control flow (jumps, labels, directives)

    MIPS header setup and data definitions (scalars, arrays, strings)

    Input/output handling and expression evaluation

Features

    Command-line interface to specify output files

    Support for basic ALGOL constructs including:

        Variable declarations

        Arithmetic expressions

        Assignments

        Conditional statements (if)

        Loops (while)

        Arrays (simplified handling)

    AST traversal for code emission (emit.c)

    Modular structure encouraging documentation and clear function-level understanding

Usage

Compilation

- cd lab9
- make

Running the Compiler

To compile an .al ALGOL source file and generate MIPS assembly:

- ./lab9 -o output.asm < program.al

Running the Generated MIPS Code

To run the resulting MIPS assembly using the MARS simulator:
- java -jar Mars4_5.jar sm output.asm

Note: Ensure Mars4_5.jar is in your working directory or provide the correct path, but is included anyways into lab9 directory

Project Structure

    lab9.l – Lexical analyzer

    lab9.y – YACC grammar for ALGOL

    ast.c/h – Abstract Syntax Tree construction

    emit.c – MIPS code generation

    main.c – Main driver with command-line handling

    lab9/ – Folder containing the complete compiler

Learning Objectives

    Understand and implement a multi-stage compiler pipeline

    Translate high-level constructs into low-level MIPS code

    Practice structured compiler design and modular development

    Gain familiarity with activation records and memory layout in MIPS
