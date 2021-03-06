// OS_handle.cpp: определяет точку входа для консольного приложения.

#include "stdafx.h"
#include <conio.h>
#include <Windows.h>
#include <locale.h>

void GetErrorMsg();

DWORD api_in(CHAR* str, DWORD length)
{
	if (length <= 0)
		ExitProcess(-1);

	DWORD n;
	HANDLE std_in = GetStdHandle(STD_INPUT_HANDLE);
	if (std_in == INVALID_HANDLE_VALUE)
		ExitProcess(-1);
	if (!ReadFile(std_in, str, length, &n, NULL))
		ExitProcess(-1);

	return n;
}

int toFile() {
	CHAR_INFO console[3360];	//28x120
	COORD dwBufferSize;
	dwBufferSize.X = 120;
	dwBufferSize.Y = 28;
	COORD dwBufferCoord;
	dwBufferCoord.X = 0;
	dwBufferCoord.Y = 0;
	HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rect;
	rect.Top = 1;
	rect.Left = 0;
	rect.Bottom = 29;
	rect.Right = 119;

	if (!ReadConsoleOutput(std_out, console, dwBufferSize, dwBufferCoord, &rect))
		ExitProcess(-7);

	LPCWSTR outputfile = L"C:/Users/Kernel/source/repos/savedData.dat";
	HANDLE fileHandle = CreateFile(outputfile, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD filelength = SetFilePointer(fileHandle, 0, 0, FILE_END);
	DWORD n;

	if (fileHandle == INVALID_HANDLE_VALUE)
		GetErrorMsg();

	CHAR chars[6720];
	for (int i = 0; i < 3360; i++) {
		chars[2*i] = console[i].Char.AsciiChar;
		chars[2*i+1] = (UCHAR)console[i].Attributes;
	}
	CHAR* str = chars;

	WriteFile(fileHandle, str, 6720, &n, NULL);

	CloseHandle(fileHandle);
	return 0;
}

int fromFile(CHAR* str) {
	CHAR_INFO console[3360];	//28x120
	COORD dwBufferSize;
	dwBufferSize.X = 120;
	dwBufferSize.Y = 28;
	COORD dwBufferCoord;
	dwBufferCoord.X = 0;
	dwBufferCoord.Y = 0;
	HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rect;
	rect.Top = 1;
	rect.Left = 0;
	rect.Bottom = 29;
	rect.Right = 119;
	LPCWSTR inputfile = L"C:/Users/Kernel/source/repos/savedData.dat";
	HANDLE fileHandle = CreateFile(inputfile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD n;

	if (fileHandle == INVALID_HANDLE_VALUE)
		GetErrorMsg();

	ReadFile(fileHandle, str, 6720, &n, NULL);

	for (int i = 0; i < 3360; i++) {
		console[i].Char.AsciiChar = str[2 * i];
		console[i].Char.UnicodeChar = str[2 * i];
		console[i].Attributes = (UCHAR)str[2 * i + 1];
	}

	if (!WriteConsoleOutput(std_out, console, dwBufferSize, dwBufferCoord, &rect))
		ExitProcess(-8);

	CloseHandle(fileHandle);
	return 0;
}

int clearWorkSpace() {
	CHAR_INFO console[3360];	//28x120
	COORD dwBufferSize;
	dwBufferSize.X = 120;
	dwBufferSize.Y = 28;
	COORD dwBufferCoord;
	dwBufferCoord.X = 0;
	dwBufferCoord.Y = 0;
	HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rect;
	rect.Top = 1;
	rect.Left = 0;
	rect.Bottom = 29;
	rect.Right = 119;

	for (int i = 0; i < 3360; i++) {
		console[i].Char.AsciiChar = ' ';
		console[i].Char.UnicodeChar = ' ';
		console[i].Attributes = (UCHAR)0;
	}

	if (!WriteConsoleOutput(std_out, console, dwBufferSize, dwBufferCoord, &rect))
		ExitProcess(-8);
}

int api_out(CHAR* str, DWORD length)
{
	if (length <= 0)
		ExitProcess(-1);

	DWORD n;
	HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	if (std_out == INVALID_HANDLE_VALUE)
		ExitProcess(-1);
	if (!WriteFile(std_out, str, length, &n, NULL))
		ExitProcess(-1);

	return 0;
}

int api_out(CHAR* str, DWORD length, LPCWSTR outputfile)
{
	HANDLE fileHandle = CreateFile(outputfile, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (length <= 0)
		ExitProcess(-1);

	DWORD filelength = SetFilePointer(fileHandle, 0, 0, FILE_END);
	DWORD n;

	if (fileHandle == INVALID_HANDLE_VALUE)
		GetErrorMsg();

	if (!WriteFile(fileHandle, str, length, &n, NULL))
		ExitProcess(-1);

	return 0;
}

int api_out(CHAR* str, DWORD length, COORD dwCursorPosition) {
	if (length <= 0)
		ExitProcess(-1);

	DWORD n;
	HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	if (std_out == INVALID_HANDLE_VALUE)
		ExitProcess(-1);
	if (!SetConsoleCursorPosition(std_out, dwCursorPosition))
		ExitProcess(-2);
	if (!WriteFile(std_out, str, length, &n, NULL))
		ExitProcess(-1);

	return 0;
}

int api_out(CHAR* str, DWORD length, COORD dwCursorPosition, WORD wAttributes) {
	if (length <= 0)
		ExitProcess(-1);

	DWORD n;
	HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	if (std_out == INVALID_HANDLE_VALUE)
		ExitProcess(-1);
	if (!SetConsoleTextAttribute(std_out, wAttributes))
		ExitProcess(-3);
	if (!SetConsoleCursorPosition(std_out, dwCursorPosition))
		ExitProcess(-2);
	if (!WriteFile(std_out, str, length, &n, NULL))
		ExitProcess(-1);

	return 0;
}

void GetErrorMsg() 
{
	
	DWORD errorcode = GetLastError();
	LPVOID msg;
	DWORD length = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorcode,
		MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
		(LPTSTR)&msg,
		0,
		NULL);
	char *s = new char[length + 1];
	WideCharToMultiByte(CP_ACP, 0, (LPTSTR)msg, -1, s, length, NULL, NULL);
	api_out(s, length);
	_getch();
}

void updateFGState(int wAttributes) {
	if (wAttributes != 0) {
		CHAR FCOLOR[] = "[FG]";
		COORD colorxy;
		colorxy.X = 0; colorxy.Y = 0;
		api_out(FCOLOR, sizeof(FCOLOR), colorxy, wAttributes);
	}
}

void updateBGState(int wAttributes) {
	if (wAttributes != 0) {
		CHAR BCOLOR[] = "[BG]";
		COORD bcolorxy;
		bcolorxy.X = 21; bcolorxy.Y = 0;
		api_out(BCOLOR, sizeof(BCOLOR), bcolorxy, wAttributes);
	}
}

int main()
{
	CHAR str[6720];
	COORD consolexy;
	WORD wAttributes = 15;
	int color = 0;

	//foreground colors
	consolexy.X = 4;
	consolexy.Y = 0;
	updateFGState(wAttributes);
	CHAR colorchar[] = { (char)219 };

	for (int x = 0; x < 16; x++) {
			wAttributes = color++;
			consolexy.X++;
			api_out(colorchar, sizeof(colorchar), consolexy, wAttributes);
	}
	//background colors
	wAttributes = 15;
	color = 0;
	consolexy.X = 25;
	consolexy.Y = 0;
	updateBGState(wAttributes);
	CHAR nullchar[] = { 219 };
	for (int x = 0; x < 16; x++) {
		wAttributes = color++;
		consolexy.X++;
		api_out(nullchar, sizeof(nullchar), consolexy, wAttributes);
	}

	//clear button
	wAttributes = 15;
	color = 0;
	consolexy.X = 44;
	consolexy.Y = 0;
	CHAR CLR[] = "[CLR]";
	api_out(CLR, sizeof(CLR), consolexy, wAttributes);

	//save button
	wAttributes = 15;
	color = 0;
	consolexy.X = 50;
	consolexy.Y = 0;
	CHAR SAVE[] = "[SAVE]";
	api_out(SAVE, sizeof(SAVE), consolexy, wAttributes);

	//load button
	wAttributes = 15;
	color = 0;
	consolexy.X = 57;
	consolexy.Y = 0;
	CHAR LOAD[] = "[LOAD]";
	api_out(LOAD, sizeof(LOAD), consolexy, wAttributes);

	//pen
	wAttributes = 15;
	color = 0;
	consolexy.X = 64;
	consolexy.Y = 0;
	CHAR PEN[] = "[PEN: *#Oox ]";
	api_out(PEN, sizeof(PEN), consolexy, wAttributes);
	//eventHandler

	INPUT_RECORD rbuffer;
	DWORD events;
	COORD mouseXY;
	WORD foreGround = 15, backGround = 0;
	CHAR defaultChar[] = { 'x' };
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_MOUSE_INPUT);
	while (true) {
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rbuffer, 1, &events);
			if (rbuffer.EventType == MOUSE_EVENT) {
				events = rbuffer.Event.MouseEvent.dwButtonState;
				mouseXY = rbuffer.Event.MouseEvent.dwMousePosition;
				CHAR X[4];
				_itoa_s(mouseXY.X, X, 10);
				CHAR Y[4];
				_itoa_s(mouseXY.Y, Y, 10);
				CHAR ESC[3] = {"  "};
				consolexy.X = 103;
				consolexy.Y = 0;
				api_out(X, sizeof(X), consolexy, 15);
				consolexy.X = 103 + strlen(X) + 1;
				api_out(Y, sizeof(Y), consolexy, 15);
				consolexy.X += strlen(Y);
				api_out(ESC, sizeof(ESC), consolexy, 15);

				if (events == FROM_LEFT_1ST_BUTTON_PRESSED) {
					if (mouseXY.X > 4 && mouseXY.X < 21 && mouseXY.Y == 0) {
						WORD lpAttribute;
						DWORD numOfAttr;
						ReadConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), &lpAttribute, 1, mouseXY, &numOfAttr);
						wAttributes = backGround << 4 | lpAttribute;
						foreGround = lpAttribute;
						updateFGState(foreGround);
					}
					else if (mouseXY.X > 25 && mouseXY.X < 42 && mouseXY.Y == 0) {
						WORD lpAttribute;
						DWORD numOfAttr;
						ReadConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), &lpAttribute, 1, mouseXY, &numOfAttr);
						wAttributes = lpAttribute << 4 | foreGround;
						backGround = lpAttribute;
						updateBGState(backGround);
					}
					else if (mouseXY.X > 44 && mouseXY.X < 48 && mouseXY.Y == 0) {
						clearWorkSpace();
					}
					else if (mouseXY.X > 50 && mouseXY.X < 55 && mouseXY.Y == 0) {
						toFile();
					}
					else if (mouseXY.X > 57 && mouseXY.X < 62 && mouseXY.Y == 0) {
						fromFile(str);
					}
					else if (mouseXY.X > 64 && mouseXY.X < 68 && mouseXY.Y == 0) {
						//fromFile(ascii); 
					}
					else if (mouseXY.X > 69 && mouseXY.X < 75 && mouseXY.Y == 0) {
						WCHAR symb;
						DWORD numOfAttr;
						ReadConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), &symb, 1, mouseXY, &numOfAttr);
						defaultChar[0] = symb;
					}
					else if (mouseXY.Y > 0) {
						api_out(defaultChar, 1, mouseXY, wAttributes);
					}
				}

				else if (events == RIGHTMOST_BUTTON_PRESSED) {
					api_out(new CHAR{ ' ' }, 1, mouseXY, 0);
				}
			}
	}
}