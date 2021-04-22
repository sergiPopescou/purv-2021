#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void funkcija()
{
    char * c;
    c = malloc(3);
    int i;
    for(i=0; i<3;i++)   //program je izlazio iz opsega hepa 
                        // u ispravku je modifikovan da ne izlazi iz opsega alociranog dijela znaci iz prva 3 bajta
        c[i] = 'a';
    printf("\n %s \n", c);
    return;
}

int main(int argc, char **argv)
{
	funkcija();
	return 0;
}
