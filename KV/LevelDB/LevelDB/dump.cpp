//
// HiperDrop (0.0.2) process memory downloader tool
// code by gynvael.coldwind//vx
// mailto: gynvael@coldwind.pl
// www   : http://gynvael.coldwind.pl/
//
// LICENSE
// Copyright (C) 2012 Gynvael Coldwind
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// TODO List:
// * all the things marked as TODO in this file :)
// * kernel mode memory fetch
// * 64-bit mode
// * memory fetch from the inside
// * add flush-after-each-write option (ehm, really ?)
// * get rights to access process (DACL) (only needed if the process denies access)
// * what about re-mappable memory (from user space) like 
//   1. CreateFileMapping / MapViewOfFile
//   2. AllocateUserPhysicalPages / MapUserPhysicalPages
//   3. other (NUMA?)
// * add memory block information to the map file (access, type, etc)
// * calc entropy of each page or/and block
// * fix comment style, line size to 80 chars

//
// Includes and namespace
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <windows.h>
#include <winioctl.h>
#include <list>
#include <io.h>
#include <errno.h>
#include "stdafx.h"

using namespace std;

//
// Defines
//

#define HIPERDROP_VER "0.0.2"

//
// Structures, Enums, and Global Variables
//
enum AccessMethodEnum
{
	OPENPROCESS_METHOD,
	DEBUGGER_API_METHOD
	// TODO KERNEL_LEVEL_METHOD
	// TODO INSIDER_METHOD
};

enum DumpMethodEnum
{
	MULTIPLE_FILE_METHOD,
	ONE_FILE_METHOD,
	ONE_BIG_FILE_METHOD
};

enum AcquireMethodEnum
{
	VIRTUALQUERYEX_METHOD,
	READPROCESSMEMORY_METHOD
};

// Struct
struct MemoryDesc
{
	DWORD VirtualAddress;
	DWORD Size;
	DWORD RawOffset;
};

struct WorkStruct
{
	// Access, acquisition and dump methods
	enum AccessMethodEnum  AccessMethod;
	enum AcquireMethodEnum AcquireMethod;
	enum DumpMethodEnum    DumpMethod;

	// Additional options.

	// Should sparse files be used? Used only with ONE_BIG_FILE_METHOD.
	// True by default (use -n to disable).
	bool UseSparseFiles;

	// PID
	DWORD TargetPID;

	// Handles, structures, other stuff
	HANDLE hProcess;
	DEBUG_EVENT DebugEvent;

	// Write stuff
	DWORD LastOffset;
	FILE *f;
	list<MemoryDesc> MemoryList;

	// Other
	HANDLE hFileCache; // This is used only for setting sparse sections.
	bool Is4GT; // Is 4GB tuning enabled?
};

// For some reason MinGW platform API doesn't have these declarations.
#ifdef SPARSE_NOT_DECLARED
typedef struct _FILE_ZERO_DATA_INFORMATION {
	LARGE_INTEGER FileOffset;
	LARGE_INTEGER BeyondFinalZero;
} FILE_ZERO_DATA_INFORMATION, *PFILE_ZERO_DATA_INFORMATION;
#define FSCTL_SET_ZERO_DATA CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 50, METHOD_BUFFERED, FILE_WRITE_DATA)
#endif

// Default methods
enum AccessMethodEnum  AccessMethod = OPENPROCESS_METHOD;
enum DumpMethodEnum    DumpMethod = MULTIPLE_FILE_METHOD;
enum AcquireMethodEnum AcquireMethod = VIRTUALQUERYEX_METHOD;
DWORD TargetPID = 0;

//
// Function declarations
//
void ShowHelp();
bool HandleArgs(WorkStruct *WorkDescriptor, int argc, char **argv);
bool HandleAccessProcess(WorkStruct *WorkDescriptor);
bool HandleDataReadWrite(WorkStruct *WorkDescriptor);
bool HandleDetachProcess(WorkStruct *WorkDescriptor);
bool HandleFinalizeWrite(WorkStruct *WorkDescriptor);
bool AcquirePrivilege(const char *PrivilegeName, bool Enable);
bool CanUseSparseFiles();

// Write functions
bool WriteDataOneFile(WorkStruct *WorkDescriptor, DWORD VirtualAddress, BYTE *Data, SIZE_T Size, bool FillSpace, bool Sparse);
bool WriteDataMultipleFile(WorkStruct *WorkDescriptor, DWORD VirtualAddress, BYTE *Data, SIZE_T Size);

