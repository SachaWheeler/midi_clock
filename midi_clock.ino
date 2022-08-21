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

int prevHour;
int buttonPin = 7;
bool once_through = false;
bool done_15, done_30, done_45 = false;

void setup()
{
  //Set up serial output with standard MIDI baud rate
  Serial.begin(31250);

  //pinMode(buttonPin, INPUT);

  lcd.init();
  lcd.backlight();

  if (! rtc.begin()) {
    lcd.print("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    lcd.print("RTC lost power, lets set the time!");

    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //rtc.adjust(DateTime(2022, 10, 13, 10, 59, 55));
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2022, 10, 13, 10, 29, 55));
  prevHour = rtc.now().hour();

  /*
    // reset midi just in case
    for (int note = 0; note < 72; note++)
    {
      //Turn note off (velocity = 0)
      playMIDINote(MOOG, note, 0);
    }
    for (int note : drums) { // for each drum
      playMIDINote(VERMONA, note, 0);
    }
  */
}

void loop()
{
  DateTime now = rtc.now();
  printTime(now);

  if (once_through && now.hour() != prevHour)
  {
    // announce the HOUR
    chime_hour(now);
    prevHour = now.hour();
    done_15 = done_30 = done_45 = false;
  } else if (!done_15 && now.minute() == 15) {
    chime_quarter(15);
    done_15 = true;
  } else if (!done_30 && now.minute() == 30) {
    chime_quarter(30);
    done_30 = true;
  } else if (!done_45 && now.minute() == 45) {
    chime_quarter(45);
    done_45 = true;
  }

  /*
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
  */
  //for (int note : drums) { // for each element in the array
  // playDrumBeat(note);
  //}
  once_through = true;
}
void play_drum(int drum) {
  playMIDINote(VERMONA, drum, 100);
  delay(60);
  playMIDINote(VERMONA, drum, 0);
}

void chime_hour(DateTime now) {
  play_drum(KICK);
  delay(110);
  play_drum(KICK);
  delay(150);
  play_drum(HI_HAT_1_OPEN);
  delay(200);

  for (int x = 0; x < now.hour(); x++) {
    play_drum(CLAP);
    if ((x + 1) % 3 == 0) delay(110);
    delay(90);
  }
}

void chime_quarter(int min) {
  play_drum(KICK);
  delay(60);
  play_drum(CLAP);
  delay(200);
  for (int x = 0; x < min / 15; x++) {
    play_drum(SNARE);
    delay(200);
  }
}

void printTime(DateTime now) {
  char date_str[15];
  char time_str[15];

  lcd.setCursor(1, 0);
  sprintf(date_str, "%d-%02d-%02d", now.year(), now.month(), now.day());
  lcd.print(date_str);

  lcd.setCursor(1, 1);
  sprintf(time_str, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  lcd.print(time_str);
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
