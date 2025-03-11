//
//		Emulator class - supports the emulation of VC370 programs
//
#ifndef _EMULATOR_H      // A previous way of preventing multiple inclusions.
#define _EMULATOR_H

#include "stdafx.h"

class emulator {

public:

	const static int MEMSZ = 10'000;	// The size of the memory of the VC370.
	emulator() {

		memset(m_memory, 0, MEMSZ * sizeof(int));
		m_accum = 0;
	}
	// Records instructions and data into VC370 memory.
	bool insertMemory(int a_location, int a_contents)
	{
		if (a_location >= 0 && a_location < MEMSZ)
		{
			m_memory[a_location] = a_contents;
			return true;
		}
		else
		{
			cerr << "Error: Invalid memory location " << a_location << " for insertion." << endl;
			return false;
		}
	}

	// Runs the VC370 program recorded in memory.
	bool runProgram()
	{
		cout << "\nResults from emulating program:\n\n";

		int loc = 100; // Starting location
		while (true)
		{
			if (loc < 0 || loc >= MEMSZ)
			{
				cerr << "Error: Program counter out of bounds at location " << loc << "." << endl;
				return false;
			}

			int contents = m_memory[loc];
			int opcode = contents / 10000;
			int address = contents % 10000;

			switch (opcode)
			{
			case 5: // LOAD
				m_accum = m_memory[address];
				loc += 1;
				break;
			case 6: // STORE
				m_memory[address] = m_accum;
				loc += 1;
				break;
			case 7: // READ
				cout << "? ";
				cin >> m_memory[address];
				// Removed: cout << m_memory[address] << endl;
				loc += 1;
				break;
			case 8: // WRITE
				cout << m_memory[address] << endl;
				loc += 1;
				break;
			case 12: // BP (Branch if Positive)
				if (m_accum > 0)
				{
					loc = address;
				}
				else
				{
					loc += 1;
				}
				break;
			case 13: // HALT
				cout << "\nEnd of emulation" << endl;
				return true;
			default:
				cerr << "Illegal opcode " << opcode << " at location " << loc << "." << endl;
				return false;
			}
		}
	}

private:

	int m_memory[MEMSZ];    // The memory of the VC370.
	int m_accum;		    	// The accumulator for the VC370
};

#endif
