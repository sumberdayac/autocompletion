#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/mainProcess.h"
#include "../include/trie.h"
#include "../include/autocomplete.h"
#include "../include/fileProcess.h"
#include "autocomplete.c"

void displayMenu()
{
    system("cls");
    printGridUI("Main Menu");

    printf("Welcome to our autocomplete\n");
    printf("program, we are team 6 from 1B-D4\n");
    printf("with 3 people:\n");
    printf("1. Elizabeth Gita Gracia Trimurti [231524040]\n");
    printf("2. Satryo Haryo Bimo              [231524062]\n");
    printf("3. Yahya Alfon Sinaga             [231524064]\n\n");

    printf("\t\t	              ...:-::...---:..                \n");
    printf("\t\t            ...--....:::...:::::-::-....          \n");
    printf("\t\t         ...:-:..              .....--:-:..       \n");
    printf("\t\t       .--:-:                       ....=-.       \n");
    printf("\t\t     ..--....                           .:=-:..   \n");
    printf("\t\t   .::-:..                               ...:::.. \n");
    printf("\t\t ..::...                                    .:... \n");
    printf("\t\t ..::.                                     ..::.. \n");
    printf("\t\t .:-..                                     ..::.. \n");
    printf("\t\t .-+-.                                     ...:-..\n");
    printf("\t\t .-:..                                       ...=:\n");
    printf("\t\t-:..                                           .=:\n");
    printf("\t\t-...                .:*.......                ..:-\n");
    printf("\t\t.+..            ....%=*.::.#--.-.            ...-.\n");
    printf("\t\t .:.            .::.%.+...:+%...:...           .-.\n");
    printf("\t\t ..:=:..    ...--+=. .::-..... .+...       ...=#-.\n");
    printf("\t\t   .:-........:--::-...  ......+:-..........-=-:. \n");
    printf("\t\t     ..:.*+-:-. ...:=-.. .-:::.*:--::...:=.       \n");
    printf("\t\t       ...          ..=. .=-.:..  ....:..         \n");
    printf("\t\t                     ..- ..:..                    \n");
    printf("\t\t                     .:+ .-..                     \n");
    printf("\t\t                     .-*  ...                     \n");
    printf("\t\t                     .:-  ...                     \n");
    printf("\t\t                     ..-  ...                     \n");
    printf("\t\t                     ...  ....                    \n");
    printf("\t\t                     ... ..-..                    \n");
    printf("\t\t                 .....=. ..::. .                  \n");
    printf("\t\t               ..:::::.......:-::...              \n");
    printf("\t\t              ..::==:::--::-::::::..              \n");

    gotoxy(0, 2);
    printHalfScreen("Menu:", true, false);
    printHalfScreen("1. Autocomplete", true, false);
    printHalfScreen("2. Display Dictionary", true, false);
    printHalfScreen("3. Print Trie", true, false);
    printHalfScreen("4. Insert new word with weight", true, false);
    printHalfScreen("5. Delete Word", true, false);
    printHalfScreen("6. Load reference file", true, false);
    printHalfScreen("0. Exit", true, false);
}

void handlePrintTrie(TrieNode *root)
{
    while (true)
    {
        system("cls");
        printGridUI("MENU PRINT TRIE");

        gotoxy(0, 2);
        printHalfScreen("Trie:", true, false);
        printHalfScreen("1. Print All", true, false);
        printHalfScreen("2. Print with Prefix", true, false);
        printHalfScreen("3. Back", true, false);
        printHalfScreen("Enter your choice: ", true, false);
        bool isLast[MAX_WORD_LENGTH] = {0};
        char prefix[2] = " ";
        int choice;
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printHalfScreen("Press any key to continue...", false, false);
            gotoxy(0, 3);
            printTrie(root, prefix, isLast, 0);
            getch();
            break;
        case 2:
            printHalfScreen("Enter prefix: ", false, false);
            scanf("%s", prefix);
            printHalfScreen("Press any key to continue...", false, false);
            gotoxy(0, 3);
            printTrieWithPrefix(root, prefix);
            getch();
            break;
        case 3:
            return;
            break;

        default:
            printHalfScreen("Invalid choice. Please try again.", true, false);
            break;
        }
    }
}

void handlePrintDictionary(TrieNode *root)
{
    while (true)
    {
        system("cls");
        printGridUI("MENU DISPLAY DICTIONARY");
        gotoxy(0, 2);
        printHalfScreen("Dictionary:", true, false);
        printHalfScreen("1. Print All", true, false);
        printHalfScreen("2. Print with Prefix", true, false);
        printHalfScreen("3. Back", true, false);
        printHalfScreen("Enter your choice: ", true, false);
        int choice;
        scanf("%d", &choice);
        char prefix[2] = "";
        switch (choice)
        {
        case 1:
            printHalfScreen("Press any key to continue...", false, false);
            gotoxy(0, 3);
            displayDictionary(root, prefix);
            getch();
            break;
        case 2:
            printHalfScreen("Enter prefix: ", true, false);
            scanf("%s", prefix);
            printHalfScreen("Press any key to continue...", false, false);
            gotoxy(0, 3);
            displayDictionary(root, prefix);
            getch();
            break;
        case 3:
            return;
            break;
        default:
            printHalfScreen("Invalid choice. Please try again.", true, false);
            break;
        }
    }
}

