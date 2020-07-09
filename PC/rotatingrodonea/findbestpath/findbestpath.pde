// Coding Train
// Ported to processing by Max (https://github.com/TheLastDestroyer)
// Origional JS by Daniel Shiffman
// http://patreon.com/codingtrain
// Code for this video: https://youtu.be/BAejnwN4Ccw

PVector[] cities;
int totalCities = 360;

float recordDistance;
PVector[] bestEver;

void setup() {
  size(400, 300);
  byte b[] = loadBytes("firstposition.bin"); 
  for (int i = 0; i < 720; i++) { 
    // Every tenth number, start a new line 
    if ((i % 10) == 0) { 
      println(); 
    } 
    // bytes are from -128 to 127, this converts to 0 to 255 
    int a = b[i] & 0xff; 
    print(a + " "); 
  }    
  // Print a blank line at the end 
  println(); 
  cities = new PVector[totalCities];
  bestEver = new PVector[totalCities];
  
  int a = 0;
  for (int i = 0; i < totalCities; i++) {
    //PVector v = new PVector(random(width), random(height));
    PVector v = new PVector(b[a] & 0xff,b[a+1] & 0xff);
    a+=2;
    cities[i] = v;
  }

  float d = calcDistance(cities);
  recordDistance = d;
  arrayCopy(cities, bestEver);

}

void draw() {
  background(0);
  fill(255);
  for (int i = 0; i < cities.length; i++) {
    ellipse(cities[i].x, cities[i].y, 8, 8);
  }

  stroke(255);
  strokeWeight(1);
  noFill();
  beginShape();
  for (int i = 0; i < cities.length; i++) {
    vertex(cities[i].x, cities[i].y);
  }
  endShape();

  stroke(255, 0, 255);
  strokeWeight(1);
  noFill();
  beginShape();
  for (int i = 0; i < cities.length; i++) {
    vertex(bestEver[i].x, bestEver[i].y);
  }
  endShape();



  int i = floor(random(cities.length));
  int j = floor(random(cities.length));
  swap(cities, i, j);

  float d = calcDistance(cities);
  if (d < recordDistance) {
    recordDistance = d;
    arrayCopy(cities, bestEver);
    
    byte[] data={};
    data = new byte[720];
    int index = 0;
    for (int count=0;count<bestEver.length;count++)
    {
      int x = (int)bestEver[count].x;
      int y = (int)bestEver[count].y;
      data[index++]=(byte)(x&0x000000FF);
      data[index++]=(byte)(y&0x000000FF);
      saveBytes("positionsCompressable.bin", data);
    }
  }
}

void swap(PVector[] a, int i, int j) {
  PVector temp = a[i];
  a[i] = a[j];
  a[j] = temp;
}


float calcDistance(PVector[] points) {
  float sum = 0;
  for (int i = 0; i < points.length - 1; i++) {
    float d = dist(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    sum += d;
  }
  return sum;
}
