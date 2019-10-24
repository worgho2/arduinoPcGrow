#include "DHT.h"
#include <LiquidCrystal.h>

#define DHTPIN 30
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(10, 9, 45, 44, 43, 42);

const int pot = 0;
int value = 0;
unsigned long int count = 0;

void setup() {  
  Serial.begin(9600);
  
  pinMode(2, OUTPUT); //LSB
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT); //MSB
  
  pinMode(11, OUTPUT); // rele
  pinMode(12, INPUT_PULLUP); // reset
  pinMode(13, OUTPUT); // buzzer

  lcd.begin(16,2); //Inicializa LCD
  lcd.clear();
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  lcd.setCursor(0,0);
  lcd.print("Temp : ");
  lcd.print(" ");
  lcd.setCursor(7,0);
  lcd.print(t,1);
  lcd.setCursor(12,0);
  lcd.print((char)223);
  lcd.print("C");
  
  lcd.setCursor(0,1);
  lcd.print("Umid : ");
  lcd.print(" ");
  lcd.setCursor(7,1);
  lcd.print(h,1);
  lcd.setCursor(12,1);
  lcd.print("%");
  
  if (digitalRead(12) == HIGH) { //APERTAR BOTAO RESET
    defaultBinDisplay(0);
    resetTimerAlert();
    count = 0;
  }

  count = count >= hourToMs(24) ? 0 : count; //REINICIAR NO FINAL DO CICLO DE 24H
  
  value = convertedValueRange(analogRead(pot));
  setBinDisplay(value); //MOSTRAR A TIMER BINARIO
  
  digitalWrite(11, count < hourToMs(value) ? LOW : HIGH); //ATIVAR O RELE
  
  count += 100;
  delay(100);
}

void resetTimerAlert() {
  delay(200);
  tone(13,900,150);
  delay(250);
  tone(13,900,150);
  delay(250);
  tone(13,900,150);
}

unsigned long int hourToMs(int num) {
  return num * 3600000;
}

void setBinDisplay(int num) {
  digitalWrite(2, num % 2);
  digitalWrite(3, (num >> 1) % 2);
  digitalWrite(4, (num >> 2) % 2);
  digitalWrite(5, (num >> 3) % 2);
  digitalWrite(6, (num >> 4) % 2);
}

void defaultBinDisplay(int num) {
  digitalWrite(2, num);
  digitalWrite(3, num);
  digitalWrite(4, num);
  digitalWrite(5, num);
  digitalWrite(6, num);
}

int convertedValueRange(int num) {
  return num * 24 / 703;
}
