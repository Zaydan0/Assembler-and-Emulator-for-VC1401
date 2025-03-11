//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"
#include "stdafx.h"

// Struct to hold the intermediate representation of each line of assembly code.
struct IntermediateInstruction {
    Instruction::InstructionType type; // The type of instruction (e.g., machine language, assembler directive).
    string label;                     // Label associated with the instruction, if any.
    string opcode;                    // The operation code of the instruction.
    string operand;                   // Operand for the instruction, if any.
    int location;                     // Memory location of the instruction.
    string originalLine;              // The original line of assembly code for reference.
};

class Assembler {

public:
    Assembler(int argc, char* argv[]); // Constructor: Initialize the assembler with command-line arguments.
    ~Assembler();                     // Destructor: Clean up resources used by the assembler.

    // Pass I - Analyze the assembly file to determine symbol locations.
    void PassI();

    // Pass II - Convert assembly instructions into machine code.
    void PassII();

    // Display the contents of the symbol table (useful for debugging).
    void DisplaySymbolTable() const;

    // Run the translated program using the emulator to verify functionality.
    void RunProgramInEmulator();

private:
    FileAccess m_facc;  // File Access object
    SymbolTable m_symtab;   // Symbol table object
    Instruction m_inst; //Instruction object
    emulator m_emul;   // Emulator object
    vector<IntermediateInstruction> m_intermediate; // Stores the intermediate representation of the assembly program.
};
