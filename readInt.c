#include <stdio.h>

int main(void)
{
    FILE* file = fopen ("input.txt", "r");
    char str1[10];
    int i = 0;
    
    while (!feof (file)) 
    {
        fscanf (file, "%d %s", &i, str1);  
        printf ("%d\n", i);
    }
    
    fclose (file);

    return 0;
}

