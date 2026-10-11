#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ShiftRegister74HC595.h>

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 32, 2, NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);
ShiftRegister74HC595<2> sr(11, 12, 10);

uint32_t time1 = 500;
float time2 = 500;
int x = -1, y = 30, x1 = 4;
int s = 5, s1 = 2, s2 = 30, s3;
bool x2 = true;

uint8_t pinValues[10][2] = {
  { 0x3F, 0x3F },
  { 0x3F, 0x06 },
  { 0x3F, 0x5B },
  { 0x3F, 0x4F },
  { 0x3F, 0x66 },
  { 0x3F, 0x6D },
  { 0x3F, 0x7D },
  { 0x3F, 0x07 },
  { 0x3F, 0x7F },
  { 0x3F, 0x6F }
};

void Score_win() {
  if (s2 - y == 10) {
    s2 = y, s3++;
    for (int i = 0; i < 10; i++)
      pinValues[i][0] = pinValues[s3][1];
  }
  sr.setAll(&pinValues[s2 - y][0]);
  if (y == 0) {
    for (int i = 0; i < 20; i++) {
      Serial.println(" ");
      if (i == 9) {
        Serial.println("------------------GAME WIN!!!.------------------");
        continue;
      }
    }
    while (1)
      if (Serial.available() && Serial.read() == 32)
        digitalWrite(13, 1);
  }
}

void Game_Over() {
  if (x1 <= 0) {
    for (int i = 0; i < 20; i++) {
      if (i == 9) {
        Serial.println("------------------GAME OVER...------------------");
        continue;
      }
      Serial.println(" ");
    }
    while (1)
      if (Serial.available() && Serial.read() == 32)
        digitalWrite(13, 1);
  }
}

void check_Score() {
  if ((x != s && x2 == 1)) {
    if (s > x) {
      x1 -= s - x;
      s = x;
    } else {
      x1 -= x - s;
      s1 += x - s;
    }
    Game_Over();
    Serial.println("Good!!");
  } else if (x != s1 && x2 == 0) {
    if (s1 > x) {
      x1 -= s1 - x;
      s -= s1 - x;
    } else {
      x1 -= x - s1;
      s1 = x;
    }
    Game_Over();
    Serial.println("Good!!");
  } else Serial.println("Perfect!!");

  for (int i = s + 1; i < 8; i++)
    matrix.drawPixel(i, y, matrix.Color(0, 0, 0));
  for (int i = s1 - 1; i > -1; i--)
    matrix.drawPixel(i, y, matrix.Color(0, 0, 0));
  matrix.show();
}

void Up_floor() {
  y--, time2 -= 16.25;
  if (y % 2 == 0) x = -1, x2 = 1;
  else x = 8, x2 = 0;
  time1 = millis();
  if (x1 == 2 && x == 8 || x1 == 2 && x == -1) {
    displayShow();
    time1 = millis() + time2;
  }
}

void Check_limits() {
  if (x == x1 + 6) x2 = 0, x = 7;
  else if (x == -3 + (4 - x1)) x2 = 1, x = 0;
}

void displayShow() {
  if (x2 == 1) matrix.drawPixel(++x - x1, y, matrix.Color(0, 0, 0));
  else matrix.drawPixel(--x + x1, y, matrix.Color(0, 0, 0));
  matrix.drawPixel(x, y, matrix.Color(255, 0, 0));
  matrix.show();
}

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.setBrightness(255);
  for (int i = 2; i < 6; i++)
    matrix.drawPixel(i, 31, matrix.Color(255, 0, 0));
  Serial.println("----------Press the Enter key to play.----------");
  sr.setAll(&pinValues[30 - y][0]);
  pinMode(13, OUTPUT);
  matrix.show();
}

void loop() {
  if (Serial.available() && Serial.read() == 13) {
    time1 = millis() + 500;
    delay(500);
    for (int i = 0; i < 10; i++)
      Serial.read();
    while (1) {
      if (millis() > time1) {
        Check_limits();
        displayShow();
        time1 += time2;
      }
      if (Serial.available() && Serial.read() == 13) {
        check_Score();
        Up_floor();
        Score_win();
      }
    }
  }
}
