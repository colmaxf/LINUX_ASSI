// main.cpp
#include <iostream>
#include <string.h>
#include "strutils.h" 

using namespace std;

int main() {
    // Test str_reverse
    char s1[] = "hello world";
    cout << "Original: " << s1 << endl;
    str_reverse(s1);
    cout << "Reversed: " <<s1 << endl;

    char s2[] = "abc";
    cout << "Original: " <<  s2 << endl;
    str_reverse(s2);
    cout << "Reversed: " << s2 << endl;

    char s3[] = "";
    cout << "Original: " << s3<< endl;
    str_reverse(s3);
    cout << "Reversed: " << s3<< endl;

    // Test str_trim
    char t1[] = "   Hello World   ";
    cout << "Original: " << t1<< endl;
    str_trim(t1);
    cout <<"Trimmed: " << t1<< endl;

    char t2[] = "NoTrim";
    cout << "Original: " << t2<< endl;
    str_trim(t2);
    cout <<"Trimmed: " << t2<< endl;

    char t3[] = "   ";
    cout << "Original: " << t3<< endl;
    str_trim(t3);
    cout <<"Trimmed: " << t3<< endl;

    char t4[] = "";
    cout << "Original: " << t4<< endl;
    str_trim(t4);
    cout <<"Trimmed: " << t4<< endl;

    // Test str_to_int
    const char *n1 = "12345";
    int num1;
    if (str_to_int(n1, &num1) == 0) {
        cout << "Converted " << n1 << "to integer " << num1<< endl;
    } else {
        cout << "Failed to convert " << n1 <<" to integer."<< endl;
    }

    const char *n2 = "-987";
    int num2;
    if (str_to_int(n2, &num2) == 0) {
        cout << "Converted " << n2 << "to integer " << num2<< endl;
    } else {
        cout << "Failed to convert " << n2 <<" to integer."<< endl;
    }

    const char *n3 = "abc";
    int num3;
    if (str_to_int(n3, &num3) == 0) {
        cout << "Converted " << n3 << "to integer " << num3<< endl;
    } else {
        cout << "Failed to convert " << n3 <<" to integer."<< endl;
    }

    const char *n4 = "123xyz";
    int num4;
    if (str_to_int(n4, &num4) == 0) {
        cout << "Converted " << n4 << "to integer " << num3<< endl;
    } else {
        cout << "Failed to convert " << n4 <<" to integer."<< endl;
    }

    const char *n5 = "   456   ";
    int num5;
    if (str_to_int(n5, &num5) == 0) {
        cout << "Converted " << n5 << "to integer " << num5<< endl;
    } else {
        cout << "Failed to convert " << n5 <<" to integer. (Expected failure due to spaces)\n"<< endl;
    }

    const char *n6 = "2147483647"; // INT_MAX
    int num6;
    if (str_to_int(n6, &num6) == 0) {
        cout << "Converted " << n6 << "to integer " << num6<< endl;
    } else {
        cout << "Failed to convert " << n6 <<" to integer. (Should succeed)\n"<< endl;
    }

    const char *n7 = "2147483648"; // INT_MAX + 1 
    int num7;
    if (str_to_int(n7, &num7) == 0) {
        cout << "Converted " << n7 << "to integer " << num7<< endl;
    } else {
        cout << "Failed to convert "<< n7 <<" to integer. (Expected failure due to overflow)\n"<< endl;
    }

    return 0;
}