//
// Function definitions
//
int
main(int argc, char **argv)
{
	// Display banner
	puts("HiperDrop 0.0.1 by gynvael.coldwind//vx\n"
		"Visit http://gynvael.coldwind.pl for more info...");

	// Setup default stuff
	static WorkStruct WorkDescriptor;
	WorkDescriptor.AccessMethod = OPENPROCESS_METHOD;    // Default Access Method
	WorkDescriptor.AcquireMethod = VIRTUALQUERYEX_METHOD; // Default Acquisition Method
	WorkDescriptor.DumpMethod = MULTIPLE_FILE_METHOD;  // Default Dump Method
	WorkDescriptor.UseSparseFiles = true;                  // Use by default

	// Handle args
	if (!HandleArgs(&WorkDescriptor, argc, argv))
		return 1;

	// Check if partition handles sparse files (in case this is needed).
	if (WorkDescriptor.DumpMethod == ONE_BIG_FILE_METHOD &&
		WorkDescriptor.UseSparseFiles &&
		!CanUseSparseFiles())
	{
		fprintf(stderr, "Error: Your partition doesn't seem to support sparse files\n"
			"     : Make sure the cwd is on an NTFS partition or use the -n option\n");
		return false;
	}

	// Try to access the process
	if (!HandleAccessProcess(&WorkDescriptor))
		return 2;

	// Get the data and write to file
	if (!HandleDataReadWrite(&WorkDescriptor))
		return 3;

	// Finalize write and save database
	if (!HandleFinalizeWrite(&WorkDescriptor))
		return 4;

	// Detach or close process
	if (!HandleDetachProcess(&WorkDescriptor))
		return 5;

	// Done!
	return 0;
}

