/*

CocoMidi Library (based on GnusbuinoMidi)

*/


#ifndef __CocoMidi_h__
#define __CocoMidi_h__

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <avr/wdt.h>

#include <string.h>

#include <CocoMidiConfig.h>

#include "usbdrv.h"

// this is from original midi_gnusb.h
#define ADC_MUX_MASK            0x0F
#define ADC_PRESCALE_DIV64      0x06    ///< 0x06 -> CPU clk/64
#define ADC_PRESCALE_MASK       0x07

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

#define MIDI_NOTEOFF      0x80
#define MIDI_NOTEON       0x90
#define MIDI_POLYAFTERTOUCH   0xA0
#define MIDI_CONTROLCHANGE    0xB0
#define MIDI_PROGRAMCHANGE    0xC0
#define MIDI_CHANNELAFTERTOUCH  0xD0
#define MIDI_PITCHBEND      0xE0

//Realtime Messages
#define MIDI_QUARTERFRAME   0xF1
#define MIDI_SONGPOS      0xF2
#define MIDI_SONGSELECT     0xF3
#define MIDI_TIMINGCLOCK    0xF8
#define MIDI_START        0xFA
#define MIDI_CONTINUE     0xFB
#define MIDI_STOP       0xFC

/* Name: desc_midi.inc
 * Project: V-USB MIDI device on Low-Speed USB
 * Author: Martin Homuth-Rosemann
 * Creation Date: 2008-03-11
 * Copyright: (c) 2008,2009,2010 by Martin Homuth-Rosemann.
 * License: GPL.
 * Version: 0.03
 */

// This descriptor is based on http://www.usb.org/developers/devclass_docs/midi10.pdf
// constants from midi10.pdf

// Appendix A

// A.1 MS Class-Specific Interface Descriptor Subtypes
#define M10_MS_DESCRIPTOR_UNDEFINED 0x00
#define M10_MS_HEADER     0x01
#define M10_MIDI_IN_JACK    0x02
#define M10_MIDI_OUT_JACK   0x03
#define M10_ELEMENT     0x04

// A.2 MS Class-Specific Endpoint Descriptor Subtypes
#define M10_DESCRIPTOR_UNDEFINED  0x00
#define M10_MS_GENERAL      0x01

// A.3 MS MIDI IN and OUT Jack types
#define M10_JACK_TYPE_UNDEFINED   0x00
#define M10_EMBEDDED      0x01
#define M10_EXTERNAL      0x02


#define M10_DEVICE      0x01
#define M10_CONFIGURATION   0x02
#define M10_INTERFACE     0x04
#define M10_ENDPOINT      0x05
#define M10_CS_INTERFACE    0x24
#define M10_CS_ENDPOINT     0x25

#define M10_BULK      0x02
#define M10_INTERRUPT     0x03

#define M10_AUDIO     0x01
#define M10_AUDIO_CONTROL   0x01
#define M10_MIDISTREAMING   0x03

#define M10_UNUSED      0x00

#define M10_DIR_IN      0x80
#define M10_DIR_OUT     0x00


#define COCOMIDI_WITH_AUDIO_CONTROL

// don't include (mandatory) dummy AudioControl descriptor
#ifndef COCOMIDI_WITH_AUDIO_CONTROL
#define WITHOUT_AUDIO_CONTROL
#endif

// This descriptor is based on http://www.usb.org/developers/devclass_docs/midi10.pdf
//
// Appendix B. Example: Simple MIDI Adapter (Informative)
// B.1 Device Descriptor
//
static PROGMEM const char deviceDescrMIDI[] = { /* USB device descriptor */
    18,     /* sizeof(usbDescriptorDevice): length of descriptor in bytes */
    M10_DEVICE,   /* descriptor type */
    0x10, 0x01,   /* bcdUSB USB version supported */
    M10_UNUSED,   /* device class: defined at interface level */
    M10_UNUSED,   /* subclass */
    M10_UNUSED,   /* protocol */
    8,      /* bMaxPacketSize max packet size */
    0xc0, 0x16,   /* idVendor  16c0 VOTI 2 bytes */
    0xe4, 0x05,   /* idProduct 05e4 MIDI 2 bytes */
    0x03, 0x00,   /* bcdDevice 0.03 2 bytes */
    1,      /* iManufacturer manufacturer string index */
    2,      /* iProduct product string index */
    M10_UNUSED,   /* serial number string index */
    1,      /* number of configurations */
};



