// SymbolTable.cpp

#include "SymTab.h"
#include "Errors.h"
#include "stdafx.h"

/*
NAME

    SymbolTable::AddSymbol - Adds a new symbol to the symbol table.

SYNOPSIS

    void SymbolTable::AddSymbol(string& a_symbol, int a_loc)

DESCRIPTION

    This method adds a new symbol and its associated location to the symbol table.
    If the symbol already exists in the table, it is marked as multiply defined,
    and an error is recorded. The method also ensures that the order of symbols
    is maintained in a separate ordered list.
*/


// Add a new symbol to the symbol table.
void SymbolTable::AddSymbol(string& a_symbol, int a_loc)
{
    // Check if the symbol is already in the table
    if (m_symbolTable.find(a_symbol) != m_symbolTable.end()) {
        // Symbol is already defined, mark it as multiply defined
        m_symbolTable[a_symbol] = multiplyDefinedSymbol;
        Errors::RecordError("Symbol '" + a_symbol + "' is multiply defined.");
        // Update the order of symbols too
        for (int i = 0; i < m_orderedSymbols.size(); i++) {
            if (m_orderedSymbols[i].first == a_symbol) {
                m_orderedSymbols[i].second = multiplyDefinedSymbol;
                break;
            }
        }
    }
    else {
        // Add the new symbol and its location to the table
        m_symbolTable[a_symbol] = a_loc;
        m_orderedSymbols.push_back(make_pair(a_symbol, a_loc));
    }
}

/*
NAME

    SymbolTable::DisplaySymbolTable - Displays the symbol table.

SYNOPSIS

    void SymbolTable::DisplaySymbolTable() const

DESCRIPTION

    This method displays the contents of the symbol table in a formatted manner.
    It provides a header with column labels and lists all symbols along with their
    corresponding locations. If a symbol is marked as multiply defined, it indicates
    this status next to the symbol.
*/

// Display the symbol table.
void SymbolTable::DisplaySymbolTable() const
{
    // Header for the symbol table
    cout << "\nSymbol Table:\n";
    cout << "Symbol #\tSymbol\tLocation\n";
    cout << "--------------------------------------\n";

    // Make a copy of the symbols so we can sort them
    vector<pair<string, int>> sortedSymbols = m_orderedSymbols;

    // Sort the symbols alphabetically
    for (int i = 0; i < sortedSymbols.size(); i++) {
        for (int j = i + 1; j < sortedSymbols.size(); j++) {
            if (sortedSymbols[i].first > sortedSymbols[j].first) {
                pair<string, int> temp = sortedSymbols[i];
                sortedSymbols[i] = sortedSymbols[j];
                sortedSymbols[j] = temp;
            }
        }
    }

    // Print out the sorted symbols
    for (int i = 0; i < sortedSymbols.size(); i++) {
        cout << i << "\t\t" << sortedSymbols[i].first << "\t" << sortedSymbols[i].second;
        if (sortedSymbols[i].second == multiplyDefinedSymbol) {
            cout << " (Multiply Defined)";
        }
        cout << "\n";
    }
    cout << "--------------------------------------\n\n";
}

/*
NAME

    SymbolTable::GetSymbolTable - Retrieves the symbol table entries in order.

SYNOPSIS

    vector<pair<string, int>> SymbolTable::GetSymbolTable() const

DESCRIPTION

    This method returns the entries of the symbol table while maintaining their
    original order as they were added. Each entry is represented as a pair,
    where the first element is the symbol name (string) and the second element
    is the associated location (int).
*/

// Getter for symbol table entries maintaining order
vector<pair<string, int>> SymbolTable::GetSymbolTable() const
{
    // Just return the ordered symbols
    return m_orderedSymbols;
}

/*
NAME

    SymbolTable::LookupSymbol - Checks the existence and location of a symbol.

SYNOPSIS

    bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc) const

DESCRIPTION

    This method checks if a given symbol exists in the symbol table and retrieves
    its location if found. It also handles the case where the symbol is multiply
    defined or undefined.
*/

// Lookup a symbol in the symbol table.
bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc) const
{
    // Check if the symbol exists
    if (m_symbolTable.find(a_symbol) != m_symbolTable.end()) {
        // Check if it is multiply defined
        if (m_symbolTable.at(a_symbol) == multiplyDefinedSymbol) {
            Errors::RecordError("Symbol '" + a_symbol + "' is multiply defined.");
            return false;
        }
        // Get the location of the symbol
        a_loc = m_symbolTable.at(a_symbol);
        return true;
    }
    else {
        // Symbol is not found
        Errors::RecordError("Symbol '" + a_symbol + "' is undefined.");
        return false;
    }
}
