// Instruction.cpp
//
// Implementation of the Instruction class.
// Handles parsing of assembly instructions and provides relevant information.
//

#include "Instruction.h"
#include "Errors.h"
#include "stdafx.h"

/*
NAME

    Instruction::Instruction - Constructor for the Instruction class.

SYNOPSIS

    Instruction::Instruction()

DESCRIPTION

    This constructor initializes an Instruction object by setting its default values:
    - m_NumOpCode is set to 0 to represent no opcode initially.
    - m_type is set to ST_Invalid to indicate the instruction is not yet parsed or valid.
    - m_IsNumericOperand is set to false, indicating the operand is not numeric by default.
    - m_OperandNumValue is set to 0 as the default numeric value for the operand.

    These default settings prepare the Instruction object for use in parsing and processing assembly instructions.

*/

// Constructor
Instruction::Instruction()
{
    m_NumOpCode = 0;
    m_type = ST_Invalid;
    m_IsNumericOperand = false;
    m_OperandNumValue = 0;
}

/*
NAME

    Instruction::~Instruction - Destructor for the Instruction class.

SYNOPSIS

    Instruction::~Instruction()

DESCRIPTION

    This destructor is defined to clean up resources associated with an Instruction object. Since no dynamic
    memory allocation is performed in this class, the destructor does not contain any specific cleanup logic.
    It provides a placeholder for future expansion if the class is modified to include dynamically allocated resources.

*/

// Destructor
Instruction::~Instruction()
{
}

/*
NAME

    trim - Helper function to remove leading and trailing whitespace.

SYNOPSIS

    static std::string trim(const std::string &str)
        const std::string &str --> The input string to be trimmed.

DESCRIPTION

    This function removes any leading and trailing whitespace characters (including spaces, tabs, carriage
    returns, and newline characters) from the input string. If the input string contains only whitespace,
    an empty string is returned.

RETURNS

    std::string - A new string with leading and trailing whitespace removed.

*/

// Helper function to trim whitespace
static string trim(const string& str) {
    // Find the first and last non-whitespace characters.
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");

    // If no non-whitespace characters are found, return an empty string.
    if (start == string::npos || end == string::npos) {
        return "";
    }

    // Return the trimmed string.
    return str.substr(start, end - start + 1);
}

/*
NAME

    Instruction::ParseInstruction - Parse a line of assembly code to extract details.

SYNOPSIS

    Instruction::InstructionType Instruction::ParseInstruction(std::string a_buff)
        std::string a_buff --> Unprocessed line of code from the file.

DESCRIPTION

    This function parses a line of assembly code to determine its type (comment, end, assembler instruction,
    or machine instruction). It extracts relevant details such as the label, opcode, and operand, and stores
    them in the corresponding member variables of the Instruction object.

    The function also removes comments and trims whitespace from the line to process it accurately. If the
    operand is numeric, it sets a flag and records its numeric value.

RETURNS

    Instruction::InstructionType - Indicates the type of instruction being executed in the parsed line.
*/

// Parse a line of assembly code
Instruction::InstructionType Instruction::ParseInstruction(string a_buff)
{
    // Clear all previous data for a fresh start.
    m_Label = "";
    m_OpCode = "";
    m_Operand = "";
    m_instruction = a_buff;
    m_NumOpCode = 0;
    m_type = ST_Invalid;
    m_IsNumericOperand = false;
    m_OperandNumValue = 0;

    // Remove any comments from the line.
    size_t commentPos = a_buff.find(';');
    string line = (commentPos != string::npos) ? a_buff.substr(0, commentPos) : a_buff;

    // Trim spaces from the line.
    line = trim(line);

    // If the line is empty, it's a comment.
    if (line.empty()) {
        m_type = ST_Comment;
        return m_type;
    }

    // Use a stringstream to split the line into parts.
    istringstream iss(line);
    string firstToken, secondToken, thirdToken;

    iss >> firstToken;

    // List of valid opcodes.
    vector<string> opcodes = { "READ", "LOAD", "STORE", "WRITE", "BP", "HALT", "ORG", "DC", "DS", "END" };

    // Convert the first token to uppercase for comparison.
    string firstTokenUpper = firstToken;
    transform(firstTokenUpper.begin(), firstTokenUpper.end(), firstTokenUpper.begin(), ::toupper);

    // Check if the first token is an opcode.
    if (find(opcodes.begin(), opcodes.end(), firstTokenUpper) != opcodes.end()) {
        // It's an opcode. Determine the type of instruction.
        if (firstTokenUpper == "END") {
            m_type = ST_End;
        }
        else if (firstTokenUpper == "ORG" || firstTokenUpper == "DC" || firstTokenUpper == "DS") {
            m_type = ST_AssemblerInstr;
        }
        else {
            m_type = ST_MachineLanguage;
        }
        m_OpCode = firstTokenUpper;

        // Check if there's an operand.
        if (iss >> secondToken) {
            m_Operand = secondToken;
            try {
                // Try to convert the operand to a numeric value.
                m_OperandNumValue = stoi(m_Operand);
                m_IsNumericOperand = true;
            }
            catch (...) {
                // If conversion fails, the operand is not numeric.
                m_IsNumericOperand = false;
            }
        }
        return m_type;
    }

    // If the first token is not an opcode, treat it as a label.
    m_Label = firstToken;
    if (!(iss >> secondToken)) {
        // If no opcode follows the label, it's invalid.
        m_type = ST_Invalid;
        Errors::RecordError("Missing opcode after label: " + m_Label);
        return m_type;
    }

    // Process the second token as the opcode.
    string secondTokenUpper = secondToken;
    transform(secondTokenUpper.begin(), secondTokenUpper.end(), secondTokenUpper.begin(), ::toupper);
    m_OpCode = secondTokenUpper;

    if (m_OpCode == "END") {
        m_type = ST_End;
    }
    else if (m_OpCode == "ORG" || m_OpCode == "DC" || m_OpCode == "DS") {
        m_type = ST_AssemblerInstr;
    }
    else {
        m_type = ST_MachineLanguage;
    }

    // Check if there's an operand.
    if (iss >> thirdToken) {
        m_Operand = thirdToken;
        try {
            // Try to convert the operand to a numeric value.
            m_OperandNumValue = stoi(m_Operand);
            m_IsNumericOperand = true;
        }
        catch (...) {
            // If conversion fails, the operand is not numeric.
            m_IsNumericOperand = false;
        }
    }

    return m_type;
}

