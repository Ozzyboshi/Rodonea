import controlP5.*;

ControlP5 cp5;

boolean goDraw=false;
byte[] data = {};
byte[] dataDelta = {};
int index=0;
int indexDelta = 0;
int mypointsIndex =0 ;
float angle =0;
int xyOutOfBoundsCounter = 0;

CheckBox checkbox;
DropdownList dropdown;
Numberbox xSpacing;
Numberbox ySpacing;


myPoint[] myPoints = new myPoint[360];
PVector[] points = new PVector[360];
int d=1;
   int n=3;
   int petalLength=100;
   
   float[][] projection = {
     {1,0,0},
     {0,1,0}
   };

void setup()
{
  //noLoop();
   size(320,256);
   cp5 = new ControlP5(this);
   
   
                    
   /*xSpacing = cp5.addNumberbox("X Spacing")
     .setPosition(20,20)
     .setSize(60,20)
     .setScrollSensitivity(1)
     .setValue(60)
     .setRange(0,200)
     .setDirection(Controller.HORIZONTAL)
     ;
                    
    ySpacing = cp5.addNumberbox("Y Spacing")
     .setPosition(80,20)
     .setSize(60,20)
     .setScrollSensitivity(1)
     .setValue(28)
     .setRange(0,200)
     .setDirection(Controller.HORIZONTAL)
     ;*/

  
  cp5.addButton("StartSim")
     .setValue(0)
     .setPosition(220,0)
     .setSize(100,19)
     .addCallback(new CallbackListener() {
      public void controlEvent(CallbackEvent event) {
        if (event.getAction() == ControlP5.ACTION_RELEASED) {
          println("button released.");
          cp5.remove(event.getController().getName());
          goDraw=true;
          n = (int)cp5.getController("Numeratore").getValue();
          cp5.getController("Numeratore").hide();
          checkbox.hide();
          dropdown.hide();
          /*xSpacing.hide();
          ySpacing.hide();*/
          float k = (float)n/(float)d;
          for ( int a = 0; a < 360*d; a ++ )
          {//
            float r = petalLength * cos(k * a);
            points[a] = new PVector(r*cos(a),r*sin(a),0);
          }
        }
      }
    });
    
    cp5.addButton("Pause/Resume Sim")
     .setValue(0)
     .setPosition(220,30)
     .setSize(100,19)
     .addCallback(new CallbackListener() {
      public void controlEvent(CallbackEvent event) {
        if (event.getAction() == ControlP5.ACTION_RELEASED) {
          goDraw=!goDraw;
        }
      }
    });
    
    cp5.addSlider("Numeratore")
     .setPosition(100,070)
     .setWidth(200)
     .setHeight(100)
     .setRange(2,7) 
     .setValue(2)
     .setNumberOfTickMarks(6)
     .setSliderMode(Slider.FLEXIBLE)
   /*  .addCallback(new CallbackListener() {
      public void controlEvent(CallbackEvent event) {
          println("Slider changed val");        
      }
    })*/
     ;
     
     checkbox = cp5.addCheckBox("checkBox")
                .setPosition(100, 200)
                .setSize(40, 40)
                .setItemsPerRow(3)
                .setSpacingColumn(30)
                .setSpacingRow(20)
                .addItem("X rot", 0)
                .addItem("Y Rot", 50)
                .addItem("Z Rot", 100)
                .toggle(2)
                ;
                
    dropdown = cp5.addDropdownList("Stop angle")
                .setPosition(0, 100)
                .setBarHeight(15)
                //.setSize(40, 40)
                .setItemHeight(20)
                .addItem("360", 360)
                .addItem("180", 180)
                .addItem("1", 1)
                .setValue(0)
                ;
   
   /*float k = (float)n/(float)d;
   int a;
  for ( a = 0; a < 360*d; a ++ )
  {
    float r = petalLength * cos(k * a);
    points[a] = new PVector(r*cos(a),r*sin(a),0);
  }*/
  data = new byte[
    360 // 360 punti
  //  *2 // ogni posizione occupa 2 bytes
    *2 // 2 coordinate , x e y
    * 360 // angolo giro
    ];
    
    dataDelta = new byte[
    360 // 360 punti
  //  *2 // ogni posizione occupa 2 bytes
    *2 // 2 coordinate , x e y
    * 360 // angolo giro
    ];
    
  frameRate(25);

}


