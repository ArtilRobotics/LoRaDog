/*****************************************
 * ESP32 GPS VKEL 9600 Bds
 ******************************************/

#include <Arduino.h>

// LibrerÃ­as para LoRa
#include <LoRa.h>
#include <SPI.h>

// Libraries para comunicar con y dibujar en la pantalla OLED integrada
#include <Wire.h>
#include <TinyGPS++.h>

#define SCK 5   // GPIO5  -- SX1278's SCK
#define MISO 19 // GPIO19 -- SX1278's MISnO
#define MOSI 27 // GPIO27 -- SX1278's MOSI
#define SS 18   // GPIO18 -- SX1278's CS
#define RST 23  // GPIO14 -- SX1278's RESET
#define DI0 26  // GPIO26 -- SX1278's IRQ(Interrupt Request)

#define RXD2 34 // 16
#define TXD2 12 // 17

#define BAND 915E6

// The TinyGPSPlus object
TinyGPSPlus gps;

static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        while (Serial1.available())
            gps.encode(Serial1.read());
    } while (millis() - start < ms);
}

void setup()
{
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1, 34, 12); // 17-TX 18-RX
    Serial.println("Prueba de enví­o LoRa");
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DI0);
    if (!LoRa.begin(BAND))
    {
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }
    Serial.println("Inicio exitoso de LoRa!"); // Mensaje de todo bien en puerto serial
}

void loop()
{
    Serial.print("Latitude  : ");
    Serial.println(gps.location.lat(), 5);
    Serial.print("Longitude : ");
    Serial.println(gps.location.lng(), 4);
    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());
    Serial.print("Altitude  : ");
    Serial.print(gps.altitude.feet() / 3.2808);
    Serial.println("M");
    Serial.print("Time      : ");
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.println(gps.time.second());
    Serial.println("**********************");
    LoRa.beginPacket();                // Inicia protocolo
    LoRa.print(gps.location.lat(), 5); // Manda cuenta actual
    LoRa.print(",");                   // Manda cuenta actual
    LoRa.print(gps.location.lng(), 4); // Manda cuenta actual
    LoRa.endPacket();                  // Fin de paquete enviado
    smartDelay(1000);

    if (millis() > 5000 && gps.charsProcessed() < 10)
        Serial.println(F("No GPS data received: check wiring"));
}