/*
NAME

    Instruction::LocationNextInstruction - Compute the next instruction's memory location.

SYNOPSIS

    int Instruction::LocationNextInstruction(int a_loc)
        int a_loc --> The current memory location of the instruction.

DESCRIPTION

    This function calculates the memory location of the next instruction based on the type of the current
    instruction and its specific opcode. For machine language instructions and DC directives, the next
    location is incremented by one. For DS directives, the operand specifies the amount to increment.

    For ORG directives, the operand is interpreted as the new memory location. If the operand is invalid,
    an error is recorded, and a default increment is applied.

RETURNS

    int - The memory location of the next instruction.
*/

// Compute next instruction location
int Instruction::LocationNextInstruction(int a_loc)
{
    // Check the type of the instruction.
    if (m_type == ST_MachineLanguage) {
        return a_loc + 1; // Machine language instructions take 1 memory slot.
    }
    else if (m_type == ST_AssemblerInstr) {
        if (m_OpCode == "DC") {
            return a_loc + 1; // DC reserves one memory slot.
        }
        else if (m_OpCode == "DS") {
            try {
                int size = stoi(m_Operand); // Parse the size operand for DS.
                return a_loc + size; // DS reserves multiple slots based on size.
            }
            catch (...) {
                Errors::RecordError("Invalid size for DS at location: " + to_string(a_loc));
                return a_loc + 1; // Default to one slot if parsing fails.
            }
        }
        else if (m_OpCode == "ORG") {
            try {
                int newLoc = stoi(m_Operand); // Parse the operand to set new location.
                return newLoc; // Set the location counter to the operand value.
            }
            catch (...) {
                Errors::RecordError("Invalid operand for ORG at location: " + to_string(a_loc));
                return a_loc + 1; // Default to next slot if parsing fails.
            }
        }
    }
    return a_loc; // Default: location remains unchanged.
}


/*
NAME

    Instruction::GetLabel - Retrieve the label of the instruction.

SYNOPSIS

    std::string& Instruction::GetLabel()

DESCRIPTION

    This function returns a reference to the label of the current instruction. The label is typically
    used to identify a memory location or symbolic reference in assembly code.

RETURNS

    std::string& - A reference to the label of the instruction.
*/

// Accessor methods
string& Instruction::GetLabel() {
    return m_Label;
}

/*
NAME

    Instruction::isLabel - Check if the instruction contains a label.

SYNOPSIS

    bool Instruction::isLabel() const

DESCRIPTION

    This function determines whether the current instruction contains a label. A label is considered
    present if the m_Label member variable is not empty.

RETURNS

    bool - True if the instruction contains a label, false otherwise.
*/

bool Instruction::isLabel() const {
    return !m_Label.empty();
}

/*
NAME

    Instruction::GetOpCode - Retrieve the opcode of the instruction.

SYNOPSIS

    std::string Instruction::GetOpCode() const

DESCRIPTION

    This function returns the opcode of the current instruction. The opcode represents the operation
    to be performed by the instruction, typically stored in the m_OpCode member variable.

RETURNS

    std::string - A string representing the opcode of the instruction.
*/


string Instruction::GetOpCode() const {
    return m_OpCode;
}

/*
NAME

    Instruction::GetOperand - Retrieve the operand of the instruction.

SYNOPSIS

    std::string Instruction::GetOperand() const

DESCRIPTION

    This function returns the operand of the current instruction. The operand is an argument
    or data value associated with the instruction, typically stored in the m_Operand member variable.

RETURNS

    std::string - A string representing the operand of the instruction.
*/

string Instruction::GetOperand() const {
    return m_Operand;
}

/*
NAME

    Instruction::IsNumericOperand - Check if the operand is numeric.

SYNOPSIS

    bool Instruction::IsNumericOperand() const

DESCRIPTION

    This function determines whether the operand of the current instruction is numeric. It returns
    true if the operand can be interpreted as a numeric value, and false otherwise. The numeric
    status is stored in the m_IsNumericOperand member variable.

RETURNS

    bool - True if the operand is numeric, false otherwise.
*/

bool Instruction::IsNumericOperand() const {
    return m_IsNumericOperand;
}

/*
NAME

    Instruction::GetOperandNumValue - Retrieve the numeric value of the operand.

SYNOPSIS

    int Instruction::GetOperandNumValue() const

DESCRIPTION

    This function returns the numeric value of the operand for the current instruction. If the
    operand is not numeric, the value stored in m_OperandNumValue will typically be zero or undefined.
    This function is used when numeric operations or validations are required.

RETURNS

    int - The numeric value of the operand.
*/

int Instruction::GetOperandNumValue() const {
    return m_OperandNumValue;
}
