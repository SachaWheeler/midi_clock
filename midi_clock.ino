#include <Wire.h>
// #include <DS3231.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
RTC_DS3231 rtc;

const int SNARE = 36;
const int CLAP = 37;
const int HI_HAT_2 = 41;
const int HI_HAT_2_OPEN = 43;
const int KICK = 44;
const int DRUM_1 = 45;
const int DRUM_2 = 46;
const int MULTI = 47;
const int HI_HAT_1 = 49;
const int HI_HAT_1_OPEN = 51;
const int drums[] = {
  SNARE,
  CLAP,
  HI_HAT_2,
  HI_HAT_2_OPEN,
  KICK,
  DRUM_1,
  DRUM_2,
  MULTI,
  HI_HAT_1,
  HI_HAT_1_OPEN,
};

const int MOOG = 1;
const int VERMONA = 10;

int prevHour;
int buttonPin = 7;
bool once_through = false;
bool done_15, done_30, done_45, backlight = false;
int prev_seconds = 0;

const int BACKLIGHT_TIMEOUT = 6;  // seconds

void setup() {
  //Set up serial output with standard MIDI baud rate
  Serial.begin(31250);
  while (!Serial)
    ;  // wait until Serial is available

  lcd.init();
  //lcd.backlight();

  if (!rtc.begin()) {
    lcd.print("Couldn't find RTC");
    while (1)
      ;
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // sets the time to the compile time
  }

  /rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2022, 9, 4, 21, 59, 58));
  prevHour = rtc.now().hour();
}

void loop() {
  DateTime now = rtc.now();
  printTime(now);

  if (once_through && now.hour() != prevHour && now.second() == 0) {
    chime_hour(now);
    prevHour = now.hour();
    done_15 = done_30 = done_45 = false;
  } else if (!done_15 && now.minute() == 15 && now.second() == 0) {
    chime_quarter(15);
    done_15 = true;
  } else if (!done_30 && now.minute() == 30 && now.second() == 0) {
    chime_quarter(30);
    done_30 = true;
  } else if (!done_45 && now.minute() == 45 && now.second() == 0) {
    chime_quarter(45);
    done_45 = true;
  }

  if (backlight && now.second() == BACKLIGHT_TIMEOUT) {  // turn it off after TIMEOUT secs
    lcd.noBacklight();
    backlight = false;
  }

  once_through = true;
}

void play_drum(int drum) {
  playMIDINote(VERMONA, drum, 100);
  delay(60);
  playMIDINote(VERMONA, drum, 0);
}

void chime_hour(DateTime now) {
  lcd.backlight();
  backlight = true;

  int hour = now.hour();
  int hour_tone = hour + 24;
  int hour_chime = hour;
  if (hour_chime > 12) hour_chime -= 12;

  // play hour chiume
  playMIDINote(MOOG, hour_tone, 100);  // turn it on
  delay(110);

  play_drum(KICK);
  delay(110);
  play_drum(KICK);
  delay(150);
  play_drum(HI_HAT_1_OPEN);
  delay(200);

  for (int x = 0; x < hour_chime; x++) {
    play_drum(CLAP);
    if ((x + 1) % 3 == 0) delay(110);
    delay(90);
  }
  delay(400);
  playMIDINote(MOOG, hour_tone, 50); // turn it off
  delay(400);
  playMIDINote(MOOG, hour_tone, 0); // turn it off
}

void chime_quarter(int min) {
  lcd.backlight();
  backlight = true;

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

  if (prev_seconds != (int)now.second()) {
    sprintf(date_str, "%d-%02d-%02d", now.year(), now.month(), now.day());
    lcd.setCursor(1, 0);
    lcd.print(date_str);

    sprintf(time_str, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    lcd.setCursor(1, 1);
    lcd.print(time_str);

    prev_seconds = now.second();
  }
}

void playMIDINote(byte channel, byte note, byte velocity) {
  //MIDI channels 1-16 are really 0-15
  byte noteOnStatus = 0x90 + (channel - 1);

  //Send notes to MIDI output:
  Serial.write(noteOnStatus);
  Serial.write(note);
  Serial.write(velocity);
}