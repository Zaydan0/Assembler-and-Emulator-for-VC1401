// Errors.h
//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>
#include <vector>
using namespace std;

class Errors {

public:

    // Initializes error reports.
    static void InitErrorReporting();

    // Records an error message.
    static void RecordError(const string a_emsg);

    static bool WasThereErrors();

    // Displays the collected error messages.
    static void DisplayErrors();

private:
    //vector to store error strings.
    static vector<string> m_ErrorMsgs;
    //bool that keeps track if there was an error for a line of the file.
    static bool m_WasErrorMessages;
};

#endif
