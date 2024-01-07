#include "Keyboard.h"

bool Keyboard::readKey(){
    DWORD read;

    if (hStd == NULL)
        hStd = GetStdHandle(STD_INPUT_HANDLE);

    while (true){
        if (ReadConsoleInputA(hStd, &record, 1, &read)){
            if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown && record.Event.KeyEvent.wRepeatCount < 2)
                break;
        }
        else {
            return false;
        }
    }

    return true;
}

Key Keyboard::getKey(){
    Key key(-1, 0);

    if (readKey()){
        key.setCode(record.Event.KeyEvent.uChar.AsciiChar, record.Event.KeyEvent.wVirtualKeyCode);
    }

    return key;
}