#include "../src/vectors.h"
#include "../src/matrix.h"

#define MAXD 8

typedef struct _tPoint
{
    WORD x[360];
    WORD y[360];
}point;

point vertexDataRotated[360];

typedef struct _tFlower
{
    int n; // numerator
    int d; // denominator
    WORD vertexData[360*2*MAXD];
}tFlower;

#define MAXFLOWERS 1
tFlower FLOWERS[MAXFLOWERS];



void createFlower(int n,int d,int iMaxPetalLength, int iTranslation)
{
    static int flowerIndex=0;
    if (flowerIndex>MAXFLOWERS-1)
    {
        fprintf(stderr,"Not able to build more than %d flowers \n",MAXFLOWERS);
        return ;
    }
    if (d>MAXD)
    {
        fprintf(stderr,"Not able to build a flower with d>%d\n",MAXD);
        return ;
    }
    //printf("Creating flower for %d %d\n",n,d);
    FLOWERS[flowerIndex].n=n;
    FLOWERS[flowerIndex].d=d;

    //float k = (float)n/(float)d;
    fix16_t k =fix16_div(  fix16_from_int(n),   fix16_from_int(d)    );
    //float stepper = (float)1 / (float)STEP;
    
    int a;
    for ( a = 0; a < 360*d; a ++ )
    {
        
            //float r = 5 * cos(k * a);
            //char buf[100];
            
            int iCosIndex = fix16_to_int( fix16_mul( k,fix16_from_int(a) ) );

            fix16_t r = fix16_mul(  fix16_from_int(iMaxPetalLength)   , fix16_coslist[  iCosIndex%360 ]  );

            //printf("Init %d\n",index*3);
            //FLOWERS[flowerIndex].vertexData[index * 3] = r * cos(a);
            
            FLOWERS[flowerIndex].vertexData[a * 2] = /*(WORD) iTranslation*/ + (WORD) fix16_to_int(fix16_mul(  r   ,  fix16_coslist[a%360]   ));

            /*fix16_to_str(r, buf, 2);
            //printf("r vale %s uscita %d\n",buf,FLOWERS[flowerIndex].vertexData[a * 2]);*/

            //fix16_to_str(r, buf, 2);

            //getchar();
            
            FLOWERS[flowerIndex].vertexData[a * 2 + 1] = /*(WORD) iTranslation +*/ (WORD) fix16_to_int ( fix16_mul(  r   ,  fix16_sinlist[a%360]   ));
            
            
    }

    
    flowerIndex++;
}


int rotatePoints(int iDegree)
{
    // projection matrix
    mf16 projection = {
      2,  // Rows
      3,  // Columns
      0,  // Errors

      // Matrix actual data
      {
         { fix16_from_int(1), fix16_from_int(0), fix16_from_int(0) }, // Row 1
         { fix16_from_int(0), fix16_from_int(1), fix16_from_int(0) } // Row 2
      }
    };

    fix16_t rotationAngle = fix16_from_int(iDegree);

    fix16_t rotationAngleRad = fix16_deg2rad(rotationAngle);

    mf16 rotationZ = 
      {
         3,  // Rows
         3,  // Columns
         0,  // Errors
         // Matrix actual data
         {
            { fix16_cos(rotationAngleRad), fix16_mul(fix16_sin(rotationAngleRad),fix16_from_int(-1)),   fix16_from_int(0) }, // Row 1
            { fix16_sin(rotationAngleRad), fix16_cos(rotationAngleRad),                                 fix16_from_int(0) }, // Row 2
            { fix16_from_int(0),           fix16_from_int(0),                                           fix16_from_int(1) }  // Row 3
         }
      };

    mf16 rotationX = 
    {
         3,  // Rows
         3,  // Columns
         0,  // Errors

         // Matrix actual data
         {
         { fix16_from_int(1),       fix16_from_int(0),                 fix16_from_int(0),                                          },  // Row 1
         { fix16_from_int(0),       fix16_cos(rotationAngleRad),       fix16_mul(fix16_sin(rotationAngleRad),fix16_from_int(-1))   },  // Row 2
         { fix16_from_int(0),       fix16_sin(rotationAngleRad),       fix16_cos(rotationAngleRad)                                 }   // Row 3
         }
    };

    for (int i=0;i<360;i++)
    {
        // Init my point
        //v3d point = points[i];
        v3d point ;
        point.x = fix16_from_int((int)FLOWERS[0].vertexData[i * 2]);
        point.y = fix16_from_int((int)FLOWERS[0].vertexData[i * 2 + 1]);
        point.z = 0;

        // Rotate along X axis
        v3d rotatedX;
        if (mf16_mul_v3d_to_v3d(&rotatedX, &rotationZ, &point))
        {
            printf("Error on v3d mult (1)\n");
            return 1;
        }

        // Project the rotated vector
        mf16 projected2d;
        mf16_mul_v3d(&projected2d, &projection, &rotatedX);
        if (projected2d.errors)
        {
            printf("Error on v3d mult (2)\n");
            return 1;
        }

        v2d projected2dvector;
        mf16_to_v2d(&projected2dvector,&projected2d);

        // memorizzo iesimo punto a 45 gradi
        vertexDataRotated[iDegree].x[i] = fix16_to_int(projected2dvector.x)+150;
        vertexDataRotated[iDegree].y[i] = fix16_to_int(projected2dvector.y)+150;

     /*  char buf[100];

        printf("Point %d: ",i+1);
        fix16_to_str(projected2dvector.x,buf,2);
        printf("%s,",buf);
        fix16_to_str(projected2dvector.y,buf,2);
        printf("%s\n",buf);*/
    }

    return 0;
}