#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

/*
NAME

    Assembler::Assembler - Constructor for the Assembler class.

SYNOPSIS

    Assembler::Assembler(int argc, char* argv[])
        int argc        --> Number of command-line arguments provided.
        char* argv[]    --> Array of strings representing the command-line arguments, including the file location.

        NOTE: argc and argv are passed to the FileAccess constructor to initialize file operations.

DESCRIPTION

    This constructor initializes the Assembler object and passes the command-line arguments
    to the FileAccess class to handle file opening and processing.

*/

// Constructor
Assembler::Assembler(int argc, char* argv[])
    : m_facc(argc, argv), m_emul() {
    Errors::InitErrorReporting(); // Initialize error reporting system
}

/*
NAME

    Assembler::~Assembler - Destructor for the Assembler class.

SYNOPSIS

    Assembler::~Assembler()

DESCRIPTION

    This destructor ensures that any recorded errors during the assembly process are displayed by calling
    the Errors::DisplayErrors function. It provides a clean and informative conclusion to the assembly
    process, showing all accumulated errors if present.

*/

// Destructor
Assembler::~Assembler() {
    Errors::DisplayErrors(); // Display any errors that were recorded
}

/*
NAME

    Assembler::PassI - Determine the locations of labels.

SYNOPSIS

    void Assembler::PassI()

DESCRIPTION

    This function identifies and records the memory locations of all labels in the input text file.
    It achieves this by parsing each instruction into its components and verifying the presence of a label on the line.

*/

// Pass I - Establish the locations of the symbols
void Assembler::PassI() {
    int loc = 0; // Location counter

    while (true) {
        string line;
        if (!m_facc.GetNextLine(line)) {
            Errors::RecordError("Missing END directive."); // Record error if END is missing
            break;
        }

        Instruction::InstructionType instType = m_inst.ParseInstruction(line); // Parse the instruction

        IntermediateInstruction interm; // Create an intermediate representation for this line
        interm.type = instType;
        interm.label = m_inst.isLabel() ? m_inst.GetLabel() : ""; // Get label if present
        interm.opcode = m_inst.GetOpCode();
        interm.operand = m_inst.GetOperand();
        interm.location = loc;
        interm.originalLine = line; // Store original line for reference

        m_intermediate.push_back(interm); // Save the parsed line

        if (instType == Instruction::ST_Invalid) continue; // Skip invalid instructions
        if (instType == Instruction::ST_End) break; // Stop at END directive
        if (instType == Instruction::ST_Comment) continue; // Skip comments

        if (instType == Instruction::ST_AssemblerInstr) {
            if (m_inst.isLabel()) {
                m_symtab.AddSymbol(m_inst.GetLabel(), loc); // Add label to the symbol table
            }

            if (m_inst.GetOpCode() == "ORG") {
                try {
                    loc = stoi(m_inst.GetOperand()); // Set location to operand value
                }
                catch (...) {
                    Errors::RecordError("Invalid operand for ORG directive."); // Handle invalid operand
                }
                continue;
            }
            else if (m_inst.GetOpCode() == "DC" || m_inst.GetOpCode() == "DS") {
                loc = m_inst.LocationNextInstruction(loc); // Update location for data storage
                continue;
            }
            else {
                Errors::RecordError("Unknown assembler instruction: " + m_inst.GetOpCode()); // Handle unknown instructions
                continue;
            }
        }

        if (instType == Instruction::ST_MachineLanguage) {
            if (m_inst.isLabel()) {
                m_symtab.AddSymbol(m_inst.GetLabel(), loc); // Add label to the symbol table
            }
            loc = m_inst.LocationNextInstruction(loc); // Update location for machine language instruction
        }
    }
}

/*
NAME

    Assembler::PassII - Perform error checking, translation, and memory preparation.

SYNOPSIS

    Assembler::PassII()

DESCRIPTION

    This function performs a second pass through the input file, translating assembly instructions into
    machine code, preparing data for emulation, and outputting formatted results for the user. During this pass:

    - Each instruction is parsed, and its type is identified (e.g., assembler directive, machine language, comment).
    - Assembler directives (e.g., ORG, DC, DS) are processed and converted into memory representations.
    - Machine language instructions are translated into machine code, and errors such as undefined symbols or
      unknown opcodes are reported.
    - The translated instructions and data are inserted into the emulator's memory for execution.
    - The function outputs a formatted table displaying memory locations, generated contents, and original source lines.
    - Errors and warnings encountered during the translation process are recorded and reported.

    This function ensures the program is ready for execution by the emulator.

*/

