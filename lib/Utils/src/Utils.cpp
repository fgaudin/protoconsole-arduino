#include "Utils.h"
#include <Arduino.h>

void padRight(char padChar, char * str, int max)
{
    int strLen = strlen(str);
    int padding = max - strLen;
    if (padding > 0) {
        memset(&str[strLen], padChar, padding);
        str[max] = 0;
    }
}