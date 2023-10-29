#ifndef MAMAN14_TOKEN_H
#define MAMAN14_TOKEN_H

enum commands
{
    /* Commands */
    mov = 0,
    cmp = 1,
    add = 2,
    sub = 3,
    lea = 6,
    not = 4,
    clr = 5,
    inc = 7,
    dec = 8,
    jmp = 9,
    bne = 10,
    red = 11,
    prn = 12,
    jsr = 13,
    rts = 14,
    stop = 15,

    /* Registers */
    r0 = 16,
    r1 = 17,
    r2 = 18,
    r3 = 19,
    r4 = 20,
    r5 = 21,
    r6 = 22,
    r7 = 23,

    /* Other */
    label = 24,
    number = 25,
    labelCall = 26,
    string = 27,
    stringData = 28,
    data = 29,
    externL = 30,
    entryL = 31,
    non = 32
};

typedef struct
{
    enum commands tokenType;
    char* value;
    unsigned int lineNumber;
} Token;

#endif
