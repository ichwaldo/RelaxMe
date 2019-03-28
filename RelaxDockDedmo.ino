/*
 SD card read/write
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
 */
#include "Nextion.h"
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

#define ARDUINO_RX 19  //should connect to TX of the Serial MP3 Player module
#define ARDUINO_TX 18  //connect to RX of the module

#define fromandroid_rx 17
#define fromandroid_tx 16

#define relayled 2
#define relayfan 3
#define relaymusic 8

SoftwareSerial fromandroid (fromandroid_rx, fromandroid_tx);
SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);


NexText t0 = NexText(0,6,"t0");
NexText t1 = NexText(0,7,"t1");
NexText t2 = NexText(0,9,"t2");
NexText t3 = NexText(0,8,"t3");
NexText t4 = NexText(0,11,"t4");
NexText t5 = NexText(0,4,"t5");
NexProgressBar j0 = NexProgressBar(0,2,"j0");
NexProgressBar j1 = NexProgressBar(0,5,"j1");
NexProgressBar j2 = NexProgressBar(0,3,"j2");
NexCrop q0 = NexCrop(0, 10, "q0");
NexCrop q1 = NexCrop(0, 12, "q1");
NexCrop q2 = NexCrop(0, 13, "q2");

String FromAndroid;
String date;
String a, b, c, d, e, f;
int tempvalue = 0;
int bpmvalue = 0;
int gsrvalue = 0;
int stressvalue = 0;
int treatmenttime = 0;
int timer;
int start = millis ();

float tempper;
float gsrper;
float bpmper;

char buffer0[13] = {0};
char buffer1[100] = {0};
char buffer2[100] = {0};
char buffer3[100] = {0};
char buffer4[100] = {0};
char buffer5[100] = {0};


File myFile;

static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
static uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY

String mp3Answer;           // Answer from the MP3.

/************ Command byte **************************/
#define CMD_NEXT_SONG     0X01  // Play next song.
#define CMD_PREV_SONG     0X02  // Play previous song.
#define CMD_PLAY_W_INDEX  0X03
#define CMD_VOLUME_UP     0X04
#define CMD_VOLUME_DOWN   0X05
#define CMD_SET_VOLUME    0X06

#define CMD_SNG_CYCL_PLAY 0X08  // Single Cycle Play.
#define CMD_SEL_DEV       0X09
#define CMD_SLEEP_MODE    0X0A
#define CMD_WAKE_UP       0X0B
#define CMD_RESET         0X0C
#define CMD_PLAY          0X0D
#define CMD_PAUSE         0X0E
#define CMD_PLAY_FOLDER_FILE 0X0F

#define CMD_STOP_PLAY     0X16
#define CMD_FOLDER_CYCLE  0X17
#define CMD_SHUFFLE_PLAY  0x18 //
#define CMD_SET_SNGL_CYCL 0X19 // Set single cycle.

#define CMD_SET_DAC 0X1A
#define DAC_ON  0X00
#define DAC_OFF 0X01

#define CMD_PLAY_W_VOL    0X22
#define CMD_PLAYING_N     0x4C
#define CMD_QUERY_STATUS      0x42
#define CMD_QUERY_VOLUME      0x43
#define CMD_QUERY_FLDR_TRACKS 0x4e
#define CMD_QUERY_TOT_TRACKS  0x48
#define CMD_QUERY_FLDR_COUNT  0x4f

/************ Opitons **************************/
#define DEV_TF            0X02


/************************************************/

#define REDPIN 9
#define GREENPIN 10
#define BLUEPIN 11

 
#define FADESPEED 15     // make this higher to slow down

void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  Serial.begin(9600);
  pinMode(relayled, OUTPUT);  // Set Pin connected to Relay as an OUTPUT
  pinMode(relayfan, OUTPUT);
  pinMode(relayspeaker, OUTPUT);
  digitalWrite(relayled, LOW);  // Set Pin to LOW to turn Relay OFF
  digitalWrite(relayfan, LOW);
  digitalWrite(relayspeaker, LOW);
  
  mp3.begin(9600);
  nexInit();

  sendCommand(CMD_SEL_DEV, DEV_TF);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly: 
  speakeron();
  nextionloop();
  imaginary();
  fanon();

  delay(900000);
}

