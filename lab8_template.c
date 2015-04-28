/*
  CPSC 223C
  Spring 2015
  CSU Fullerton

  Lab 8: word frequencies in O(n) time

  Authors: Justin Stewart (scubastew@csu.fullerton.edu)

*/ 

#include <assert.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOT_FOUND -1

/* Safe estimate of the length of the longest English word. */
#define MAX_WORD_LENGTH 200

/* This program uses the glib idiom of logging memory allocation
   errors immediately instead of communicating errors through return
   values. */

/* Read the contents of the text file path, count how many times each
   whitespace-separated word appears, then for each distinct word, print one line

   <WORD> <COUNT>

   to standard output, where <WORD> is the non-whitespace characters
   of the word, and <COUNT> is the number of times it appears, which
   must be a positive integer. */
void print_word_frequencies(char* path);

/* Opaque data type for a data structure that tracks the number of
   occurrences of strings. */
typedef GHashTable WordCounts;

/* Allocate a new word count object. */
WordCounts* word_counts_make();

/* Free a WordCounts object. */
void word_counts_free(WordCounts* wc);

/* If word is already in wc, increment its count by 1. If word is not
   present, add it with a count of 1. */
void word_counts_increment(WordCounts* wc, char* word);

/* If word is in wc, return its frequency count. If word is not in wc,
   return NOT_FOUND. */
int word_counts_lookup(WordCounts* wc, char* word);

/* Print out all the words and counts in wc, using the same format as
   print_word_frequences. */
void word_counts_print(WordCounts* wc);

/* GHashFunction that uses the user_data value in a fprintf function to ut the key and value */
void printHash(gpointer key, gpointer value, gpointer user_data);

int main(int argc, char** argv) {
  if (argc != 2) {
    fputs("error: you must supply a filename as an argument\n", stderr);
    return 1;
  }

  print_word_frequencies(argv[1]);

  return 0;
}

void print_word_frequencies(char* path) {
  /* This function is essentially the same as in the lab 5 solution,
     except that it is no longer responsible for checking whether each
     function call failed or succeeded. */

  FILE* f;
  WordCounts* wc;
  char str[MAX_WORD_LENGTH + 1];

  f = fopen(path, "r");
  if (f == NULL) {
    fprintf(stderr, "error: could not open '%s'", path);
    abort();
  }

  wc = word_counts_make();

  fprintf(stderr, "processing '%s'...", path);
  
  while (fscanf(f, "%s", str) != EOF) {
    word_counts_increment(wc, str);
  }

  fclose(f);

  fprintf(stderr, "done\n");

  word_counts_print(wc);
  
  word_counts_free(wc);
}

WordCounts* word_counts_make() {

  WordCounts* hash = g_hash_table_new(g_str_hash, g_str_equal);

  return hash;
}

void word_counts_free(WordCounts* wc) {

  g_hash_table_destroy(wc);

  assert((int) g_hash_table_size (wc) == 0);
}

int word_counts_lookup(WordCounts* wc, char* word) {

  return (int) g_hash_table_lookup(wc, word);
}

void word_counts_increment(WordCounts* wc, char* word) {

  char *g_word = g_strdup(word);
  void *p = g_hash_table_lookup(wc, g_word);

  if(p == NULL) {
    g_hash_table_insert(wc, g_word, (void *) 1);
  }
  else {
    g_hash_table_insert(wc, g_word, p + 1);
  }
}

void printHash(gpointer key, gpointer value, gpointer user_data) {

  fprintf(stderr,user_data, (char *) key, (int) value);
}

void word_counts_print(WordCounts* wc) {

  g_hash_table_foreach(wc, (GHFunc)printHash, "%s %i \n");
}

