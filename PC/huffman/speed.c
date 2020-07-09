/*
 * speed.c - tests speed of pseudo-dynamic allocation
 */

#include <stdio.h>
#include <time.h>

#define MACRO 0

typedef struct node_ {
  struct node_ *left, *right;
  int c;
} node;

#define MAX 10000

#define freenodes() avail = 0
static node memory[MAX];
static int avail = 0;
#if MACRO
#define mknode() (memory[avail].left = memory[avail].right = NULL, &memory[avail++])
#else
node *mknode (void)
{
 memory[avail].left = memory[avail].right = NULL;
 return (&memory[avail++]);
}
#endif

main()
{
 int i, j;
 node *p;
 clock_t start, end;

 start = clock ();
 for (j = 0; j < 100; j++)
 {
   for (i = 0; i < MAX; i++)
     p = mknode ();
   freenodes ();
 }
 end = clock ();

 printf ("%ld / %ld s\n", (long) (end - start), (long)CLK_TCK);
}