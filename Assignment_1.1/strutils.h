// strutils.h
#ifndef STRUTILS_H
#define STRUTILS_H

/**
* @brief Reverses a string.
* @param str Pointer to the string to reverse.
 */
void str_reverse(char *str);

/**
* @brief Remove whitespace from the beginning and end of the string.
* @param str Pointer to the string to trim.
 */
void str_trim(char *str);

/**
* @brief Safely converts a string to an integer.
* @param str The string to convert.
* @param out_num A pointer to a variable to store the converted integer.
* @return 0 if the conversion was successful, -1 if there was an error (e.g. the string was invalid).
 */
int str_to_int(const char *str, int *out_num);

#endif // STRUTILS_H