void mainProcess()
{
    vocabNode *head = NULL;
    const char *initialDataFile = "../data/words.txt";
    loadInitialData(&head, initialDataFile);

    TrieNode *root = createNodeTrie();
    initializeTrieFromList(root, head);

    startingMenu();

    int choice;
    do
    {
        system("cls");
        displayMenu();
        printHalfScreen("Enter your choice: ", true, false);
        scanf("%d", &choice);
        printf("\n");

        switch (choice)
        {
        case 1:
            system("cls");
            printGridUI("MENU AUTOCOMPLETE");
            gotoxy(0, 4);
            handleAutocomplete(root);
            getch();
            break;
        case 2:
            handlePrintDictionary(root);
            break;
        case 3:
            handlePrintTrie(root);
            break;
        case 4:
            system("cls");
            printGridUI("MENU INSERT NEW WORD");
            handleInsertNewWord(root);
            getch();
            break;
        case 5:
            system("cls");
            printGridUI("MENU DELETE WORD");
            handleDeleteWord(root, &head);
            getch();
            break;
        case 6:
            system("cls");
            printGridUI("MENU LOAD REFERENCE FILE");
            updateDictionaryFromFile(&head);
            getch();
            break;
        case 0:
            system("cls");
            printGridUI("EXITING");
            printf("Exiting...\n");
            getch();
            break;
        default:
            printHalfScreen("Invalid choice. Please try again.\n", true, false);
            break;
        }
    } while (choice != 0);

    saveData(head, initialDataFile);
    freeList(head);
}

void csrs(void)
{
    COORD pos = {0, 1};
    SetConsoleCursorPosition(hConsole, pos);
}

void gotoxy(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(hConsole, c);
}

void initSystem()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    WindowsSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    WindowsSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void printGridUI(char Pesan[])
{
    initSystem();

    printCenterLine('|', 2);
    printc(Pesan);
    printLine('-');
}

COORD ReadResolution()
{
    COORD Result;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    Result.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    Result.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return Result;
}

void printc(char Pesan[])
{
    int usedPos = (WindowsSize.X - strlen(Pesan)) / 2;
    if (usedPos > 1)
    {
        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        COORD cursorPos = consoleInfo.dwCursorPosition;
        gotoxy(usedPos, cursorPos.Y + 1);
    }
    printf("%s\n", Pesan);
}

void printLine(char line)
{
    for (int i = 0; i < WindowsSize.X; i++)
    {
        printf("%c", line);
    }
    printf("\n");
}
void printCenterLine(char line, int StartPos)
{
    for (int i = StartPos; i < WindowsSize.Y - 1; i++)
    {
        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        gotoxy((WindowsSize.X / 2) - 2, i);
        printf("%c", line);
    }
    gotoxy(0, 0);
}

void printHalfScreen(char Pesan[], bool isNewLine, bool cancelEnter)
{
    int usedPos = (WindowsSize.X) / 2;
    if (usedPos > 1)
    {
        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        int cursorPos = consoleInfo.dwCursorPosition.Y;
        if (isNewLine && !cancelEnter)
            cursorPos++;
        if (cancelEnter)
        {
            cursorPos--;
        }
        gotoxy(usedPos, cursorPos);
    }
    printf("%s", Pesan);
}

void startingMenu()
{
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\t\t     ___      __    __  .___________.  ______     ______   ______   .___  ___. .______    __       _______ .___________. _______       \n");
    printf("\t\t    /   \\    |  |  |  | |           | /  __  \\   /      | /  __  \\  |   \\/   | |   _  \\  |  |     |   ____||           ||   ____| \n");
    printf("\t\t   /  ^  \\   |  |  |  | `---|  |----`|  |  |  | |  ,----'|  |  |  | |  \\  /  | |  |_)  | |  |     |  |__   `---|  |----`|  |__       \n");
    printf("\t\t  /  /_\\  \\  |  |  |  |     |  |     |  |  |  | |  |     |  |  |  | |  |\\/|  | |   ___/  |  |     |   __|      |  |     |   __|     \n");
    printf("\t\t /  _____  \\ |  `--'  |     |  |     |  `--'  | |  `----.|  `--'  | |  |  |  | |  |      |  `----.|  |____     |  |     |  |____      \n");
    printf("\t\t/__/     \\__\\ \\______/      |__|      \\______/   \\______| \\______/  |__|  |__| | _|      |_______||_______|    |__|     |_______|\n");

    printf("\t\t\t\t\t\t\t____    __    ____  ______   .______       _______         \n");
    printf("\t\t\t\t\t\t\t\\   \\  /  \\  /   / /  __  \\  |   _  \\     |       \\  \n");
    printf("\t\t\t\t\t\t\t \\   \\/    \\/   / |  |  |  | |  |_)  |    |  .--.  |    \n");
    printf("\t\t\t\t\t\t\t  \\            /  |  |  |  | |      /     |  |  |  |      \n");
    printf("\t\t\t\t\t\t\t   \\    /\\    /   |  `--'  | |  |\\  \\----.|  '--'  |   \n");
    printf("\t\t\t\t\t\t\t    \\__/  \\__/     \\______/  | _| `._____||_______/     \n");

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\t\t\t\t\t\t\tbefore we start, please maximize your console window\n");
    getch();
}
