/* build sbs_words.txt from words.txt */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

int main(int argc, char *argv[])
{
  FILE *inf, *outf, *out7;
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
  out7 = fopen("seven_words.txt","w+");
  if ( !out7 ) { printf("could not create seven_words.txt\n"); exit(0); }

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
    switch ( len ) {
	case 4:
	case 5:
	case 6:
	case 7:
		ok_flag = 1;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
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
		break;
	default:
		break;
    } /* switch */

    if ( ok_flag ) {
	fprintf(outf,"%s\n",work_buffer);
	if ( 7 == is_uniq(work_buffer)) fprintf(out7,"%s\n",work_buffer);
	//printf("%s\n",work_buffer);
    } else {
	printf("BAD: %s\n",work_buffer);
    }

  next:;
  } /* while inf */
  fclose(inf); fclose(outf);
  return 0;
}

