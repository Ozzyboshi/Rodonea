import controlP5.*;

byte[] data = {};

int STEPS=45;
byte b[] ;

byte dest[] ;
ControlP5 cp5;
void setup()
{
  size(320,256);
  b = loadBytes("./position_0_deg.bin"); 
  dest = loadBytes("./positiondest_0_deg.bin"); 
  
  
     cp5 = new ControlP5(this);

  cp5.addSlider("Numeratore")
     .setPosition(00,00)
     .setWidth(100)
     .setHeight(10)
     .setRange(1,STEPS) 
     .setValue(STEPS)
     .setNumberOfTickMarks(STEPS)
     .setSliderMode(Slider.FLEXIBLE)
   /*  .addCallback(new CallbackListener() {
      public void controlEvent(CallbackEvent event) {
          println("Slider changed val");        
      }
    })*/
     ;
     
     data = new byte[
    360 // 360 punti
  //  *2 // ogni posizione occupa 2 bytes
    *2 // 2 coordinate , x e y
    * 44 // 
    ];
     
     frameRate(1);
}
int n=STEPS-1;
int index=0;
void draw()
{
  
  //int n = (int)cp5.getController("Numeratore").getValue();
  background(0);
  stroke(255);
  strokeWeight(1);
  
  for (int cont = 0; cont<720;cont+=2)
  {
  
    int x=0x000000FF&b[cont];
    //x=x>>8;
    int y=0x000000FF&b[cont+1];
    
    //print(x);
    
    //if (cont<=10) 
    //point(x,y);
    
    int xDest  = 0x000000FF&dest[cont];;
    int yDest  = 0x000000FF&dest[cont+1];
    
    
    
    //if (cont<=10)
    {
      //PVector v1 = new PVector(200-x, 200-y);
      PVector v1 = new PVector(xDest-x, yDest-y);
      float m = v1.mag();
      //print(m);
      v1.normalize();
      float mapoutput = map(n, 0, STEPS, 0, m);
      v1.setMag(m-mapoutput);
      strokeWeight(1);
      /*println("x:"+v1.x+x);
      println("y:"+v1.y+y);*/
      
      int xFinal = (int)(v1.x+x);
      int yFinal = (int)(v1.y+y);
      
      if (xFinal<0||xFinal>255||yFinal<0||yFinal>255)
      {
        print("Error: x out of bounds:"+xFinal+" "+yFinal);
        exit();
        return ;

      }
      
      
      point (xFinal,yFinal);
      
      data[index++]=(byte)(xFinal&0x000000FF);
   data[index++]=(byte)(yFinal&0x000000FF);
      
    }
    
    strokeWeight(10);

    //point (xDest,yDest);
  }
  
  
    n=n-1;
  if (n==0)
  {
    saveBytes("transition1.bin", data);
  exit();
  }
}