void draw()
{
  if (goDraw==false) return;
  background(0);
  
  float stopAngle = (float) cp5.getController("Stop angle").getValue()==0?360:(float) cp5.getController("Stop angle").getValue()==1?180:1;

  textSize(16);
  text("Angle:"+angle+"/"+stopAngle, 180, 230); 
  
  stroke(255);
  strokeWeight(1);
  
  float angleRad = radians(angle);
  
  float[][] rotationX = {
    { 1, 0, 0},
    { 0, cos(angleRad), -sin(angleRad)},
    { 0, sin(angleRad), cos(angleRad)}
  };

  float[][] rotationY = {
    { cos(angleRad), 0, sin(angleRad)},
    { 0, 1, 0},
    { -sin(angleRad), 0, cos(angleRad)}
  };
  
  float[][] rotationZ = {
    { cos(angleRad), -sin(angleRad), 0},
    { sin(angleRad), cos(angleRad), 0},
    { 0, 0, 1}
  };
  
  int xRotFlag =  (int)checkbox.getArrayValue()[0];
  int yRotFlag =  (int)checkbox.getArrayValue()[1];
  int zRotFlag =  (int)checkbox.getArrayValue()[2];
  
  if (xRotFlag == 0 && yRotFlag == 0 && zRotFlag == 0)
  {
    println("please select at least one rotation flag");
    exit();
  }
  
  int oldx=0;
  int oldy=0;
  
  for (int a = 0; a < 360*d; a ++ )
  {
    PVector rotated = null;
    if (xRotFlag != 0)
    {
      rotated = matmul(rotationX, points[a]);
      if (yRotFlag != 0) rotated = matmul(rotationY, rotated);
      if (zRotFlag != 0) rotated = matmul(rotationZ, rotated);
    }
    else if (yRotFlag != 0)
    {
      rotated = matmul(rotationY, points[a]);
      if (zRotFlag != 0) rotated = matmul(rotationZ, rotated);
    }
    else rotated = matmul(rotationZ, points[a]);
    
    PVector projected2d = matmul(projection,rotated);
    //point(projected2d.x+width/2,projected2d.y+height/2);
    int x = (int)projected2d.x+petalLength;
    int y = (int)projected2d.y+petalLength;
    
    if (x<0||x>255||y<0||y>255)
    {
      print("Error: x out of bounds:"+(int)x);
      exit();
    }
    
    //byte xbytehigh =(byte) ((x&0x0000FF00)>>8);
    //point(x,y);
    //println("Posizione X "+x);
    //println("Posizione Y "+y);
   data[index++]=(byte)(x&0x000000FF);
   data[index++]=(byte)(y&0x000000FF);
   myPoints[a] = new myPoint (x,y);
   
   if (a==0)
   {
     dataDelta[indexDelta++]=(byte)(x&0x000000FF);
     dataDelta[indexDelta++]=(byte)(y&0x000000FF);
     point(x,y);
   }
   else
   {
     int deltaX=x-oldx;
     int deltaY=y-oldy;
     /*deltaX=1;
     deltaY=2;*/
     //println("xdiff : "+deltaX);

     dataDelta[indexDelta++]=(byte)(deltaX&0x000000FF);
     dataDelta[indexDelta++]=(byte)(deltaY&0x000000FF);
     point(oldx+deltaX,oldy+deltaY);
   }
   oldx=x;
   oldy=y;
   //exit();
  }
  reorderpoints();
  angle ++;
  if (angle>=stopAngle)
  {
    saveBytes("positions.bin", data);
    saveBytes("positionsDelta.bin", dataDelta);
    //reorderpoints();
    exit();
  }
}

