/* build sbs_words.txt from words.txt */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "config.h"

unsigned char work_buffer[256];
unsigned char ok_buf[256];
unsigned char symbols[256];

char uniq[256];
int is_uniq(char *str)
{
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

int main1(int argc, char *argv[])
{
  FILE *inf, *outf;
  char *c;
  int ok_flag;
  int symbol_count;
  int i;
  char *work_buffer_ptr;
  
  for ( i = 'a' ; i <= 'z' ; i++ ) {
    ok_buf[i] = 1;
  }

  inf = fopen("itwac_nouns_lemmas_notail_2_0_0.csv", "r");
  if ( !inf ) { printf("itwac_nouns_lemmas_notail_2_0_0.csv not found\n"); exit(0); }
  outf = fopen("sbs_ita_words.txt","w+");
  if ( !outf) { printf("could not create sbs_ita_words.txt\n"); exit(0); }

  while ( fgets(work_buffer,sizeof(work_buffer), inf) ) {
    int len;

    work_buffer_ptr = work_buffer;
    
    if ( strlen(work_buffer) == 0 ) continue;
    c = strchr(work_buffer,'\n'); if ( c ) *c = 0;

    /* of the form "word", ... */
    if ( *work_buffer_ptr != '"' ) continue;
    work_buffer_ptr += 1;
    c = strchr(work_buffer_ptr,'"');
    if ( !c ) continue;
    *c = 0;
    
    len = strlen(work_buffer_ptr);
    if ( len < 4 ) continue;

    //printf("%4d : %s\n",len,work_buffer);

    /* make all lower case */
    c = work_buffer_ptr;
    while ( *c != 0 ) {
      *c = tolower(*c);
      c += 1;
    }

    //printf("%4d : %s\n",len,work_buffer);

    /* only take these symbols */
    c = work_buffer_ptr;
    while ( *c != 0 ) {
      if ( !ok_buf[*c] ) goto next;
      c += 1;
    }

    //printf("%4d %s\n",len,work_buffer);

    ok_flag = 0;
    if ( len >= 4 && len <= 7 ) {
      ok_flag = 1;
    } else {
      if ( len <= MAX_WORD_LENGTH ) {
	/* must contain 1->7 symbols */
	memset(symbols,0,sizeof(symbols));
	c = work_buffer_ptr;
	while ( *c != 0 ) {
	  symbols[*c] = 1;
	  c += 1;
	}
	for ( symbol_count = i = 0 ; i < 256 ; i++ ) {
	  if ( symbols[i] ) symbol_count += 1;
	}
	if ( symbol_count >= 1 && symbol_count <= 7 ) ok_flag=1;
      }
    }
    
    if ( ok_flag ) {
      fprintf(outf,"%s\n",work_buffer_ptr);
    } else {
      printf("BAD: %s\n",work_buffer_ptr);
    }

  next:;
  } /* while inf */
  fclose(inf); fclose(outf);
  return 0;
}

int main2(int argc, char *argv[])
{
  FILE *inf, *outf;
  char *c;
  int ok_flag;
  int symbol_count;
  int i;

  for ( i = 'a' ; i <= 'z' ; i++ ) {
    ok_buf[i] = 1;
  }

  inf = fopen("words.txt", "r");
  if ( !inf ) { printf("words.txt not found\n"); exit(0); }
  outf = fopen("sbs_words.txt","w+");
  if ( !outf) { printf("could not create sbs_words.txt\n"); exit(0); }

  while ( fgets(work_buffer,sizeof(work_buffer), inf) ) {
    int len;

    if ( strlen(work_buffer) == 0 ) continue;
    c = strchr(work_buffer,'\n'); if ( c ) *c = 0;
    len = strlen(work_buffer);
    if ( len < 4 ) continue;

    //printf("%4d : %s\n",len,work_buffer);

    /* make all lower case */
    c = work_buffer;
    while ( *c != 0 ) {
      *c = tolower(*c);
      c += 1;
    }

    //printf("%4d : %s\n",len,work_buffer);

    /* only take these symbols */
    c = work_buffer;
    while ( *c != 0 ) {
      if ( !ok_buf[*c] ) goto next;
      c += 1;
    }

    //printf("%4d %s\n",len,work_buffer);

    ok_flag = 0; 
    if ( len >= 4 && len <= 7 ) {
      ok_flag = 1;
    } else {
      if ( len <= MAX_WORD_LENGTH ) {
	char *c;
	/* must contain 1->7 symbols */
	memset(symbols,0,sizeof(symbols));
	c = work_buffer;
	while ( *c != 0 ) {
	  symbols[*c] = 1;
	  c += 1;
	}
	for ( symbol_count = i = 0 ; i < 256 ; i++ ) {
	  if ( symbols[i] ) symbol_count += 1;
	}
	if ( symbol_count >= 1 && symbol_count <= 7 ) ok_flag=1;
      }
    }

    if ( ok_flag ) {
      fprintf(outf,"%s\n",work_buffer);
    } else {
      printf("BAD: %s\n",work_buffer);
    }

  next:;
  } /* while inf */
  fclose(inf); fclose(outf);
  return 0;
}

int main(int argc, char *argv[] )
{
  main1(argc,argv);
  main2(argc,argv);
  return 0;
}
