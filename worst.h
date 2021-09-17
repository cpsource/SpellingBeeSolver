#ifndef __worst_h__
#define __worst_h__

#define NUM_VOWELS_NEEDED 0

// much with search space starting point for test
#define IS_TEST
#if defined(IS_TEST)
#define ST_0 'a'
#define ST_1 'g'
#define ST_2 'r'
#define ST_3 'o'
#define ST_4 'u'
#define ST_5 'n'
#define ST_6 'd'
#else
#define ST_0 'a'
#define ST_1 'a'
#define ST_2 'a'
#define ST_3 'a'
#define ST_4 'a'
#define ST_5 'a'
#define ST_6 'a'
#endif // IS_TEST

#endif /* __worst_h__ */

