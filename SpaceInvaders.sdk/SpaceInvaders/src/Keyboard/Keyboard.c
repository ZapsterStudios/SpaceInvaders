// Header
#include "Keyboard.h"

// Initialize the keyboard.
int initializeKeyboard() {
	//INITIALIZE AXI PS2
	axi_ps2_CfgInitialize(&PS2_Inst, &PS2ConfigPtr, PS2_Baseaddr);

	return 0;
}

unsigned char readKey() {
	// Create buffer and receipt array
	u8 *buffer;
	unsigned char array[2] = { 0 };
	unsigned char key = 0;
	buffer = &key;

	// Read byte(s)
	int returnvalue = axi_ps2_Recv(&PS2_Inst, buffer, 1);

	// If bytes are sent key is pressed
	if (returnvalue != 0) {
//		xil_printf("1, %d Byte(s) was returned!: %u or 0x%X \n", returnvalue, array[0], array[0]);
//		xil_printf("2, %d Byte(s) was returned!: %u or 0x%X \n", returnvalue, array[1], array[1]);
		xil_printf("%d Byte(s) was returned!: %u or 0x%X \n", returnvalue, key, key);
		keyboardCMD(buffer);
//		return *buffer;
	}
	return key;
}

//----------------------------------------------------
// MAIN FUNCTION
//----------------------------------------------------
int testKeyboard(void) {

	//initializePS2(); // Initializes the PS 2 Keyboard

	unsigned char key = 0;
	while (1) {
		key = readKey();
		if(key != 0) {
			xil_printf("%u, was pressed\n\n", key);
			key = 0;
		}
	}
	return 0;
}

/*
 * Takes the buffer and hands it over to the keyboardMap() that will
 * return the char which corresponds to the key pressed or released
 *
 * @param	Unsigned char buffer where a char is located at index 0
 *
 *
 * @note 	When a key is pressed, the scan code for the button will be
 * 			in the buffer. BUT it's a little more complicated when the button
 * 			is released.
 * 				*Either a E0 will be recieved and then the following
 * 					scancode will tell which button was released
 * 				*OR a E0 will be recieved, then a F0 will be recieved
 * 					and THEN a scancode will tell which "Extended"
 * 					button was released.
 *
 Also buttons can be hold down for a longer periode of time and a new "button press"
 will then be recieved every ~ 100 ms. BUT if other buttons also get pushed down
 it will interrupt this "repeat button press every 100 ms",
 like in this serial monitor dump:

 "<-" was pressed: "107" or "0x6B"
 "^" was pressed: "117" or "0x75"
 "->" was pressed: "116" or "0x74"
 ..
 "->" was pressed: "116" or "0x74"
 "Space" was pressed: "41" or "0x29"
 "Space" button was released: "41" or "0x29"
 "<-" abutton was released: "107" or "0x6B"
 "^" extended button was released: "117" or "0x75"
 "->" button was released: "116" or "0x74"


 Buttons can be released during a button being hold down. This will
 also interrupt the original buttons "repeat button press every 100 ms".

 * Programmer : Christian Mark 12-01-2019
 *****************************************************************************/
unsigned char keyboardCMD(unsigned char *character /*unsigned char array[2]*/) {
	unsigned char printchar = 0;
	static int E0wasRecieved = 0; // A E0 was recieved on the last keyboardMap()
	static int F0wasRecieved = 0; // A F0 was recieved on the last keyboardMap()
	printchar = keyboardMap(character, &E0wasRecieved, &F0wasRecieved);
	if (printchar == 0) {return 0;}

	if (F0wasRecieved == 1) {
		if (E0wasRecieved == 1) {
			xil_printf("\t\"%c\" extended button was released", printchar);
			E0wasRecieved = 0;
		} else {
			switch (printchar) {
			case '<':
				xil_printf("\t\"<-\" button was released");
				break;
			case '>':
				xil_printf("\t\"->\" button was released");
				break;
			case 'E':
				xil_printf("\t\"Enter\" button was released");
				break;
			case 's':
				xil_printf("\t\"Space\" button was released");
				break;
			default:
				xil_printf("\t\"%c\" button was released", printchar);
				break;
			}
			//printf("\"%c\" button was released\n",printchar);
		}
		F0wasRecieved = 0;
	} else {
		switch (printchar) {
		case '<':
			xil_printf("\"<-\" was pressed");
			break;
		case '>':
			xil_printf("\"->\" was pressed");
			break;
		case 'E':
			xil_printf("\"Enter\" was pressed");
			break;
		case 's':
			xil_printf("\"Space\" was pressed");
			break;
		default:
			xil_printf("\"%c\" was pressed", printchar);
			break;
		}
	}
	xil_printf(": \"%u\" or \"0x%X\"\n", *character, *character);
	E0wasRecieved = 0;
	return printchar;
}

unsigned char keyboardMap(unsigned char *array/*[2]*/, int *E0wasRecieved, int *F0wasRecieved) {

	switch (*array/*[0]*/) {
	case 0xE0: // Extended button was released
		*E0wasRecieved = 1;
		return 0;
	case 0xF0: // Button was released
		*F0wasRecieved = 1;
		return 0;
	case 0x1C: // A
		return 'A';
	case 0x1B: // S
		return 'S';
	case 0x23: // D
		return 'D';
	case 0x1D: // W
		return 'W';
	case 0x75: // Arrow up
		return '^';
	case 0x72: // Arrow down
		return 'v';
	case 0x6b: // Arrow left
		*E0wasRecieved = 0;
		return '<';
	case 0x74: // Arrow right
		*E0wasRecieved = 0;
		return '>';
	case 0x5A: // Enter
		*E0wasRecieved = 0;
		return 'E';
	case 0x29: // Space
		*E0wasRecieved = 0;
		return 's';
	default:
//		xil_printf("convertion of %u//%u ", *array, array);
		*E0wasRecieved = 0;
		*F0wasRecieved = 0;
		return 0;
	}

}