// B.2 Configuration Descriptor
static PROGMEM const char configDescrMIDI[] = { /* USB configuration descriptor */
    9,      /* sizeof(usbDescrConfig): length of descriptor in bytes */
    USBDESCR_CONFIG,  /* descriptor type */

#ifdef WITHOUT_AUDIO_CONTROL
    18+65, 0,   /* 9+9+65 total length of data returned */
                /* (including inlined descriptors) */
                /* B.2 + (B.3) + B.4.1 + length in B.4.2 */
    1,      /* bNumInterfaces AC + MS = 2 */
                /* (number of interfaces in this configuration) */
#else
    36+65, 0,   /* 9+(9)+(9)+9+65 total length of data returned */
                /* (including inlined descriptors) */
                /* B.2 + (B.3) + B.4.1 + length in B.4.2 */
    2,      /* bNumInterfaces AC + MS = 2 */
                /* (number of interfaces in this configuration) */
#endif

    1,      /* index of this configuration */
    M10_UNUSED,   /* configuration name string index */
#if USB_CFG_IS_SELF_POWERED
    (1 << 7) | USBATTR_SELFPOWER, /* attributes */
#else
    (1 << 7),   /* attributes */
#endif
    USB_CFG_MAX_BUS_POWER / 2,  /* max USB current in 2mA units */


#ifndef WITHOUT_AUDIO_CONTROL
//////////////////////////////////////////
// TEST: leave out AudioControl   //
//  changes:      //
//  set total length in B.2   //
//  set bNumInterfaces=1 in B.2 //
//  set bInterfaceNumber=0 in B.4.1 //
//////////////////////////////////////////
//
// B.3 AudioControl Interface Descriptors
// The AudioControl interface describes the device structure (audio function topology)
// and is used to manipulate the Audio Controls. This device has no audio function
// incorporated. However, the AudioControl interface is mandatory and therefore both
// the standard AC interface descriptor and the classspecific AC interface descriptor
// must be present. The class-specific AC interface descriptor only contains the header
// descriptor.

// B.3.1 Standard AC Interface Descriptor
// The AudioControl interface has no dedicated endpoints associated with it. It uses the
// default pipe (endpoint 0) for all communication purposes. Class-specific AudioControl
// Requests are sent using the default pipe. There is no Status Interrupt endpoint provided.
    /* AC interface descriptor follows inline: */
    9,      /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE, /* descriptor type */
    0,      /* index of this interface */
    0,      /* alternate setting for this interface */
    0,      /* endpoints excl 0: number of endpoint descriptors to follow */
    M10_AUDIO,    /* bInterfaceClass*/
    M10_AUDIO_CONTROL,  /* bInterfaceSubclass*/
    M10_UNUSED,   /* bInterfaceProtocol*/
    M10_UNUSED,   /* string index for interface */

// B.3.2 Class-specific AC Interface Descriptor
// The Class-specific AC interface descriptor is always headed by a Header descriptor
// that contains general information about the AudioControl interface. It contains all
// the pointers needed to describe the Audio Interface Collection, associated with the
// described audio function. Only the Header descriptor is present in this device
// because it does not contain any audio functionality as such.
    /* AC Class-Specific descriptor */
    9,      /* sizeof(usbDescrCDC_HeaderFn): length of descriptor in bytes */
    M10_CS_INTERFACE, /* descriptor type */
    M10_MS_HEADER,    /* header functional descriptor */
    0x0, 0x01,    /* bcdADC */
    9, 0,     /* wTotalLength */
    1,      /* bInCollection */
    1,      /* baInterfaceNr(1) */

#endif

// B.4 MIDIStreaming Interface Descriptors

// B.4.1 Standard MS Interface Descriptor
    /* interface descriptor follows inline: */
    9,      /* length of descriptor in bytes */
    USBDESCR_INTERFACE, /* descriptor type */
#ifdef WITHOUT_AUDIO_CONTROL
    0,      /* bInterfaceNumber index of this interface */
#else
    1,      /* bInterfaceNumber index of this interface */
#endif
    0,      /* alternate setting for this interface */
    2,      /* endpoints excl 0: number of endpoint descriptors to follow */
    M10_AUDIO,    /* bInterfaceClass */
    M10_MIDISTREAMING,  /* bInterfaceSubclass */
    M10_UNUSED,   /* bInterfaceProtocol*/
    M10_UNUSED,   /* iInterface string index for interface */

// B.4.2 Class-specific MS Interface Descriptor
    /* MS Class-Specific descriptor */
    7,      /* bLength */
    M10_CS_INTERFACE, /* bDescritorType */
    M10_MS_HEADER,    /* header functional descriptor */
    0x0, 0x01,    /* bcdADC */
    65, 0,      /* wTotalLength B.4.2 .. B.6.2 */
                /* = 7 + 6 + 6 + 9 + 9 + 9 + 5 + 9 + 5 */

// B.4.3 MIDI IN Jack Descriptor
    6,      /* bLength */
    M10_CS_INTERFACE, /* descriptor type */
    M10_MIDI_IN_JACK, /* desc subtype */
    M10_EMBEDDED,   /* bJackType */
    1,      /* bJackID */
    M10_UNUSED,   /* iJack */

    6,      /* bLength */
    M10_CS_INTERFACE, /* descriptor type */
    M10_MIDI_IN_JACK, /* desc subtype */
    M10_EXTERNAL,   /* EXTERNAL bJackType */
    2,      /* bJackID */
    M10_UNUSED,   /* iJack */

//B.4.4 MIDI OUT Jack Descriptor
    9,      /* length of descriptor in bytes */
    M10_CS_INTERFACE, /* descriptor type */
    M10_MIDI_OUT_JACK,  /* descriptor */
    M10_EMBEDDED,   /* bJackType */
    3,      /* bJackID */
    1,      /* No of input pins */
    2,      /* BaSourceID */
    1,      /* BaSourcePin */
    M10_UNUSED,   /* iJack */

    9,      /* bLength of descriptor in bytes */
    M10_CS_INTERFACE, /* bDescriptorType */
    M10_MIDI_OUT_JACK,  /* bDescriptorSubtype */
    M10_EXTERNAL,   /* bJackType */
    4,      /* bJackID */
    1,      /* bNrInputPins */
    1,      /* baSourceID (0) */
    1,      /* baSourcePin (0) */
    M10_UNUSED,   /* iJack */


// B.5 Bulk OUT Endpoint Descriptors

//B.5.1 Standard Bulk OUT Endpoint Descriptor
    9,      /* bLenght */
    USBDESCR_ENDPOINT,  /* bDescriptorType = endpoint */
    1 | M10_DIR_OUT,  /* bEndpointAddress OUT endpoint number 1 */
#ifdef COCOMIDI_ENABLE_ANDROID
    M10_BULK,   /* bmAttributes: 2:Bulk, 3:Interrupt endpoint */
#else
    M10_INTERRUPT,
#endif
    8, 0,     /* wMaxPacketSize */
    2,      /* bIntervall in ms */
    M10_UNUSED,   /* bRefresh */
    M10_UNUSED,   /* bSyncAddress */

// B.5.2 Class-specific MS Bulk OUT Endpoint Descriptor
    5,      /* bLength of descriptor in bytes */
    M10_CS_ENDPOINT,  /* bDescriptorType */
    M10_MS_GENERAL,   /* bDescriptorSubtype */
    1,      /* bNumEmbMIDIJack */
    1,      /* baAssocJackID (0) */

//B.6 Bulk IN Endpoint Descriptors

//B.6.1 Standard Bulk IN Endpoint Descriptor
    9,      /* bLenght */
    USBDESCR_ENDPOINT,  /* bDescriptorType = endpoint */
    M10_DIR_IN | 1,   /* bEndpointAddress IN endpoint number 1 */
#ifdef COCOMIDI_ENABLE_ANDROID
    M10_BULK,   /* bmAttributes: 2:Bulk, 3:Interrupt endpoint */
#else
    M10_INTERRUPT,
#endif
    8, 0,     /* wMaxPacketSize */
    2,      /* bIntervall in ms */
    M10_UNUSED,   /* bRefresh */
    M10_UNUSED,   /* bSyncAddress */

// B.6.2 Class-specific MS Bulk IN Endpoint Descriptor
    5,      /* bLength of descriptor in bytes */
    M10_CS_ENDPOINT,  /* bDescriptorType */
    M10_MS_GENERAL,   /* bDescriptorSubtype */
    1,      /* bNumEmbMIDIJack */
    3     /* baAssocJackID (0) */
};


