/* sbs.c - SpellingBeeSolutions */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/stat.h>
#include <fcntl.h>

// -a flag - use aspell
int use_a = 0;

unsigned char work_buffer[256];
unsigned char ok_buf[256];
int center_letter;

#define FLAGS_none    0
#define FLAGS_pangram 1
#define FLAGS_bad     2

#define WORD_MAX 32
typedef struct ans_txt {
  struct ans_txt *next;
  int flags;
  char word[WORD_MAX];
} ANS_TXT;

ANS_TXT *root_ans_txt = NULL;

#define ANS_TXT_STR "ans.txt"
#define BADS_TXT_STR "bads.txt"

int fd = 0;
char *bad_buf = NULL;

void open_fd_ro ( void )
{
  struct stat sb;
  int sts;

  fd = open ( BADS_TXT_STR, O_RDONLY );
  if ( !fd ) {
    fprintf(stderr,"%d: can't open %s\n", __LINE__,BADS_TXT_STR);
    exit(0);
  }
  sts = fstat ( fd, &sb );
  if ( sts ) {
    fprintf(stderr,"%d: fstat failed for %s\n",__LINE__,BADS_TXT_STR);
    exit(0);
  }
  
  bad_buf = malloc( sb.st_size + 1 );
  if ( !bad_buf ) {
    fprintf(stderr,"%d: can't malloc bad_buf\n",__LINE__);
    exit(0);
  }
  
  sts = read ( fd, bad_buf, sb.st_size );
  if ( sts != sb.st_size ) {
    fprintf(stderr,"%d: read failed for %s\n",__LINE__,BADS_TXT_STR);
    exit(0);
  }

  // don't forget last byte must be 0
  bad_buf [ sb.st_size - 1 ] = 0;

  // done
  close(fd);
}

// search root_ans_txt for bad_word and mark it FLAGS_bad
void mark_bads(char *bad_word )
{
  ANS_TXT *next = root_ans_txt;

  while ( next ) {

    if ( 0 == strcmp(next->word,bad_word )) {
      next->flags |= FLAGS_bad;
    }
    //onward:
    next = next->next;
  }
}

// walk *'s in bad_buf and mark them as bad
void walk_bad_buf (void)
{
  char *c, *tgt;
  char my_bad_word[WORD_MAX];

  c = strchr(bad_buf,'*');
  while ( c ) {
    // get a bad word
    tgt = my_bad_word;
    c += 1; // skip leading *
    while ( *c != '*' ) {
      *tgt++ = *c++;
    }
    *tgt = 0;

    // housekeeping
    c += 1; // skip trailing star
    c = strchr(c,'*');

    // mark my_bad_word as bad
    mark_bads(my_bad_word);
  }
}

// save a possible word
void save_word ( char *word, int flags )
{
  ANS_TXT *new;

  new = malloc(sizeof(ANS_TXT));
  if ( !new ) {
    fprintf(stderr,"%d: malloc failed\n",__LINE__);
    exit(0);
  }

  new->flags = flags;
  strcpy(new->word,word);
  new->next = root_ans_txt;
  root_ans_txt = new;
}

int main(int argc, char *argv[])
{
  FILE *inf, *outf;
  char *c;
  int all_seven_flag;
  int use_popular = 0;
  int l_index = 1;
  int use_ita = 0;
  char filename[64];
  char system_cmd[64];
  ANS_TXT *tmp;
  
  if ( argc < 2 ) {
    printf("usage: ./sbs [-a] [-p] [-i] <seven-letters>\n");
    printf(" where the first letter must be the center letter\n");
    exit(0);
  }
  if ( argv[l_index][0] == '-' && argv[l_index][1] == 'p' ) {
    l_index += 1;
    use_popular = 1;
  }
  if ( argv[l_index][0] == '-' && argv[l_index][1] == 'i' ) {
    l_index += 1;
    use_ita = 1;
  }
  if ( argv[l_index][0] == '-' && argv[l_index][1] == 'a' ) {
    l_index += 1;
    use_a = 1;
  }
  
  center_letter = (int)argv[l_index][0];

  /* allow only these letters */
  c = &argv[l_index][0];
  while ( *c != 0 ) {
    ok_buf[*c] = 1;
    c += 1;
  }

  /* get right dictionary */
  if ( use_ita ) {
    strcpy(filename,"sbs_ita_words.txt");
  } else {
    if ( use_popular ) {
      strcpy(filename,"popular.txt");
    } else {
      strcpy(filename,"sbs_words.txt");
    }
  }
  inf = fopen(filename, "r");
  if ( !inf ) {
    printf("%s not found\n",filename);
    exit(0);
  }

  printf("Letters: %s\n",argv[l_index]);
  printf("Open: %s\n",filename);
  
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
   save_word(work_buffer, all_seven_flag ? FLAGS_pangram : FLAGS_none);

#if 0
   if ( all_seven_flag )
      printf("       * %s\n",work_buffer);
    else
      printf("       %s\n",work_buffer);
#endif
   
    if ( !use_ita ) {
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
#if 1
	  save_word(c,FLAGS_none);
#else	  
	  printf("ed(%d): %s\n",rule,c);
#endif	  
	}
      } // ed check
    } // use_ita

  next:;
  }
  fclose(inf);

  // so we have a possible list stored at root_ans_list

  // generate a prototype ans.txt
  unlink(ANS_TXT_STR);
  outf = fopen(ANS_TXT_STR,"w");
  if ( !outf ) {
    fprintf(stderr, "%d: %s can't be created\n",__LINE__,ANS_TXT_STR);
    exit(0);
  }
  tmp = root_ans_txt;
  while ( tmp ) {
    fprintf(outf,"%s\n",tmp->word);
    tmp = tmp->next;
  }
  fclose(outf);

  // lets run aspell against it
#define SYSTEM_CALL "aspell -c %s > %s"

  if ( use_a ) {
    sprintf(system_cmd,SYSTEM_CALL, ANS_TXT_STR, BADS_TXT_STR);
  } else {
    sprintf(system_cmd,"rm -f %s; touch %s", BADS_TXT_STR, BADS_TXT_STR);
  }

  //printf("%d: system command <%s>\n",__LINE__,system_cmd);
    
  if ( system(system_cmd) ) {
    fprintf(stderr,"%d: system call %s failed\n",__LINE__,system_cmd);
    exit(0);
  }
  
  // lets load bads.txt into memory
  open_fd_ro ( );

  // mark any bads
  walk_bad_buf();
  
  // dump list root_ans_txt if not bad
  // generate corrected ans.txt
  unlink(ANS_TXT_STR);
  outf = fopen(ANS_TXT_STR,"w");
  if ( !outf ) {
    printf("%d: %s can't be created\n",__LINE__,ANS_TXT_STR);
    exit(0);
  }
  fprintf(outf,"Letters: %s\n",argv[l_index]);
  fprintf(outf,"Open: %s\n",filename);
  tmp = root_ans_txt;
  while ( tmp ) {
    if ( !(tmp->flags & FLAGS_bad) )  {
      if ( tmp->flags & FLAGS_pangram )  {
	fprintf(outf,"* %s\n",tmp->word);
	fprintf(stdout,"* %s\n",tmp->word);
      } else {
	fprintf(outf,"%s\n",tmp->word);
      }
    }
    tmp = tmp->next;
  }
  fclose(outf);
  
  // done. Note - let exit do all memory cleanup
  return 0;
}
