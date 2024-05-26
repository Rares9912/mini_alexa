#include <LiquidCrystal.h> 
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySoftwareSerial(6, 7); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
String readSerial;
String songName;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define pauseButton 9
#define nextButton 8


int isPaused = 0;
int isPlaying = 0;


void setup() {
  
  pinMode(10, OUTPUT);
  pinMode(nextButton, INPUT);
  pinMode(pauseButton, INPUT);
  analogWrite(10, 80);
  lcd.begin(16, 2);
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));
  lcd.print("Waiting...");

  myDFPlayer.setTimeOut(500); 

  //----Set volume----
  myDFPlayer.volume(30);  //Set volume value (0~30).

  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

}

void playSong(int songNumber) {
  myDFPlayer.play(songNumber);
  songName = readSerial;
  lcd.clear();
  isPlaying = 1;
  lcd.print("Playing " + songName);
}

void loop() {
  

  if (Serial.available()) {
    readSerial = Serial.readStringUntil('#');
    Serial.println(readSerial);
    

    if (readSerial == "Burj Khalifa") {
      playSong(1);
    }

    else if (readSerial == "vorba francezului"){
      playSong(2);
    }

    else if (readSerial == "stop song" && isPlaying) {
        myDFPlayer.stop();
        lcd.clear();
        lcd.print("Waiting...");
        isPlaying = 0;

    }             

    else if (readSerial == "pause song" && isPlaying) {
      myDFPlayer.pause();
      isPaused = 1;
      lcd.clear();
      lcd.print("Paused...");
    }             

    else if (readSerial == "resume song" && isPaused)  {     
      myDFPlayer.start();
      lcd.clear();
      lcd.print("Playing " + songName);
      isPaused = 0;
    }

    else if (readSerial == "play next song" && isPlaying) {
      myDFPlayer.next();
    }

    else if (readSerial == "play previous song" && isPlaying) {
      myDFPlayer.previous();
    }
    delay(1000);

  }

  if (digitalRead(pauseButton) == HIGH && isPlaying) {
    myDFPlayer.pause();
    isPaused = 1;
    lcd.clear();
    lcd.print("Paused...");
    delay(1000);
  } else if (digitalRead(pauseButton) == HIGH && isPaused) {
    myDFPlayer.start();
    lcd.clear();
    lcd.print("Playing " + songName);
    isPaused = 0;
    delay(1000);
  }

  if (digitalRead(nextButton) == HIGH && !isPaused) {
    myDFPlayer.next();
    isPaused = 1;
    lcd.clear();
    lcd.print("Playing");
    delay(1000);
  }
  
  
  

  
}