// Pass II - Generate a translation
void Assembler::PassII() {
    unordered_map<string, int> opcodeMap = {
        {"READ", 7}, {"LOAD", 5}, {"STORE", 6},
        {"WRITE", 8}, {"BP", 12}, {"HALT", 13}
    };

    cout << "\nTranslation of Program:\n\n";
    cout << left << setw(12) << "Location" << setw(12) << "Contents" << "Original Statement\n";
    cout << "-------------------------------------------------------------\n";

    for (const auto& interm : m_intermediate) {
        if (interm.type == Instruction::ST_Invalid) continue; // Skip invalid instructions

        if (interm.type == Instruction::ST_End) {
            cout << setw(12) << "" << setw(12) << "" << interm.originalLine << "\n";
            continue;
        }

        if (interm.type == Instruction::ST_Comment) {
            cout << setw(36) << "" << interm.originalLine << "\n";
            continue;
        }

        if (interm.type == Instruction::ST_AssemblerInstr) {
            if (interm.opcode == "ORG") {
                cout << setw(12) << interm.location << setw(12) << "" << interm.originalLine << "\n";
            }
            else if (interm.opcode == "DC") {
                try {
                    int value = stoi(interm.operand); // Parse operand value
                    stringstream ss;
                    ss << setw(6) << setfill('0') << value; // Format the value
                    cout << setw(12) << interm.location << setw(12) << ss.str() << interm.originalLine << "\n";
                    m_emul.insertMemory(interm.location, value); // Insert value into memory
                }
                catch (...) {
                    Errors::RecordError("Invalid operand for DC directive."); // Handle invalid operand
                }
            }
            else if (interm.opcode == "DS") {
                cout << setw(12) << interm.location << setw(12) << "" << interm.originalLine << "\n";
            }
            continue;
        }

        if (interm.type == Instruction::ST_MachineLanguage) {
            auto it = opcodeMap.find(interm.opcode);
            if (it != opcodeMap.end()) {
                int machineOpcode = it->second;
                int operandAddr = 0;

                if (!interm.operand.empty() && !m_symtab.LookupSymbol(interm.operand, operandAddr)) {
                    Errors::RecordError("Undefined symbol: " + interm.operand); // Handle undefined symbols
                }

                int machineCode = machineOpcode * 10000 + operandAddr; // Calculate machine code

                stringstream ss;
                ss << setw(6) << setfill('0') << machineCode; // Format machine code

                cout << setw(12) << interm.location << setw(12) << ss.str() << interm.originalLine << "\n";
                m_emul.insertMemory(interm.location, machineCode); // Insert machine code into memory
            }
            else {
                Errors::RecordError("Unknown opcode: " + interm.opcode); // Handle unknown opcode
            }
        }
    }
    cout << "-------------------------------------------------------------\n\nPress Enter to continue...\n";
    cin.get(); // Pause for user input
}

/*
NAME

    Assembler::DisplaySymbolTable - Display the symbol table.

SYNOPSIS

    void Assembler::DisplaySymbolTable() const

DESCRIPTION

    This function displays the contents of the symbol table, which includes all labels
    and their corresponding memory locations. It delegates the actual display logic
    to the SymbolTable class's DisplaySymbolTable method.

    The symbol table provides a comprehensive view of the program's labels and their
    locations, useful for debugging and verification of the assembly process.

*/

// Display the symbols in the symbol table.
void Assembler::DisplaySymbolTable() const {
    m_symtab.DisplaySymbolTable(); // Call the SymbolTable's display function
}

/*
NAME

    Assembler::RunProgramInEmulator - Execute the program in the emulator.

SYNOPSIS

    void Assembler::RunProgramInEmulator()

DESCRIPTION

    This function attempts to run the assembled program in the emulator.
    It first checks for errors in the assembly process using the Errors module.
    If no errors are detected, it invokes the emulator's runProgram method to
    execute the translated instructions.

    If the emulator encounters an error during execution, a message is displayed
    indicating the issue. Similarly, if the assembly process encountered errors,
    the emulator will not run, and an appropriate message is displayed.

*/

// Run the program in the emulator
void Assembler::RunProgramInEmulator() {
    if (!Errors::WasThereErrors()) {
        if (!m_emul.runProgram()) {
            cout << "Emulator encountered an error." << endl; // Report emulator error
        }
    }
    else {
        cout << "Cannot run emulator due to errors." << endl; // Report assembly errors
    }
}
