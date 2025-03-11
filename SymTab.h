// SymbolTable.h

#pragma once

#include "stdafx.h"

class SymbolTable {

public:
    SymbolTable() = default;
    ~SymbolTable() = default;

    const int multiplyDefinedSymbol = -999;
    const int m_INVALIDSYMBOL = -998;

    // Add a new symbol to the symbol table.
    void AddSymbol(string& a_symbol, int a_loc);

    // Display the symbol table.
    void DisplaySymbolTable() const;

    // Lookup a symbol in the symbol table.
    bool LookupSymbol(const string& a_symbol, int& a_loc) const;

    // Getter for symbol table entries maintaining order
    vector<pair<string, int>> GetSymbolTable() const;

private:
    // The actual symbol table. The symbol is the key to the map.
    map<string, int> m_symbolTable;
    vector<pair<string, int>> m_orderedSymbols; // To maintain insertion order
};