#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny84__) //fix buffer for attiny84 by stahl
    #define MIDI_MAX_BUFFER   10
#else
    #define MIDI_MAX_BUFFER   64
#endif


typedef struct
{
    unsigned char command;
    unsigned char key;
    unsigned char value;
} MIDIMessage;

typedef struct _midi_msg
{
    uchar cn : 4;
    uchar cin : 4;
    uchar byte[3];
} midi_msg;

class CocoMidiDevice {
private:
    unsigned char _midiOutData[4];

    unsigned char _midiSendEnqueueIdx;
    unsigned char _midiSendDequeueIdx;
    unsigned char _midiSendQueue [MIDI_MAX_BUFFER * 3];

    unsigned char _midiRecvEnqueueIdx;
    unsigned char _midiRecvDequeueIdx;
    unsigned char _midiRecvQueue [MIDI_MAX_BUFFER * 3];

 public:
  void init () {
    cli();
    usbDeviceDisconnect();
    _delay_ms(250);
    usbDeviceConnect();
    usbInit();
    sei();
  }
	
	// delay while updating until we are finished delaying
	void delay(long milli) {
		unsigned long last = millis();
	  while (milli > 0) {
	    unsigned long now = millis();
	    milli -= now - last;
	    last = now;
	    update();
	  }
	}

