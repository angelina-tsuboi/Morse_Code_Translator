#include <arduino-timer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


const int dashLED = 2;
const int dotLED = 3;
const int buzzerPin = 4;
const int rightButton = 5;
const int centerButton = 6;
const int leftButton = 7;
const int translateButton = 8;
const int potPin = A0;

int freqValue = 0;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

char codeArray[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' '};
char topRow[16];

int cursorX = 0;
auto timer = timer_create_default();

void setup() {
  // put your setup code here, to run once:
  pinMode(dotLED, OUTPUT);
  pinMode(dashLED, OUTPUT);
  pinMode(centerButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(leftButton, INPUT);
  pinMode(translateButton, INPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  intitializeBoard();
  lcd.blink();
}

void loop() {
    timer.tick();
    freqValue = analogRead(potPin);
    lcd.setCursor(cursorX, 0);

  delay(200);
  if(digitalRead(leftButton)){
    moveCursor(-1);
  }

  if(digitalRead(centerButton)){
   changeCharacter();
  }
  
  if(digitalRead(rightButton)){
    moveCursor(1);
  }

  if(digitalRead(translateButton)){
    translateMorse();
  }

}

void moveCursor(int direction){
  if(direction == -1 && cursorX - direction >= 0){ //move left
    cursorX -= 1;
  }

  if(direction == 1 && cursorX + direction < 16){ //move right
    cursorX += 1;
  }
}

void translateMorse(){
  for(int i = 0; i < sizeof(topRow); i++){
    String morse = morseEncode(topRow[i]);
    if(morse == ""){
      noTone(buzzerPin);
      delay(100);
    }else{
      for(int j = 0; j < sizeof(morse); j++){
        char morseSign = morse.charAt(j);
        Serial.print("morse sign: ");
        Serial.println(morseSign);
        timer.in(300, runMorse, morseSign);
      }
    }
    Serial.println(morse);
  }
}

String morseEncode(char letter){
  switch(letter){
    case 'A': 
        return ".-"; 
    case 'B': 
        return "-..."; 
    case 'C': 
        return "-.-."; 
    case 'D': 
        return "-.."; 
    case 'E': 
        return "."; 
    case 'F': 
        return "..-."; 
    case 'G': 
        return "--."; 
    case 'H': 
        return "...."; 
    case 'I': 
        return ".."; 
    case 'J': 
        return ".---"; 
    case 'K': 
        return "-.-"; 
    case 'L': 
        return ".-.."; 
    case 'M': 
        return "--"; 
    case 'N': 
        return "-."; 
    case 'O': 
        return "---"; 
    case 'P': 
        return ".--."; 
    case 'Q': 
        return "--.-"; 
    case 'R': 
        return ".-."; 
    case 'S': 
        return "..."; 
    case 'T': 
        return "-"; 
    case 'U': 
        return "..-"; 
    case 'V': 
        return "...-"; 
    case 'W': 
        return ".--"; 
    case 'X': 
        return "-..-"; 
    case 'Y': 
        return "-.--"; 
    // for space 
    case 'Z': 
        return "--.."; 
   } 
   return "";  
}

void changeCharacter(){  
  Serial.print("next char");
  char nextChar = getNextChar(topRow[cursorX]);
  Serial.println(nextChar);
  Serial.println(topRow[cursorX]);
  topRow[cursorX] = nextChar;
  Serial.println(topRow[cursorX]);
  lcd.print(nextChar);
  
}

char getNextChar(char currentChar){
  Serial.print("current char");
  Serial.print(currentChar);
  int letterIndex = getLetterIndex(currentChar);

  Serial.println(letterIndex);

  if(letterIndex >= sizeof(codeArray)){
      Serial.println(codeArray[0]);
      return codeArray[0];
  }else{
    Serial.println(codeArray[letterIndex + 1]);
    return codeArray[letterIndex + 1];
  }
}

void intitializeBoard() {
  for (int i = 0; i < sizeof(topRow); i++) {
     lcd.setCursor(i, 0);
     lcd.print("");
  }
  lcd.setCursor(0, 0);
}

void runMorse(char morseSign){
  if(morseSign == "."){
      digitalWrite(dotLED, HIGH);
      digitalWrite(dashLED, LOW);
//      tone(buzzerPin, 1000);
  }else{
      digitalWrite(dotLED, LOW);
      digitalWrite(dashLED, HIGH);
//      tone(buzzerPin, 500);
  }
}

int getLetterIndex(char letter) {
  for (int i = 0; i < sizeof(codeArray); i++) {
    if (codeArray[i] == letter) {
      return i;
    }
  }
  return 27;
}
