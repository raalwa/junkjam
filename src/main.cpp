/*
 * MIDIUSB_test.ino
 *
 * Created: 4/6/2015 10:47:08 AM
 * Author: gurbrinder grewal
 * Modified by Arduino LLC (2015)
 */

#include "MIDIUSB.h"

// Define debug levels
#define NONE 0
#define ERROR 1
#define WARN 2
#define DEBUG 3
#define ALL 4
#define DEBUG_LEVEL DEBUG

// Set pins for instruments
#define INSTRUMENT_1 7
#define INSTRUMENT_2 8
#define INSTRUMENT_3 9
#define INSTRUMENT_4 10
#define INSTRUMENT_5 11

// Associate MIDI input with pin
#define MIDI_1 0x1D
#define MIDI_2 0x1F
#define MIDI_3 0x21
#define MIDI_4 0x23
#define MIDI_5 0x24

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity)
{
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity)
{
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup()
{
  Serial.begin(115200);

  pinMode(INSTRUMENT_1, OUTPUT);
  pinMode(INSTRUMENT_2, OUTPUT);
  pinMode(INSTRUMENT_3, OUTPUT);
  pinMode(INSTRUMENT_4, OUTPUT);
  pinMode(INSTRUMENT_5, OUTPUT);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value)
{
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void midiToPin(midiEventPacket_t received)
{
  // header: 9 = ON, 8 = OFF is translated to 1 (HIGH) and 0 (LOW)
  // which can be used as pin state switches
  int state = received.header - 8;
  switch (received.byte2)
  {
  case MIDI_1:
    if (DEBUG_LEVEL >= DEBUG)
    {
      Serial.println("Switching Instrument 1");
    }
    digitalWrite(INSTRUMENT_1, state);
    break;
  case MIDI_2:
    if (DEBUG_LEVEL >= DEBUG)
    {
      Serial.println("Switching Instrument 2");
    }
    digitalWrite(INSTRUMENT_2, state);
    break;
  case MIDI_3:
    if (DEBUG_LEVEL >= DEBUG)
    {
      Serial.println("Switching Instrument 3");
    }
    digitalWrite(INSTRUMENT_3, state);
    break;
  case MIDI_4:
    if (DEBUG_LEVEL >= DEBUG)
    {
      Serial.println("Switching Instrument 4");
    }
    digitalWrite(INSTRUMENT_4, state);
    break;
  case MIDI_5:
    if (DEBUG_LEVEL >= DEBUG)
    {
      Serial.println("Switching Instrument 5");
    }
    digitalWrite(INSTRUMENT_4, state);
    break;
  }
}

void loop()
{
  midiEventPacket_t rx;
  do
  {
    rx = MidiUSB.read();
    if (rx.header != 0)
    {
      if (DEBUG_LEVEL >= ALL)
      {
        Serial.print("Received: ");
        Serial.print(rx.header, HEX);
        Serial.print("-");
        Serial.print(rx.byte1, HEX);
        Serial.print("-");
        Serial.print(rx.byte2, HEX);
        Serial.print("-");
        Serial.println(rx.byte3, HEX);
      }
      // handle tones
      midiToPin(rx);
    }
  } while (rx.header != 0);
}