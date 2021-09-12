/***************************************************
  This is our Bitmap drawing example for the Adafruit HX8357 Breakout
  ----> http://www.adafruit.com/products/2050
***************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <SoftwareSerial.h>
#include "Adafruit_HX8357.h"
#include <SPI.h>
#include <SD.h>
#include <stdint.h>
#include "TouchScreen.h"

// TFT
#define YP A0
#define XM A1
#define YM 7
#define XP 8
#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define TS_MINX 110
#define TS_MINY 80
#define TS_MAXX 900
#define TS_MAXY 940
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define TFT_RST -1  // dont use a reset pin, tie to arduino RST if you like
#define TFT_DC 9
#define TFT_CS 10
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC);
#define SD_CS 4
#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;


//악기부분-----------------------
SoftwareSerial Proto(2, 3);
const int Pins[4] = {A2, A3, A4, A5};

// 악기variables
int Values[4] = {0, 0, 0, 0};
int prevs[4] = {0,};
int curs[4];

char inst;


void setup(void) {
  Serial.begin(115200);
  Proto.begin(9600);
  tft.begin(HX8357D);
  tft.fillScreen(HX8357_BLACK);

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
  }
  Serial.println("OK!");

  bmpDraw("jumpers.bmp", 0, 0);
}

void loop(void) {

  TSPoint p = ts.getPoint();

  if (p.z < MINPRESSURE || p.z > MAXPRESSURE || p.x < 10) {
    //Proto.print(inst);
    Finger(inst);
    return;
  }

  inst = SelectIns(p);
  Serial.println(inst);
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

}

#define BUFFPIXEL 20

void bmpDraw(char *filename, uint8_t x, uint16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if ((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if (read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())  w = tft.width()  - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

        for (row = 0; row < h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col = 0; col < w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r, g, b));
          } // end pixel
        } // end scanline
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if (!goodBmp) Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File & f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File & f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}



char SelectIns(TSPoint p) {

  if (p.y > 860) {
    if (p.x > 650 && p.x < TS_MAXX) {
      Proto.print('d');
      Proto.print(0);
      delay(10);
      Proto.print('b');
      Proto.print(8);
      delay(1000);
      return 'b';//Base
    }
    else if (p.x > 505 && p.x < 650) {
      Proto.print('d');
      Proto.print(0);
      delay(1000);
      return 'd'; //Drum
    }
    else if (p.x > 350 && p.x < 505) {
      return 'o'; //OMG
    }
  }
  else if (p.y < 860) {
    if (p.x > 650 && p.x < TS_MAXX) {
      Proto.print('d');
      Proto.print(0);
      delay(10);
      Proto.print('p');
      Proto.print(8);
      delay(1000);
      return 'p';//piano  y 820~850
    }
    else if (p.x > 505 && p.x < 650) {
      Proto.print('d');
      Proto.print(0);
      delay(10);
      Proto.print('e');
      Proto.print(0);
      delay(1000);
      return 'e'; //elec guitar y 780~850
    }
    else if (p.x > 350 && p.x < 505) {
      Proto.print('d');
      Proto.print(0);
      delay(10);
      Proto.print('v');
      Proto.print(8);
      delay(1000);
      return 'v'; // va yol in
    }
  }

  return 0;

}

//가속도센서부분 센서리딩
void Finger(char inst)
{

  // Read values of finger
  for (int i = 0; i < 4; i++) {
    Values[i] = analogRead(Pins[i]);
  }


  //mapping values
  //map(변수,높은값,낮은값,변할최솟값,변할최대값);
  for (int i = 0; i < 4; i++) {
    curs[i] = map(Values[i], 500, 290, 0, 210);
  }



  if (prevs[0] - curs[0] > 70) {
    Serial.println("bang0");
    Proto.print(inst);
    Proto.print(0);
  }
  else if (prevs[1] - curs[1] > 70) {
    Serial.println("bang1");
    Proto.print(inst);
    Proto.print(1);
  }
  else if (prevs[2] - curs[2] > 70) {
    Serial.println("bang2");
    Proto.print(inst);
    Proto.print(2);
  }
  else if (prevs[3] - curs[3] > 70) {
    Serial.println("bang3");
    Proto.print(inst);
    Proto.print(3);
  }
  delay(50);
  for (int i = 0; i <= 3; i++) {
    prevs[i] = curs[i];
  }
  return;
}

