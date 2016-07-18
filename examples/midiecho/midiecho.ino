/*---------------------------------------------------------------------------------------------

  CocoMidi Library
  based on Gnusbuino MIDI Library 2012 by Michael Egger
 
  MIDIECHO EXAMPLE
  Echoes anything it receives back over MIDI.
 
  This example code is in the public domain.

--------------------------------------------------------------------------------------------- */

 
#include "CocoMidi.h" // you have to include the TeenyMidi library

MIDIMessage message;// Variable to hold most recent midi message

void setup() {
  CocoMidi.init();
}


void loop() {
  
    if (CocoMidi.read(&message)) {                                    // need to put the ampersand "&" before "message"
        CocoMidi.send(message.command,message.key,message.value);
    }

    CocoMidi.update();
    
 }
 
 
 /*---------------------------------------------------------------------------------------------
Some exercises:
   
   - build a patch in pd / MaxMSP that measures MIDI latency of the Gnusbuino
*/   