void nextionloop(){
     int i;
    for (i = 1; i = 1;){
    a = "AUG 11, 2017";
    b = "26";
    c = "83";
    d = "90";
    e = "85";
    f = "45";

    a.toCharArray(buffer0,20);
    tempvalue = b.toInt();
    bpmvalue = c.toInt();
    gsrvalue = d.toInt();
    stressvalue = e.toInt();
    treatmenttime = f.toInt();

    changelevel();
    j0.setValue(tempvalue);
    j1.setValue(bpmvalue); 
    j2.setValue(bpmvalue);
  
    t0.setText(buffer0);
    t1.setText(buffer1);
    t2.setText(buffer2);
    t3.setText(buffer3);
    t4.setText(buffer4);
    t5.setText(buffer5);
    
//    memset(buffer0, 0, sizeof(buffer0));
//    itoa(date, buffer0, 10);
    memset(buffer1, 0, sizeof(buffer1));
    itoa(tempvalue, buffer1, 10);
    memset(buffer2, 0, sizeof(buffer2));
    itoa(gsrvalue, buffer2, 10);
    memset(buffer3, 0, sizeof(buffer3));
    itoa(bpmvalue, buffer3, 10);
    memset(buffer4, 0, sizeof(buffer4));
    itoa(stressvalue, buffer4, 10);
    memset(buffer5, 0, sizeof(buffer5));
    itoa(treatmenttime, buffer5, 10);
    
    
  
    tempper = (tempvalue/40)*100;
    gsrper = (gsrvalue/1000)*100;
    bpmper = (bpmvalue/121)*100;
    
    break;
  }
  }

void changelevel()
{
    if (stressvalue <= 40)
    {
    q0.setPic(1);
    //Serial.println("LOW");
    }
    else if ((stressvalue > 40) && (stressvalue <=70))
    {
    q1.setPic(2);
    //Serial.println("MEDIUM");
    }
    else if (stressvalue >70)
    {
    q2.setPic(3);
    //Serial.println("HIGH");
    } 
}

void getstring(String text){
  int colon1,colon2,colon3,colon4,colon5,pjg;
  pjg = text.length();
  colon1 = text.indexOf('-');
  colon2 = text.indexOf('_');
  colon3 = text.indexOf('*');
  colon4 = text.indexOf('|');
  colon5 = text.indexOf('/');
  a = text.substring(0,colon1);
  b = text.substring(colon1+1,colon2);
  c = text.substring(colon2+1,colon3);
  d = text.substring(colon3+1,colon4);
  e = text.substring(colon4+1,colon5);
  f = text.substring(colon5+1,pjg);
}

void imaginary(){
  while (millis() - start <= timer){
    digitalWrite(relayled, HIGH);
    ledon();
  }
  digitalWrite(relayled,LOW);
}

void ledon(){
    
    int r, g, b;
    
  // fade from blue to violet
//  for (r = 0; r < 256; r++) { 
//    analogWrite(REDPIN, r);
//    delay(FADESPEED);
//  } 
  // fade from violet to red
  for (b = 255; b > 0; b--) { 
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
  } 
  // fade from red to yellow
  for (g = 0; g < 256; g++) { 
    analogWrite(GREENPIN, g);
    delay(FADESPEED);
  } 
  // fade from yellow to green
//  for (r = 255; r > 0; r--) { 
//    analogWrite(REDPIN, r);
//    delay(FADESPEED);
//  } 
  // fade from green to teal
  for (b = 0; b < 256; b++) { 
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
  } 
  // fade from teal to blue
  for (g = 255; g > 0; g--) { 
    analogWrite(GREENPIN, g);
    delay(FADESPEED);
  } 
}


void fanon(){
  while (millis() - start <= timer){
  digitalWrite(relayfan, HIGH);
  }
  digitalWrite(relayfan,LOW);
}

void speakeron(){
  while (millis() - start <= timer){
  digitalWrite(relaymusic, HIGH);
  musicloop();
  int i;
  for (i = 0; i = 1; i++)
  {
  if (i = 0)
  {
    sendCommand(CMD_RESET, 0x00);
  }
  if (i = 1)
  {
    sendCommand(CMD_PLAY, 0);
  }
  }
  }
  digitalWrite(relaymusic,LOW);
}

