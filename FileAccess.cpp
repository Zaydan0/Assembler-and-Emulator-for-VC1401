//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/*
NAME

    FileAccess::FileAccess - Constructor for FileAccess that opens the file and performs error checking.

SYNOPSIS

    FileAccess::FileAccess(int argc, char *argv[])
        int argc   --> The number of command-line arguments provided.
        char *argv[] --> An array of strings representing the command-line arguments, including the file location.

DESCRIPTION

    This constructor opens the specified file for reading and ensures that exactly one runtime parameter is provided.
    It performs basic error checking to validate the correctness of the command-line arguments passed to the program.

*/


// Don't forget to comment the function headers.
FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}

/*
NAME

    FileAccess::~FileAccess - Destructor for the FileAccess class.

SYNOPSIS

    FileAccess::~FileAccess()

DESCRIPTION

    This destructor ensures that the file stream associated with FileAccess is properly closed. While the file
    would typically be closed automatically when the program terminates, explicitly closing the file is good
    practice and reinforces proper resource management.

*/

FileAccess::~FileAccess( )
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close( );
}

/*
NAME

    FileAccess::GetNextLine - Retrieve the next line from the file.

SYNOPSIS

    bool FileAccess::GetNextLine(std::string &a_buff)
        std::string &a_buff --> Reference to a string where the next line will be stored.

DESCRIPTION

    This function reads the next line from the file and stores it in the provided string buffer. If the end
    of the file has been reached, the function returns false. Otherwise, it successfully retrieves the next
    line and returns true.

RETURNS

    bool - True if a line was successfully retrieved, false if the end of the file was reached.

*/

// Get the next line from the file.
bool FileAccess::GetNextLine( string &a_buff )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_buff );
    
    // Return indicating success.
    return true;
}

/*
NAME

    FileAccess::rewind - Reset the file to the beginning.

SYNOPSIS

    void FileAccess::rewind()

DESCRIPTION

    This function resets the file stream to the beginning of the file. It clears any error flags that
    may have been set on the file stream and then seeks the file position back to the start.
    This ensures the file is ready for re-reading from the beginning.

*/

void FileAccess::rewind( )
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
    
