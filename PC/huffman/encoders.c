/*
 * encoders.c - huffman/ASCII code conversion and manipulation functions
 *
 * Lucia Darsa & Bruno Costa - 14 Sep 90 - 14 Sep 90
 */

#define DEBUG 1

#include <stdio.h>
#include <limits.h>
#include "bits.h"
#include "encoders.h"

#define TRUE  1
#define FALSE 0


typedef struct node_ {
  struct node_ *left;
  struct node_ *right;
  unsigned char c;
} node;

typedef struct tree_ {
  node *root;
  long int freq;
} tree;

/*
 * internal functions prototypes
 */
static void rbuild (bitstream code, node *root, bitstream codes[]);
static void buildcodes (node *root, bitstream codes[]);
static node *buildtree (bitstream codes[]);

/*
 * pseudo-dynamic memory allocation scheme
 * (the algorithms assure that memory is never exhausted)
 *
 *  NOTE: for those that simply cannot understand the line below, the comma
 *        operator executes its operands from left to right, and returns the
 *        value of the rightmost one.
 */
#define mknode() (memory[avail].left = memory[avail].right = NULL, &memory[avail++])
#define freenodes() avail = 0
static node memory[512];
static int avail = 0;

static void rbuild (bitstream code, node *root, bitstream codes[])
{
 if (root->left == NULL)		/* is it a leaf? */
   codes[root->c] = code;
 else
 {
   lshift (&code);
   rbuild (code, root->left, codes);
   code.data[3] |= 1;
   rbuild (code, root->right, codes);
 }
}


static void buildcodes (node *root, bitstream codes[])
{
 int i;
 static bitstream zerocode = {
   {0, 0, 0, 0}, 0
 };

 for (i = 0; i < 256; i++)
   codes[i] = zerocode;

 rbuild (zerocode, root, codes);
}


#define HUGEFREQ LONG_MAX

unsigned long int huffman (long int frequency[], bitstream codes[])
{
 tree forest[256], *last;
 unsigned long int size = 0;
 int i;

 for (i = 0, last = forest; i < 256; i++)
   if (frequency[i] != 0)
   {
     node *r = mknode ();
     r->c = i;
     last->root = r;
     last->freq = frequency[i];
     last++;
   }

 for (last--; last > forest; last--)
 {
   node *newroot;
   tree *first, *second, *p;
   long int firstfreq, secondfreq;

   first = second = NULL;
   firstfreq = secondfreq = HUGEFREQ;
   for (p = forest; p <= last; p++)
     if (p->freq < firstfreq)
     {
       second = first;
       secondfreq = firstfreq;
       first = p;
       firstfreq = p->freq;
     }
     else if (p->freq < secondfreq)
     {
       second = p;
       secondfreq = p->freq;
     }

   newroot = mknode ();
   newroot->left = first->root;
   newroot->right = second->root;
   first->root = newroot;
   first->freq = firstfreq + secondfreq;

   second->root = last->root;
   second->freq = last->freq;
 }

 buildcodes (forest[0].root, codes);

 freenodes ();

 for (i = 0; i < 256; i++)
   size += frequency[i] * codes[i].len;

 return (size);
}


static void traverse (node **current, int direction)
{
 if (direction)
 {
   if ((*current)->left == NULL)
     (*current)->left = mknode ();
   *current = (*current)->left;
 }
 else
 {
   if ((*current)->right == NULL)
     (*current)->right = mknode ();
   *current = (*current)->right;
 }
}


static node *buildtree (bitstream code[])
{
 int i;
 node *root = mknode ();
 node *current = root;

 for (i = 0; i < 256; i++)
   if (code[i].len)
   {
     unsigned long int bit;
     int word;

     word = 3 - ((code[i].len - 1) / 32);
     for (bit = 1UL << ((code[i].len - 1) % 32); bit; bit >>= 1)
       traverse (&current, (code[i].data[word] & bit) ? 1 : 0);

     for (word++; word < 4; word++)
       for (bit = 0x80000000UL; bit; bit >>= 1)
         traverse (&current, (code[i].data[word] & bit) ? 1 : 0);

     current->c = i;
     current = root;
   }
 return (root);
}


int matchbit (int bit, bitstream codes[])
{
 static node *root = NULL;
 static node *current = NULL;
 unsigned char c;

 if (bit == EOF)
 {
   freenodes ();
   current = root = NULL;
   return (-1);
 }

 if (root == NULL)
   current = root = buildtree (codes);

 if (bit)
   current = current->left;
 else
   current = current->right;

 c = current->c;

 if (current->left == NULL  ||  current->right == NULL)
 {
   current = root;
   return (c);
 }
 else
   return (-1);
}

