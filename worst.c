/* find the word list with the least number of words */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
//#include <unistd.h>

#include "worst.h"

unsigned char work_buffer[256];
unsigned char ok_buf[256];
int center_letter;
int vowel_list[256];
int search_count = 0;
int vowel_skip_count = 0;
int three_skip_count = 0;
int max_word_count = 0;
int check_to_word_count = 0;

char file_name[64];
int old_center_letter;
FILE *inf = NULL;
char *tri_list[] = {
#include "tri_list.h"
NULL
};

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

char *exported_c;
/* str is a 7 character string */
/* tri_list is an array of 4 characters */
int check_tri_list(char *str)
{
  int i;
  char *c, A, B, C, D, e0,e1,e2,e3,e4,e5,e6;

  c = str; e0=*c++; e1=*c++; e2=*c++; e3=*c++; e4=*c++; e5=*c++; e6=*c; 
  for ( i = 0 ; (c=tri_list[i]) ; i++ ) {
 	A=c[0]; B=c[1]; C=c[2]; D=c[3];
	if ( (A==e0 || A==e1 || A==e2 || A==e3 || A==e4 || A==e5 || A==e6) &&
	     (B==e0 || B==e1 || B==e2 || B==e3 || B==e4 || B==e5 || B==e6) &&
	     (C==e0 || C==e1 || C==e2 || C==e3 || C==e4 || C==e5 || C==e6) &&
	     (D==e0 || D==e2 || D==e2 || D==e3 || D==e4 || D==e5 || D==e6)
	) {exported_c=c; return 1;}
  }
  return 0;
}

/* return true if try_list[4] characters are in str[7] */
int in_tri_list(char *str)
{
  char myquad[5];
  int i;
  char *c;

  for ( i = 0 ; i < 4 ; i++ ) {
	myquad[i] = str[i];
  }
  myquad[4] = 0;
 
#if 1
  for ( i = 0 ; (c=tri_list[i]) ; i++ ) {
	if ( 0 == strcmp(c,myquad)) return 1;
  }
  three_skip_count += 1;
  return 0;
#else 
  if ( check_tri_list(str) ) {
	//printf("in %s %s\n",str,exported_c);
	return 1;
  }
  printf("IsOut %s\n",str);
#endif // 1

  return 0;
}

int build_file_name(int c)
{
  int sts = 0;
  sprintf(file_name,"sbs_%c_words.txt",c);
  if ( c != old_center_letter ) {
	printf("new file %s\n",file_name);
        max_word_count = 0;
        sts = 1;
  }
  old_center_letter = c;
  return sts;
}

/* setup vowel_list */
void init ( void )
{
  vowel_list['a'] = 1;
  vowel_list['e'] = 1;
  vowel_list['i'] = 1;
  vowel_list['o'] = 1;
  vowel_list['u'] = 1;
}

// determine the number of vowels in a string */
int vowel_count(char *str)
{
int res = 0;
char *c = str;

while ( *c ) {
if ( vowel_list[*c] ) res += 1;
c += 1;
}
return res;
}

#if 1
/* find the number of actual words */
int find_count(int argc, char *argv[], int *all_seven_flag, int *trial_exists)
{
  char *c;
  int word_count = 0;
  int new_file_flag;
  int trace_flag;
  
  *all_seven_flag = 0;
  *trial_exists = 0;

  if ( argc < 2 ) {
    printf("usage: ./sbs <seven-letters>\n");
    printf(" where the first letter must be the center letter\n");
    exit(0);
  }
  center_letter = (int)argv[1][0];

  memset(ok_buf,0,sizeof(ok_buf));
  c = &argv[1][0];
  while ( *c != 0 ) {
    ok_buf[*c] = 1;
    c += 1;
  }

  new_file_flag = build_file_name(center_letter);

#if 1
  inf = fopen(file_name,"r");
  if ( !inf ) { printf("%s not found\n",file_name); exit(0); }
#else
  if ( new_file_flag ) {  
	if ( inf != NULL ) fclose(inf);
	inf = fopen(file_name, "r");
	if ( !inf ) { printf("%s not found\n",file_name); exit(0); }
  } else {
	fseek(inf,0,SEEK_SET);
  }
#endif

//  printf("using file %s\n",file_name);

  /* scan entire word list file */
  while ( fgets(work_buffer,sizeof(work_buffer), inf) ) {
    int len;

    trace_flag = 0;
    if ( strlen(work_buffer) == 0 ) continue;
    c = strchr(work_buffer,'\n'); if ( c ) *c = 0;
    len = strlen(work_buffer);

#if 0
    if ( 0 == strcmp(work_buffer,"fowl") ) {
      printf("fowl found\n");
      //trace_flag = 1;
    }
#endif
    //trace_flag = 1;

    /* must contain center letter */
    if ( !strchr(work_buffer, center_letter) ) continue;

    //if ( trace_flag ) printf("cp1: center_letter found\n");

    /* can only use these letters */
    c = work_buffer;
    while ( *c != 0 ) {
      if ( !ok_buf[*c] ) goto next;
      c += 1;
    }

    word_count += 1;

    //if ( trace_flag ) printf("cp2: all letters ok\n");

/* does trial exist as a real word ??? */
   if ( 0 == strcmp(argv[1],work_buffer) ) *trial_exists = 1;

/* does word in sbs_words.txt contain all seven letters ??? */
   c = &argv[1][0];
   while ( *c != 0 ) {
    if ( !strchr(work_buffer, *c) ) goto onward;
    c += 1;
   }
   *all_seven_flag = 1;
   if ( trace_flag ) printf("%s %s\n",work_buffer,argv[1]);

onward:;
#if 0
    if ( all_seven_flag )
      printf("* %s\n",work_buffer);
    else
      printf("%s\n",work_buffer);
#endif

  next:;
  } /* do for all words in sbs_?_words.txt */
#if 1
  fclose(inf);
#endif
  if ( trace_flag ) printf("cp3: word_count = %d, all_seven_flag = %d\n",
	word_count,*all_seven_flag);
  return word_count;
}
#endif

