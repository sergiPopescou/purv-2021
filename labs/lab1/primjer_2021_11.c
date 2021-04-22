#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void funkcija()
{
    char * c;
    c = malloc(3);
    int i;
    for(i=0; i<0xffffffff;i++)
        c[i] = 'a';
    printf("\n %s \n", c);
    return;
}

int main(int argc, char **argv)
{
	funkcija();
	return 0;
}

