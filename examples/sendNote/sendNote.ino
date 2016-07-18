/*---------------------------------------------------------------------------------------------

  CocoMidi Library
  based on Gnusbuino MIDI Library 2012 by Michael Egger
 
---------------------------------------------------------------------------------------------*/


#include "CocoMidi.h"

unsigned long lastTime;            // variable to store the last time we sent a chord

void setup() {
  CocoMidi.init();
}


void loop() {

  if (millis()-lastTime >= 500)  {             // every 500 miliseconds = 2 times per second        
         CocoMidi.send(MIDI_NOTEON,60,127);            // put three notes into the MIDI send queue
         CocoMidi.send(MIDI_NOTEON,64,127);
         CocoMidi.send(MIDI_NOTEON,67,127);
         lastTime = millis();      
  }   

  CocoMidi.update();

}