  void sendMidiQueue()
  {
        if (usbInterruptIsReady())    // ready to send some MIDI ?
        {

            if (_midiSendEnqueueIdx != _midiSendDequeueIdx)
                {

                    unsigned char cmd;
                    cmd = _midiSendQueue[_midiSendDequeueIdx];

                    this->_midiOutData[0] = ((cmd>>4) & 0x0F) | ((cmd<<4) & 0xF0); //swap high/low nibble
                    this->_midiOutData[1] = cmd;
                    this->_midiOutData[2] = _midiSendQueue[_midiSendDequeueIdx+1];
                    this->_midiOutData[3] = _midiSendQueue[_midiSendDequeueIdx+2];


                    _midiSendQueue[_midiSendDequeueIdx++] = 0;
                    _midiSendQueue[_midiSendDequeueIdx++] = 0;
                    _midiSendQueue[_midiSendDequeueIdx++] = 0;

                    _midiSendDequeueIdx %= MIDI_MAX_BUFFER * 3;

                    usbSetInterrupt(this->_midiOutData, 4);

                }
        }
  }





  void send(unsigned char command, unsigned char pitch,unsigned char velocity)
{

    // see if this command is already in queue, replace value
    for (unsigned char i = 0; i < MIDI_MAX_BUFFER; i++)
        {
            if (_midiSendQueue[3*i] == command)
                {
                    if (_midiSendQueue[3*i+1] == pitch)
                        {
                            _midiSendQueue[3*i+2] = velocity;
                            return;
                        }
                }
        }
    _midiSendQueue[_midiSendEnqueueIdx++] = command;
    _midiSendQueue[_midiSendEnqueueIdx++] = pitch;
    _midiSendQueue[_midiSendEnqueueIdx++] = velocity;

    _midiSendEnqueueIdx %= MIDI_MAX_BUFFER * 3;
  }



  unsigned char read(MIDIMessage* msg)
{
    if (_midiRecvEnqueueIdx != _midiRecvDequeueIdx)
        {
            msg->command  = _midiRecvQueue[_midiRecvDequeueIdx];
            msg->key    = _midiRecvQueue[_midiRecvDequeueIdx+1];
            msg->value    = _midiRecvQueue[_midiRecvDequeueIdx+2];

            _midiRecvQueue[_midiRecvDequeueIdx++] = 0;
            _midiRecvQueue[_midiRecvDequeueIdx++] = 0;
            _midiRecvQueue[_midiRecvDequeueIdx++] = 0;

            _midiRecvDequeueIdx %= MIDI_MAX_BUFFER * 3;
            return 1;
        }
    else
        {
            return 0;
        }
}


void receiveMIDI(unsigned char command, unsigned char pitch,unsigned char velocity)
{
    // see if this command is already in queue, replace value
    for (unsigned char i = 0; i < MIDI_MAX_BUFFER; i++)
        {
            if (_midiRecvQueue[3*i] == command)
                {
                    if (_midiRecvQueue[3*i+1] == pitch)
                        {
                            _midiRecvQueue[3*i+2] = velocity;
                            return;
                        }
                }
        }
    _midiRecvQueue[_midiRecvEnqueueIdx++] = command;
    _midiRecvQueue[_midiRecvEnqueueIdx++] = pitch;
    _midiRecvQueue[_midiRecvEnqueueIdx++] = velocity;

    _midiRecvEnqueueIdx %= MIDI_MAX_BUFFER * 3;

}

void sendCCHires(uint16_t value, uint8_t ch)
{
    uint8_t valH = value >> 3;
    uint8_t valL = value & 0b0111;
    this->send(MIDI_CONTROLCHANGE, ch+1, valH);
    this->send(MIDI_CONTROLCHANGE, ch+2, valL);
}

void sendCCHires(int value, uint8_t ch)
{
    uint8_t valH, valL;

    if (value < 0)
    {
        value = abs(value);
        ch = ch + 2;
    }

    valH = value >> 3;
    valL = value & 0b0111;

    this->send(MIDI_CONTROLCHANGE, ch, valH);
    this->send(MIDI_CONTROLCHANGE, ch+1, valL);

}

void sendCC(uint8_t ch, uint8_t value)
{
    this->send(MIDI_CONTROLCHANGE, ch, value);
}

