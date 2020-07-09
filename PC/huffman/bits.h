/*
 * bits.h - bit manipulation functions definition
 *
 * Bruno Costa & Lucia Darsa - 14 Sep 90 - 14 Sep 90
 */


typedef struct bitstream_ {	/* up to 128 bits long stream */
  unsigned long data[4];
  unsigned short int len;
} bitstream;


#define flushbits(file) putbit(EOF,file)

int lshiftn (bitstream *bits, int n, unsigned long int *carry);
void rshiftn (bitstream *bits, int n);
int lshift (bitstream *bits);
void rshift (bitstream *bits);
int putbit (int bit, FILE *f);
int getbit (FILE *f, unsigned long int maxsize);
int writebyte (int data, FILE *f);
int readbyte (FILE *f);
int writelong (unsigned long int data, FILE *f);
unsigned long int readlong (FILE *f);
int writebits (bitstream *bits, FILE *f);

