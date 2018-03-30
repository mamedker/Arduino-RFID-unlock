# Arduino RFID unlock

This project is an educational one where we thought that playing with RFID on an **Arduino Leonardo** and a **RFID-RC522 module** seemed fun.

## Getting started

When you buy your RC522 module, you get one RFID card and one badge.
These two have tags that you have to read thanks to the example [auth_read_write](https://github.com/ljos/MFRC522/blob/master/examples/auth_read_write/auth_read_write.ino) below from the [MFRC522 GitHub](https://github.com/ljos/MFRC522) project we used.


## Import our project

Now that you have your **TAG_CODE**, add it to Authorized variable on our **RFID_arduino.ino** file.

We didn't want to store our password without any encryption so we encrypted the one we used for our demo.
In order to use another one, don't forget to encode the password you want with aes128_enc_single(key, data) method, follow the [example](https://github.com/DavyLandman/AESLib#usage) from [AESLib](https://github.com/DavyLandman/AESLib).

Enjoy the ride !

## Built with

* [MFRC522](https://github.com/ljos/MFRC522)
* [AESLib](https://github.com/DavyLandman/AESLib)

## Acknowledgments

The idea came from this [project](https://create.arduino.cc/projecthub/masteruan/arduino-rfid-unlock-mac-linux-and-win-e9d7a3) on Arduino project hub.

Check out the author, [Giovanni Gentile](https://create.arduino.cc/projecthub/masteruan) awesome works !