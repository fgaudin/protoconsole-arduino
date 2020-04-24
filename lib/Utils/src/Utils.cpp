#include "Utils.h"
#include <Arduino.h>

void padRight(char padChar, char * str, int max)
{
    int strLen = strlen(str);
    int padding = max - strLen;
    if (padding > 0) {
        memset(&str[strLen], padChar, padding);
        str[max] = '\0';
    }
}

void metricfy(long value, char * buffer)
{
    char unit = ' ';
    if (abs(value) >= 1000000000) {
        unit = 'G';
        value = value / 1000000;
    } else if (abs(value) >= 1000000) {
        unit = 'M';
        value = value / 1000;
    } else if (abs(value) >= 1000) {
        unit = 'k';
        value = value / 1000;
    }
    ltoa(value, buffer, 10);
    if (unit != ' ') {
        buffer[strlen(buffer)] = unit;
        buffer[strlen(buffer)] = '\0';
    }
}