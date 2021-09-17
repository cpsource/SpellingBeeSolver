#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

unsigned char work_buffer[256];

void print_tri(char *str)
{
#if 1
  char res[5];

  for ( int i = 0 ; i < 4 ; i++ ) {
    res[i] = str[i];
  }
  res[4] = 0;

  printf("%s\n",res);
#else
  int i = 0;
  char res[4] = { 0 };

  while ( (res[0]=str[i]) && (res[1]=str[i+1]) && (res[2]=str[i+2]) ) {
    printf("%s\n",res);
  i += 1;
  }
#endif
}  

int main(int argc, char *argv[])
{
  FILE *inf;
  char *c;
  int all_seven_flag;

  if ( argc != 1 ) {
    printf("usage: ./tri\n");
    exit(0);
  }

  inf = fopen("sbs_words.txt", "r");
  if ( !inf ) { printf("sbs_words.txt not found\n"); exit(0); }

  while ( fgets(work_buffer,sizeof(work_buffer), inf) ) {
    int len;

    if ( strlen(work_buffer) == 0 ) continue;
    c = strchr(work_buffer,'\n'); if ( c ) *c = 0;
    len = strlen(work_buffer);

    print_tri(work_buffer);

    }
  fclose(inf);
  return 0;
}

