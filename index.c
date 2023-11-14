#include "LiquidCrystal_I2C.h"
#define sensor A0
#define buzzerPin 12
#define boton 11
#define relayPin 10

#define pinRojo A1
#define pinAzul A2
#define pinVerde A3
LiquidCrystal_I2C lcd(0x27, 16,2);

byte drop[8] = {
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110
};
  byte lightning[8] = {
  B00010,
  B00100,
  B01000,
  B11111,
  B00010,
  B00100,
  B01000,
  B10000
};
bool modoAutomatico=false;
void setup(){
  pinMode(sensor,INPUT);
  pinMode(relayPin,OUTPUT);
  pinMode(pinRojo,OUTPUT);
  pinMode(pinVerde,OUTPUT);
  pinMode(pinAzul,OUTPUT);

  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.createChar(0,drop);
  lcd.setCursor(0, 0);
  
  lcd.createChar(1,lightning); 
  lcd.setCursor(1, 0);
  digitalWrite(relayPin,LOW);
}

void loop(){
  if (digitalRead(boton)) {
    modoAutomatico = !modoAutomatico; 
    if (modoAutomatico) {
      playSongAut(); 
    } else {
      playSongNoAut(); 
    }
    delay(1000); 
  }
  if (modoAutomatico) {
    int porc = getSoilmoisture();
    if (porc < 80) {
    RelayOn();
    } else {
     RelayOff();
    }
  } else { 
    int porc = getSoilmoisture();
    showValue(porc);
    ledindicator(porc);
  }
  delay(1000);
  lcd.clear();
}

//Lee el sensor y retorna porcentage
uint8_t getSoilmoisture(){
  int a = 1023-(analogRead(sensor));
  int porcentaje = map(a,720,0,100,0);
  Serial.println(a);
  return porcentaje;
}
void RelayOn(){
      digitalWrite(relayPin, HIGH);
      tone(buzzerPin,  500); 
      delay(1000);
      noTone(buzzerPin);
      analogWrite(pinRojo, 255);
      analogWrite(pinVerde, 255); 
      analogWrite(pinAzul, 0);
}

void RelayOff(){
 digitalWrite(relayPin, LOW); 
      noTone(buzzerPin);
      playSongNoAut();
      noTone(buzzerPin);
      modoAutomatico=false;
}

//obtiene el porcentaje y cambia el color de los leds
void ledindicator(uint8_t data) {
  if (data < 20) {
    digitalWrite(pinRojo, HIGH);
    digitalWrite(pinVerde, LOW);
    digitalWrite(pinAzul, LOW);
  } else if (data >= 20 && data < 55) {
    digitalWrite(pinRojo, HIGH);
    digitalWrite(pinVerde, HIGH);
    digitalWrite(pinAzul, LOW);
  } else if (data >= 55 && data < 75) {
    digitalWrite(pinRojo, HIGH);
    digitalWrite(pinVerde, HIGH);
    digitalWrite(pinAzul, HIGH);
  } else if (data >= 75 && data < 90) {
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinVerde, HIGH);
    digitalWrite(pinAzul, LOW);
  } else if (data >= 90 && data <= 100) {
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinVerde, LOW);
    digitalWrite(pinAzul, HIGH);
  } else {
    digitalWrite(pinRojo, LOW);
    digitalWrite(pinVerde, LOW);
    digitalWrite(pinAzul, LOW);
  }
}

//muestra en la pantalla el porcentage de humedad
void showValue(uint8_t data){
  int b= 1023-(analogRead(sensor));
  lcd.write(0);
  lcd.print(data);
  lcd.print("%  ");
 lcd.write(1);
  lcd.print(b);
}

//melodia
void playSongAut() {
 tone(buzzerPin, 659.26);
  delay(125);
  noTone(buzzerPin);
  delay(125);
  tone(buzzerPin, 659.26);
  delay(125);
  noTone(buzzerPin);
  delay(250);
  tone(buzzerPin, 659.26);
  delay(150);
  noTone(buzzerPin);
  delay(225);
  tone(buzzerPin, 523.25);
  delay(150);
  noTone(buzzerPin);
  delay(150);
  tone(buzzerPin, 659.26);
  delay(125);
  noTone(buzzerPin);
  delay(125);
  tone(buzzerPin, 783.99);
  delay(250);
  noTone(buzzerPin);
  delay(325);
  tone(buzzerPin, 383.99);
  delay(250);
  noTone(buzzerPin);
  delay(125);
}

void playSongNoAut() {
  tone(buzzerPin, 523.25);
  delay(300);
  noTone(buzzerPin);
  delay(125);
  tone(buzzerPin, 783.99);
  delay(300);
  noTone(buzzerPin);
  delay(125);
  tone(buzzerPin, 659.26);
  delay(300);
  noTone(buzzerPin);
  delay(125);
  tone(buzzerPin, 880);
  delay(200);
  noTone(buzzerPin);
  delay(125);
  tone(buzzerPin, 987.77);
  delay(200);
  noTone(buzzerPin);
  delay(125);
  tone(buzzerPin, 880);
  delay(200);
  noTone(buzzerPin);
  delay(125);
  tone(buzzerPin, 830.61);
  delay(200);
  noTone(buzzerPin);
  delay(125);
  tone(buzzerPin, 932.33);
  delay(200);
  noTone(buzzerPin);
  delay(125);
  tone(buzzerPin, 830.61);
  delay(300);
  noTone(buzzerPin);
  delay(125);
   tone(buzzerPin, 659.26);
  delay(150);
  noTone(buzzerPin);
  delay(125);
   tone(buzzerPin, 587.33);
  delay(150);
  noTone(buzzerPin);
  delay(125);
   tone(buzzerPin, 659.26);
  delay(350);
  noTone(buzzerPin);
  delay(125);
}
