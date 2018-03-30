//Library MFRC522 from https://github.com/ljos/MFRC522
//Library AESLib from https://github.com/DavyLandman/AESLib
#include <MFRC522.h>
#include <SPI.h>
#include "Keyboard.h"
#include <AESLib.h>

#define SAD 10 //SAD pin
#define RST 9 //RST pin
#define AUTHORIZED_COUNT 1 //For more authorized cards, add the serials below

byte Authorized[AUTHORIZED_COUNT][4] = {
  {0xA9, 0xCF, 0x8E, 0x63,} //Where to add more authorized cards
};

boolean isSame(byte *key, byte *serial); //Compares two tags and return true if they are the same
boolean isAuthorized(byte *serial); //Verify if card tag is in our authorized table cards

MFRC522 nfc(SAD, RST); //Initialize class MFRC522 object with SAD and RST pins

void setup() {
  Serial.begin(9600); //Serial port for logging
  SPI.begin(); //SPI library launching
  delay(1000);
  nfc.begin(); //Object launching

  delay(500);
  byte version = nfc.getFirmwareVersion(); //Allows us to know if our RFID RC522 module is connected correctly
  if (!version) {
    Serial.print("Didn't find MFRC522 board."); //Board RC522 not found
    while (1); //Exit program
  }

  Serial.print("Found chip MFRC522 "); //Board RC522 found
  Serial.print("Firmware ver. 0x");
  Serial.print(version, HEX);
  Serial.println(".");
}

String pass = decryptPass(); //Get our user session password from AES encrypted data

void loop() {
  byte status;
  byte data[MAX_LEN]; //Dump out the full 16 bytes
  byte serial[5]; //The fifth byte is for the checksum -- Serial number 4 bytes -- Checkum 1 byte

  /*Send a general request out into the aether. If there is a tag in
  the area it will respond and the status will be MI_OK.*/
  status = nfc.requestTag(MF1_REQIDL, data);
  if (status == MI_OK) {
    /* Calculate the anti-collision value for the currently detected
       tag and write the serial into the data array.*/
    status = nfc.antiCollision(data);
    memcpy(serial, data, 5);

    if (isAuthorized(serial)) //If serial authorized, write our password
    {
      writePass(pass);
    }
    nfc.haltTag(); //Stop the tag and get ready for reading a new tag
    delay(2000);
  }
  delay(500);
}

void writePass(String pass) {
  for (int i = 0; i < pass.length(); i++) {
    if (isDigit(pass.charAt(i))) {
      Keyboard.press(KEY_LEFT_SHIFT); //Keyboard french localization, pressing shift is needed to press on the numbers keys
      Keyboard.print(pass.charAt(i));
      Keyboard.release(KEY_LEFT_SHIFT); //Release left shift button
    } else {
      Keyboard.print(pass.charAt(i)); //Else press normally on our keyboard keys
    }
  }
  delay(500);
  Keyboard.press(KEY_RETURN); //Press /n key to validate password entry
  delay(200);
  Keyboard.release(KEY_RETURN); //Release /n key
}

String decryptPass() {
  byte key[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; //AES 128 bits key to decode our encrypted password
  char data[] = {-65, -51, 82, 6, 39, 7, -25, 115, 96, 65, -35, 115, -125, -39, 121, -78}; //Encrypted user session password
  aes128_dec_single(key, data); //AESLib function that allows us to decode our passphrase with our 128 bits key
  Serial.print("decrypted :");
  /*This code allowed us to read the encoded password thanks to AESLib aes_enc_single(key, data) method
    We then stored it in our data variable above
    for (int i=0; i < 16; i++) {
    Serial.println(data[i], DEC);
    }*/
  String pass;
  for (int i = 0; i < 16; i++) {
    Serial.write(data[i]);
    pass += data[i];
  }
  return pass; //Save and return our decoded password
}

boolean isSame(byte *key, byte *serial)
{
  for (int i = 0; i < 4; i++)
  {
    if (key[i] != serial[i])
    {
      return false;
    }
  }
  return true;
}

boolean isAuthorized(byte *serial)
{
  for (int i = 0; i < AUTHORIZED_COUNT; i++)
  {
    if (isSame(serial, Authorized[i]))
      return true;
  }
  return false;
}