void sdsetup(){
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (!SD.begin(4)) {
    return;
  }
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("data.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

//void receivedata(){
//  int i;
//  String Str;
//  if (Serial.available()>0){
//  FromAndroid = Serial.readString();
//  for (i = 1; i = 5;i++){
//    String indexD1 = Str.indexOf('|');
//    Str.substring(0,
//      
////    date = Serial.readStringUntil('|');
////    i+ = 1;
////      switch(i){
////        case 2: temp = Serial.readStringUntil('|');
////                break;
////        case 3: tempvalue = Serial.readStringUntil('|');
////                break;
////        case 4: gsrvalue = Serial.readStringUntil('|');
////                break;
////        case 5: bpmvalue = Serial.readStringUntil('|');
////                break;
////        case 6: stresslevel = Serial.readStringUntil('|');
////                break;
////        case 6: stresslevel = Serial.readStringUntil('|');
////                break;
//                    
//    }
//    
//  }
//}

void musicloop()
{
    char c = ' ';

  // If there a char on Serial call sendMP3Command to sendCommand
  if ( Serial.available() )
  {
    c = Serial.read();
    sendMP3Command(c);  
  }

  // Check for the answer.
  if (mp3.available())
  {
    //Serial.println(decodeMP3Answer());
  }
  delay(100);
}

/********************************************************************************/
/*Function sendMP3Command: seek for a 'c' command and send it to MP3  */
/*Parameter: c. Code for the MP3 Command, 'h' for help.                                                                                                         */
/*Return:  void                                                                */

void sendMP3Command(char c) {
  switch (c) {
    case '?':
    case 'h':
      Serial.println("HELP  ");
      Serial.println(" p = Play");
      Serial.println(" P = Pause");
      Serial.println(" > = Next");
      Serial.println(" < = Previous");
      Serial.println(" + = Volume UP");
      Serial.println(" - = Volume DOWN");
      Serial.println(" c = Query current file");
      Serial.println(" q = Query status");
      Serial.println(" v = Query volume");
      Serial.println(" x = Query folder count");
      Serial.println(" t = Query total file count");
      Serial.println(" 1 = Play folder 1");
      Serial.println(" 2 = Play folder 2");
      Serial.println(" 3 = Play folder 3");
      Serial.println(" 4 = Play folder 4");
      Serial.println(" 5 = Play folder 5");
      Serial.println(" S = Sleep");
      Serial.println(" W = Wake up");
      Serial.println(" r = Reset");
      break;


    case 'p':
      Serial.println("Play ");
      sendCommand(CMD_PLAY, 0);
      break;

    case 'P':
      Serial.println("Pause");
      sendCommand(CMD_PAUSE, 0);
      break;

    case '>':
      Serial.println("Next");
      sendCommand(CMD_NEXT_SONG, 0);
      sendCommand(CMD_PLAYING_N, 0x0000); // ask for the number of file is playing
      break;

    case '<':
      Serial.println("Previous");
      sendCommand(CMD_PREV_SONG, 0);
      sendCommand(CMD_PLAYING_N, 0x0000); // ask for the number of file is playing
      break;

    case '+':
      Serial.println("Volume Up");
      sendCommand(CMD_VOLUME_UP, 0);
      break;

    case '-':
      Serial.println("Volume Down");
      sendCommand(CMD_VOLUME_DOWN, 0);
      break;

    case 'c':
      Serial.println("Query current file");
      sendCommand(CMD_PLAYING_N, 0);
      break;

    case 'q':
      Serial.println("Query status");
      sendCommand(CMD_QUERY_STATUS, 0);
      break;

    case 'v':
      Serial.println("Query volume");
      sendCommand(CMD_QUERY_VOLUME, 0);
      break;

    case 'x':
      Serial.println("Query folder count");
      sendCommand(CMD_QUERY_FLDR_COUNT, 0);
      break;

    case 't':
      Serial.println("Query total file count");
      sendCommand(CMD_QUERY_TOT_TRACKS, 0);
      break;

    case '1':
      Serial.println("Play folder 1");
      sendCommand(CMD_FOLDER_CYCLE, 0x0101);
      break;

    case '2':
      Serial.println("Play folder 2");
      sendCommand(CMD_FOLDER_CYCLE, 0x0201);
      break;

    case '3':
      Serial.println("Play folder 3");
      sendCommand(CMD_FOLDER_CYCLE, 0x0301);
      break;

    case '4':
      Serial.println("Play folder 4");
      sendCommand(CMD_FOLDER_CYCLE, 0x0401);
      break;

    case '5':
      Serial.println("Play folder 5");
      sendCommand(CMD_FOLDER_CYCLE, 0x0501);
      break;

    case 'S':
      Serial.println("Sleep");
      sendCommand(CMD_SLEEP_MODE, 0x00);
      break;

    case 'W':
      Serial.println("Wake up");
      sendCommand(CMD_WAKE_UP, 0x00);
      break;

    case 'r':
      Serial.println("Reset");
      sendCommand(CMD_RESET, 0x00);
      break;
  }
}



/********************************************************************************/
/*Function decodeMP3Answer: Decode MP3 answer.                                  */
/*Parameter:-void                                                               */
/*Return: The                                                  */

String decodeMP3Answer() {
  String decodedMP3Answer = "";

  decodedMP3Answer += sanswer();

  switch (ansbuf[3]) {
    case 0x3A:
      decodedMP3Answer += " -> Memory card inserted.";
      break;

    case 0x3D:
      decodedMP3Answer += " -> Completed play num " + String(ansbuf[6], DEC);
      break;

    case 0x40:
      decodedMP3Answer += " -> Error";
      break;

    case 0x41:
      decodedMP3Answer += " -> Data recived correctly. ";
      break;

    case 0x42:
      decodedMP3Answer += " -> Status playing: " + String(ansbuf[6], DEC);
      break;

    case 0x48:
      decodedMP3Answer += " -> File count: " + String(ansbuf[6], DEC);
      break;

    case 0x4C:
      decodedMP3Answer += " -> Playing: " + String(ansbuf[6], DEC);
      break;

    case 0x4E:
      decodedMP3Answer += " -> Folder file count: " + String(ansbuf[6], DEC);
      break;

    case 0x4F:
      decodedMP3Answer += " -> Folder count: " + String(ansbuf[6], DEC);
      break;
  }

  return decodedMP3Answer;
}






/********************************************************************************/
/*Function: Send command to the MP3                                         */
/*Parameter:-int8_t command                                                     */
/*Parameter:-int16_ dat  parameter for the command                              */

void sendCommand(int8_t command, int16_t dat)
{
  delay(20);
  Send_buf[0] = 0x7e;   //
  Send_buf[1] = 0xff;   //
  Send_buf[2] = 0x06;   // Len
  Send_buf[3] = command;//
  Send_buf[4] = 0x01;   // 0x00 NO, 0x01 feedback
  Send_buf[5] = (int8_t)(dat >> 8);  //datah
  Send_buf[6] = (int8_t)(dat);       //datal
  Send_buf[7] = 0xef;   //
  Serial.print("Sending: ");
  for (uint8_t i = 0; i < 8; i++)
  {
    mp3.write(Send_buf[i]) ;
    Serial.print(sbyte2hex(Send_buf[i]));
  }
  Serial.println();
}



/********************************************************************************/
/*Function: sbyte2hex. Returns a byte data in HEX format.                 */
/*Parameter:- uint8_t b. Byte to convert to HEX.                                */
/*Return: String                                                                */


String sbyte2hex(uint8_t b)
{
  String shex;

  shex = "0X";

  if (b < 16) shex += "0";
  shex += String(b, HEX);
  shex += " ";
  return shex;
}




/********************************************************************************/
/*Function: sanswer. Returns a String answer from mp3 UART module.          */
/*Parameter:- uint8_t b. void.                                                  */
/*Return: String. If the answer is well formated answer.                        */

String sanswer(void)
{
  uint8_t i = 0;
  String mp3answer = "";

  // Get only 10 Bytes
  while (mp3.available() && (i < 10))
  {
    uint8_t b = mp3.read();
    ansbuf[i] = b;
    i++;

    mp3answer += sbyte2hex(b);
  }

  // if the answer format is correct.
  if ((ansbuf[0] == 0x7E) && (ansbuf[9] == 0xEF))
  {
    return mp3answer;
  }

  return "???: " + mp3answer;
}

