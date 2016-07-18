/*---------------------------------------------------------------------------------------------

  Gnusbuino MIDI Library 2012 by Michael Egger
 
  SEND NOTES EXAMPLE
  Send a chord of three notes to the host over MIDI
 
  This example code is in the public domain.


--------------------------------------------------------------------------------------------- */


#include <CocoMidi.h>        // you MUST include the TeenyMidi library

unsigned long lastTime;            // variable to store the last time we sent a chord

void setup() {
  CocoMidi.init(); 
}


// the loop routine runs over and over again forever:
void loop() {

  if (millis()-lastTime >= 500)  {             // every 500 miliseconds = 2 times per second
        
         CocoMidi.send(MIDI_NOTEON,60,127);            // put three notes into the MIDI send queue
         CocoMidi.send(MIDI_NOTEON,64,127);
         CocoMidi.send(MIDI_NOTEON,67,127);
         lastTime = millis();      
  }   

  CocoMidi.update();
}
