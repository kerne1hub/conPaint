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
	HANDLE FileHandle = CreateFile(outputfile, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (length <= 0)
		ExitProcess(-1);

	DWORD filelength = SetFilePointer(FileHandle, 0, 0, FILE_END);
	DWORD n;

	if (FileHandle == INVALID_HANDLE_VALUE)
	{
		GetErrorMsg();
	}
	if (!WriteFile(FileHandle, str, length, &n, NULL))
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
					else if (mouseXY.Y > 0) {
						api_out(defaultChar, 1, mouseXY, wAttributes);
					}
				}
			}
	}

//	api_out(s, n, L"C:/Users/Kernel/source/repos/file.txt");
}