#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

unsigned char work_buffer[256];
unsigned char ok_buf[256];
int center_letter;

int main(int argc, char *argv[])
{
  FILE *inf, *outf;
  char *c;
  int all_seven_flag;
  int use_popular = 0;
  int l_index = 1;
  
  if ( argc < 2 ) {
    printf("usage: ./sbs [-p] <seven-letters>\n");
    printf(" where the first letter must be the center letter\n");
    exit(0);
  }
  if ( argv[l_index][0] == '-' && argv[l_index][1] == 'p' ) {
    l_index += 1;
    use_popular = 1;
  }
  
  center_letter = (int)argv[l_index][0];

  c = &argv[l_index][0];
  while ( *c != 0 ) {
    ok_buf[*c] = 1;
    c += 1;
  }

  if ( use_popular ) {
    inf = fopen("popular.txt", "r");
  } else {
    inf = fopen("sbs_words.txt", "r");
  }
  
  if ( !inf ) { printf("%s not found\n",use_popular ? "popular.txt" : "sbs_words.txt"); exit(0); }
  
  while ( fgets(work_buffer,sizeof(work_buffer), inf) ) {
    int len;

    if ( strlen(work_buffer) == 0 ) continue;
    c = strchr(work_buffer,'\n'); if ( c ) *c = 0;
    len = strlen(work_buffer);

    if ( len < 4 ) continue;
#if 0
    c = work_buffer;
    while ( *c != 0 ) {
      *c = tolower(*c);
      c += 1;
    }
#endif // 0

    /* must contain center letter */
    if ( !strchr(work_buffer, center_letter) ) continue;

    /* can only use these letters */
    c = work_buffer;
    while ( *c != 0 ) {
      if ( !ok_buf[*c] ) goto next;
      c += 1;
    }

/* display * only if contains all letters */
   c = &argv[l_index][0];
   all_seven_flag = 0;
   while ( *c != 0 ) {
    if ( !strchr(work_buffer, *c) ) goto onward;
    c += 1;
   }
   all_seven_flag = 1;

onward:;
    if ( all_seven_flag )
      printf("       * %s\n",work_buffer);
    else
      printf("       %s\n",work_buffer);

/* add ed if necessary */
  if ( strchr(argv[l_index],'e') && strchr(argv[l_index],'d') ) {
	int l;
	char ne[32];
	int rule=0;
	strcpy(ne,work_buffer);
	c = ne;
	l = strlen(c);
	if ( c[l-1] == 'e' ) {
	  c[l] = 'd'; c[l+1]=0;
	  rule=1;
	} else {
	  if ( !(c[l-2] == 'e' && c[l-1] == 'd') ) {
	    c[l] = 'e'; c[l+1]='d'; c[l+2] = 0;
	    rule=2;
	  }
	}
	if ( rule && strlen(c) <= 8 ) {
	  printf("ed(%d): %s\n",rule,c);
	}
  } // ed check

  next:;
  }
  fclose(inf);
  return 0;
}
