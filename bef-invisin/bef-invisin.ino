#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);
byte note = 0;
byte resetMIDI = 4;
byte ledPin = 13;
int  instrument = 0;

int echopin = 8;
int trigpin = 9;

//tlt
int tlt1 = 6; //오른손 검지
int tlt2 = 5; //오른손 중지
int tlt3 = A0;//오른손 약지
int tlt4 = A1;//오른손 새끼
int tlt5 = A3;//왼손 엄지
int tlt6 = A4;//오른발
//val
int val = A2; //왼손 새끼
int lightVal = 0;
int val2 = 7;//왼손 엄지
int lightVal2 = 0;
int val3 = A5;//왼손 약지
int lightVal3 = 0;
//record
int start_button = 10;
int finish_button = 11;
int dataA[100] = {};
int dataB[100] = {};
int data2;
int data3;
void setup() {
  pinMode(start_button, INPUT);
  pinMode(finish_button, INPUT);
  Serial.begin(57600);
  mySerial.begin(31250);
  pinMode(resetMIDI, OUTPUT);
  digitalWrite(resetMIDI, LOW);
  delay(100);
  digitalWrite(resetMIDI, HIGH);
  delay(100);
  talkMIDI(0xB0, 0x07, 127);

  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
}

void loop()
{
  melody(0);
  if (digitalRead(start_button) == HIGH)
  {
    int dataA[100] = {};
    int dataB[100] = {};
    talkMIDI(0xB0, 0, 0x78);
    talkMIDI(0xC0, 30, 0);
    noteOn(0, 42, 120);
    delay(1000);
    talkMIDI(0xB0, 0, 0x78);
    talkMIDI(0xC0, 30, 0);
    noteOn(0, 42, 120);
    delay(1000);
    talkMIDI(0xB0, 0, 0x78);
    talkMIDI(0xC0, 30, 0);
    noteOn(0, 42, 120);
    delay(1000);
    talkMIDI(0xB0, 0, 0x78);
    talkMIDI(0xC0, 30, 0);
    noteOn(0, 42, 120);
    while (digitalRead(finish_button) ==  LOW)
    {
      for (int i = 0; i < 80; i++)
      {
        if (digitalRead(finish_button) ==  HIGH)
        {
          Serial.println("break");
          break;
        }
        if (i < 1)
        {
          Serial.println("start");
        }
        if (dataA[i] == 0)
        {
          data2 = 0;
          data3 = 0;
          melody(1);
          dataA[i] = data2;
          dataB[i] = data3;
          Serial.print(i);
          Serial.print("  ");
          Serial.print(dataA[i]);
          Serial.print("  ");
          Serial.println(dataB[i]);
          if (data2 == 0)
          {
            delay(50);
          }
          else
          {
            i = i + 3;
            delay(200);
          }
        }
        else
        {
          if (dataB[i] == 30)
          {
            talkMIDI(0xB0, 0, 0x78);
            talkMIDI(0xC0, dataB[i], 0);
            noteOn(0, dataA[i], 120);
            noteOn(0, 42, 120);
          }
          else
          {
            talkMIDI(0xB0, 0, 0x79);
            talkMIDI(0xC0, dataB[i], 0);
            noteOn(0, dataA[i], 120);
          }
          Serial.print(i);
          Serial.print("  ");
          Serial.print(dataA[i]);
          Serial.print("  ");
          Serial.println(dataB[i]);
          delay(50);
        }
        if (i > 78)
        {
          Serial.println("finish");
        }
      }
    }
  }
}

