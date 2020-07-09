/*
 * encoders.h - definitions for encoders.c
 *
 * Bruno Costa & Lucia Darsa - 14 Sep 90 - 14 Sep 90
 */

unsigned long int huffman (long int frequency[], bitstream codes[]);
int matchbit (int bit, bitstream codes[]);

