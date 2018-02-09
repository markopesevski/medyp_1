// Graphing sketch

  // This program takes ASCII-encoded strings from the serial port at 9600 baud
  // and graphs them. It expects values in the range 0 to 1023, followed by a
  // newline, or newline and carriage return

  // created 20 Apr 2005
  // updated 24 Nov 2015
  // by Tom Igoe
  // This example code is in the public domain.

  import processing.serial.*;

  Serial myPort;        // The serial port
  int xPos = 1;         // horizontal position of the graph
  int lastxPos = 1;         // horizontal position of the graph
  int lastheight1 = 1;         // horizontal position of the graph
  int lastheight2 = 1;         // horizontal position of the graph
  int lastheight3 = 1;         // horizontal position of the graph
  float inByte1 = 0;
  float inByte2 = 0;
  float inByte3 = 0;
  int draw_now = 0;
  int height_of_red_line = 115*300/150;
  int height_of_orange_line = 90*300/150;

  void setup () {
    // set the window size:
    size(1000, 900);

    // List all the available serial ports
    // if using Processing 2.1 or later, use Serial.printArray()
    println(Serial.list());

    // I know that the first port in the serial list on my Mac is always my
    // Arduino, so I open Serial.list()[0].
    // Open whatever port is the one you're using.
    myPort = new Serial(this, Serial.list()[0], 115200);
    
    // set initial background:
    background(0);
    
    stroke(255, 255, 255);
    line(0, 300, width, 300);
    line(0, 600, width, 600);
    stroke(255, 0, 0);
    line(0, 900-height_of_red_line, width, 900-height_of_red_line);
    stroke(255, 144, 0);
    line(0, 900-height_of_orange_line, width, 900-height_of_orange_line);

    // don't generate a serialEvent() unless you get a newline character:
    myPort.bufferUntil('\n');
  }

  void draw () {
    if(draw_now == 1)
    {
      draw_now = 0;
      // draw the line:
      stroke(255, 0, 0);
      strokeWeight(2);        //stroke wider
      line(lastxPos, lastheight1, xPos, 300 - inByte1); 
      lastheight1 = int(300-inByte1);
      //rect(width-100, 50, 100, -50);
      //textSize(32);
      //text(inByte1,width-100,50);
      stroke(0, 255, 0);
      strokeWeight(2);        //stroke wider
      line(lastxPos, lastheight2, xPos, 600 - inByte2);
      lastheight2 = int(600-inByte2);
      stroke(0, 0, 255);
      strokeWeight(2);        //stroke wider
      line(lastxPos, lastheight3, xPos, 900 - inByte3);
      lastheight3 = int(900-inByte3);
      lastxPos= xPos;
  
      // at the edge of the screen, go back to the beginning:
      if (xPos >= width) {
        xPos = 0;
        lastxPos= 0;
        background(0);
        stroke(255, 255, 255);
        strokeWeight(2);        //stroke wider
        line(0, 300, width, 300);
        line(0, 600, width, 600);
        stroke(255, 0, 0);
        line(0, 900-height_of_red_line, width, 900-height_of_red_line);
        stroke(255, 144, 0);
        line(0, 900-height_of_orange_line, width, 900-height_of_orange_line);
      } else {
        // increment the horizontal position:
        xPos++;
      }
    }
  }

  void serialEvent (Serial myPort) {
    draw_now = 1;
    // get the ASCII string:
    String inString = myPort.readStringUntil('\n');
    String dacdds_string = inString;
    String level_string = inString;
    String analog_string = inString;
    dacdds_string = dacdds_string.substring(3,6);
    level_string = level_string.substring(9,12);
    analog_string = analog_string.substring(15,18);
    //println(inString.charAt(19));
    if(inString.charAt(19) == 'O')
    {
      print(inString);
      print('\n');
    }

    if (inString != null) {
      // trim off any whitespace:
      dacdds_string = trim(dacdds_string);
      //print(dacdds_string);
      level_string = trim(level_string);
      //print('\t');
      //print(level_string);
      analog_string = trim(analog_string);
      //print('\t');
      //println(analog_string);
      // convert to an int and map to the screen height:
      inByte1 = float(dacdds_string);
      inByte2 = float(level_string);
      inByte3 = float(analog_string);
      // println(inByte);
      inByte1 = map(inByte1, 0, 255, 0, 300);
      inByte2 = map(inByte2, 0, 255, 0, 300);
      inByte3 = map(inByte3, 0, 150, 0, 300);
    }
  }