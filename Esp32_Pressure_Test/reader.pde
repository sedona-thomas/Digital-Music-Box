/**
 * Simple Read
 * 
 * Read data from the serial port and change the color of a rectangle
 * when a sensor connected to an Arduino board is changed.
 */


import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;      // Data received from the serial port
int lf = 10;      // ASCII linefeed

void setup() 
{
  size(200, 200);
  // Open whatever port is the one you're using.
  print(Serial.list());
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
}

void draw()
{
  String s = "";
  if ( myPort.available() > 0) {  // If data is available,
    s = myPort.readString();
    try {
      s = s.split("\n")[1].strip();
      val = Integer.parseInt(s);
    }
    catch (Exception e) {
      println(e);
    }
  }
  
  background(0);  
  fill(val,255-val,val);
  println(val);
  rect(50, 50, val, val);
}


