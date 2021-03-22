#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <SD.h>

static const int RXPin = 5, TXPin = 3;
static const uint32_t GPSBaud = 9600;
static const int CARD_PIN = 4; // This changes based on shield / SD card configuration. See SD sketch examples

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
File dataFile;

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only.
  }
  
  Serial.print("Initializing card...");
  
  if (!SD.begin(CARD_PIN)) {
    Serial.println("Card initialization failed!");
    while (1);
  }
  
  Serial.println("Card initialization done");
}

void loop() {
  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      dataFile = SD.open("newtest.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.print(gps.location.lat(), 6);
        dataFile.print(", ");
        dataFile.print(gps.location.lng(), 6);
        dataFile.print(", ALT=");
        dataFile.println(gps.altitude.meters());
        dataFile.close();
      } else {
        Serial.println("Failed to open file");
        Serial.println();
      }
    }
  }
}
