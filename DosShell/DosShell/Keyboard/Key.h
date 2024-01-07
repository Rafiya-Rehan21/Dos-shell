class Key {
public:
    bool isPrintable;

    union {
        char character;
        int keyCode;
    };

    void setCode(char, int);
    Key(char, int);
};