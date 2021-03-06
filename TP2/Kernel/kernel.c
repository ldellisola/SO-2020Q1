// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <idtLoader.h>
#include <Curses.h>
#include <VideoDriver.h>
#include <font.h>
#include <keyboard.h>
#include <Debugger.h>

#include <Curses.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{

	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

#include <ConsoleDriver.h>
#include <font.h>
#include <keyboard.h>
#include <Pipe.h>
#include <Scheduler.h>
int main()
{	
	startVideoDriver();
	initializeKeyboard();
	initializeConsoleDriver(CHAR_HEIGHT,CHAR_WIDTH, SCREEN_HEIGHT,SCREEN_WIDTH); 

	SetKeyboardShortcut(semInfo,'s');
	SetKeyboardShortcut(semInfo,'S');
	
	SetKeyboardShortcut(clearConsole,'C');
	SetKeyboardShortcut(clearConsole,'c');

	SetKeyboardShortcut(pipes,'i');
	SetKeyboardShortcut(pipes,'I');

	SetKeyboardShortcut(ps,'p');
	SetKeyboardShortcut(ps,'P');

	// printf("SCREEN H: %d, W: %d\n", SCREEN_WIDTH, SCREEN_WIDTH);

				load_idt();

	((EntryPoint)sampleCodeModuleAddress)();
	

	return 0;
}
