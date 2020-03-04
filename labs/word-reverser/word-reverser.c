#include <stdio.h>

void reverse(int length, char *word){
    
    int i, tmp;

    for (i = 0;  i < length/2; i++) {
	tmp = word[i];
	word[i] = word[length - i - 1];
	word[length - i - 1] = tmp;
    }
    
}

int main(int argc, char **argv)
{
    
    int length = 0;
    int c;
    char word[256];
    
    while ((c = getchar()) != EOF) {
        if (c == '\n'){
            word[length] = '\0';
            reverse(length, word);
            length = 0;
            printf("%s\n", word);
        } else {
            word[length] = c;
            length++;
        }
    }
    
    return 0;
}

