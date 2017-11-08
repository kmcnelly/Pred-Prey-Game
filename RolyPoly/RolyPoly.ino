/*
   Kenneth McNelly
   457250
   CSE 132 C

   Predator vs. Prey Game
*/
#include <Wire.h> // Must include Wire library for I2C
#include <SparkFun_MMA8452Q.h> // Includes the SFE_MMA8452Q library

MMA8452Q accel;

const int frameTime = 2;// 2ms for normal
const int maxCol = 5;
const int maxRow = 7;

int pinRow[maxRow];//array of rows
int pinCol[maxCol];//array of columns

int preyRow = 2; //temporary values
int preyCol = 3;

int predRow = 4; //temporary values
int predCol = 1;


//prey delta time
const int preyWaitTime = 300;
unsigned long nextPreyTime = preyWaitTime;

//pred delta time
const int predWaitTime = preyWaitTime * 2;
unsigned long nextPredTime = predWaitTime;

//pred blink delta time
const int predBlink = 250; //4 Hz
unsigned long nextPredBlink = predBlink;

//game mechanics:======================
enum State {
  preGame, inGame, roundOver
};
//preGame mechanics:===================

//round mechanics:

//round delta time
const long roundTime = 15000; //15s
unsigned long nextRoundTime = roundTime;

//Score mechanics:=====================

//if prey won last game
boolean preyWin; 
//accumulated score of prey
int preyScore = 0;
//accumulated score of pred
int predScore = 0;

//row for score
const int bottomRow = 6;
const int pinPrey3 = 0; //prey winning by 3 or more
const int pinPrey2 = 1; //prey winning by 2-1
const int pinTie = 2; //tied score
const int pinPred2 = 3; //pred winning by 1-2
const int pinPred3 = 4; //pred winning by 3 or more
//score timing
const int pauseTime = 4000; //4s pause at end of game of game



State s = preGame;


void setup()
{
  Serial.begin(9600);
  for (int r = 0; r < maxRow; r++)
  {
    pinRow[r] = r + 2;
    pinMode(pinRow[r], OUTPUT);
  }
  for (int c = 0; c < maxCol; c++)
  {
    pinCol[c] = c + 9;
    pinMode(pinCol[c], OUTPUT);
  }
  accel.init();

  randomSeed(analogRead(0));
}

//if it is delta timing turn for accelerometer
void checkPrey() {
  if (accel.available())
  {
    accel.read();
    if (accel.cx < -0.20 && preyCol > 0) {
      preyCol = preyCol - 1;
    }
    if (accel.cx > 0.20 && preyCol < (maxCol - 1)) {
      preyCol = preyCol + 1;
    }
    if (accel.cy < -0.20 && preyRow < (maxRow - 1)) {
      preyRow = preyRow + 1;
    }
    if (accel.cy > 0.20 && preyRow > 0) {
      preyRow = preyRow - 1;
    }
  }
}


//if it is delta timing turn for accelerometer
void checkPred() {
  if (Serial.available()) {
    byte input = Serial.read();
    if (input == 'u' && predRow > 0) {
      predRow = predRow - 1;
    }
    if (input == 'd' && predRow < (maxRow - 3)) {
      predRow = predRow + 1;
    }
    if (input == 'l' && predCol > 0) {
      predCol = predCol - 1;
    }
    if (input == 'r' && predCol < (maxCol - 2)) {
      predCol = predCol + 1;
    }
  }
}

//checks collision between prey and pred //returns boolean
boolean checkCollide() {
  if (preyRow == predRow || preyRow == (predRow + 1))
  {
    if (preyCol == predCol || preyCol == (predCol + 1))
    {
      return true;
    }
  }
  return false;
}

//clears the light grid & display curr positions of pred & prey
void displayGrid() {
  for (int c = 0; c < maxCol; c++) {
    delay(frameTime / 2);
    digitalWrite(pinCol[c], LOW);

    //sets rest of cols to high (clears grid)
    for (int i = 0; i < maxCol; i++) {
      if (i != c)
      {
        digitalWrite(pinCol[i], HIGH);
      }
    }

    //steps through rows
    for (int r = 0; r < maxRow; r++)
    {
      //Draw Prey------------------------
      if (preyCol == c && preyRow == r)
      {
        digitalWrite(pinRow[r], HIGH);
      }
      // Draw Predator---------------------
      else if (predCol == c && predRow == r)
      {
        digitalWrite(pinRow[r], HIGH);
      }
      else if (predCol == (c - 1) && predRow == r)
      {
        digitalWrite(pinRow[r], HIGH);
      }
      else if (predCol == c && predRow == (r - 1))
      {
        digitalWrite(pinRow[r], HIGH);
      }
      else if (predCol == (c - 1) && predRow == (r - 1))
      {
        digitalWrite(pinRow[r], HIGH);
      }
      else
      {
        digitalWrite(pinRow[r], LOW);
      }
    }
  }
}

void displayScore(int row)
{
  //prey won last round
  if(preWins)
  {
    int diff = predScore - preyScore;
    if(diff == 0)
    {
      
    }
  }
  //predator won last round
  else
  {
    
  }
 
}

//display lastround winning character and ratio
void displayEndRound(){
   for (int c = 0; c < maxCol; c++) {
    delay(frameTime / 2);
    digitalWrite(pinCol[c], LOW);

    //sets rest of cols to high (clears grid)
    for (int i = 0; i < maxCol; i++) {
      if (i != c)
      {
        digitalWrite(pinCol[i], HIGH);
      }
    }

    //steps through rows
    for (int r = 0; r < maxRow; r++)
    {
      //Draw score
      if (r == bottomRow)
      {
        
      }
    
   }
}

State nextState(State state) {
  switch (state) {
    case preGame:
      {
        //set predator and prey in random locations

        //random predator start position
        int randPredCol = random(maxCol - 1);
        int randPredRow = random(maxRow - 1);

        predCol = randPredCol;
        predRow = randPredRow;

        //random prey start position (does not allow them to be right next to each other)
        do
        {
          preyCol = random(maxCol);
        } while (preyCol == predCol || preyCol == predCol - 1 || preyCol == predCol + 1 || preyCol == predCol + 2);

        do 
        {
          preyRow = random(maxRow);
        } while (preyRow == predRow || preyRow == predRow - 1 || preyRow == predRow + 1 || preyRow == predRow + 2);

        displayGrid();
        s = inGame;
      }
    break;
    case inGame:
    {
      s = roundOver;
    }
    break;
    case roundOver:
    {
      //display score
      //delay(pauseTime);
      s = preGame;
    }
    break;
  }
}







//===============================================================
void loop ()
{
  if(s == preGame)
  {
    s = nextState(s);
  }
  //while game is running
  if (s == inGame)
  {
    if (millis() > nextPreyTime)
    {
      checkPrey();
      nextPreyTime = millis() + preyWaitTime;
    }
    if (millis() > nextPredTime)
    {
      checkPred();
      nextPredTime = millis() + predWaitTime;
    }
    displayGrid();

    //end of round check
    //predator wins
    if (checkCollide())
    {
      preyWins = false;
      predScore++;
      s = nextState(s);
    }
    //prey wins
    else if ( millis() > roundTime)
    {
      preyWins = true;
      preyScore++;
      s = nextState(s);
    }
  }
  else if (s == roundOver)
  {
      s = nextState(s);
  }
}


