/* Including libraries needed to throuw the error message: */
#include <stdio.h>
#include <stdlib.h>

void printError(const char *error);
int isValidBase(int base);
int charToValue(char ch);
int toDecimal(int base);
char valueToChar(int digit);
void printDigits(int num, int base);
void fromDecimal(int num, int base);

int main() {
    /*
     * Collects 3 user inputs:
     * 1. The source base ( must be a positive integer between 2 and 16! )
     * 2. The target base ( must be a positive integet between 2 and 16! )
     * 3. The number in the source base ( must represent a positive integer! If (source <= 10), must consist of digits alone!
     * Otherwise, can contain a combination of lower-case letters and digits, but nothing else! )
     * 
     * Prints the representation of the given number in the target base, and returns its representation in decimal base.
     */

    int source, target, num;

    printf("enter the source base:\n");
    scanf("%d", &source);
    /* Validating that source is a valid base: */
    if (isValidBase(source) == 0) { printError("Invalid source base!"); }

    printf("enter the target base:\n");
    scanf("%d", &target);
    /* Validating that target is a valid base: */
    if (isValidBase(target) == 0) { printError("Invalid target base!"); }

    printf("enter a number in base %d:\n", source);
    num = toDecimal(source);

    printf("the number in base %d is:\n", target);
    fromDecimal(num, target);
    printf("\n");

    return num;
}

void printError(const char *error) {
    /*
     * Given an error message, prints said message to both the standard data stream and the standard error data stream,
     * and then terminates the method.
     */
    fprintf(stderr, "%s\n", error); /* Printing the error message to the error standard data stream, as that's where I believe
                                       the message belongs. */
    exit(EXIT_FAILURE);
}

int isValidBase(int base) {
    /*
     * Given an array of the valid bases, return 1 if the given base is one of them, and 0 otherwise.
     */
    return base >= 2 && base <= 16;
}

int charToValue(char ch) {
    /*
     * Given a character (in 2 <= base <= 16), return the decimal representation of the character.
     */
    if (ch >= '0' && ch <= '9') { return ch - '0'; }
    else if (ch >= 'a' && ch <= 'f') { return ch - 'a' + 10; }
    else { printError("Invalid source base!"); }
    return -1; /* Would never be reached due to the returns in the 'if' / 'else if' statements and the printError in the
                * 'else' statemet.
                */
}

int toDecimal(int base) {
    int res = 0;
    int value;
    char ch;

    /* getting rid of the '\n' character from the last scanf: */
    getchar();

    ch = getchar();

    while (ch != '\n' && ch != EOF) {
        value = charToValue(ch);
        /* Dealing with the possibility that ch isn't a valid representation in the given base: */
        if (value < 0 || value >= base) { printError("Invalid source base!"); }
        /* Yes, the condition of 'value < 0' is redundant, but I left it here to possibly ease the code's clarity. */

        res = res * base + value;
        ch = getchar();
    }

    return res;
}

char valueToChar(int value) {
    /*
     * Given a value, converts it to its representation as a character.
     */
    if (value <= 9) { return '0' + value; }
    else { return 'a' + value - 10; }
}

void printDigits(int num, int base) {
    /*
     * Given a number, printing its representation in the given base.
     */
    int digit;

    if (num == 0) { return; }
    digit = num % base;
    printDigits((int) num / base, base);
    putchar(valueToChar(digit));    /* Printing in reverse order so that the left-most printed digit represents base**0. */
}

void fromDecimal(int num, int base) {
    /*
     * Given a decimal number, converts it to its representation in the given base.
     */
    if (num == 0) {
        putchar('0');
        return; /* To avoid putting '0' twice in this case. */
    }
    printDigits(num, base);
}
