// strutils.cpp
#include "strutils.h"
#include <string.h> 
#include <ctype.h>
#include <limits.h> 
#include <stdlib.h> 
//#include <stdio.h> 

/**
* @brief Reverses a string.
* @param str Pointer to the string to reverse.
 */
void str_reverse(char *str) {
    if (str == NULL) {
        return;
    }
    int length = strlen(str);
    int i, j;
    char temp;
    for (i = 0, j = length - 1; i < j; i++, j--) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

/**
* @brief Remove whitespace from the beginning and end of the string.
* @param str Pointer to the string to trim.
 */
void str_trim(char *str) {
    if (str == NULL) {
        return;
    }

    char *start = str; // Lưu con trỏ ban đầu của chuỗi
    // char *end;
    // int len = strlen(str);

    //Handling leading whitespace
    while (isspace((unsigned char)*start)) {
        start++;
    }

    //If the string is empty after trimming leading whitespace
    if (*start == '\0') {
        *str = '\0'; // empty string
        return;
    }

    // Handling ending whitespace
    char *end = start + strlen(start) - 1;
    while (end >= start && isspace((unsigned char)*end)) {
        end--;
    }

    *(end + 1) = '\0';

    // Move string to beginning if leading whitespace is removed
    if (start  != str) { // Compare the original pointer with the shifted pointer
        memmove(str, start, strlen(start) + 1);
    }
}

/**
* @brief Safely converts a string to an integer.
* @param str The string to convert.
* @param out_num A pointer to a variable to store the converted integer.
* @return 0 if the conversion was successful, -1 if there was an error (e.g. the string was invalid).
 */
int str_to_int(const char *str, int *out_num) {
    if (str == NULL || out_num == NULL) {
        return -1; // Error: null pointer
    }

    char *endptr;
    long val = strtol(str, &endptr, 10); // Convert string to long

    // Check for error conditions:
    // 1. No character converted (endptr still points to str)
    // 2. There is a non-numeric character after the number (endptr does not point to a null character)
    // 3. The value is out of range for int
    if (endptr == str || *endptr != '\0' || val > INT_MAX || val < INT_MIN) {
        return -1; // Conversion error or overflow/underflow
    }

    *out_num = (int)val; // Assign the converted value
    return 0; // Success
}