// Function: HandleArgs
// Params  : int    argc - Argument count
//         : char** argv - Argument list
// Returns : bool True if everything went OK, False on error
// Desc    : Handles command line arguments.
bool
HandleArgs(WorkStruct *WorkDescriptor, int argc, char **argv)
{
	// PID has to be provided once
	// Other stuff can be provided only once
	bool PIDProvided = false;
	bool AccessMethodProvided = false;
	bool DumpMethodProvided = false;
	bool AcquireMethodProvided = false;
	bool UseSparseFilesProvided = false;

	// The loop
	int i = 1;

	for (; i < argc; i++)
	{
		// Help ?
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
		{
			// Just show help and run away!
			ShowHelp();
			exit(0);
		}

		// Compare
		// TODO: This is kinda ugly. Use a standard method to handle params.
#define OPTION(PARAM,TYPE,VALUE,STRERROR) \
    if(strcmp(argv[i], PARAM) == 0) \
    { \
      if(TYPE ## Provided) \
      { \
        fprintf(stderr, "Error:" STRERROR "(type -h or --help for usage)\n"); \
        return false; \
      } \
      TYPE ## Provided = true; \
      WorkDescriptor->TYPE = VALUE; \
      continue; \
    }

#define OPTIONEX(PARAM,TYPE,CHECK,VALUE,STRERROR) \
    if(strcmp(argv[i], PARAM) == 0) \
    { \
      if(TYPE ## Provided || !(CHECK)) \
      { \
        fprintf(stderr, "Error:" STRERROR "(type -h or --help for usage)\n"); \
        return false; \
      } \
      TYPE ## Provided = true; \
      WorkDescriptor->TYPE = VALUE; \
      continue; \
    }    

	// Access method
		OPTION("-o", AccessMethod, OPENPROCESS_METHOD, "You can only provide one access method, either -o od -d!\n");
		OPTION("-d", AccessMethod, DEBUGGER_API_METHOD, "You can only provide one access method, either -o od -d!\n");

		// Dump method
		OPTION("-1", DumpMethod, ONE_FILE_METHOD, "You can only provide one dump method, either -1, -1f or -m!\n");
		OPTION("-1f", DumpMethod, ONE_BIG_FILE_METHOD, "You can only provide one dump method, either -1, -1f or -m!\n");
		OPTION("-m", DumpMethod, MULTIPLE_FILE_METHOD, "You can only provide one dump method, either -1, -1f or -m!\n");

		// Acquire method
		OPTION("-q", AcquireMethod, VIRTUALQUERYEX_METHOD, "You can only provide one acquisition method, either -q or -b\n");
		OPTION("-b", AcquireMethod, READPROCESSMEMORY_METHOD, "You can only provide one acquisition method, either -q or -b\n");

		// Perhaps it's an additional option.
		OPTIONEX("-n", UseSparseFiles, WorkDescriptor->DumpMethod == ONE_BIG_FILE_METHOD, false,
			"You can only provide one option -n once and it can be only used with -1f\n");

		// Seems it it a PID
		if (PIDProvided)
		{
			fprintf(stderr, "Error: You can only provide one PID\n(type -h or --help for usage)\n");
			return false;
		}

		if (sscanf(argv[i], "%i", (int*)&WorkDescriptor->TargetPID) != 1)
		{
			fprintf(stderr, "Invalid PID format\n(type -h or --help for usage)\n");
			return false;
		}

		PIDProvided = true;
	}

	// Check if everything that had to be provided, was provided
	if (!PIDProvided)
	{
		fprintf(stderr, "Error: PID was not provided\n(type -h or --help for usage)\n");
		return false;
	}

	// Done
	return true; // All OK!
}

// Function: ShowHelp
// Desc    : Displays help message.
void
ShowHelp()
{
	puts("usage: HiperDrop <PID> [-o|-d] [-m|-1|-1f [-n]] [-q|-b]\n"
		"options:\n"
		"  -o      Use OpenProcess to access the memory (DEFAULT)\n"
		"  -d      Use Debugger API to access the memory\n"
		"  -1      Dump the memory as a single file\n"
		"  -1f     Same as -1, but also write the nonexisting pages as 0-filed pages\n"
		"          WARNING: this will produce a 2-4GB file; however it will be a\n"
		"          sparse file if the partition supports it (so the on-disk size\n"
		"          should be smaller)\n"
		"  -n      (to be used with -1f) Do not use sparse files (use normal files)\n"
		"  -m      Dump the memory as multiple files (DEFAULT)\n"
		"  -q      Use VirtualQueryEx to get information about existing pages (DEFAULT)\n"
		"  -b      Use ReadProcessMemory on every page (brute force mode)\n"
		"  -h      Display this message\n"
		"  --help  Same as above\n"
		"\n"
		"exampels:\n"
		"  HiperDrop 0x1234\n"
		"  HiperDrop 4660 -d -1f -b\n");
}

// Function: HandleAccessProcess
// Params  : WorkStruct *WorkDescriptor - A structure containing all the needed stuff
// Returns : bool True if everything went OK, False otherwise
// Desc    : Opens the process using the proper way
bool
HandleAccessProcess(WorkStruct *WorkDescriptor)
{
	// What method ?
	switch (WorkDescriptor->AccessMethod)
	{
		// OpenProcess!
	case OPENPROCESS_METHOD:
	{
		// TODO: Target process security tokens?

		// Try to adjust a token priv
		// Ignore any errors
		AcquirePrivilege(SE_DEBUG_NAME, true);

		// Let's try a simple open first
		HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, WorkDescriptor->TargetPID);
		if (hProcess == NULL)
		{
			fprintf(stderr, "Error: Could not open process (%u)\n", (unsigned int)GetLastError());
			return false;
		}

		// Set handle
		WorkDescriptor->hProcess = hProcess;

		// Done!
	}
	break;

	// Attach!
	case DEBUGGER_API_METHOD:
	{
		// Try to adjust a token priv
		// Ignore any errors
		AcquirePrivilege(SE_DEBUG_NAME, true);

		// Try to attach!
		if (!DebugActiveProcess(WorkDescriptor->TargetPID))
		{
			fprintf(stderr, "Error: Could not attach as a debugger to process (%i)\n", (unsigned int)GetLastError());
			return false;
		}

		// Wait for the CREATE_PROCESS_DEBUG_EVENT debug event
		if (!WaitForDebugEvent(&WorkDescriptor->DebugEvent, 5000) ||
			WorkDescriptor->DebugEvent.dwDebugEventCode != CREATE_PROCESS_DEBUG_EVENT)
		{
			// Issue an error
			fprintf(stderr, "Error: Could not get CREATE_PROCESS_DEBUG_EVENT debugger event (%i)\n", (unsigned int)GetLastError());

			// Stop debugging
			if (!DebugActiveProcessStop(WorkDescriptor->TargetPID))
			{
				// Uh... if we terminate, the app may die...
				// Better inform the user!
				fprintf(stderr, "Problem: Could not detach from application! When HiperDrop will exit\n"
					"         it is very possible that the application will be terminated!\n"
					"         Please save your data, and press ENTER when you are ready to\n"
					"         exit HiperDrop.\n");

				// TODO: Clean stdin

				// Wait for any char
				getchar();
			}

			// Return false
			return false;
		}

		// Setup the handle
		WorkDescriptor->hProcess = WorkDescriptor->DebugEvent.u.CreateProcessInfo.hProcess;

		// Close the hFile handle, it won't be needed
		CloseHandle(WorkDescriptor->DebugEvent.u.CreateProcessInfo.hFile);

		// TODO: Think about letting the user decide wheather we should pause
		// the app, or let it continue. For now I'm pausing the app.

		// Done!
	}
	break;
	}

	// All OK!
	return true;
}

// Function: WriteDataOneFile
// Params  : WorkStruct *WorkDescriptor - all the things that are needed to do work
//         : DWORD       VirtualAddress - virtual address of the memory
//         : BYTE       *Data           - memory content
//         : SIZE_T      Size           - size of data
//         : bool        FillSpace      - true if fill space between pages with 0
//         : bool        Sparse         - use sparse files if avaiable (use only if FillSpace is true)
// Returns : bool True if everything went OK, False otherwise
// Desc    : Write the data to disk to a single file
bool
WriteDataOneFile(WorkStruct *WorkDescriptor, DWORD VirtualAddress, BYTE *Data, SIZE_T Size, bool FillSpace, bool Sparse)
{
	// But first, check if the file is opened
	if (!WorkDescriptor->f)
	{
		// Forge name
		char FileName[MAX_PATH];
		snprintf(FileName, sizeof(FileName), "mem_%u.bin", (unsigned int)WorkDescriptor->TargetPID);

		// Open file
		WorkDescriptor->f = fopen(FileName, "wb");
		if (!WorkDescriptor->f)
		{
			// Issue an error and return false
			fprintf(stderr, "Error: Could not create file \"%s\"\n", FileName);
			return false;
		}

		// If this is a sprase file, we can retrieve the HANDLE here.
		// Also, this is a good time to set the sprase flag.
		if (Sparse)
		{
			int FileNo = _fileno(WorkDescriptor->f);
			if (FileNo == -1 && errno == EINVAL)
			{
				fprintf(stderr, "Error: Could not derive HANDLE from FILE* (1)\n");

				// Clean up.
				fclose(WorkDescriptor->f);
				WorkDescriptor->f = NULL;
				return false;
			}

			WorkDescriptor->hFileCache = (HANDLE)_get_osfhandle(FileNo);
			if (WorkDescriptor->hFileCache == INVALID_HANDLE_VALUE && errno == EBADF)
			{
				fprintf(stderr, "Error: Could not derive HANDLE from FILE* (2)\n");

				// Clean up.
				fclose(WorkDescriptor->f);
				WorkDescriptor->f = NULL;
				return false;
			}

			// Set the sparse flag.
			DWORD IgnoredRet;
			if (!DeviceIoControl(WorkDescriptor->hFileCache, FSCTL_SET_SPARSE,
				NULL, 0, NULL, 0, &IgnoredRet, NULL))
			{
				fprintf(stderr, "Error: Sparse flag setting failed (%u)\n",
					(unsigned int)GetLastError());

				// Clean up.
				fclose(WorkDescriptor->f);
				WorkDescriptor->f = NULL;
				return false;
			}
		}
	}

	// Fill space?
	if (FillSpace)
	{
		DWORD Distance = VirtualAddress - WorkDescriptor->LastOffset;

		// Seems we have to write some padding first
		if (Distance > 0)
		{
			// Sparse or fill?
			if (Sparse)
			{
				// Flush.
				// TODO: This flush would not be required if I were to use
				// the WinAPI CreateFile/etc API. If this flush slows it down,
				// I should rewrite it.
				fflush(WorkDescriptor->f);

				// Set empty sparse region.
				FILE_ZERO_DATA_INFORMATION Fzdi;
				Fzdi.FileOffset.QuadPart = WorkDescriptor->LastOffset;
				Fzdi.BeyondFinalZero.QuadPart = VirtualAddress;

				DWORD IgnoredRet;

				if (!DeviceIoControl(WorkDescriptor->hFileCache, FSCTL_SET_ZERO_DATA,
					&Fzdi, sizeof(Fzdi), NULL, 0, &IgnoredRet, NULL))
				{
					fprintf(stderr, "Error: Sparse area setting failed (%u)\n",
						(unsigned int)GetLastError());

					return false;
				}

				// Skip the zeroes.
				// XXX: I have bad feelings about this.
				fseek(WorkDescriptor->f, VirtualAddress, SEEK_SET);
			}
			else
			{
				// How many pages?
				DWORD PageCount = Distance / 0x1000;

				// Physically fill with zeros.

				// Create an empty page with 4096 zeroes
				static const BYTE EmptyPage[0x1000] = { 0 };

				// Write them
				DWORD i;
				for (i = 0; i < PageCount; i++)
				{
					if (fwrite(EmptyPage, 1, 0x1000, WorkDescriptor->f) != 0x1000)
					{
						fprintf(stderr, "Error: Could not write padding to file (disk full?)\n");
						return false;
					}
				}
			}

			// Done.
		}

		// Reset the LastOffset
		WorkDescriptor->LastOffset = VirtualAddress + Size;
	}

	// Write data
	if (fwrite(Data, 1, Size, WorkDescriptor->f) != Size)
	{
		fprintf(stderr, "Error: Could not write to file (disk full?)\n");
		return false;
	}

	// TODO: Should I fflush the file? It will make the file cache system not used,
	// but one can see the file while it's dumped, and the app won't hang sometimes

	// Done!
	return true;
}

// Function: WriteDataMultipleFile
// Params  : WorkStruct *WorkDescriptor - all the things that are needed to do work
//         : DWORD       VirtualAddress - virtual address of the memory
//         : BYTE       *Data           - memory content
//         : SIZE_T      Size           - size of data
// Returns : bool True if everything went OK, False otherwise
// Desc    : Write the data to disk to multiple file
bool
WriteDataMultipleFile(WorkStruct *WorkDescriptor, DWORD VirtualAddress, BYTE *Data, SIZE_T Size)
{
	// Calculate the distance
	DWORD Distance = VirtualAddress - WorkDescriptor->LastOffset;

	// We need to create a new file if Distance is non zero
	// Also, we need to create a new file if non is open
	if (Distance != 0 || !WorkDescriptor->f)
	{
		// If there is a file, close it
		if (WorkDescriptor->f)
		{
			// Close it
			fclose(WorkDescriptor->f);
			WorkDescriptor->f = NULL;
		}

		// Now, reopen a new file

		// Forge thhe name
		char FileName[MAX_PATH];
		snprintf(FileName, sizeof(FileName), "mem_%u_%.8x.bin",
			(unsigned int)WorkDescriptor->TargetPID,
			(unsigned int)VirtualAddress);

		// Open file
		WorkDescriptor->f = fopen(FileName, "wb");
		if (!WorkDescriptor->f)
		{
			// Issue an error and return false
			fprintf(stderr, "Error: Could not create file \"%s\"\n", FileName);
			return false;
		}
	}

	// Reset the LastOffset
	WorkDescriptor->LastOffset = VirtualAddress + Size;

	// Write data
	if (fwrite(Data, 1, Size, WorkDescriptor->f) != Size)
	{
		fprintf(stderr, "Error: Could not write to file (disk full?)\n");
		return false;
	}

	// TODO: Should I fflush the file? It will make the file cache system not used,
	// but one can see the file while it's dumped, and the app won't hang sometimes

	// Done!
	return true;
}

// Function: HandleDataWrite
// Params  : WorkStruct *WorkDescriptor - all the things that are needed to do work
//         : DWORD       VirtualAddress - virtual address of the memory
//         : BYTE       *Data           - memory content
//         : SIZE_T      Size           - size of data
// Returns : bool True if everything went OK, False otherwise
// Desc    : Write the data to disk
bool
HandleDataWrite(WorkStruct *WorkDescriptor, DWORD VirtualAddress, BYTE *Data, SIZE_T Size)
{
	// Common stuff - add to list
	size_t Offset = WorkDescriptor->f ? ftell(WorkDescriptor->f) : 0;
	MemoryDesc Memory = { VirtualAddress, Size, Offset };
	WorkDescriptor->MemoryList.push_back(Memory);

	// Return variable
	bool bRet = false;

	// Method?
	switch (WorkDescriptor->DumpMethod)
	{
	case MULTIPLE_FILE_METHOD:
		// Just call the function
		bRet = WriteDataMultipleFile(WorkDescriptor, VirtualAddress, Data, Size);
		break;

		// Single file only?
	case ONE_FILE_METHOD:
		// This is the easiest method - just write the data
		bRet = WriteDataOneFile(WorkDescriptor, VirtualAddress, Data, Size, false, false);
		break;

	case ONE_BIG_FILE_METHOD:
		// Same as the above, with one difference - all the space between the allocated pages
		// must be filled with zeroes
		bRet = WriteDataOneFile(WorkDescriptor, VirtualAddress, Data, Size, true,
			WorkDescriptor->UseSparseFiles);
		break;
	}

	// Done!
	return bRet;
}

// Function: HandleDataReadWrite
// Params  : WorkStruct *WorkDescriptor - all the things that are needed to do work
// Returns : bool True if everything went OK, False otherwise
// Desc    : Get the data and write the stuff
bool
HandleDataReadWrite(WorkStruct *WorkDescriptor)
{
	// Is 4GT enabled?
	MEMORYSTATUSEX MemStatEx;

	MemStatEx.dwLength = sizeof(MemStatEx);
	GlobalMemoryStatusEx(&MemStatEx);

	if (MemStatEx.ullTotalVirtual == 0xC0000000)
		WorkDescriptor->Is4GT = true;

	// Common
	DWORD Accessed = 0;
	DWORD Total = 0;

	// What method?
	switch (WorkDescriptor->AcquireMethod)
	{
		// VirtualQueryEx?
	case VIRTUALQUERYEX_METHOD:
	{
		DWORD i;
		DWORD Limit = WorkDescriptor->Is4GT ? 0xC0000000 : 0x80000000;

		// For each page in memory
		for (i = 0; i < Limit;)
		{
			// Mem info
			MEMORY_BASIC_INFORMATION MemInfo;

			// Query the page
			SIZE_T sRet = VirtualQueryEx(
				WorkDescriptor->hProcess,
				(LPCVOID)i,
				&MemInfo,
				sizeof(MemInfo)
			);

			// So... are there some pages there ?
			if (sRet == 0 || MemInfo.State == MEM_FREE || MemInfo.State == MEM_RESERVE)
			{
				// We can skip lots of pages or only one..
				if (sRet == 0)
				{
					// Try the next page I guess
					i += 0x1000;
					Total++;
				}
				else
				{
					// Skip the whole block
					i += MemInfo.RegionSize;
					Total += MemInfo.RegionSize / 0x1000;
				}

				// Let's print out something
				if ((Total & 0xff) == 0)
				{
					printf("Reading memory at %.8x... (Accessed pages / Total pages: %u / %u)\r",
						(unsigned int)i, (unsigned int)Accessed, (unsigned int)Total);
					fflush(stdout);
				}

				// Continue
				continue;
			}

			// Get the data!
			// TODO: Think about acquireing larger amounts of data
			//       like... 1 MB at a time sounds good
			DWORD PageCount = MemInfo.RegionSize / 0x1000;
			DWORD j;
			for (j = 0; j < PageCount; j++)
			{
				// A static buffer
				static BYTE PageBuffer[0x1000];
				SIZE_T BytesRead = 0;

				// Read (and ignore the errors)!
				if (ReadProcessMemory(WorkDescriptor->hProcess, (LPCVOID)(i + j * 0x1000), PageBuffer, sizeof(PageBuffer), &BytesRead) &&
					BytesRead != 0)
				{
					// Counter!
					Accessed++;
					Total++;

					// Write the page to disk
					HandleDataWrite(WorkDescriptor, i + j * 0x1000, PageBuffer, BytesRead);
				}

				// Let's print out something
				if ((Total & 0xff) == 0)
					printf("Reading memory at %.8x... (Accessed pages / Total pages: %u / %u)\r",
					(unsigned int)i, (unsigned int)Accessed, (unsigned int)Total);
			}

			// Iterate
			i += 0x1000 * PageCount;
		}

		// Done!
		printf("Done reading memory. Final statistics: Accessed pages / Total pages: %u / %u\n",
			(unsigned int)Accessed, (unsigned int)Total);
	}
	break;

	// ReadProcessMemory brute force?
	case READPROCESSMEMORY_METHOD:
	{
		DWORD i;
		DWORD Limit = WorkDescriptor->Is4GT ? 0xC0000000 : 0x80000000;

		// For each page in memory
		for (i = 0; i < Limit; i += 0x1000, Total++)
		{
			// A static buffer
			static BYTE PageBuffer[0x1000];
			SIZE_T BytesRead = 0;

			// Read (and ignore the errors)!
			if (ReadProcessMemory(WorkDescriptor->hProcess, (LPCVOID)i, PageBuffer, sizeof(PageBuffer), &BytesRead) &&
				BytesRead != 0)
			{
				// Counter!
				Accessed++;

				// Write the page to disk
				HandleDataWrite(WorkDescriptor, i, PageBuffer, BytesRead);
			}

			// Let's print out something
			if ((Total & 0xff) == 0)
				printf("Reading memory at %.8x... (Accessed pages / Total pages: %u / %u)\r",
				(unsigned int)i, (unsigned int)Accessed, (unsigned int)Total);
		}

		// Done!
		printf("Done reading memory. Final statistics: Accessed pages / Total pages: %u / %u\n",
			(unsigned int)Accessed, (unsigned int)Total);
	}
	break;
	}

	// Done
	return true;
}

// Function: WriteMapFileEntry
// Params  : FILE *f - map file, opened for text write access
//         : WorkStruct *WorkDescriptor - all the things needed to work
//         : DWORD StartAddress - start address of the block
//         : DWORD PageCount - block size, in pages (0x1000)
//         : DOWRD RawStart - offset in the single small dump file of the block
// Returns : bool True if everything went OK, False otherwise
// Desc    : Write one entry to the map file
bool
WriteMapFileEntry(FILE *f, WorkStruct *WorkDescriptor, DWORD StartAddr, DWORD PageCount, DWORD RawStart)
{
	// TODO: Error detection

	// What kind of method?
	switch (WorkDescriptor->DumpMethod)
	{
	case MULTIPLE_FILE_METHOD:
		fprintf(f, "%.8x - %.8x [file mem_%u_%.8x.bin]: %u pages\n",
			(unsigned int)StartAddr,
			(unsigned int)StartAddr + (unsigned int)PageCount * 0x1000 - 1,
			(unsigned int)WorkDescriptor->TargetPID,
			(unsigned int)StartAddr,
			(unsigned int)PageCount);
		break;

	case ONE_FILE_METHOD:
		// Write
		fprintf(f, "%.8x - %.8x [offset %.8x]: %u pages\n",
			(unsigned int)StartAddr,
			(unsigned int)StartAddr + (unsigned int)PageCount * 0x1000 - 1,
			(unsigned int)RawStart,
			(unsigned int)PageCount);
		break;

	case ONE_BIG_FILE_METHOD:
		// Write
		fprintf(f, "%.8x - %.8x: %u pages\n",
			(unsigned int)StartAddr,
			(unsigned int)StartAddr + (unsigned int)PageCount * 0x1000 - 1,
			(unsigned int)PageCount);
		break;
	}

	// Done
	return true;
}


// Function: HandleFinalizeWrite
// Params  : WorkStruct *WorkDescriptor - all the things that are needed to do work
// Returns : bool True if everything went OK, False otherwise
// Desc    : Finalize write and save map file
bool
HandleFinalizeWrite(WorkStruct *WorkDescriptor)
{
	// TODO: 1f - add the final zeroed pages

	// Close file ?
	if (WorkDescriptor->f) {
		fclose(WorkDescriptor->f);
		WorkDescriptor->f = NULL;
	}

	// Write map file

	// Forge name
	char FileName[MAX_PATH];
	snprintf(FileName, sizeof(FileName), "mem_%u.map", (unsigned int)WorkDescriptor->TargetPID);

	// Open file
	FILE *f = fopen(FileName, "w");
	if (!f)
	{
		// Issue an error and return false
		fprintf(stderr, "Error: Could not create the map file \"%s\"\n", FileName);
		return false;
	}

	// Write the map file
	DWORD StartAddr = 0xffffffff;
	DWORD PageCount = 0;
	DWORD LastOffset = 0;
	DWORD RawStart = 0;

	list<MemoryDesc>::iterator i = WorkDescriptor->MemoryList.begin();
	for (; i != WorkDescriptor->MemoryList.end(); i++)
	{
		DWORD Distance = (*i).VirtualAddress - LastOffset;

		// Is distance to big?
		// Is it the first one?
		if (Distance > 0 || StartAddr == 0xffffffff)
		{
			// Something to write?
			if (StartAddr != 0xffffffff)
			{
				// TODO: Error detection
				WriteMapFileEntry(f, WorkDescriptor, StartAddr, PageCount, RawStart);
			}

			// Reset
			StartAddr = (*i).VirtualAddress;
			RawStart = (*i).RawOffset;
			PageCount = 0;
		}

		// Add
		PageCount += (*i).Size / 0x1000;

		// Reset
		LastOffset = (*i).VirtualAddress + (*i).Size;
	}

	// Final write
	if (StartAddr != 0xffffffff && PageCount > 0)
	{
		// TODO: Error detection
		WriteMapFileEntry(f, WorkDescriptor, StartAddr, PageCount, RawStart);
	}

	// Close the file
	fclose(f);

	// Done
	return true;
}

// Function: HandleDetachProcess
// Params  : WorkStruct *WorkDescriptor - all the things that are needed to do work
// Returns : bool True if everything went OK, False otherwise
// Desc    : Gently detach the process
bool
HandleDetachProcess(WorkStruct *WorkDescriptor)
{
	// What method?
	switch (WorkDescriptor->AccessMethod)
	{
		// OpenProcess?
	case OPENPROCESS_METHOD:
	{
		// Close handle
		CloseHandle(WorkDescriptor->hProcess);
	}
	break;

	// Debugger API?
	case DEBUGGER_API_METHOD:
	{
		// Continue the process (ignore errors)
		ContinueDebugEvent(WorkDescriptor->DebugEvent.dwProcessId, WorkDescriptor->DebugEvent.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);

		// TODO: What if any debug message comes while the process is debugged ?
		//       Guess have to do sth with it

		// Stop debugging
		if (!DebugActiveProcessStop(WorkDescriptor->TargetPID))
		{
			// Uh... if we terminate, the app may die...
			// Better inform the user!
			fprintf(stderr, "Problem: Could not detach from application! When HiperDrop will exit\n"
				"         it is very possible that the application will be terminated!\n"
				"         Please save your data, and press ENTER when you are ready to\n"
				"         exit HiperDrop.\n");

			// Wait for any char
			getchar();
		}

		// Done
	}
	break;
	}

	return true;
}

// Function: AcquirePrivilege
// Params  : const char *PrivilegeName - name of the privilege
//         : bool Enable               - enable or disable ?
// Returns : bool True if everything went OK, False otherwise
// Desc    : Gets or removes a privilege
bool
AcquirePrivilege(const char *PrivilegeName, bool Enable)
{
	// Open process token
	HANDLE hToken = NULL;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		// Write out an error
		fprintf(stderr, "Error: Could not open own token (%i)\n", (unsigned int)GetLastError());

		// Return
		return false;
	}

	// Lookup privilege
	LUID Luid;

	if (!LookupPrivilegeValue(NULL, PrivilegeName, &Luid))
	{
		// Write out an error
		fprintf(stderr, "Error: Could not find privilege name (%i)\n", (unsigned int)GetLastError());

		// Close handle before returning
		CloseHandle(hToken);

		// Return
		return false;
	}

	// Setup structure
	TOKEN_PRIVILEGES TokenPrivs;

	TokenPrivs.PrivilegeCount = 1;
	TokenPrivs.Privileges[0].Luid = Luid;
	TokenPrivs.Privileges[0].Attributes = Enable ? SE_PRIVILEGE_ENABLED : 0;

	// Adjust!
	if (!AdjustTokenPrivileges(hToken, FALSE, &TokenPrivs, sizeof(TokenPrivs), NULL, NULL) ||
		GetLastError() != ERROR_SUCCESS)
	{
		// Issue a warning, but do not return
		fprintf(stderr, "Warning: Could not acquire SeDebugPrivilege (%i)\n", (unsigned int)GetLastError());
	}

	// Close handle
	CloseHandle(hToken);

	// Done!
	return true;
}

// Function: CanUseSparseFiles
// Params  : none
// Returns : bool True if partition supports sparse files.
// Desc    : Checks if cwd/partition supports sparse files.
bool CanUseSparseFiles()
{
	DWORD Flags;

	if (!GetVolumeInformation(NULL, NULL, 0, NULL, NULL, &Flags, NULL, 0))
	{
		fprintf(stderr, "Error: GetVolumeInformation failed (%u), assuming sparse files not supported.\n",
			(unsigned int)GetLastError());
		return false;
	}

	// Done.
	return (bool)(Flags & FILE_SUPPORTS_SPARSE_FILES);
}

