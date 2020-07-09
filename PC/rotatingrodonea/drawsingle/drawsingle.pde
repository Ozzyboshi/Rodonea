int angle = 0;
byte b[] ;
void setup()
{
  size(320,256);
  //noLoop();
   b = loadBytes("../pos/positionsCompressed30.bin"); 
 
  // Print each value, from 0 to 255 
  int x=0;
  int y=0;
  for (int i = 0; i < b.length; i+=2) { 
    // Every tenth number, start a new line 
    if ((i % 10) == 0) { 
      println(); 
    } 
    // bytes are from -128 to 127, this converts to 0 to 255 
    int a = b[i] & 0xff; 
    
    print(b[i] + " "); 
    
    x+=(int)b[i];
    y+=(int)b[i+1];
    
    point(x,y);
  } 
  // Print a blank line at the end 
  println(); 
}

void draw()
{
  background(255);
  b = loadBytes("../pos/positionsCompressed"+angle+".bin"); 
  int x=0;
  int y=0;
  for (int i = 0; i < b.length; i+=2) { 
    // Every tenth number, start a new line 
    if ((i % 10) == 0) { 
      println(); 
    } 
    
    
    x+=(int)b[i];
    y+=(int)b[i+1];
    
    point(x,y);
  } 
  angle++;
  if (angle>=360) angle = 0;
}
