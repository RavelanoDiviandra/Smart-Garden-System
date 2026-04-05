#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD I2C (Alamat 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Definisi Pin
const int pinSensorTanah = A3; 
const int pinSensorLDR = A1;   
const int pinRelay = 7;        
const int pinLED = 13;         

// Konfigurasi Relay (Active LOW)
const int RELAY_ON = LOW;   
const int RELAY_OFF = HIGH; 

// Data Kalibrasi
const int batasLDR_Senja = 700; 
int batasTanah_Siang = 800; 
int batasTanah_Malam = 950; 

// Variabel Global
int nilaiADC_Tanah = 0;
int nilaiADC_Cahaya = 0;
int batasAktif = 0; 
String statusSistem = "";

void setup() {
  Serial.begin(9600); 

  pinMode(pinRelay, OUTPUT);
  pinMode(pinLED, OUTPUT);
  pinMode(pinSensorTanah, INPUT);
  pinMode(pinSensorLDR, INPUT);
  
  // Inisialisasi status awal aktuator
  digitalWrite(pinRelay, RELAY_OFF);
  digitalWrite(pinLED, LOW);
  
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0,0);
  lcd.print("SYSTEM STABILIZER");
  delay(1000);
  lcd.clear();

  // Header Serial Monitor
  Serial.println("WAKTU(s)\tTANAH\tLDR\tBATAS\tSTATUS POMPA");
}

void loop() {
  // 1. Pembacaan Sensor (Moving Average)
  nilaiADC_Tanah = bacaSensorStabil(pinSensorTanah);
  nilaiADC_Cahaya = bacaSensorStabil(pinSensorLDR);

  // 2. Logika Adaptif Siang/Malam
  if (nilaiADC_Cahaya > batasLDR_Senja) {
    batasAktif = batasTanah_Malam; 
    statusSistem = "MALAM";
  } else {
    batasAktif = batasTanah_Siang; 
    statusSistem = "SIANG";
  }

  // 3. Kalkulasi Batas Hysteresis
  int batasMatikan = batasAktif - 100; 

  // 4. Eksekusi Aktuator
  if (nilaiADC_Tanah > batasAktif) {
    // Tanah kering: Hidupkan pompa
    digitalWrite(pinRelay, RELAY_ON); 
    digitalWrite(pinLED, HIGH);   
  } 
  else if (nilaiADC_Tanah < batasMatikan) {
    // Tanah basah: Matikan pompa
    digitalWrite(pinRelay, RELAY_OFF);  
    digitalWrite(pinLED, LOW);    
  }

  // 5. Pembaruan Antarmuka LCD
  lcd.setCursor(0,0);
  lcd.print("T:"); lcd.print(nilaiADC_Tanah); 
  lcd.print(" L:"); lcd.print(nilaiADC_Cahaya);
  
  lcd.setCursor(0,1);
  if (digitalRead(pinRelay) == RELAY_ON) {
     lcd.print("POMPA: MENYIRAM ");   
  } else {
     lcd.print("Bts:"); lcd.print(batasAktif); 
     lcd.print(" ");
     if(batasAktif == 800) lcd.print("(Siang)"); else lcd.print("(Malam)");
  }

  // 6. Output Serial Monitor
  Serial.print(millis() / 1000); 
  Serial.print("\t\t");
  Serial.print(nilaiADC_Tanah);
  Serial.print("\t");
  Serial.print(nilaiADC_Cahaya);
  Serial.print("\t");
  Serial.print(batasAktif);
  Serial.print("\t");
  
  if (digitalRead(pinRelay) == RELAY_ON) {
    Serial.println("ON (SIRAM)");
  } else {
    Serial.println("OFF");
  }
  
  delay(1000); 
}

// Fungsi Penstabil Data Sensor
int bacaSensorStabil(int pin) {
  long total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(pin);
    delay(10); 
  }
  return total / 10; 
}