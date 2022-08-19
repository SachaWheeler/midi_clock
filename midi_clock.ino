#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

int SNARE         = 36;
int CLAP          = 37;
int HI_HAT_2      = 41;
int HI_HAT_2_OPEN = 43;
int KICK          = 44;
int DRUM_1        = 45;
int DRUM_2        = 46;
int MULTI         = 47;
int HI_HAT_1      = 49;
int HI_HAT_1_OPEN = 51;
const int drums [] = {SNARE, CLAP, HI_HAT_2, HI_HAT_2_OPEN, KICK, DRUM_1, DRUM_2, MULTI, HI_HAT_1, HI_HAT_1_OPEN,};

void setup()
{
  //Set up serial output with standard MIDI baud rate
  Serial.begin(31250);

  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("hello everyone");
  lcd.setCursor(1, 1);
  lcd.print("konichiwaa");
}

void loop()
{
  //Play a chromatic scale starting on middle C (60)
  for (int note = 0; note < 72; note++)
  {
    //Play a note
    playMIDINote(1, note, 100);
    //Hold note for 60 ms (delay() used for simplicity)
    delay(60);

    //Turn note off (velocity = 0)
    playMIDINote(1, note, 0);
    //Pause for 60 ms
    delay(60);
  }


  for (int note : drums) { // for each element in the array
    playMIDINote(10, note, 100);
    delay(300);

    //Turn note off (velocity = 0)
    playMIDINote(10, note, 0);
    //Pause for 60 ms
    delay(600);
  }

  //playDrumBeat(36);
  //playDrumBeat(48);
  //playDrumBeat(41);
  //playDrumBeat(58);
  //playDrumBeat(40);
  //playDrumBeat(49);
  //playDrumBeat(51);
  //playDrumBeat(42);
  //playDrumBeat(44);
  //playDrumBeat(39);
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
  playMIDINote(10, note, 100);
  //Hold note for 60 ms (delay() used for simplicity)
  delay(60);

  //Turn note off (velocity = 0)
  playMIDINote(10, note, 0);
  //Pause for 60 ms
  delay(60);
}
