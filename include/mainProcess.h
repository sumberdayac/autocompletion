#ifndef MAIN_H
#define MAIN_H

#include "trie.h"
#include <windows.h>
#include <conio.h>

HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
WORD saved_attributes;

CONSOLE_SCREEN_BUFFER_INFO csbi;
COORD WindowsSize;

void displayMenu();
void mainProcess();
void csrs(void);
void gotoxy(int x, int y);
void initSystem();
void printGridUI(char Pesan[]);
COORD ReadResolution();
void printc(char Pesan[]);
void printLine(char line);
void printCenterLine(char line, int StartPos);
void printHalfScreen(char Pesan[], bool isNewLine, bool cancelEnter);
void startingMenu();
#endif // MAIN_H
