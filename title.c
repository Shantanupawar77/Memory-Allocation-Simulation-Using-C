#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int title()
{
    FILE *ptr;
    char ch;
    ptr = fopen("title.txt", "r");

    if (NULL == ptr)
    {
        printf("file can't be opened \n");
    }

    do
    {
        ch = fgetc(ptr);
        printf("%c", ch);
    } while (ch != EOF);

    fclose(ptr);
    return 0;
}