  void update()
  {
    usbPoll();
    this->sendMidiQueue();
  }




};

CocoMidiDevice CocoMidi = CocoMidiDevice();

#ifdef __cplusplus
extern "C"{
#endif 


static inline void restartToBootloader()
{
    cli();                          // turn off interrupts
    wdt_disable();                  // disable watchdog timer
    usbDeviceDisconnect();          // disconnect gnusb from USB bus
    _delay_ms(100);

    USB_INTR_ENABLE = 0;
    USB_INTR_CFG = 0;       /* also reset config bits */

    wdt_enable(WDTO_30MS);  // enable watchdog timer
    while(1)            // let WDT reset gnusb
        {
        }
}

// ------------------------------------------------------------------------------
// - usbFunctionDescriptor
// ------------------------------------------------------------------------------

unsigned char usbFunctionDescriptor(usbRequest_t * rq)
{

    if (rq->wValue.bytes[1] == USBDESCR_DEVICE)
        {
            usbMsgPtr = (unsigned char *) deviceDescrMIDI;
            return sizeof(deviceDescrMIDI);
        }
    else      /* must be config descriptor */
        {
            usbMsgPtr = (unsigned char *) configDescrMIDI;
            return sizeof(configDescrMIDI);
        }
}

// ------------------------------------------------------------------------------
// - usbFunctionSetup
// ------------------------------------------------------------------------------
// this function gets called when the usb driver receives a non standard request
// that is: our own requests defined in ../common/gnusb_cmds.h
unsigned char usbFunctionSetup(unsigned char data[8])
{
    void *pVoid = data;
    usbRequest_t    *rq = static_cast<usbRequest_t*>(pVoid);

    if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)    /* class request type */
        {

            /*  Prepare bulk-in endpoint to respond to early termination   */
            if ((rq->bmRequestType & USBRQ_DIR_MASK) ==
                    USBRQ_DIR_HOST_TO_DEVICE) {}
        }
    return 0xff;
}


// ---------------------------------------------------------------------------
//  usbFunctionRead
// ---------------------------------------------------------------------------

unsigned char usbFunctionRead(unsigned char * data, unsigned char len)
{

//  statusLedToggle(StatusLed_Yellow);

//???? thats from http://cryptomys.de/horo/V-USB-MIDI/index.html
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;
    data[5] = 0;
    data[6] = 0;

    return 7;
}


/*---------------------------------------------------------------------------*/
/* usbFunctionWrite                                                          */
/*---------------------------------------------------------------------------*/

unsigned char usbFunctionWrite(unsigned char * data, unsigned char len)
{
    return 1;
}

/*---------------------------------------------------------------------------*/
/* usbFunctionWriteOut                                                       */
/*                                                                           */
/* this Function is called if a MIDI Out message (from PC) arrives.          */
/*                                                                           */
/*---------------------------------------------------------------------------*/
void usbFunctionWriteOut(unsigned char * data, unsigned char len)
{
    if (len == 4)
        {
            //Sysex message  07 F0 08 F7
            if (
                data[0] == 0x07 && data[1] == 0xF0 && data[2] == 0x08 && data[3] == 0xF7
            )
                {
                    restartToBootloader();
                }
        }

    while (len >= sizeof(midi_msg))
        {

            midi_msg* msg = (midi_msg*)data;

            CocoMidi.receiveMIDI(msg->byte[0],msg->byte[1],msg->byte[2]);

            data += sizeof(midi_msg);
            len -= sizeof(midi_msg);
        }
}



#ifdef __cplusplus
} // extern "C"
#endif


#endif // __CocoMidi_h__
