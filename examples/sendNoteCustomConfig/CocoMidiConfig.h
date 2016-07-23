// If you are not using babygnusbuino board from arduino ide you can directly edit this file

//
// Misc Options with midi descriptor.. 
//

//#define COCOMIDI_ENABLE_ANDROID		//hack to make device compatible with android device but broke on windows
//#define COCOMIDI_WITH_AUDIO_CONTROL	//enable virtual dummy audio device
#define COCOMIDI_ENABLE_DIRECT_BOOT	//disable calibration

//
// Custom USB D+ and D- pin
//

#define COCOMIDI_ENABLE_CUSTOM_USB_CFG
#define USB_CFG_IOPORTNAME B
#define USB_CFG_DMINUS_BIT 5
#define USB_CFG_DPLUS_BIT 1


//
// Change interrupt for D+
//

/*
#define COCOMIDI_ENABLE_CUSTOM_DPLUS_INTERRUPT
#define USB_INTR_CFG            PCMSK
#define USB_INTR_CFG_SET        (1 << USB_CFG_DPLUS_BIT)
#define USB_INTR_CFG_CLR        0
#define USB_INTR_ENABLE         GIMSK
#define USB_INTR_ENABLE_BIT     PCIE
#define USB_INTR_PENDING        GIFR
#define USB_INTR_PENDING_BIT    PCIF
#define USB_INTR_VECTOR         PCINT0_vect    	
*/