int main()
{
	int a[7];
	char my_trial[8];
	int word_count, worst_word_count = 0;
	char *call_args[2] = {"worst", my_trial };
	int all_seven_flag;
	int trial_exists;

	init();

/* interate */
	for ( a[0] = ST_0 ; a[0] <= 'z' ; a[0] += 1 ) {
	for ( a[1] = ST_1 ; a[1] <= 'z' ; a[1] += 1 ) {
	  if ( a[0] == a[1] ) {
		//printf("s1\n");
		continue;
	  }
	for ( a[2] = ST_2 ; a[2] <= 'z' ; a[2] += 1 ) {
	  if ( a[2] == a[1] || a[2] == a[0] ) {
		//printf("s2\n");
		continue;
	  }

#if 0
/* eye candy */
	printf("checking %c%c%c......\n",
		a[0],a[1],a[2]);
#endif

	for ( a[3] = ST_3 ; a[3] <= 'z' ; a[3] += 1 ) {
	  if ( a[3] == a[2] || a[3] == a[1] || a[3] == a[0] ) {
		//printf("s3\n");
		continue;
	  }

/* eye candy */
#if 0
	printf("checking %c%c%c%c...\n",
		a[0],a[1],a[2],a[3]);
	//fsync(1);
#endif

	for ( a[4] = ST_4 ; a[4] <= 'z' ; a[4] += 1 ) {
	  if ( a[4] == a[3] || a[4] == a[2] || a[4] == a[1]
		|| a[4] == a[0] ) continue;
	for ( a[5] = ST_5; a[5] <= 'z' ; a[5] += 1 ) {
	  if ( a[5] == a[4] || a[5] == a[3] || a[5] == a[2]
		|| a[5] == a[1] || a[5] == a[0] ) continue;
	for ( a[6] = ST_6 ; a[6] <= 'z' ; a[6] += 1 ) {
	  if ( a[6] == a[5] || a[6] == a[4] || a[6] == a[3]
		|| a[6] == a[2] || a[6] == a[1] || a[6] == a[0] ) continue;

/* build my_trial */
	for ( int k = 0 ; k <= 6 ; k++ )
		my_trial[k] = a[k];
	my_trial[7] = 0;

//	printf("word = %s\n",my_trial);

/* right number of vowels ??? */
#if NUM_VOWELS_NEEDED > 0
	if ( vowel_count(my_trial) < NUM_VOWELS_NEEDED ) {
		vowel_skip_count += 1;
//		printf("skipping %s\n",my_trial);
		continue;
	}
#endif // NUM_VOWELS_NEEDED

	if ( !in_tri_list(my_trial) ) {
	  //printf("not in tri_list %s\n",my_trial);
	  continue;
	} else {
	  //printf("in tri_list %s\n",my_trial);
	}

	search_count += 1;
	//word_count = 0;
	word_count = find_count(2,call_args,&all_seven_flag,&trial_exists);

#if 1
	if ( all_seven_flag && word_count && trial_exists ) {
		printf("find_count returns: %s %d %d %d\n",my_trial, word_count, all_seven_flag,trial_exists);
	}
#endif
	if ( 0 == word_count || !all_seven_flag || !trial_exists) continue;
	if ( 0 == worst_word_count ) {
		worst_word_count = word_count;
		printf("setting new low %s, %d\n",
			my_trial, word_count);
	}
	if ( word_count < worst_word_count ) {
	  printf("new low %s, %d\n",
		my_trial, word_count);
	  worst_word_count = word_count;
	}

	}
	}	
	}
#if 1
	if ( search_count ) printf("my_trial = %s, search_count = %d, vowel_skip_count = %d, three_skip_count = %d\n",
		my_trial,search_count, vowel_skip_count,three_skip_count);
#endif
	search_count = vowel_skip_count = three_skip_count = 0;
	}
	}
	}
	}

	fclose(inf);
	return 0;
}