int melody(int a)
{
  lightVal = analogRead(val);
  lightVal2 = digitalRead(val2);
  lightVal3 = analogRead(val3);
  Serial.print(lightVal);
  Serial.print("  ");
  Serial.print(lightVal2);
  Serial.print("  ");
  Serial.println(lightVal3);
  //피아노
  if (lightVal3 < 680 && lightVal2  == 0 && lightVal < 680) {
    if (digitalRead(tlt1) == LOW) {
      if (digitalRead(tlt5) == LOW) {
        data2 = 67;
        data3 = 1;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 1, 0);
        noteOn(0, 67, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 60;
        data3 = 1;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 1, 0);
        noteOn(0, 60, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
    if (digitalRead(tlt2) == LOW) {
      if (digitalRead(tlt5) == LOW) {
        data2 = 69;
        data3 = 1;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 1, 0);
        noteOn(0, 69, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 62;
        data3 = 1;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 1, 0);
        noteOn(0, 62, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
    if (digitalRead(tlt3) == LOW) {
      if (digitalRead(tlt5) == LOW) {
        data2 = 71;
        data3 = 1;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 1, 0);
        noteOn(0, 71, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 64;
        data3 = 1;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 1, 0);
        noteOn(0, 64, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
    if (digitalRead(tlt4) == LOW) {
      if (digitalRead(tlt5) == LOW) {
        data2 = 72;
        data3 = 1;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 1, 0);
        noteOn(0, 72, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 65;
        data3 = 1;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 1, 0);
        noteOn(0, 65, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
  }

  //베이스
  if (lightVal3 < 680 && lightVal2 == 0 && lightVal > 680) {
    if (digitalRead(tlt1) == LOW) {
      if (digitalRead(tlt5) == HIGH) {
        data2 = 47;
        data3 = 38;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 38, 0);
        noteOn(0, 47, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 40;
        data3 = 38;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 38, 0);
        noteOn(0, 40, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
    if (digitalRead(tlt2) == LOW) {
      if (digitalRead(tlt5) == HIGH) {
        data2 = 48;
        data3 = 38;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 38, 0);
        noteOn(0, 48, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 41;
        data3 = 38;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 38, 0);
        noteOn(0, 41, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
    if (digitalRead(tlt3) == LOW) {
      if (digitalRead(tlt5) == HIGH) {
        data2 = 50;
        data3 = 38;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 38, 0);
        noteOn(0, 50, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 43;
        data3 = 38;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 38, 0);
        noteOn(0, 43, 120);
        if (a == 0)
        {
          delay(200);
        }

      }
    }
    if (digitalRead(tlt4) == LOW) {
      if (digitalRead(tlt5) == HIGH) {
        data2 = 52;
        data3 = 38;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 38, 0);
        noteOn(0, 52, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 45;
        data3 = 38;
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 38, 0);
        noteOn(0, 45, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
  }

  //어쿠스틱기타
  if (lightVal3 > 680 && lightVal2 == 0 && lightVal < 680) {
    if (digitalRead(tlt1) == LOW) {
      if (digitalRead(tlt5) == HIGH) {
        data2 = 67;
        data3 = 29;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 29, 0);
        noteOn(0, 67, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 60;
        data3 = 29;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 29, 0);
        noteOn(0, 60, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
    if (digitalRead(tlt2) == LOW) {
      if (digitalRead(tlt5) == HIGH) {
        data2 = 69;
        data3 = 29;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 29, 0);
        noteOn(0, 69, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 62;
        data3 = 29;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 29, 0);
        noteOn(0, 62, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
    if (digitalRead(tlt3) == LOW) {
      if (digitalRead(tlt5) == HIGH) {
        data2 = 71;
        data3 = 29;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 29, 0);
        noteOn(0, 71, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 64;
        data3 = 29;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 29, 0);
        noteOn(0, 64, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
    if (digitalRead(tlt4) == LOW) {
      if (digitalRead(tlt5) == HIGH) {
        data2 = 72;
        data3 = 29;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 29, 0);
        noteOn(0, 72, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 65;
        data3 = 29;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 29, 0);
        noteOn(0, 65, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
  }


  //트럼펫
  if (lightVal3 > 680 && lightVal2 == 0 && lightVal > 680) {
    if (digitalRead(tlt1) == LOW) {
      if (digitalRead(tlt5) == HIGH) {
        data2 = 67;
        data3 = 57;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 57, 0);
        noteOn(0, 67, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 60;
        data3 = 57;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 57, 0);
        noteOn(0, 60, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
    if (digitalRead(tlt2) == LOW) {
      if (digitalRead(tlt5) == HIGH) {
        data2 = 69;
        data3 = 57;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 57, 0);
        noteOn(0, 69, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 62;
        data3 = 57;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 57, 0);
        noteOn(0, 62, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
    if (digitalRead(tlt3) == LOW) {
      if (digitalRead(tlt5) == HIGH) {
        data2 = 71;
        data3 = 57;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 57, 0);
        noteOn(0, 71, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 64;
        data3 = 57;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 57, 0);
        noteOn(0, 64, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
    if (digitalRead(tlt4) == LOW) {
      if (digitalRead(tlt5) == HIGH) {
        data2 = 72;
        data3 = 57;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 57, 0);
        noteOn(0, 72, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
      else {
        data2 = 65;
        data3 = 57;
        noteOff(0, 60, 0);
        noteOff(0, 62, 0);
        noteOff(0, 64, 0);
        noteOff(0, 65, 0);
        noteOff(0, 67, 0);
        noteOff(0, 69, 0);
        noteOff(0, 71, 0);
        noteOff(0, 72, 0);
        talkMIDI(0xB0, 0, 0x79);
        talkMIDI(0xC0, 57, 0);
        noteOn(0, 65, 120);
        if (a == 0)
        {
          delay(200);
        }
      }
    }
  }

  //드럼 1
  if (lightVal3 > 680 && lightVal2 == 1 && lightVal > 680) {
    if (digitalRead(tlt6) == LOW) {
      data2 = 36;
      data3 = 30;
      talkMIDI(0xB0, 0, 0x78);
      talkMIDI(0xC0, 30, 0);
      noteOn(0, 36, 120);
      noteOn(0, 42, 120);
      if (a == 0)
      {
        delay(200);
      }
    }
    if (digitalRead(tlt1) == LOW) {
      data2 = 42;
      data3 = 30;
      talkMIDI(0xB0, 0, 0x78);
      talkMIDI(0xC0, 30, 0);
      noteOn(0, 42, 120);
      if (a == 0)
      {
        delay(200);
      }
    }
    if (digitalRead(tlt5) == LOW) {
      data2 = 38;
      data3 = 30;
      talkMIDI(0xB0, 0, 0x78);
      talkMIDI(0xC0, 30, 0);
      noteOn(0, 38, 120);
      noteOn(0, 42, 120);
      if (a == 0)
      {
        delay(200);
      }
    }
  }
  //바이올린
  if (lightVal3 > 680 && lightVal2 == 1 && lightVal < 680) {
    digitalWrite(trigpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin, LOW);
    int dist
      = pulseIn(echopin, HIGH) * 17 / 1000;
    delay(50);
    if (dist < 5 && dist >= 0)
    {
      data2 = 60;
      data3 = 41;
      noteOff(0, 60, 0);
      noteOff(0, 62, 0);
      noteOff(0, 64, 0);
      noteOff(0, 65, 0);
      noteOff(0, 67, 0);
      noteOff(0, 69, 0);
      noteOff(0, 71, 0);
      noteOff(0, 72, 0);
      talkMIDI(0xB0, 0, 0x79);
      talkMIDI(0xC0, 41, 0);
      noteOn(0, 60, 120);
      if (a == 0)
      {
        delay(200);
      }
    }
    if (dist < 10 && dist >= 5)
    {
      data2 = 62;
      data3 = 41;
      noteOff(0, 60, 0);
      noteOff(0, 62, 0);
      noteOff(0, 64, 0);
      noteOff(0, 65, 0);
      noteOff(0, 67, 0);
      noteOff(0, 69, 0);
      noteOff(0, 71, 0);
      noteOff(0, 72, 0);
      talkMIDI(0xB0, 0, 0x79);
      talkMIDI(0xC0, 41, 0);
      noteOn(0, 62, 120);
      if (a == 0)
      {
        delay(200);
      }
    }
    if (dist < 15 && dist >= 10)
    {
      data2 = 64;
      data3 = 41;
      noteOff(0, 60, 0);
      noteOff(0, 62, 0);
      noteOff(0, 64, 0);
      noteOff(0, 65, 0);
      noteOff(0, 67, 0);
      noteOff(0, 69, 0);
      noteOff(0, 71, 0);
      noteOff(0, 72, 0);
      talkMIDI(0xB0, 0, 0x79);
      talkMIDI(0xC0, 41, 0);
      noteOn(0, 64, 120);
      if (a == 0)
      {
        delay(200);
      }
    }
    if (dist < 20 && dist >= 15)
    {
      data2 = 65;
      data3 = 41;
      noteOff(0, 60, 0);
      noteOff(0, 62, 0);
      noteOff(0, 64, 0);
      noteOff(0, 65, 0);
      noteOff(0, 67, 0);
      noteOff(0, 69, 0);
      noteOff(0, 71, 0);
      noteOff(0, 72, 0);
      talkMIDI(0xB0, 0, 0x79);
      talkMIDI(0xC0, 41, 0);
      noteOn(0, 65, 120);
      if (a == 0)
      {
        delay(200);
      }
    }
    if (dist < 25 && dist >= 20)
    {
      data2 = 67;
      data3 = 41;
      noteOff(0, 60, 0);
      noteOff(0, 62, 0);
      noteOff(0, 64, 0);
      noteOff(0, 65, 0);
      noteOff(0, 67, 0);
      noteOff(0, 69, 0);
      noteOff(0, 71, 0);
      noteOff(0, 72, 0);
      talkMIDI(0xB0, 0, 0x79);
      talkMIDI(0xC0, 41, 0);
      noteOn(0, 67, 120);
      if (a == 0)
      {
        delay(200);
      }
    }
    if (dist < 30 && dist >= 25)
    {
      data2 = 69;
      data3 = 41;
      noteOff(0, 60, 0);
      noteOff(0, 62, 0);
      noteOff(0, 64, 0);
      noteOff(0, 65, 0);
      noteOff(0, 67, 0);
      noteOff(0, 69, 0);
      noteOff(0, 71, 0);
      noteOff(0, 72, 0);
      talkMIDI(0xB0, 0, 0x79);
      talkMIDI(0xC0, 41, 0);
      noteOn(0, 69, 120);
      if (a == 0)
      {
        delay(200);
      }
    }
    if (dist < 35 && dist >= 30)
    {
      data2 = 71;
      data3 = 41;
      noteOff(0, 60, 0);
      noteOff(0, 62, 0);
      noteOff(0, 64, 0);
      noteOff(0, 65, 0);
      noteOff(0, 67, 0);
      noteOff(0, 69, 0);
      noteOff(0, 71, 0);
      noteOff(0, 72, 0);
      talkMIDI(0xB0, 0, 0x79);
      talkMIDI(0xC0, 41, 0);
      noteOn(0, 71, 120);
      if (a == 0)
      {
        delay(200);
      }
    }
    if (dist < 40 && dist >= 35)
    {
      data2 = 72;
      data3 = 41;
      noteOff(0, 60, 0);
      noteOff(0, 62, 0);
      noteOff(0, 64, 0);
      noteOff(0, 65, 0);
      noteOff(0, 67, 0);
      noteOff(0, 69, 0);
      noteOff(0, 71, 0);
      noteOff(0, 72, 0);
      talkMIDI(0xB0, 0, 0x79);
      talkMIDI(0xC0, 41, 0);
      noteOn(0, 72, 120);
      if (a == 0)
      {
        delay(200);
      }
    }


  }
  //초기화
  if (lightVal3 < 680 && lightVal2 == 1 && lightVal < 680) {
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
