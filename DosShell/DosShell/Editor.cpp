#pragma once
#include "Editor.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <conio.h>
#include <windows.h>
#pragma comment(lib, "user32.lib")

using namespace std;

bool isCtrlPressed() {
    return GetAsyncKeyState(VK_CONTROL) & 0x8000;
}

void gotoxy(int x, int y) {
    COORD coordinates;
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

TextBuffer::TextBuffer() {
    head = new Node();
    cursorLine = head;
}

TextBuffer::~TextBuffer() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void TextBuffer::insert(char c) {
    cursorLine->data.insert(cursorOffset, 1, c);
    incrementCursor(1);
}

void TextBuffer::newLine() {
    Node* newNode = new Node();

    if (!doAppend) {
        newNode->data.append(cursorLine->data.substr(cursorOffset));
        cursorLine->data.erase(cursorOffset);
    }

    if (cursorLine->next != nullptr) {
        newNode->next = cursorLine->next;
        cursorLine->next->prev = newNode;
    }

    cursorLine->next = newNode;
    newNode->prev = cursorLine;
    cursorLine = newNode;
    cursorOffset = 0;
    doAppend = true;
}

void TextBuffer::incrementCursor(int inc) {
    if (doAppend) {
        cursorOffset = cursorLine->data.length();
        doAppend = false;
    }

    setCursor(cursorOffset + inc);
}

void TextBuffer::setCursor(int cursor) {
    if (cursor == cursorLine->data.length()) {
        doAppend = true;
        return;
    }

    if (cursor >= 0 && cursor <= cursorLine->data.length())
        cursorOffset = cursor;

}

void TextBuffer::add(char c) {
    addToUndoStack();

    if (doAppend)
        append(c);
    else
        insert(c);
}

void TextBuffer::backspace() {
    addToUndoStack();

    string* s = &cursorLine->data;

    if (!s->empty()) {
        if (doAppend) {
            s->pop_back();
        }
        else if (cursorOffset == 0 && cursorLine->prev != nullptr) {
            cursorLine->prev->data.append(cursorLine->data);
            cursorLine->prev->next = cursorLine->next;

            if (cursorLine->next != nullptr) {
                cursorLine->next->prev = cursorLine->prev;
            }

            Node* temp = cursorLine->prev;
            delete cursorLine;
            cursorLine = temp;
        }
        else {
            s->erase(cursorOffset - 1, 1);
            cursorOffset--;
        }
    }
    else if(cursorLine->prev!=nullptr) {
        cursorLine->prev->next = cursorLine->next;

        if (cursorLine->next != nullptr) {
            cursorLine->next->prev = cursorLine->prev;
        }

        Node* temp = cursorLine->prev;
        delete cursorLine;
        cursorLine = temp;
    }
}

void TextBuffer::append(char c) {
    cursorLine->data.append(1, c);
}

void TextBuffer::moveCursorLeft() {
    incrementCursor(-1);
}

void TextBuffer::moveCursorRight() {
    incrementCursor(1);
}

void TextBuffer::moveCursorUp() {
    if (cursorLine->prev != nullptr) {
        cursorLine = cursorLine->prev;

        if (!doAppend) {
            if (cursorOffset > cursorLine->data.length())
                doAppend = true;
        }
    }
}

void TextBuffer::moveCursorDown() {
    if (cursorLine->next != nullptr) {
        cursorLine = cursorLine->next;

        if (!doAppend) {
            if (cursorOffset > cursorLine->data.length())
                doAppend = true;
        }
    }
}


void TextBuffer::display() {
    int linesBeforeCursor = 0;
    Node* current = head->next;
    bool cursorLineSeen = false;

    gotoxy(0, 0);
    cout << head->data;

    while (current != nullptr) {
        if (current == cursorLine)
            cursorLineSeen = true;

        cout << endl;
        cout << current->data;
        current = current->next;

        if (!cursorLineSeen)
            linesBeforeCursor++;
    }

    if (cursorLine != head)
        linesBeforeCursor++;
    else
        linesBeforeCursor = 0;

    if (!doAppend)
        gotoxy(cursorOffset, linesBeforeCursor);
    else
        gotoxy(cursorLine->data.length(), linesBeforeCursor);
}

void TextBuffer::addToUndoStack() {
    undoStack.push(new UndoRedoElement(cursorLine));
}

void TextBuffer::undo() {
    if (!undoStack.empty()) {
        UndoRedoElement* elem = undoStack.top();

        redoStack.push(new UndoRedoElement(elem->node));
        undoStack.pop();

        elem->node->data = elem->data;
    }
}

void TextBuffer::redo() {
    if (!redoStack.empty()) {
        UndoRedoElement* elem = redoStack.top();

        undoStack.push(new UndoRedoElement(elem->node));
        redoStack.pop();

        elem->node->data = elem->data;
    }
}

bool isKeyPressed(int vkey) {
    return (GetAsyncKeyState(vkey) & 0x8001) && !isCtrlPressed();
}

string TextBuffer::save() {
    Node* current = head->next;
    string str;
    str=head->data;

    while (current != nullptr) {
        str += '\n';
        str += current->data;
        current = current->next;
    }

    return str;
}

void TextBuffer::load(string str) {
    if (str.empty()) {
        return;
    }
    else {
        int lines = count(str.begin(), str.end(), '\n');
        if (lines == 0) {
            head->data.append(str);
        }
        else {
            string sub = str;

            for (int i = 0; i < lines; i++) {
                cursorLine->data.append(sub.substr(0, sub.find_first_of('\n')));
                sub = sub.substr(sub.find_first_of('\n') + 1);
                newLine();
            }

            cursorLine->data.append(sub);
        }
    }
}