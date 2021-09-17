#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

unsigned char work_buffer[256];

int build(int argc, char *argv[])
{
  FILE *inf, *outf;
  char *c;

  if ( argc < 2 ) {
    printf("usage: ./letter \"a-letter\"\n");
    printf(" where the letter must be included in sbs_words.txt\n");
    exit(0);
  }

  inf = fopen("sbs_words.txt", "r");
  if ( !inf ) { printf("sbs_words.txt not found\n"); exit(0); }

  sprintf(work_buffer,"sbs_%s_words.txt",argv[1]);
  outf = fopen(work_buffer,"w+");
  if ( !outf ) { printf("can't create %s\n",work_buffer); exit(0); }

  while ( fgets(work_buffer,sizeof(work_buffer), inf) ) {

    if ( strlen(work_buffer) == 0 ) continue;
    c = strchr(work_buffer,'\n'); if ( c ) *c = 0;

    /* must contain center letter */
    if ( !strchr(work_buffer, argv[1][0]) ) continue;

    fprintf(outf,"%s\n",work_buffer);

  next:;
  }
  fclose(inf);
  fclose(outf);
  return 0;
}

int main()
{
  char buf[32];
  char *arg_list[2] = {"letter",buf};

  for ( int i = 'a' ; i <= 'z' ; i++ ) {
	buf[0] = i;
	buf[1] = 0;
	build(2,arg_list);
  }
  return 0;
}
