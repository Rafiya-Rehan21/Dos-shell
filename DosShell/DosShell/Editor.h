#pragma once
#include <iostream>
#include <string>
#include <stack>

using namespace std;

// Node structure for doubly linked list
class Node {
public:
    string data;
    Node* prev;
    Node* next;
};

class UndoRedoElement {
public:
    Node* node;
    string data;

    UndoRedoElement(Node* node) : node(node), data(node->data) {}
};

class TextBuffer {
public:
    TextBuffer();
    ~TextBuffer();

    void insert(char c);
    void backspace();
    void append(char c);
    void newLine();
    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorUp();
    void moveCursorDown();
    void display();
    void undo();
    void redo();
    void add(char c);
    string save();
    void load(string str);

private:
    Node* head;        // Head of the doubly linked list
    Node* cursorLine;      // Cursor position in the list
    size_t cursorOffset = 0;
    stack<UndoRedoElement*> undoStack; // Stack for undo operations
    stack<UndoRedoElement*> redoStack; // Stack for redo operations
    bool doAppend = true;

    void addToUndoStack();
    void incrementCursor(int);
    void setCursor(int);
};

bool isCtrlPressed();
bool isKeyPressed(int vkey);
