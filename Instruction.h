// Instruction.h
//
// Class to parse and provide information about instructions.
// Note: You will be adding more functionality.
//
#pragma once

#include "stdafx.h"

class Instruction {

public:

    Instruction();             // Constructor: Initializes an Instruction object.
    ~Instruction();            // Destructor: Cleans up an Instruction object.

    // Codes to indicate the type of instruction we are processing.
    enum InstructionType {
        ST_Invalid,             // Invalid instruction.
        ST_MachineLanguage,     // A machine language instruction.
        ST_AssemblerInstr,      // Assembler Language instruction.
        ST_Comment,             // Comment or blank line.
        ST_End                  // End instruction.
    };

    // Parses the given instruction string and identifies its type.
    InstructionType ParseInstruction(string a_buff);

    // Computes the memory location of the next instruction based on the current location.
    int LocationNextInstruction(int a_loc);

    // Accessors
    string& GetLabel();                // Retrieves the label of the instruction.
    bool isLabel() const;              // Checks if the instruction contains a label.
    string GetOpCode() const;          // Retrieves the operation code.
    string GetOperand() const;         // Retrieves the operand.
    bool IsNumericOperand() const;     // Checks if the operand is numeric.
    int GetOperandNumValue() const;    // Retrieves the numeric value of the operand if applicable.

private:

    // Helper method to parse and extract label, opcode, and operand from an instruction string.
    void GetLabelOpcodeEtc(const string& a_buff);

    // The components of an instruction.
    string m_Label;         // The label part of the instruction, if any.
    string m_OpCode;        // The symbolic operation code.
    string m_Operand;       // The operand of the instruction.

    string m_instruction;   // The original instruction string.

    // Derived values.
    int m_NumOpCode;             // The numerical equivalent of the operation code (for machine instructions).
    InstructionType m_type;      // The type/category of the instruction.

    bool m_IsNumericOperand;     // True if the operand is a numeric value.
    int m_OperandNumValue;       // The numeric value of the operand, if applicable.
};
