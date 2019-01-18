#ifndef SRC_KEYBOARD_KEYBOARD_H_
#define SRC_KEYBOARD_KEYBOARD_H_

// Includes
#include "xil_io.h"
#include "axi_ps2.h"
#include "xscugic.h"
#include "xil_printf.h"
#include "xparameters.h"

// Parameter definitions
#define INTC_DEVICE_ID 		XPAR_PS7_SCUGIC_0_DEVICE_ID

//Extra XPAR_FABRIC_AXI_PS2_0_PS2_INTERRUPT_INTR
#define INTC_GPIO_INTERRUPT_ID	XPAR_FABRIC_AXI_PS2_0_PS2_INTERRUPT_INTR// XPAR_FABRIC_AXI_GPIO_1_IP2INTC_IRPT_INTR

XScuGic INTCInst;

// Til PS2
#define PS2_Baseaddr XPAR_AXI_PS2_0_S_AXI_BASEADDR
axi_ps2 PS2_Inst;
axi_ps2_Config PS2ConfigPtr;

// Prototypes
int initializePS2();
int testKeyboard();
unsigned char keyboardMap(unsigned char *character, int *E0wasRecieved, int *F0wasRecieved);
unsigned char keyboardCMD(unsigned char *charRecieved/*[2]*/);
unsigned char readKey();

#endif /* SRC_KEYBOARD_KEYBOARD_H_ */
