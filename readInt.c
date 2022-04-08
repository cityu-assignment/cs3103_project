#include <stdio.h>

int main(void)
{
  FILE* file = fopen ("input.txt", "r");
  int i = 0;

  fscanf (file, "%d", &i);    
  while (!feof (file))
    {  
      printf ("%d\n", i);
      fscanf (file, "%d", &i);      
    }
  fclose (file);

    return 0;
}
