// If you are not using babygnusbuino board from arduino ide you can directly edit this file

//
// Misc Options with midi descriptor.. 
//

//#define TEENYMIDI_ENABLE_ANDROID		//hack to make device compatible with android device but broke on windows
//#define TEENYMIDI_WITH_AUDIO_CONTROL	//enable virtual dummy audio device
//#define TEENYMIDI_ENABLE_DIRECT_BOOT	//enable virtual dummy audio device

//
// Custom USB D+ and D- pin
//

//#define TEENYMIDI_ENABLE_CUSTOM_USB_CFG
//#define USB_IOPORTNAME B	//when custom cfg disabled default is B
//#define USB_DMINUS 5	//when custom cfg disabled default is 5
//#define USB_DPLUS 3	//when custom cfg disabled default is 3


//
// Change interrupt for D+
//

/*
#define TEENYMIDI_ENABLE_CUSTOM_DPLUS_INTERRUPT
#define USB_INTR_CFG            PCMSK
#define USB_INTR_CFG_SET        (1 << USB_CFG_DPLUS_BIT)
#define USB_INTR_CFG_CLR        0
#define USB_INTR_ENABLE         GIMSK
#define USB_INTR_ENABLE_BIT     PCIE
#define USB_INTR_PENDING        GIFR
#define USB_INTR_PENDING_BIT    PCIF
#define USB_INTR_VECTOR         PCINT0_vect    	
*/