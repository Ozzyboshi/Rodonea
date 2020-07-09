#include <stdio.h>

typedef struct _HuffNode {
    unsigned char data;
    struct _HuffNode *left, *right, *parent;
} HuffNode;

//HuffNode tree[512];
HuffNode* tree = NULL;
HuffNode *node[256];
unsigned lower[256];
unsigned upper[256];
static unsigned freq[256];

static size_t g_HUFFMAN_COMPRESSED_SIZE;
static BYTE* g_HUFFMAN_COMPRESSED_DATA;

void uncompress_file(const char* filename, const char* newname);
void decode_stream(FILE* fin, FILE* fout, HuffNode* tree, unsigned padding);
void decode_stream2(FILE* fin, FILE* fout, HuffNode* tree, unsigned padding);
void construct_huffman(unsigned* freq_in,WORD);