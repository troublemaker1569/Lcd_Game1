#include "LiquidCrystal.h"
#include <Arduino.h>
#include "EEPROM.h"
const int rs = 2, en = 3, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int but1= 5, but2 = 6;
int stbut1 = 0,stbut2 = 0;
bool y_figure;
unsigned long Time;
unsigned long startTime;
int row2[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int row1[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int delayTime = 500;
long randTemple;
unsigned long scoreTime;
#define razmak = 1;
int razdalja = 0;
int score;
int highScore;
int sum1 = 0;
int sum2 = 0;

byte figureAnimatonUp1[] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000
};
byte figureAnimatonUp2[] = {
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte figureAnimatondown[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B00000
};
byte figureAnimaton2[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B10101,
  B11111,
  B00000
};
byte figureAnimaton1[] = {
  B00000,
  B00011,
  B00011,
  B00011,
  B00011,
  B00011,
  B00011,
  B00000
};
byte figure[] = {
  B00000,
  B00000,
  B11111,
  B10001,
  B10101,
  B10001,
  B11111,
  B00000
};
byte templedown[] = {
  B00000,
  B00100,
  B01110,
  B11111,
  B01010,
  B01010,
  B01010,
  B01010
};
byte templeup[] = {
  B01010,
  B01010,
  B01010,
  B01010,
  B11111,
  B01110,
  B00100,
  B00000
};


void GameOverUp(){
  lcd.setCursor(3,y_figure);
  lcd.write('\07');
  delay(250);
  lcd.setCursor(3,y_figure);
  lcd.write('\06');
  delay(250);
  lcd.setCursor(3,y_figure);
  lcd.write('\05');
  delay(250);
}
void GameOverDown(){
  lcd.setCursor(3,y_figure);
  lcd.write('\04');
  delay(250);
  lcd.setCursor(3,y_figure);
  lcd.write('\05');
  delay(250);
}
void GameOverFront(){
  lcd.setCursor(3,y_figure);
  lcd.write('\03');
  delay(250);
  lcd.setCursor(3,y_figure);
  lcd.write('\04');
  delay(250);
  lcd.setCursor(3,y_figure);
  lcd.write('\05');
  delay(250);
}
void GameOver(){
  score = (millis() - scoreTime)/1000;
  lcd.setCursor(5,0);
  lcd.print("Score: ");
  lcd.print(score);
  if(score > highScore){
  highScore  = score;
  }
  lcd.setCursor(2, 1);
  EEPROM.write(0, highScore);
  lcd.print("HighScore: ");
  lcd.println(highScore);
  while (digitalRead(but1) || digitalRead(but2));
  while (!digitalRead(but1) || !digitalRead(but2));
  y_figure = 1;
  lcd.setCursor(3,y_figure);
  lcd.write('\00');
  razdalja = 0;
  for(int i = 0; i < 16 ; i++){
    row1[i] = 0;
    row2[i] = 0;
  } 
  lcd.clear();
  startTime = millis();
  scoreTime = millis();
  delayTime = 500;

}


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(0, figure);
  lcd.createChar(1, templedown);
  lcd.createChar(2, templeup);
  lcd.createChar(3, figureAnimaton1);
  lcd.createChar(4, figureAnimaton2);
  lcd.createChar(5, figureAnimatondown);
  lcd.createChar(6, figureAnimatonUp1);
  lcd.createChar(7, figureAnimatonUp2);

  lcd.home();
  pinMode(but1, INPUT);
  pinMode(but2, INPUT);
  startTime = millis();
  scoreTime = millis();
  lcd.setCursor(3,1);
  lcd.write('\00');
  highScore = EEPROM.read(0);
  
}

void loop() {

  Time = millis();

 //map  
  if(Time - startTime > delayTime){
    //game over 
    if ( (y_figure == 0 && row1[4]) || (y_figure == 1 && row2[4]) ) {
      GameOverFront();
      GameOver();
    }

    for (int i = 1; i <= 15 ; i++){       
      row2[i-1] = row2[i];
      row1[i-1] = row1[i];
      
    }
    razdalja++;
  if(razdalja == 3){
    randTemple = random(2);
    if(randTemple == 0){
      row1[15] = 1;
      row2[15] = 0;
    }
    else if(randTemple == 1){
      row2[15] = 1;
      row1[15] = 0;
    }
    // else{
    //   row2[15] = 0;
    //   row1[15] = 0;
    // } 
    razdalja = 0;
   }

   else {
      row2[15] = 0;
      row1[15] = 0;
   }
  int sum1 = 0; 
  int sum2 = 0;
 
  for ( int i = 0; i < 16; i++ ){
    sum1 += row1[ i ];
    sum2 += row2[ i ];
   
    Serial.print("sum2 :");
    Serial.println(sum2);
    Serial.print("sum1 :");
    Serial.println(sum1);
   if (sum1 > 3){
      row2[15] = 1;
      row1[15] = 0;
   }
   if (sum2 > 3){
      row1[15] = 1;
      row2[15] = 0;
   }
   
}

    for (int i = 0; i <= 15; i++){
      lcd.setCursor(i,1);
     if(row2[i]) lcd.write('\01');
     else lcd.write(" ");
    }
    for (int i = 0; i <= 15; i++){
     lcd.setCursor(i,0);
     if(row1[i]) lcd.write('\02');
     else lcd.write(" ");
    }
    startTime = Time;
    if(delayTime > 400){
     delayTime = delayTime-10;
    }
    else if(delayTime > 120){
     delayTime = delayTime-3;
    }
    else{
     delayTime = 119;
    }
  }  


 //movement 
  stbut1 = digitalRead(but1);
  stbut2 = digitalRead(but2);
 
if(stbut1 == 1){
  if (row1[3]){
    GameOverUp();
    GameOver();
  }
  else{
  y_figure = 0;
  }
  
  }
if (stbut2 == 1){
  if (row2[3]){
    GameOverDown();
    GameOver();
  }
  else{
  y_figure = 1;
  }
 }

  lcd.setCursor(3, y_figure);
  lcd.write('\00');
  lcd.setCursor(3, !y_figure);
  if(!y_figure){
    if(row2[3]) lcd.write('\01');
    else lcd.write(" ");
  }
  else{
    if(row1[3]) lcd.write('\02');
    else lcd.write(" ");
  }
}