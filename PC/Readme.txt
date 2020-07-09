Rodonea on Amiga - quick analysis

The goal of this document is to explain (and a remainder) of how I draw the rononeas to a low power stock Amiga, trying to make the lightest and the low memory consumption version.
First of all, what is a rodonea?
As stated here https://en.wikipedia.org/wiki/Rose_(mathematics) a rodonea is just a sinusoide curve plotted in polar coordinates.
The result is very nice, after executing the math and the plotting we get some awesome figures plotted to our scree, the figure drawn depends from the omega parameter wich is n/d, here is a picture with some examples.

https://it.wikipedia.org/wiki/Rodonea#/media/File:Rose-rhodonea-curve-7x7-chart.svg

Since complexity rises accordingly with d I will consider only roses with d=1 so, I will draw every rodonea in the first raw (except the first that is a circle).
I also want the rodonea to rotate around one axis to give the demo some movement.

Since my A600 can't (without accelleration) calculate in real time the position of every point at a given angle of rotation I decided to precalculate all the data, but when precalculating, experience told me to estimate (more or less) how much data will be precalculated since my A600 has only 1Mb of chip ram and some of it will be eaten by the OS.
So... let's start some math.

Let's say I want my rodonea to be 360 points (one for each angle) to get a good accuracy and not to see scattered points.
Let's say I want to precalculate 360 rotations for each rodonea.
Let's say I want a very big rodonea that takes a lot of screen (using lowres pal).

That is a huge mission for my Amiga... it seems a lot of data.

Let's assume I want to store each point coordinate using WORDS, now i need 2 bytes to store the X coordinate and another 2 for the Y coordinate (even if I wouldn need it).
The final precalculated data size for 1 rodonea would be: 360*4*360 = 518400 bytes.
Only the precalculated data will eat half of my CHIP ram, we need to optimize something otherwise we will be able to store only ONE rodonea.

The first step that comes into mind is to use a single byte to store coordinates, but... X resolution is 320, we need more that one byte to represent 320, let's introduce some constraints.
Let's say that R is 100.
R in the rodonea equation corresponds to the maximum distance of each point of the petal from the center of the rose.
In this way we are sure that we get values from -100 to + 100 , that is 200 points, it's a very big rodonea and we half the size of our precalculated data that now shrinks to 360*2*360 = 259200 bytes.
This is good but, why not trying to compress this data and see if we can push more roses?
Compressing is a very hard topic for me and I know nothing about it, but Google comes to help and always and it found me a simple compression algorithm called "Huffman algoritm".
Basically the Idea behind Huffman is to search for repeating/redundant bytes inside a file and rewrite them saving space.
Unfortunately my file has almost no repeating data, each point has different coordinates, if I had many points with same coordinate I would end up plotting the same point over and over and made no sense.
I tested my 259200 precalculated bytes with huffman and... as suspected, no results, the compressed data is even slightly fatter that the uncompressed, is this a dead end?
Maybe not... maybe there is a way.... what if I express each point relative to the previous point instead of using absolute coordinated relative to the origin?
The idea comes from vectors. Vectors does not express a point in the space, instead I think them as a direction and a distance from another point.
Since in a curve each points are quite close to each other, I would end up with a lot of 0x01 and 0xFF (-1), that would ease the Huffman compression.
Of course to get the most 0x01 and 0xFF I need some sort of walking algorithm, i cant randomly process each point, they must be ordered in a way to put each point next to the nearest.
I end up with this algorithm that is not bad for me... first get the closest point P to the origin (0,0), then P becomes my pivot point, then find the nearest point relative to P, let's call it P1, now P = P1, that means the pivot point is P1 and find another close point (of course not yet processed) and so on... until we have not processed all points.
Applying Huffman to the new file I get a significant compression, the file now is about 87K. This is a big improvement, we started from 518400 bytes. and we are at 87206 bytes.
We probably could go further applying others compression algorithms, but I decided to stop since decompression will be made inside the amiga in realtime recycling the time left from the drawing routine on each frame and it's just a tiny portion. Applying heavier algo would mean to wait a lot of time before the next rose kicks in.

As I said, 87K is not bad but... there is a problem, when Amiga decompresses the 87k file, it must be stored somewhere, and this somewhere must be a preallocated memory of size 259200.
At the end my Amiga program must have:

1. A 259200 Bytes buffer to hold data about the rose displayed on the screen
2. Another 259200 Bytes buffer to hold data avout the next rose, in this buffer each frame I will store some decompressed data from the 87K compressed buffer.
3. Another 50000 Bytes (more or less) for the MOD music (let's use small mods here)
4. Another 87000 Bytes (more or less) for each compressed rodonea, as you  can tell from the wikipedia image we have to display 6 figures from the first row.

The final bill is 259200*2+50000+87000*6 = 1090400 bytes.
We are over the limit, considering that my workbench fully loaded leave me with 750K of chip ram available, we are very far from the final goal.

----------------------------------------------------------------
And now the ultimate optimization comes in.
Since the points are usually very close to each other i want to store coordinates deltas into nibbles rather than bytes

Let's see what i can reperesent with a signed nibble


            0000  0
                 1111  -1     0001  1


        1110  -2                       0010  2




  1101  -3                                   0011  3



1100  -4                                       0100  4



  1011  -5                                   0101  5




        1010  -6                       0110  6


                 1001  -7     0111  7
                          1000  -8
                          
                          
Ok, points with -8<x<7 and be represented with nibbles but what if a coordinate does not fall in this range? It's quite rare but it can occour.
In this case i will give -8 a special meaning, if in the decompressed file you find -8 it does not mean the distance is -8 rather it means look for this value in another full byte array that must be provided along with the compression file.

