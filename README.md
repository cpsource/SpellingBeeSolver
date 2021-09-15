SpellingBeeSolver will give you all known possible words
for the NYTIMES Spelling Bee word game.

Build Instructions

  It is known to run on Ubuntu Linux

  ./make

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
  



