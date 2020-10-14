//...............................................НАСТРОЙКИ........................................
#define leftBbtnBin 8
#define rightBbtnBin 9
#define boomBbtnBin 10
#define boomLedPin 13
#define RX 5
#define TX 4
#define SerialNxSpeed 9600
#define SerialSpeed 9600
//...............................................БИБЛИОТЕКИ.......................................
#include <SoftwareSerial.h>
#include <Nextion.h>

//...............................................ПОТКЛЮЧЕНИЯ......................................
SoftwareSerial nextion(TX, RX);
Nextion myNextion(nextion, SerialNxSpeed);
//...............................................ПЕРЕМЕННЫЕ.......................................
int kill=0;
int playerX=0;
int playerY=230;
int badX=0;
int badY=0;
int badSpeed=1;
int badSpeedTimer=badSpeed;
int Speed=70;
//...............................................НАСТРОЙКИ АВТО...................................
void setup() {
  Serial.begin(SerialSpeed);
  myNextion.init();
  movePlayer("player", 0, 0);
  badX=randomBad();
  pinMode(leftBbtnBin,INPUT_PULLUP);
  pinMode(rightBbtnBin,INPUT_PULLUP);
  pinMode(boomBbtnBin,INPUT_PULLUP);
  pinMode(boomLedPin,OUTPUT);
   delay(500);
  writeNumber("n0","val",3);
  delay(500);
  writeNumber("n0","val",2);
  delay(500);
  writeNumber("n0","val",1);
  delay(500);
}
//..............................................ОСНОВНАЯ ПРОГРАММА................................
void loop() {
  int rightBtn = !digitalRead(rightBbtnBin);
  int leftBtn = !digitalRead(leftBbtnBin);
  int boomBtn = !digitalRead(boomBbtnBin);
  
    Serial.print("rightBtn ");
    Serial.println(rightBtn);
    Serial.print("leftBtn ");
    Serial.println(leftBtn);
    if(rightBtn==1) {
      goPlayerToRight();
    }
    if(leftBtn==1) {
      goPlayerToLeft();
    }
  boolean gameOver = playBad();  
  if(gameOver) {
    return;
  }
  if(boomBtn){
    Serial.print("Boom badX="+String(badX)+" playerX="+String(playerX));
    if(playerX==badX){
     kill++;
     led();
     killBad(badX, badY);
     badX=randomBad();
     int fromBadY = badY;
     badY=0;
     moveBad("bad", badX, fromBadY, badY);
     Speed-3;
    }
  }
  writeNumber("n0","val",kill);
  delay(Speed);
}

boolean playBad() {
  if(badSpeedTimer==badSpeed) {  
    int fromBadY=badY;
    badY=badY+10;
    moveBad("bad", badX, fromBadY, badY);
    if(badY==230) {
        drawGameOver(); //кабачооок!
        return true;
      
      badY=0;
      moveBad("bad", badX, 230, badY);
    }
    badSpeedTimer--;
  } else if(badSpeedTimer==0) {
    badSpeedTimer=badSpeed;
  } else {
    badSpeedTimer--;
  }
    Serial.print("badSpeedTimer ");
    Serial.println(badSpeedTimer);
  return false;
}

void goPlayerToRight() {
  int from = playerX;
  switch(playerX) {
    case 0 :
      playerX=playerX+64;
    break;
    case 64 :
      playerX=playerX+64;
    break;
    case 128 :
      playerX=playerX+64;
    break;
    case 192 :
      playerX=playerX+64;
    break;
    default:
    break;
  }
    String msg = "ToRight="+String(playerX);
    boolean r = movePlayer("player", from, playerX);
    Serial.println(msg);
}

void goPlayerToLeft() {
  int from = playerX;
  switch(playerX) {
    case 64 :
      playerX=playerX-64;
    break;
    case 128 :
      playerX=playerX-64;
    break;
    case 192 :
      playerX=playerX-64;
    break;
    case 256 :
      playerX=playerX-64;
    break;
    default:
    break;
  }
    String msg = "ToLeft="+String(playerX);
    boolean r = movePlayer("player", from, playerX);
    Serial.println(msg);
}

boolean movePlayer(String playerComponent, int from, int to) {
  //move <comp>,<x1>,<y1>,<x2>,<y2>,<priority>,<time
  String command = "move "+playerComponent + ","+String(from) + ",230" + ","+String(to) + ",230,100,100";
  String tempCmd = "pic "+String(from)+",230,2";
  myNextion.sendCommand(tempCmd.c_str());
  tempCmd = "pic "+String(to)+",230,1";
  myNextion.sendCommand(tempCmd.c_str());
  return true;
}

boolean moveBad(String badComponent, int x, int from, int to) {
  String tempCmd = "pic "+String(x)+","+String(from)+",2";
  myNextion.sendCommand(tempCmd.c_str());
  tempCmd = "pic "+String(x)+","+String(to)+",0";
  myNextion.sendCommand(tempCmd.c_str());
  return true;
}

boolean killBad(int x, int y) {
  String tempCmd = "pic "+String(x)+","+String(y)+",2";
  myNextion.sendCommand(tempCmd.c_str());
  return true;
}

void drawGameOver() {
  writeNumber("page0","pic",4);
  }

void writeNumber(String component, String param, int number) {
  String componentText = component + "."+param+"=" + String(number);//Set Component text
  myNextion.sendCommand(componentText.c_str());
}

int randomBad (){
  byte RandomBad=random(5);
  switch(RandomBad){
    case 0:
      return 0;
    case 1:
      return 64;
    case 2:
      return 128;
    case 3:
      return 192;
    case 4:
      return 256;
  }
}

void led (){
   digitalWrite(boomLedPin,1);
      delay(100);
      digitalWrite(boomLedPin,0);
      delay(100);
      digitalWrite(boomLedPin,1);
      delay(100);
      digitalWrite(boomLedPin,01);
      delay(100);
      digitalWrite(boomLedPin,1);
      delay(100);
      digitalWrite(boomLedPin,0);
      delay(100);
}
