//for midi shield
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
SoftwareSerial Proto(5, 6);

byte note = 0;
byte resetMIDI = 4;
byte ledPin = 13;
int  instrument = 0;
String str = "";

const int Pins[4] = {A1, A2, A3, A4};

// variables
int Values[4] = {0, 0, 0, 0};
int prevs[4] = {0,};
int curs[4];
int notesOfpiano[9] = {60, 62, 64, 65, 67, 69, 71, 72, 0};
int notesOfBase[9] = {40 , 41, 43, 45, 47, 48, 50, 52, 0};
int notesOfElecC[5] = {60, 64, 67, 71, 0};
int notesOfElecD[5] = {62, 065, 69, 72, 0};
int notesOfElecE[5] = {64, 67, 71, 74, 0};
int notesOfElecF[5] = {65, 69, 72, 76, 0};

int notesOfDrum[5] = {30, 36, 38, 42, 0};

int notesOfViolin[9] = {60, 62, 64, 65, 67, 69, 71, 72, 0};

int notesOfOMG[9] = {60, 62, 64, 65, 67, 69, 71, 72, 0};

char inst = 'p';
char code = 10;

void setup() {
  Serial.begin(57600);
  mySerial.begin(31250);
  Proto.begin(9600);
  pinMode(resetMIDI, OUTPUT);
  digitalWrite(resetMIDI, LOW);
  delay(100);
  digitalWrite(resetMIDI, HIGH);
  delay(100);
  talkMIDI(0xB0, 0x07, 127);
}

void loop() {
  inst = LeftFinger();
  Finger(inst);
}


void Bang(char inst, int level) {
  if (inst == 'p') {
    talkMIDI(0xB0, 0, 0x79);
    talkMIDI(0xC0, 1, 0);
    noteOn(0, notesOfpiano[level], 120);
  }
  else if (inst == 'b') {
    talkMIDI(0xB0, 0, 0x79);
    talkMIDI(0xC0, 38, 0);
    noteOn(0, notesOfBase[level], 120);
  }
  else if (inst == 'e') {
    if (level >= 0 && level <= 3) {
      code = level;
      return;
    }
    talkMIDI(0xB0, 0, 0x79);
    talkMIDI(0xC0, 29, 0);
    if (code == 0) {
      noteOn(0, notesOfElecC[level - 4], 100);
    }
    else if (code == 1) {
      noteOn(0, notesOfElecD[level - 4], 100);
    }
    else if (code == 2) {
      noteOn(0, notesOfElecE[level - 4], 100);
    }
    else if (code == 3) {
      noteOn(0, notesOfElecF[level - 4], 100);
    }
  }
  else if (inst == 'd') {
    talkMIDI(0xB0, 0, 0x78);
    talkMIDI(0xC0, 30, 0);
    noteOn(0, notesOfDrum[level], 120);
  }
  else if (inst == 'v') {
    talkMIDI(0xB0, 0, 0x79);
    talkMIDI(0xC0, 41, 0);
    noteOff(0, 60, 0);
    noteOff(0, 62, 0);
    noteOff(0, 64, 0);
    noteOff(0, 65, 0);
    noteOff(0, 67, 0);
    noteOff(0, 69, 0);
    noteOff(0, 71, 0);
    noteOff(0, 72, 0);
    noteOn(0, notesOfViolin[level], 120);
  }
  else if (inst == 'o') {
    noteOn(0, notesOfOMG[level], 120);
  }
  else if (inst == 0) {
    noteOff(0, 60, 0);
    noteOff(0, 62, 0);
    noteOff(0, 64, 0);
    noteOff(0, 65, 0);
    noteOff(0, 67, 0);
    noteOff(0, 69, 0);
    noteOff(0, 71, 0);
    noteOff(0, 72, 0);
  }
}

char LeftFinger() {
  char ch = 0;
  if (Proto.available()) {

    while (Proto.available()) {
      inst = (char)Proto.read();
      ch = (int)Proto.read();
      if (ch - 48 >= 0) {
        Bang(inst, ch - 48);
      }



      delay(20);

    }
    Serial.print(inst);
    Serial.print("       ");
    Serial.println((int)ch - 48);

  }
  return inst;
}


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


  for (int i = 0; i < 4; i++) {
    if (i == 3) {
      if (prevs[i] - curs[i] > 80) {
        Serial.print("bang");
        Serial.println(i + 4);
        Bang(inst, i + 4);
      }
    }
    if (prevs[i] - curs[i] > 70) {
      Serial.print("bang");
      Serial.println(i + 4);
      Bang(inst, i + 4);
    }
  }

  delay(50);
  for (int i = 0; i <= 3; i++) {
    prevs[i] = curs[i];
  }
}



void noteOn(byte channel, byte note, byte attack_velocity)
{
  talkMIDI( (0x90 | channel), note, attack_velocity);
}
void noteOff(byte channel, byte note, byte release_velocity)
{
  talkMIDI( (0x80 | channel), note, release_velocity);
}
void talkMIDI(byte cmd, byte data1, byte data2)
{
  digitalWrite(ledPin, HIGH);
  mySerial.write(cmd);
  mySerial.write(data1);

  if ( (cmd & 0xF0) <= 0xB0)
    mySerial.write(data2);

  digitalWrite(ledPin, LOW);
}
