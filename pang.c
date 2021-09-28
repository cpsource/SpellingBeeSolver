/* build sbs_words.txt from words.txt */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

unsigned char ok_buf[256];
unsigned char symbols[256];
char in_name[64] = {"sbs_words.txt"};
int word_count;

/* show the hive, but randomize letters 2->7 to obscure answers */
void show_hive( char *p )
{
  char syms[256];
  char *c = p;
  char letters[8] = {0};
  char rletters[8] = {0};
  int *r_init = (int *)p;
  
  char *d = letters;
  int r;
  
  srand(*r_init);
  memset(syms,0,256);

  while ( *c != 0 ) {
    if ( syms[*c] ) goto next;
    //printf("%c",*c);
    *d++ = *c; // build letters array
    syms[*c]=1;
  next:
    c += 1;
  }

  // keep center of hive constant, build rletters
  d = rletters;
  *d++ = letters[0]; letters[0] = ' ';

  while ( 1 ) {
    r = rand() % 6; // gives # [0->5]
    r += 1; // now between [1->6]

    if ( letters[r] == ' ' ) goto next1;

    // new letter here
    *d++ = letters[r];
    letters[r] = ' ';
    
  next1:;
    if ( 0==strcmp(letters,"       ") ) break;
  }
  
  printf("%s ",rletters);
}

/* count the number of unique symbols */
int is_uniq(char *str)
{
  char uniq[256];
  int u = 0;
  char *c;
  int i;

  c = str;
  memset(uniq,0,256);
  while (*c) {
	uniq[*c] = 1;
	c += 1;
  }

  for ( i = u = 0; i < 256 ; i++ ) {
	if ( uniq[i] ) u += 1;
  }

  return u;
}

/* find the number of actual words */
int find_count(int argc, char *argv[])
{
  char *c;
  int trace_flag = 0;
  unsigned char work_buffer[256];
  char center_letter;
  FILE *inf;
  int word_12_count = 0;
  int pangram_count = 0;

  word_count = 0;
  
  if ( trace_flag ) printf("cp0: argc = %d, argv[1] = %s\n",
			   argc,argv[1]);
  
#if 0
  if ( argc < 2 ) {
    printf("usage: ./sbs <seven-letters>\n");
    printf(" where the first letter must be the center letter\n");
    exit(0);
  }
#endif
  center_letter = (int)argv[1][0];

  memset(ok_buf,0,sizeof(ok_buf));
  c = &argv[1][0];
  while ( *c != 0 ) {
    ok_buf[*c] = 1;
    c += 1;
  }

  inf = fopen(in_name,"r");
  if ( !inf ) { printf("%s not found\n",in_name); exit(0); }

  /* scan entire word list file */
  while ( fgets(work_buffer,sizeof(work_buffer), inf) ) {
    int len;

    if ( strlen(work_buffer) == 0 ) continue;
    c = strchr(work_buffer,'\n'); if ( c ) *c = 0;
    len = strlen(work_buffer);

    /* must contain center letter */
    if ( !strchr(work_buffer, center_letter) ) continue;

    if ( trace_flag ) printf("cp1: center_letter found <%c>\n",center_letter);

    /* can only use these letters */
    c = work_buffer;
    while ( *c != 0 ) {
      if ( !ok_buf[*c] ) goto next;
      c += 1;
    }

    if ( trace_flag ) printf("cp2:\n");

    /* increment word_count */
    word_count += 1;

    /* increment 12 count ??? */
    if ( 12 == strlen(work_buffer) ) {
      word_12_count += 1;
    }

    if ( trace_flag ) printf("cp3:\n");

    /* increment pangram_count ??? */
    c = &argv[1][0];
    while ( *c != 0 ) {
      if ( trace_flag ) printf("cp4: %c\n",*c);
      if ( !strchr(work_buffer, *c) ) goto next;
      c += 1;
    }
    pangram_count += 1;
    
    if ( trace_flag ) printf("cp4: all letters ok\n");

#if 0
    /* does word in sbs_words.txt contain all seven letters ??? */
    c = &argv[1][0];
    while ( *c != 0 ) {
      if ( !strchr(work_buffer, *c) ) goto onward;
      c += 1;
    }
    if ( trace_flag ) printf("cp5: %s %s\n",work_buffer,argv[1]);
  onward:;
#endif

  next:;
  } /* do for all words in in_file */
  fclose(inf);

  //if ( trace_flag ) printf("cp3: word_count = %d\n",
  // word_count);

  if ( trace_flag ) printf("cp6: pangram_count = %d\n",
			   pangram_count);
  //return word_12_count;
  //return word_count;
  return pangram_count;
}

int main(int argc, char *argv[])
{
  FILE *inf;
  char *c;
  int symbol_count;
  int i;
  char tmp_buffer[256];
  char *arglist[2] = { "pang", tmp_buffer };
  int wc;

  if ( argc == 2 ) {
    if ( argv[1][0] == '-' &&
	 argv[1][1] == 'p' ) {
      strcpy(in_name,"popular.txt");
    } else {
      printf("Usage: ./pang [-p]\n"); exit(0);
    }
  }

#if 0
  printf("using %s\n",in_name);
  exit(0);
#endif
  
  for ( i = 'a' ; i <= 'z' ; i++ ) {
    ok_buf[i] = 1;
  }

  inf = fopen(in_name, "r");
  if ( !inf ) { printf("%s not found\n",in_name); exit(0); }

  while ( fgets(tmp_buffer,sizeof(tmp_buffer), inf) ) {
    int len;

    if ( strlen(tmp_buffer) == 0 ) continue;
    c = strchr(tmp_buffer,'\n'); if ( c ) *c = 0;
    len = strlen(tmp_buffer);
    if ( len < 4 ) continue;

    symbol_count = is_uniq(tmp_buffer);
    if ( 7 != symbol_count ) continue;

    /* so here, we have a pangram */
    /* get number of words and display */
    wc = find_count(2,arglist);

    printf("%05d ",wc);
    printf("%05d ",word_count);
    show_hive(tmp_buffer);
    printf("%s\n",tmp_buffer);
    
    fprintf(stderr,"%05d %s\n",wc,tmp_buffer);
    
  } /* while inf */
  fclose(inf);
  return 0;
}
