#include "LiquidCrystal_I2C.h"
#define sensor A0 //Pin de la placa arduino que lee los datos del sensor de humedad
#define buzzerPin 12 // Pin del arduino que activa el buzzer
#define boton 11 // Pin de la placa arduino conectado al boton que activa el moddo automatico de riego
#define relayPin 10 // Pin de la placa arduino que activa o desactiva el relay 

#define pinRojo A1 //Pin de la placa arduino asociado a dar una tonalidad roja en el led RGB
#define pinAzul A2 //Pin de la placa arduino asociado a dar una tonalidad azul en el led RGB
#define pinVerde A3 //Pin de la placa arduino asociado a dar una tonalidad verde en el led RGB

LiquidCrystal_I2C lcd(0x27, 16,2); // Inicializa un objeto "lcd" de la biblioteca LiquidCrystal_I2C el cual nos servira para manipular a pantalla lcd
byte drop[8] = {
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110
}; // Aqui creamos un icono para imprimir en la pantalla lcd el cual tiene forma de gota de agua
  byte lightning[8] = {
  B00010,
  B00100,
  B01000,
  B11111,
  B00010,
  B00100,
  B01000,
  B10000
}; // Aqui creamos un icono para imprimir en la pantalla lcd el cual tiene forma de un rayo

bool modoAutomatico=false; // Esta variable la utilizamos para tener un estado del modo automatico inicializado como falso en primer instancia
void setup(){
  pinMode(sensor,INPUT); // definimos el modo del pin conectado al sensor como entrada 
  pinMode(relayPin,OUTPUT); // definimos el modo del pin conectado al relay como salida
  pinMode(pinRojo,OUTPUT); 
  pinMode(pinVerde,OUTPUT); // definimos el modo de los pines conectados al led RGB como salida 
  pinMode(pinAzul,OUTPUT);

  Serial.begin(9600); // Inicializamos la consola serial a una frecuencia de 9600 baudios para visualizar salidas en la depuracion del codigo
  lcd.begin(); // inicializamos la pantalla lcd 
  lcd.backlight(); // encendemos la luz de fondo de la pantalla lcd 
  lcd.createChar(0,drop); // creamos una variable tipo char a partir de nuestra variable drop
  lcd.setCursor(0, 0);  // indicamos la fila y columna donde se va a imprimir la gota de agua 
  
  lcd.createChar(1,lightning);   // creamos una variable tipo char a partir de nuestra variable lightning
  lcd.setCursor(1, 0);  //indicamos la fila y columna donde se va a imprimir la gota de agua 
}

void loop(){
  
  if (digitalRead(boton)) { // si se lee un valor alto en el boton del modo automatico (cuando se presiona)...
    modoAutomatico = !modoAutomatico;  // seteamos el valor opesto al inicial o sea true
    if (modoAutomatico) {
      playSongAut(); // si el modo automatico esta activo se llama a la funcion que reproduce la melodia del modo automatico
    } else {
      playSongNoAut();  // si el modo automatico se deshabilita se llama a la funcion que reproduce la melodia correspondiente 
    }
    delay(1000); 
  }
  if (modoAutomatico) { // si el modo automatico esta activo se obtendra continuamente la lectura del sensor
    int porc = getSoilmoisture(); // aqui se obtiene la lectura del sensor y se almacena en la variable porc
    if (porc < 80) { // si el valor de la variable porc es menor  a 80...
    RelayOn(); // llamamos a la funcion correspondiente par activar el relay
    } else {
     RelayOff(); // sino llamamos a la funcion correspondiente par desactivar el relay
    }
  } else { // si el valor de la variable porc es mayor a 80, se desactiva en automatico el modod automatico
    int porc = getSoilmoisture();
    showValue(porc); 
    ledindicator(porc);
  }
  delay(1000);
  lcd.clear(); // limpiamos la pantalla de caracteres
}
//Lee el sensor y retorna el porcentaje  

uint8_t getSoilmoisture(){ 

  int a = 1023-(analogRead(sensor)); // los valores del sensor se arrojan invertidos, para mostrarlos correctamente a 1023 se le resta la lectura del sensor 

  int porcentaje = map(a,720,0,100,0); // almacenamos en la variable porcentaje el mapeo para obtener un valor entre 0 y 100 apartir de los valores maximos y minimos que arroja el sensor  

  Serial.println(a); // imprimimos en consola la lectura del sensor 

  return porcentaje; 

} 

//esta función activa el relay y por consiguiente la bomba de riego 

void RelayOn(){ 

      digitalWrite(relayPin, HIGH); //Activa el relay con un valor alto en su pin de entrada 

      tone(buzzerPin,  500); // sonido de advertencia del modo automático 

      delay(1000); // cada segundo 

      noTone(buzzerPin); // desactivamos el buzzer de este sonido de advertencia 

      analogWrite(pinRojo, 255); // damos una tonalidad violeta al led RGB  

      analogWrite(pinVerde, 255);  

      analogWrite(pinAzul, 0); 

} 

//esta función desactiva el relay y por consiguiente la bomba de riego 

void RelayOff(){ 

digitalWrite(relayPin, LOW); //desactiva el relay con un valor bajo en su pin de entrada 

      noTone(buzzerPin); //desactiva el buzzer 

      playSongNoAut(); // reproduce la melodia correspondiente a cuando el modo automatico se deshabilita 

      noTone(buzzerPin); // vuelve a desactivar el buzzer debido a la melodía 

      modoAutomatico=false; // indicamos que el modo automático esta deshabilitado 

} 

//obtiene el porcentaje y cambia el color de los leds 

void ledindicator(uint8_t data) { 

  if (data < 20) { // tonalidad roja 

    digitalWrite(pinRojo, HIGH); 

    digitalWrite(pinVerde, LOW); 

    digitalWrite(pinAzul, LOW); 

  } else if (data >= 20 && data < 55) { // tonalidad amarilla 

    digitalWrite(pinRojo, HIGH); 

    digitalWrite(pinVerde, HIGH); 

    digitalWrite(pinAzul, LOW); 

  } else if (data >= 55 && data < 75) { // tonalidad blanca 

    digitalWrite(pinRojo, HIGH); 

    digitalWrite(pinVerde, HIGH); 

    digitalWrite(pinAzul, HIGH); 

  } else if (data >= 75 && data < 90) { // tonalidad azul 

    digitalWrite(pinRojo, LOW); 

    digitalWrite(pinVerde, HIGH); 

    digitalWrite(pinAzul, LOW); 

  } else if (data >= 90 && data <= 100) { //tonalidad verde 

    digitalWrite(pinRojo, LOW); 

    digitalWrite(pinVerde, LOW); 

    digitalWrite(pinAzul, HIGH); 

  } else { //tonalidad violeta 

    digitalWrite(pinRojo, LOW); 

    digitalWrite(pinVerde, LOW); 

    digitalWrite(pinAzul, LOW); 

  } 

} 

//muestra en la pantalla el porcentaje de humedad 

void showValue(uint8_t data){ 

  int b= 1023-(analogRead(sensor)); // obetenemos la lectura del sensor 

  lcd.write(0); // imprime la gota de agua 

  lcd.print(data);  // imprimimos el porcentaje 

  lcd.print("%  "); 

lcd.write(1); // imprime el rayo 

  lcd.print(b); // imprimimos la lectura  

} 

  

//esta funcion reproduce la melodia cuando el modo automatico se activa 

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

  

//esta funcion reproduce la melodia cuando el modo automatico se desactiva 

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
