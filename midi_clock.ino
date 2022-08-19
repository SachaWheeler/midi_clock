#include <Wire.h>
// #include <DS3231.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
RTC_DS3231 rtc;

const int SNARE         = 36;
const int CLAP          = 37;
const int HI_HAT_2      = 41;
const int HI_HAT_2_OPEN = 43;
const int KICK          = 44;
const int DRUM_1        = 45;
const int DRUM_2        = 46;
const int MULTI         = 47;
const int HI_HAT_1      = 49;
const int HI_HAT_1_OPEN = 51;
const int drums [] = {SNARE, CLAP, HI_HAT_2, HI_HAT_2_OPEN, KICK, DRUM_1, DRUM_2, MULTI, HI_HAT_1, HI_HAT_1_OPEN,};

const int MOOG = 1;
const int VERMONA = 10;

void setup()
{
  //Set up serial output with standard MIDI baud rate
  Serial.begin(31250);

  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  //lcd.setCursor(1, 0);
  //lcd.print("hello everyone");
  //lcd.setCursor(1, 1);
  //lcd.print("konichiwaa");

  if (! rtc.begin()) {
    lcd.print("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    lcd.print("RTC lost power, lets set the time!");

    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // reset midi just in case
  for (int note = 0; note < 72; note++)
  {
    //Turn note off (velocity = 0)
    playMIDINote(MOOG, note, 0);
  }
  for (int note : drums) { // for each element in the array
    playMIDINote(VERMONA, note, 0);
  }
}

void loop()
{
  DateTime now = rtc.now();

  lcd.setCursor(1, 0);
  lcd.print(now.year(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);

  lcd.setCursor(1, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);

  //Play a chromatic scale starting on middle C (60)
  for (int note = 0; note < 72; note++)
  {
    //Play a note
    playMIDINote(MOOG, note, 100);
    //Hold note for 60 ms (delay() used for simplicity)
    delay(60);

    //Turn note off (velocity = 0)
    playMIDINote(MOOG, note, 0);
    //Pause for 60 ms
    delay(60);
  }


  for (int note : drums) { // for each element in the array
    playDrumBeat(note);
  }

}

void playMIDINote(byte channel, byte note, byte velocity)
{
  //MIDI channels 1-16 are really 0-15
  byte noteOnStatus = 0x90 + (channel - 1);

  //Send notes to MIDI output:
  Serial.write(noteOnStatus);
  Serial.write(note);
  Serial.write(velocity);
}

void playDrumBeat(byte note)
{
  playMIDINote(VERMONA, note, 100);
  //Hold note for 60 ms (delay() used for simplicity)
  delay(60);

  //Turn note off (velocity = 0)
  playMIDINote(VERMONA, note, 0);
  //Pause for 60 ms
  delay(60);
}
