/* bits.c */
#define flushbits(file) putbit(EOF,file)
int lshiftn (bitstream *bits, int n, unsigned long int *carry);
void rshiftn (bitstream *bits, int n);
int lshift (bitstream *bits);
void rshift (bitstream *bits);
unsigned long int putbit (int bit, FILE *f);
int getbit (FILE *f);                           /* + */
int writebyte (unsigned char data, FILE *f);
int writelong (unsigned long int data, FILE *f);
int writebits (bitstream *bits, FILE *f);

/* encoders.c */
void rbuild (bitstream code, node *root, bitstream codes[]);
void buildcodes (node *root, bitstream codes[]);
unsigned long int huffman (long int frequency[], bitstream codes[]);
node *buildtree (bitstream codes[]);    /* + */
int matchbit (int bit);                 /* + */

/* huffman.c */
int writeheader (FILE *out, unsigned long int size, bitstream code[]);
int compress (FILE *in, FILE *out, bitstream code[]);
unsigned long int getfreq (FILE *in, long int freq[]);
int encode (FILE *in, FILE *out);
int decode (FILE *in, FILE *out);
void usage (void);
void main ();

