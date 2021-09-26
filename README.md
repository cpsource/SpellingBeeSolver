SpellingBeeSolver will give you all known possible words
for the NYTIMES Spelling Bee word game.

Build Instructions

  It is known to run on Ubuntu Linux

  ./make
  ./setup

Run Instructions

  ./sbs <seven-puzzle-letters>

  where the first letter must be the center letter.

Example from 09/15/2021 NYT

  ./sbs iardlno > all.txt

Notes

  If a * preceeds the word in all.txt, it is a word that contains
  all seven letters.

  The word list came from english-words on Github and contains many
  words that are not accepted by Spelling Bee.

  SpellingBee has a habbit of adding 'ed' to the ends of some
  words that are not in the dictionary. Consequently, sbs adds a
  'd' or 'ed' appropriately so you can check those too. Lines
  where this is done, are marked with 'r(1)' or 'r(2)'.

  sbs will also run on cygwin. Just copy over the Makefile, sbs.c, and sbs_words.txt.

Coming Soon

  As we supported 'ed', we will support 'ing' as well.
  
  The rest of the files here are only for playing around and are not
  documented.

