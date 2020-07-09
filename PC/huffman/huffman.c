/*
 * huffman.c - applies huffman encoding or decoding to files
 *
 * Lucia Darsa & Bruno Costa - 5 Sep 90 - 14 Sep 90
 */

#define DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "bits.h"
#include "encoders.h"

#define TRUE  1
#define FALSE 0

#define PERCENT100 10000
#define NORMFACTOR 100

#define FILEID (((long)'H' << 24) | ((long)'U' << 16) | ((long)'F' << 8))


int writeheader (FILE *out, unsigned long int size, bitstream code[])
{
 int i, j;
 unsigned long int id = FILEID;

 writelong (id, out);

 for (i = 0; i < 256; i++)
   if (code[i].len)
   {
     writebyte (i, out);
     writebyte (code[i].len, out);
     for (j = 3 - (code[i].len - 1)/ 32; j < 4; j++)
       writelong (code[i].data[j], out);
   }

 writebyte (0, out);            /* code stream terminator */
 writebyte (0, out);

 writelong (size, out);
 return TRUE;
}


int compress (FILE *in, FILE *out, bitstream code[])
{
 int c;

 while ((c = getc (in)) != EOF)
   if (!writebits (&code[c], out))
     return FALSE;

 flushbits (out);

 return TRUE;
}


unsigned long int getfreq (FILE *in, long int freq[])
{
 int i, c;
 unsigned long int size = 0;

 for (i = 0; i < 256; i++)
   freq[i] = 0;

 rewind (in);

 while ((c = getc (in)) != EOF)
 {
   freq[c]++;
   size++;
 }

 rewind (in);

 return (size * 8);
}


int encode (FILE *in, FILE *out)
{
 long int frequencies[256];
 bitstream codes[256];
 unsigned long int oldsize, newsize;

 oldsize = getfreq (in, frequencies);

 newsize = huffman (frequencies, codes);

 if (out)
   if (!writeheader (out, newsize, codes)  ||  !compress (in, out, codes))
     return (0);

 return PERCENT100 - (((newsize / 100) * PERCENT100) / (oldsize / 100));
}


/* returns the number of bits following */
unsigned long int readheader (FILE *in, bitstream code[])
{
 unsigned long int id, size;
 int i, j, len;

 id = readlong (in);
 if (id != FILEID)
   return (0);

 for (i = 0; i < 256; i++)
 {
   code[i].len = 0;
   for (j = 0; j < 4; j++)
     code[i].data[j] = 0UL;
 }

 i = readbyte (in);
 len = readbyte (in);
 while (i != 0  ||  len != 0)
 {
   code[i].len = len;
   for (j = 3 - (len - 1)/32; j < 4; j++)
     code[i].data[j] = readlong (in);
   i = readbyte (in);
   len = readbyte (in);
 }

 size = readlong (in);
 return (size);
}


int decode (FILE *in, FILE *out)
{
 unsigned long int size;
 int bit, c;
 bitstream codes[256];

 size = readheader (in, codes);

 if (size == 0)
   return FALSE;

 do
 {
   bit = getbit (in, size);
   if ((c = matchbit (bit, codes)) != -1)
     putc (c, out);
 } while (bit != EOF);

 return TRUE;
}


void usage (void)
{
 fprintf (stderr, "usage: huffman [-c | -u | -r] [-f | filenames ...]\n");
 exit (10);
}


int main (int argc, char *argv[])
{
 int i, compress;
 int valid = FALSE;
 int justrate = FALSE;

 if (argc == 1)
   usage ();

 for (i = 1; i < argc; i++)
 {
   if (argv[i][0] == '-')
     switch (argv[i][1])
     {
       case 'c':
         compress = TRUE;
         valid = TRUE;
         break;
       case 'u':
         compress = FALSE;
         valid = TRUE;
         break;
       case 'r':
         justrate = TRUE;
         valid = TRUE;
         break;
       default:
         fprintf (stderr, "huffman: unknown option %c\n", argv[i][1]);
         usage ();
         break;
     }
   else if (!valid)
     usage ();
   else
   {
     FILE *in = fopen (argv[i], "rb");
     int done = TRUE;

     if (!in)
       fprintf (stderr, "huffman: could not open file %s\n", argv[i]);
     else if (justrate)
     {
       int r = encode (in, NULL);
       printf ("%s (%d.%02d%%)\n", argv[i], r/NORMFACTOR, r%NORMFACTOR);
     }
     else
     {
       char outnam[20];
       FILE *out = fopen (tmpnam (outnam), "wb");
       if (!out)
       {
         fprintf (stderr, "huffman: could not open temp file for %s -- aborted!\n", argv[i]);
         exit (20);
       }
       else
       {
         if (compress)
         {
           int r = encode (in, out);
           printf ("%s (%d.%02d%%)\n", argv[i], r/NORMFACTOR, r%NORMFACTOR);
         }
         else
           if (!(done = decode (in, out)))
             fprintf (stderr, "huffman: could not decode %s\n", argv[i]);
         fclose (out);
       }
       fclose (in);
       if (done)
         if (remove (argv[i]) == -1)
           fprintf (stderr, "huffman: could not remove %s\n", argv[i]);
         else if (rename (outnam, argv[i]) == -1)
           fprintf (stderr, "huffman: could not rename %s to %s\n", outnam, argv[i]);
     }
   }
 }
 return (0);
}
