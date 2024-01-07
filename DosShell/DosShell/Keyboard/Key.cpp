#include <cctype>
#include "Key.h"

void Key::setCode(char c, int code){
    isPrintable = isprint(c);
    isPrintable ? character = c : keyCode = code;
}

Key::Key(char c, int code){
    setCode(c, code);
}