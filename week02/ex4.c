#include <stdio.h>
#include <string.h>
#include <ctype.h> 

void printt(char str[],int counted[]){

    for(int i = 0; ;i++){
        if(str[i]=='\0')
        break;
         char c = str[i];
        printf("%c:%d, ", c, counted[c - 'a']);
        
    }
}

int count( char str[], char c) {
    int count = 0;
    int len = strlen(str);

    c = tolower(c);

    for (int i = 0; i < len; i++) {
        if (tolower(str[i]) == c) {
            count++;
        }
    }

    return count;
}

void countAll(char str[]) {
    int len = strlen(str);
    int counted[30]={0};

    for (int i = 0; i < len; i++) {
        char c = str[i];

        c = tolower(c);
        counted[c - 'a'] = count(str,c);

        
    }
    printt(str, counted);

    printf("\n");
}

int main() {
   

     char inputString[256];
     scanf("%s", inputString);

    printf("Input: %s\n", inputString);
    countAll(inputString);

    return 0;
}
