// extract answsers from saved html file - A TOTAL CHEAT !!!

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CACHE_SIZE 4096

// argv1 date - Wednesday ?
// argv2 file - answers.txt ?
int main ( int argc, char *argv[] ) {
  FILE *inf;
  int idx = 0;
  char f[CACHE_SIZE];
  int i, j, k;
  char *c;

  if ( argc < 3 ) {
    printf("Usage: ./extract_answers date saved-html\n");
    exit(0);
  }
  
  memset(f,0,CACHE_SIZE);
  
  inf = fopen(argv[2],"r");
  if ( !inf ) {
    printf("can't open <%s>\n",argv[2]);
    exit(0);
  }
  
 loop:;
  // make room
  if ( idx == CACHE_SIZE-1 ) {
    for ( i = 0, j = 1 ; i < CACHE_SIZE ; i++, j++ ) {
      f[i] = f[j];
    }
    idx -= 1;
  }
  
  if ( EOF == (k=fgetc(inf)) ) {
    printf("At EOF\n");
    goto done;
  }
  f[idx++] = (char)k;

  if ( strncmp(f,argv[1],strlen(argv[1])) ) {
    goto loop;
  }
  
 done:;
  c = strstr(f,"\"pangrams\"");
  if ( !c ) {
    printf("no pangrams???\n");
    exit(0);
  }

  printf("pangrams\n");
  c += 11;
  while ( *c != ']' ) {
    printf("%c",*c);
    c += 1;
  }
  printf("]\n");

  c = strstr(f,"\"answers\"");
  if ( !c ) {
    printf("no answers ???\n");
    exit(0);
  }

  printf("answers\n");
  c += 10;
  while ( *c != ']' ) {
    printf("%c",*c);
    c += 1;
  }
  printf("]\n");

  return 0;
}
