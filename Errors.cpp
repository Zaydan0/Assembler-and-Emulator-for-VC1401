// Errors.cpp
//
// Implementation of the Errors class.
// Manages error reporting by collecting and displaying error messages.
//
#include "Errors.h"
#include "stdafx.h"

using namespace std;

// Initialize static members
vector<string> Errors::m_ErrorMsgs; // List to store error messages.
bool Errors::m_WasErrorMessages = false; // Flag to indicate if errors were recorded.

/*
NAME

    Errors::InitErrorReporting - Initialize the error reporting system.

SYNOPSIS

    void Errors::InitErrorReporting()

DESCRIPTION

    This function initializes the error reporting system by clearing the vector that stores error messages
    and resetting the boolean flag m_WasErrorMessages to false. It ensures that any previous errors are cleared,
    preparing the system for a new assembly process.

*/

// Initializes error reports.
void Errors::InitErrorReporting() {
    // Clear the error messages list.
    if (!m_ErrorMsgs.empty()) {
        m_ErrorMsgs.clear(); // Remove all existing error messages.
    }
    // Reset the error flag to indicate no errors yet.
    m_WasErrorMessages = false;
}

/*
NAME

    Errors::RecordError - Record an error message.

SYNOPSIS

    void Errors::RecordError(const string a_emsg)
        const string a_emsg --> The error message to be recorded.

DESCRIPTION

    This function adds a given error message to the vector of error messages and sets the boolean flag
    m_WasErrorMessages to true, indicating that at least one error has been recorded.

*/

// Records an error message.
void Errors::RecordError(const string a_emsg) {
    // Add the error message to the list.
    m_ErrorMsgs.push_back(a_emsg);
    // Set the error flag to true to indicate an error was recorded.
    m_WasErrorMessages = true;
}

/*
NAME

    Errors::WasThereErrors - Check for recorded errors.

SYNOPSIS

    bool Errors::WasThereErrors()

DESCRIPTION

    This function checks if any errors have been recorded during the assembly process. It returns the value
    of the boolean flag m_WasErrorMessages, which is set to true if at least one error has been recorded,
    and false otherwise.

RETURNS

    bool - True if there are recorded errors, false otherwise.

*/

// Checks if there were any errors.
bool Errors::WasThereErrors() {
    // Return the value of the error flag.
    return m_WasErrorMessages;
}

/*
NAME

    Errors::DisplayErrors - Display the collected error messages.

SYNOPSIS

    void Errors::DisplayErrors()

DESCRIPTION

    This function displays all error messages that were recorded during the assembly process. If errors
    are present, they are output to the console, prefixed with a description to indicate they are assembly errors.

    After displaying the messages, this function clears the vector of error messages and resets the boolean flag
    m_WasErrorMessages to false, preparing the system for subsequent assembly runs.

    If no errors are found, a message is displayed indicating that the assembly process encountered no issues.

*/


// Displays the collected error messages.
void Errors::DisplayErrors() {
    // Check if there are any errors.
    if (m_WasErrorMessages) {
        cout << "Assembler encountered the following errors:" << endl;

        // Iterate through the error messages list and display each message.
        for (int i = 0; i < m_ErrorMsgs.size(); i++) {
            cout << "- " << m_ErrorMsgs[i] << endl;
        }

        // Clear the error messages after displaying.
        for (int i = 0; i < m_ErrorMsgs.size(); i++) {
            m_ErrorMsgs[i] = ""; // Reset each message (not necessary but ensures no leftover data).
        }
        m_ErrorMsgs.clear(); // Remove all messages from the list.
        m_WasErrorMessages = false; // Reset the error flag.
    }
    else {
        // No errors to display.
        cout << "" << endl;
    }
}
