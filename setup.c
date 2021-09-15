#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

unsigned char work_buffer[256];
unsigned char ok_buf[256];

int main(int argc, char *argv[])
{
  FILE *inf, *outf;
  char *c;

  for ( int i = 'a' ; i <= 'z' ; i++ ) {
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
    c = work_buffer;
    while ( *c != 0 ) {
      *c = tolower(*c);
      c += 1;
    }

    c = work_buffer;
    while ( *c != 0 ) {
      if ( !ok_buf[*c] ) goto next;
      c += 1;
    }
  
    fprintf(outf,"%s\n",work_buffer);

  next:;
  }
  fclose(inf); fclose(outf);
  return 0;
}

