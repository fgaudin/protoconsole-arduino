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

void padLeft(char padChar, char * str, int max)
{
    int strLen = strlen(str);
    int padding = max - strLen;
    if (padding > 0) {
        for (int i=strLen - 1; i>=0; i--){
            str[i+padding] = str[i];
        }
        memset(str, padChar, padding);
        str[max] = '\0';
    }
}

void metricfy(long value, char * buffer)
{
    char unit = ' ';
    double formatted = value;
    unsigned char width = 1;
    unsigned char precision = 0;

    if (abs(value) >= 1000000000) {
        unit = 'G';
        formatted = (double) value / 1000000000;
    } else if (abs(value) >= 1000000) {
        unit = 'M';
        formatted = (double) value / 1000000;
    } else if (value >= 10000 || value <= -1000) {
        unit = 'k';
        formatted = (double) value / 1000;
    }
    if (abs(formatted) < 10 && unit != ' ') {
        precision = 1;
    }
    dtostrf(formatted, width, precision, buffer);
    if (unit != ' ') {
        buffer[strlen(buffer)] = unit;
        buffer[strlen(buffer)] = '\0';
    }
}

void timify(long time, char * buffer)
{
    unsigned int utime = abs(time);
    const char * sign = time > 0?"-":"";
    int seconds = utime % 60;
    int minutes = (utime / 60) % 60;
    int hours = (utime / 3600) % 6;  // kerbin day is 6 hrs
    int days = (utime / (3600 * 6));
    sprintf(buffer, "%s%d:%02d:%02d:%02d", sign, days, hours, minutes, seconds);
}
