
#include <stdio.h>
#include <clib/timer_protos.h>
#include <clib/exec_protos.h>
#include "../src/customtrigonometry.h"
#include "rod3X1.h"

#include "flower.h"

#include "huffman.h"



/*static inline ULONG timer2()
{
   /* static struct timeval t;
    struct timeval a, b;*/

    /*GetSysTime(&a);
  b = a;
  SubTime(&b, &t);
  t = a;*/

   // return b.tv_secs * 1000 + b.tv_micro / 1000;
//}

long add (register long a __asm("d0"), register long b __asm("d1"));



void decode_stream3(UWORD uwFrames, HuffNode* tree, unsigned padding);


static  const  unsigned char* ptrCompressedData = &rod3X1_compressed_data[0];

static UBYTE ubRodonea[259200];
static unsigned int iOutIndex = 0;


int main()
{
    long aa = add(14,16);
printf ("14 + 16 = %ld\n",aa);

return 0;
   printf("Rod compressed size %d\n",rod3X1_compressed_size);

  // FILE* fin = fopen("out.Z", "rb");
   //FILE* fout = fopen("vittoria", "wb");
   int b0;
   int b1,b2,b3;
    unsigned freq[256], padding;
    
    for (int i = 0; i < 256; i++) {
        //fread(&padding, 4, 1, fin);
        memcpy(&padding,ptrCompressedData,4);
        ptrCompressedData+=4;
        b0 = (padding & 0x000000ff) << 24u;
         b1 = (padding & 0x0000ff00) << 8u;
         b2 = (padding & 0x00ff0000) >> 8u;
         b3 = (padding & 0xff000000) >> 24u;
         padding = b0 | b1 | b2 | b3;
        freq[i] = padding;
    }

    //fread(&padding, 4, 1, fin);
    memcpy(&padding,ptrCompressedData,4);
        ptrCompressedData+=4;
    b0 = (padding & 0x000000ff) << 24u;
   b1 = (padding & 0x0000ff00) << 8u;
   b2 = (padding & 0x00ff0000) >> 8u;
   b3 = (padding & 0xff000000) >> 24u;
   padding = b0 | b1 | b2 | b3;
   printf("Paddig vale %d\n",padding);
   construct_huffman(freq,-1);

   //for (int i=0 ;i<259200;i++)   decode_stream3(1, tree, padding);

   decode_stream3(30, tree, padding);
   decode_stream3(30, tree, padding);

   decode_stream3(30, tree, padding);
   decode_stream3(30, tree, padding);


   decode_stream3(30, tree, padding);
   decode_stream3(30, tree, padding);


   decode_stream3(30, tree, padding);
   decode_stream3(30, tree, padding);


    if (ubRodonea[0]!=0x2d) printf("NOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
    else printf("SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n");

   for (int i =0;i<10;i++)
   printf("%x\n",ubRodonea[i]);


   //fclose(fout);
   return 0;
    fix16_sinlist_init();
    fix16_coslist_init();

   // timer2();
   // ULONG t2 = timer2();
    createFlower(3, 1, 100, 0 );
   // printf("\n%u ms\n", (unsigned int)t2);
    int a;
    for (a = 0; a < 360 * 3; a++)
    {
       /* char buf[100];
        char buf2[100];

        fix16_to_str(FLOWERS[0].vertexData[a * 2], buf, 2);
        fix16_to_str(FLOWERS[0].vertexData[a * 2 + 1], buf2, 2);*/
        printf("ciclo %d - %d %d\n",a, FLOWERS[0].vertexData[a * 2], FLOWERS[0].vertexData[a * 2 +1]);
       // getchar();
    }
    rotatePoints(45);
    for (a = 0; a < 360; a++)
    {
       
       // printf("ciclo %d - %d %d\n",a, vertexDataRotated[45].x[a], vertexDataRotated[45].y[a]);
    }

    
}

/*void uncompress_file(const char* filename, const char* newname) {
   int b0;
   int b1,b2,b3;

   

    unsigned freq[256], padding;
    
    for (int i = 0; i < 256; i++) {
        fread(&padding, 4, 1, fin);
        b0 = (padding & 0x000000ff) << 24u;
         b1 = (padding & 0x0000ff00) << 8u;
         b2 = (padding & 0x00ff0000) >> 8u;
         b3 = (padding & 0xff000000) >> 24u;
         padding = b0 | b1 | b2 | b3;
        freq[i] = padding;
    }

    fread(&padding, 4, 1, fin);
    b0 = (padding & 0x000000ff) << 24u;
   b1 = (padding & 0x0000ff00) << 8u;
   b2 = (padding & 0x00ff0000) >> 8u;
   b3 = (padding & 0xff000000) >> 24u;
   padding = b0 | b1 | b2 | b3;
   
    construct_huffman(freq);
    decode_stream(fin, fout, tree, padding);
    
}*/

void decode_stream3(UWORD uwFrames, HuffNode* tree, unsigned padding) {
    static int count = 0;
    UWORD uwCurFrames = 0;

    if (count==0)
    {
      /*size_t startpos = ftell(fin); // should be 1028
      fseek(fin, 0L, SEEK_END);
      size_t endpos = ftell(fin); // last byte handling
      fseek(fin, startpos, SEEK_SET);
       count = endpos - startpos;*/
       count = rod3X1_compressed_size-1028;
    }

    static BYTE buf = 0;
    static BYTE nbuf = 0;
    static  BYTE bit = 0;
    static HuffNode *p;
    while (count > 0 || nbuf > 0) 
    {
        // Start from tree top
        p = tree + 510;
        while (p->left || p->right) 
        {
            // Prepare next bit if needed
            if (nbuf == 0) {
                if (count <= 0)
                    return;

                //buf = fgetc(fin);
                buf = *ptrCompressedData;
                static int lol =0;
                if (!lol) printf("%x\n",buf);
                lol++;
                ptrCompressedData++;

                if (count == 1) {
                    // Last bit
                    nbuf = 8 - padding;
                    if (nbuf == 0) {
                        return;
                    }
                } else {
                    nbuf = 8;
                }
                count--;
            }
            // p has child
            bit = buf & 1;
            buf >>= 1;
            nbuf--;
            if (bit == 0)
                p = p->left;
            else
                p = p->right;
        }
        ubRodonea[iOutIndex++]=(UBYTE) p->data;
        if (uwCurFrames>uwFrames)
          return ;
        uwCurFrames++;
        //fputc(p->data, fout);
        //printf("####%x#####\n",p->data);
    }
}

void decode_stream2(FILE* fin, FILE* fout, HuffNode* tree, unsigned padding) {
    static int count = 0;

    if (count==0)
    {
      /*size_t startpos = ftell(fin); // should be 1028
      fseek(fin, 0L, SEEK_END);
      size_t endpos = ftell(fin); // last byte handling
      fseek(fin, startpos, SEEK_SET);
       count = endpos - startpos;*/

       count = rod3X1_compressed_size-1028;
    }

    static BYTE buf = 0;
    static BYTE nbuf = 0;
    static  BYTE bit = 0;
    static HuffNode *p;
    while (count > 0 || nbuf > 0) 
    {
        // Start from tree top
        p = tree + 510;
        while (p->left || p->right) 
        {
            // Prepare next bit if needed
            if (nbuf == 0) {
                if (count <= 0)
                    return;

                //buf = fgetc(fin);
                buf = *ptrCompressedData;
                ptrCompressedData++;
                if (count == 1) {
                    // Last bit
                    nbuf = 8 - padding;
                    if (nbuf == 0) {
                        return;
                    }
                } else {
                    nbuf = 8;
                }
                count--;
            }
            // p has child
            bit = buf & 1;
            buf >>= 1;
            nbuf--;
            if (bit == 0)
                p = p->left;
            else
                p = p->right;
        }
        fputc(p->data, fout);
        //printf("####%x#####\n",p->data);
        return ;
    }
}


void decode_stream(FILE* fin, FILE* fout, HuffNode* tree, unsigned padding) {
    size_t startpos = ftell(fin); // should be 1028
    fseek(fin, 0L, SEEK_END);
    size_t endpos = ftell(fin); // last byte handling
    fseek(fin, startpos, SEEK_SET);
    int count = endpos - startpos;

    BYTE buf = 0, nbuf = 0, bit;
    HuffNode *p;
    while (count > 0 || nbuf > 0) {
        // Start from tree top
        p = tree + 510;
        while (p->left || p->right) {
            // Prepare next bit if needed
            if (nbuf == 0) {
                if (count <= 0)
                    return;

                buf = fgetc(fin);
                if (count == 1) {
                    // Last bit
                    nbuf = 8 - padding;
                    if (nbuf == 0) {
                        return;
                    }
                } else {
                    nbuf = 8;
                }
                count--;
            }
            // p has child
            bit = buf & 1;
            buf >>= 1;
            nbuf--;
            if (bit == 0)
                p = p->left;
            else
                p = p->right;
        }
        fputc(p->data, fout);
    }
}


void construct_huffman(unsigned *freq_in,WORD stage)
{
  int count = 256;
  static unsigned freq[256];

  // Initialize data
  if (stage!=1)
  {
    for (int i = 0; i < 256; i++)
    {
      freq[i] = freq_in[i];
      tree[i].data = i;
      tree[i].left = NULL;
      tree[i].right = NULL;
      node[i] = &tree[i];
    }
  }

  if (stage==0) return ;

  // Sort by frequency, decreasing order
  /* WARNING: Although this Quick Sort is an unstable sort,
     * it should at least give the same result for the same input frequency table,
     * therefore I'm leaving this code here
     */
  {
    unsigned top = 1;
    lower[0] = 0, upper[0] = 256;
    while (top > 0)
    {
      top--;
      int left = lower[top], right = upper[top];
      int i = left, j = right - 1, flag = 0;
      if (i >= j) // Nothing to sort
        continue;
      while (i < j)
      {
        if (freq[i] < freq[j])
        {
          unsigned t = freq[i];
          freq[i] = freq[j];
          freq[j] = t;
          HuffNode *p = node[i];
          node[i] = node[j];
          node[j] = p;
          flag = !flag;
        }
        flag ? i++ : j--;
      }
      lower[top] = left, upper[top] = i;
      lower[top + 1] = i + 1, upper[top + 1] = right;
      top += 2;
    }
  }

  // Construct tree
  while (count > 1)
  {
    int pos = 512 - count;
    HuffNode *parent = &tree[pos];
    // Select lowest 2 by freq
    int i = count - 2, j = count - 1;
    // Create tree, lower freq left
    parent->left = node[j];
    parent->right = node[i];
    node[j]->parent = node[i]->parent = parent;
    node[i] = parent;
    freq[i] += freq[j];
    // Insert
    for (; i > 0 && freq[i] > freq[i - 1]; i--)
    {
      unsigned t = freq[i];
      freq[i] = freq[i - 1];
      freq[i - 1] = t;
      HuffNode *p = node[i];
      node[i] = node[i - 1];
      node[i - 1] = p;
    }
    count--;
  }
  // Now HEAD = node[0] = tree[511]
  node[0]->parent = NULL;
}