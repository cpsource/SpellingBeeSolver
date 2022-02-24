// extract answsers from saved html file - A TOTAL CHEAT !!!

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define CACHE_START 32
#define CACHE_SIZE 4096

// true if an alpha character
char is_alpha[256];

void traceit(int line, char *c)
{
  int cnt = 30;
  int back = 10;
  
  c -= back;
  printf("line: %3d: ",line);
  while ( cnt-- ) {
    printf("%c",*c++);
  }
  printf("\n");
  printf("         ");
  for ( int i = 0 ; i < back ; i++ ) {
    printf(" ");
  }
  printf("*\n");
}

//  

  
// "pangrams":["
int is_form(char *str) {

  if ( *str != '"' ) return 0;

  //traceit(__LINE__,str);
  
  str += 1;
  if ( *str == 0 ) return 0;
  
  while ( is_alpha[*str] ) str += 1;

  if ( *str == 0 ) return 0;

  //traceit(__LINE__,str);
  
  if ( *str != '"' ) return 0;

  str += 1;
  if ( *str == 0 ) return 0;

  if ( *str != ':' ) return 0;

  str += 1;
  if ( *str == 0 ) return 0;
    
  if ( *str != '[' ) return 0;

  //traceit(__LINE__,str);
  
  return 1;
}

// argv1 file - answers.txt ?
int main ( int argc, char *argv[] ) {
  FILE *inf;
  int idx = CACHE_START;
  char f[CACHE_SIZE];
  int i, j, k;
  char *c,*d;
  int eof_flag = 0;
  
  if ( argc < 2 ) {
    printf("Usage: ./extract_answers saved-html\n");
    exit(0);
  }

  for ( i = 0 ; i < 256 ; i++ ) {
    if ( isalpha(i) ) {
      is_alpha[i] = 1;
    }
  }
  
  memset(f,0,CACHE_SIZE);
  
  inf = fopen(argv[1],"r");
  if ( !inf ) {
    printf("can't open <%s>\n",argv[1]);
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
  
  if ( !eof_flag && EOF == (k=fgetc(inf)) ) {
    //printf("At EOF\n");
    eof_flag = 1;
  }
  if ( !eof_flag ) {
    f[idx++] = (char)k;
  } else {
    if ( idx <= CACHE_START ) goto finis;
    for ( i = 0, j = 1 ; i < CACHE_SIZE ; i++, j++ ) {
      f[i] = f[j];
    }
    idx -= 1;
  }

  //traceit(__LINE__,&f[CACHE_START]);

  if ( !is_form(&f[CACHE_START]) ) {
    goto loop;
  }
  
 done:;
  d = &f[CACHE_START];
  while ( *d != ']' ) {
    printf("%c",*d);
    d += 1;
  }
  printf("]\n");
  goto loop;
  
 finis:;
  return 0;
}
