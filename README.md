SpellingBeeSolver will give you all known possible words
for the NYTIMES Spelling Bee word game.

Build Instructions

  It is known to run on Ubuntu Linux

  ./make
  ./setup

Run Instructions

  ./sbs [-p] [-i] <seven-puzzle-letters>

  where the first letter must be the center letter.

  and

  if -p is specified, use dictionary popular.txt. It contains
  less garbage words than sbs_words.txt.
 
  if -i is specified, use itialian nown dictionary. This can not be used
  at the same time as -p.
 
Example from 09/15/2021 NYT

  ./sbs iardlno > ans.txt

Notes

  If a * preceeds the word in ans.txt, it is a word that contains
  all seven letters.

  The word list came from english-words on Github and contains many
  words that are not accepted by Spelling Bee.

  SpellingBee has a habbit of adding 'ed' to the ends of some
  words that are not in the dictionary. Consequently, sbs adds a
  'd' or 'ed' appropriately so you can check those too. Lines
  where this is done, are marked with 'r(1)' or 'r(2)'.

  sbs will also run on cygwin. Just copy over the Makefile,
  sbs.c, and sbs_words.txt.

Make your own SpellingBee

  ./pang -p | sort > new_pangrams.txt

  If -p is specified, use a condensed word dictionary

  Where new_pangrams.txt is a four column list as follows:
    pangrams , count of words found , pangram letters , pangram word

  You can also sort by count of words: sort --key=1.7,1.11 --key=2.1,2.5

  Then check a particular set of pangram letters against sbs to be
  sure there are real words.
  
Coming Soon

  As we supported 'ed', we will support 'ing' as well.

Note:

  You can now use aspell to filter ans.txt words. Just use the
  -a switch on the sbs command line. But to do so, replace
  prog/aspell.cpp with the file in this kit, which is hacked
  up terribly. See NO_STOP_HACK for details.

Nore:

  To add words, add to top of words.txt, then run setup. To add
  words to aspell, edit the word into script add_words.sh, then
  run the script.