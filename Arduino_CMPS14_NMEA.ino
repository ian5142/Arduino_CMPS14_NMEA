/*
 * Connects via I2C to a CMPS14, outputs NMEA0183 HDM sentences via Serial (38400 baud)
 * By James Henderson, 2014, adapted to output NMEA sentences by Ian Van Schaick
 */

#include <Wire.h>

#define CMPS14_ADDRESS 0x60  // Address of CMPS14 shifted right one bit for arduino wire library
#define ANGLE_8  1           // Register to read 8bit angle from

unsigned char high_byte, low_byte, angle8;
signed char pitch, roll;
float angle16;
int fine;
float bearingH; // Holds whole degrees of bearing
float bearingL; // Holds decimal digits of bearing
int bearing;
char nbsp;
char mystring[25];
char mystring2[25];
char mystring3[25];

uint8_t checksum(char *s)
{
  uint8_t c = 0;

  while (*s)
    c ^= *s++;
  return c;
}

void setup() {
  Serial.begin(38400);  // Start serial port
  Wire.begin();
  nbsp = 32;
}

void loop() {
  Wire.beginTransmission(CMPS14_ADDRESS);  //starts communication with CMPS14
  Wire.write(ANGLE_8);                     //Sends the register we wish to start reading from
  Wire.endTransmission();
 
  // Request 5 bytes from the CMPS14
  // this will give us the 8 bit bearing, 
  // both bytes of the 16 bit bearing, pitch and roll
  Wire.requestFrom(CMPS14_ADDRESS, 5);       
  
  while(Wire.available() < 5);        // Wait for all bytes to come back
  
  angle8 = Wire.read();               // Read back the 5 bytes
  high_byte = Wire.read();
  low_byte = Wire.read();
  pitch = Wire.read();
  roll = Wire.read();

  bearing = ((high_byte<<8) + low_byte) / 10;
  fine = ((high_byte<<8) + low_byte) % 10;
  byte data[128] = "$HCHDM,";
  int index = 8;
  int dataLen = 8;
  data[8] = bearing;

  //Print out NMEA 0183 string HDM
  snprintf(mystring, sizeof(mystring), "$HCHDM,%d.%d,M", bearing , fine);
  uint8_t crc = checksum(mystring + 1);
  Serial.print(mystring);
  Serial.print("*");
  if(crc < 16) Serial.print("0");
  Serial.println(crc, HEX);

  //Print out NMEA 0183 string XDR for Pitch
  snprintf(mystring2, sizeof(mystring2), "$HCXDR,A,%d,D,PITCH", pitch);
  uint8_t crc2 = checksum(mystring2 + 1);
  Serial.print(mystring2);
  Serial.print("*");
  if(crc2 < 16) Serial.print("0");
  Serial.println(crc2, HEX);

  //Print out NMEA 0183 string XDR for Roll/Heel
  snprintf(mystring3, sizeof(mystring3), "$HCXDR,A,%d,D,ROLL", roll);
  uint8_t crc3 = checksum(mystring3 + 1);
  Serial.print(mystring3);
  Serial.print("*");
  if(crc3 < 16) Serial.print("0");
  Serial.println(crc3, HEX);
  delay(100);
}