void reorderpoints()
{
  int indexmypoint2=0;
  myPoint bestPoint=null;
  myPoint[] myPoints2 = new myPoint[360];
  arrayCopy(myPoints, myPoints2);
  
  myPoint origPoint = new myPoint(0,0);
  float minDistance = 10000;
  int counter = 0;
  do
  {
    bestPoint=null;
    minDistance = 10000;
    counter =0 ;
    for ( myPoint point : myPoints)
    {
      // se è gia stato elaborato non vale
      if (point.elaborated==1) continue;
      float distance = origPoint.distance(point);
      if (distance<minDistance)
      {
        minDistance = distance;
        bestPoint = point;
      }
      counter++;
    }
    if (bestPoint!=null)
    {
      int xDelta=round(bestPoint.x-origPoint.x);
      int yDelta=round(bestPoint.y-origPoint.y);
      if (xDelta>7 || xDelta < -6 || yDelta>7 || yDelta < -6)
      {
        xyOutOfBoundsCounter++;
        println("X out of bounds!!!!!!!Xdelta"+xDelta+" YDelta "+yDelta+"conteggio totale "+xyOutOfBoundsCounter);
      }
      /*if (indexmypoint2==0)
      {
        if (60+bestPoint.x-origPoint.x>255)
        {
          println("Cannot save point "+(int)(60+bestPoint.x-origPoint.x));
          exit();
        }
        println("X point helper "+(int)(60+bestPoint.x-origPoint.x));
        myPoints2[indexmypoint2++]=new myPoint(60+bestPoint.x-origPoint.x,bestPoint.y-origPoint.y);
      }
      
      else*/
        myPoints2[indexmypoint2++]=new myPoint(bestPoint.x-origPoint.x,bestPoint.y-origPoint.y);
      bestPoint.mark();
      origPoint=bestPoint;
    }
    // println("counter : "+counter);
  }while (counter>0);
  
  
  byte[] dataCompressed = {};
  dataCompressed = new byte[
    360 // 360 punti
    *1 // ogni posizione occupa 1 byte
    *2 // 2 coordinate , x e y
    // angolo giro
    ];
    
    byte[] dataCompressedOneByte = {};
    dataCompressedOneByte = new byte[
    360 // 360 punti
    /2 // ogni posizione occupa mezzo byte
    *2 // 2 coordinate , x e y
    // angolo giro
    ];
    
    byte[] dataCompressedOneByteHelper = {};
    
    int b=0;
  for (int i =0 ; i<360;i++)
  {
    
    // Inizio memorizzazione su 1 byte
    int xDelta = ((int)myPoints2[i].x&0x000000FF);
    int yDelta = ((int)myPoints2[i].y&0x000000FF);
    if ((byte)xDelta>7 || (byte)xDelta < -7)
    {
      println("confermo out of bounds: xDelta",(byte)xDelta);
      dataCompressedOneByteHelper = append(dataCompressedOneByteHelper,(byte)xDelta); // save the value to put in the helper array
      xDelta=0x00000008; // Since we are off range force the value 00 00 00 08
    }
    if ((byte)yDelta>7 || (byte)yDelta<-7)
    {
      println("confermo out of bounds: yDelta",(byte)yDelta);
      dataCompressedOneByteHelper = append(dataCompressedOneByteHelper,(byte)yDelta); // save the value to put in the helper array
      yDelta = 0x00000008;
    }
    dataCompressedOneByte[i]= (byte)( (xDelta<<4) | 0x0000000F&yDelta );
    /*if (xDelta>7 || xDelta < -7 || yDelta>7 || yDelta < -7)
      dataCompressedOneByte[i]= 0b;
    else
      dataCompressedOneByte[i]= (byte)( (xDelta<<4) | 0x0000000F&yDelta );*/
    // Fine memorizzazione su 1 byte
    
    //println("Bestpoint "+i+" "+myPoints2[i].desc());
    dataCompressed[b++]=(byte)((int)myPoints2[i].x&0x000000FF);
     dataCompressed[b++]=(byte)((int)myPoints2[i].y&0x000000FF);
  }
  saveBytes("pos/positionsCompressed"+(int)angle+".bin", dataCompressed);
  
  // Salvo versione 1 byte
  saveBytes("pos1byte/positionsCompressed"+(int)angle+".bin", dataCompressedOneByte);
  saveBytes("pos1byte/positionsCompressed"+(int)angle+".bin.help", dataCompressedOneByteHelper);

}

float[][] vecToMatrix(PVector v) {
  float[][] m = new float[3][1];
  m[0][0] = v.x;
  m[1][0] = v.y;
  m[2][0] = v.z;
  return m;
}

PVector matrixToVec(float[][] m) {
  PVector v = new PVector();
  v.x = m[0][0];
  v.y = m[1][0];
  if (m.length > 2) {
    v.z = m[2][0];
  }
  return v;
}

void logMatrix(float[][] m) {
  int cols = m[0].length;
  int rows = m.length;
  println(rows + "x" + cols);
  println("----------------");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      print(m[i][j] + " ");
    }
    println();
  }
  println();
}


PVector matmul(float[][] a, PVector b) {
  float[][] m = vecToMatrix(b);
  return matrixToVec(matmul(a,m));
}

float[][] matmul(float[][] a, float[][] b) {
  int colsA = a[0].length;
  int rowsA = a.length;
  int colsB = b[0].length;
  int rowsB = b.length;

  if (colsA != rowsB) {
    println("Columns of A must match rows of B");
    return null;
  }

  float result[][] = new float[rowsA][colsB];

  for (int i = 0; i < rowsA; i++) {
    for (int j = 0; j < colsB; j++) {
      float sum = 0;
      for (int k = 0; k < colsA; k++) {
        sum += a[i][k] * b[k][j];
      }
      result[i][j] = sum;
    }
  }
  return result;
}






/// START OF CUSTOM CLASS
class myPoint {
  float x;
  float y;
  int elaborated;
  myPoint(float x,float y)
  {
      this.x = x;
      this.y = y;
      this.elaborated = 0;
  }
  String desc()
  {
     return "X: "+this.x+" Y: "+this.y;
  }
  float distance(myPoint p)
  {
    return dist(this.x,this.y,p.x,p.y);
    //return abs(sqrt(this.x*p.x+this.y*p.y));
  }
  void mark()
  {
    this.elaborated=1;
  }
}
