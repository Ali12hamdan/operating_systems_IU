#include <stdio.h>
#include <string.h>

int main() {
    char Instr[257];
    char character;
    int len = 0;

    printf("Enter a string with '.' ");

    while (1) {
        character = getchar();

        if (character == '\n') {
            break;
        }

        if (character == '.') {
            break;
        }

        if (len < 256) {
            Instr[len] = character;
            len++;
        }
    }

    Instr[len] = '\0';
    printf("In: \"%s\"\n", Instr);

    printf("Out: \"");
    for (int i = len - 1; i >= 0; i--) {
        printf("%c", Instr[i]);
    }
    printf("\"\n");

    return 0;
}