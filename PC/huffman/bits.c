/*
 * bits.c - bit manipulation function
 *
 * Lucia Darsa & Bruno Costa - 14 Sep 90 - 14 Sep 90
 */

#define DEBUG 1

#include <stdio.h>
#include "bits.h"

#define TRUE  1
#define FALSE 0

int lshiftn (bitstream *bits, int n, unsigned long int *carry)
{
 int carrylen = 0;

 *carry = bits->data[0] >> (32 - n);
 bits->data[0] = (bits->data[0] << n) | (bits->data[1] >> (32 - n));
 bits->data[1] = (bits->data[1] << n) | (bits->data[2] >> (32 - n));
 bits->data[2] = (bits->data[2] << n) | (bits->data[3] >> (32 - n));
 bits->data[3] = (bits->data[3] << n);

 bits->len += n;
 if (bits->len > 128)
 {
   carrylen = bits->len % 128;
   bits->len = 128;
 }

 return (carrylen);
}


void rshiftn (bitstream *bits, int n)
{
 bits->data[3] = (bits->data[3] >> n) | (bits->data[2] << (32 - n));
 bits->data[2] = (bits->data[2] >> n) | (bits->data[1] << (32 - n));
 bits->data[1] = (bits->data[1] >> n) | (bits->data[0] << (32 - n));
 bits->data[0] = (bits->data[0] >> n);

 bits->len -= n;
 if (bits->len < 0)
   bits->len = 0;
}


int lshift (bitstream *bits)
{
 bits->data[0] = (bits->data[0] << 1) | (bits->data[1] >> 31);
 bits->data[1] = (bits->data[1] << 1) | (bits->data[2] >> 31);
 bits->data[2] = (bits->data[2] << 1) | (bits->data[3] >> 31);
 bits->data[3] = (bits->data[3] << 1);

 bits->len++;
 if (bits->len > 128)
 {
   bits->len = 128;
   return (1);
 }
 return (0);
}


void rshift (bitstream *bits)
{
 bits->data[3] = (bits->data[3] >> 1) | (bits->data[2] << 31);
 bits->data[2] = (bits->data[2] >> 1) | (bits->data[1] << 31);
 bits->data[1] = (bits->data[1] >> 1) | (bits->data[0] << 31);
 bits->data[0] = (bits->data[0] >> 1);

 bits->len--;
 if (bits->len < 0)
   bits->len = 0;
}


int putbit (int bit, FILE *f)
{
 static unsigned char leftover = 0;
 static int nbits = 0;

 if (bit == EOF)
 {
   if (nbits)
     putc (leftover, f);
   leftover = 0;
   nbits = 0;
   return (TRUE);
 }

 leftover |= bit << (7 - nbits);

 if (++nbits == 8)
 {
   putc (leftover, f);
   nbits = 0;
   leftover = 0;
 }
 return (TRUE);
}


int getbit (FILE *f, unsigned long int maxsize)
{
 static int byte = 0;
 static int nbits = 0;
 static int size = 0;
 int bit;

 if (nbits == 0)
 {
   byte = getc (f);
   if (maxsize == size || byte == EOF)
   {
     byte = 0;
     nbits = 0;
     size = 0;
     return (EOF);
   }
   else if (maxsize - size < 8)
     nbits = maxsize - size;
   else
     nbits = 8;
   size += nbits;
 }

 bit = byte & 0x80;
 byte <<= 1;
 nbits--;

 return (bit ? 1 : 0);
}


int writebits (bitstream *code, FILE *f)
{
 unsigned long int bit;
 int word;

 word = 3 - ((code->len - 1) / 32);
 for (bit = 1UL << ((code->len - 1) % 32); bit; bit >>= 1)
   putbit ((code->data[word] & bit) != 0, f);

 for (word++; word < 4; word++)
   for (bit = 0x80000000UL; bit; bit >>= 1)
     putbit ((code->data[word] & bit) != 0, f);

 return (TRUE);
}


int writebyte (int data, FILE *f)
{
 putc ((unsigned char)data, f);
 return TRUE;
}


int readbyte (FILE *f)
{
 return (getc (f));
}


int writelong (unsigned long int data, FILE *f)
{
 putc ((unsigned char)(data >> 24), f);         /* MSB first */
 putc ((unsigned char)(data >> 16), f);
 putc ((unsigned char)(data >> 8), f);
 putc ((unsigned char)(data), f);
 return TRUE;
}


unsigned long int readlong (FILE *f)
{
 unsigned long int data;

 data  = (unsigned long) getc (f) << 24;
 data |= (unsigned long) getc (f) << 16;
 data |= (unsigned long) getc (f) << 8;
 data |= (unsigned long) getc (f);

 return (data);
}
