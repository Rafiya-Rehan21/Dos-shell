#include <Windows.h>
#include "Key.h"

class Keyboard {
private:
    INPUT_RECORD record;
    HANDLE hStd = NULL;
    bool readKey();

public:
    Key getKey();
};