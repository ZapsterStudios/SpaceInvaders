/*
 * audio.c
 *
 *  Contains all of the functions related to audio codec setup.
 */
#include "audio.h"

#include "sleep.h"


/* ---------------------------------------------------------------------------- *
 * 									Initialize_Sound_IPs()									*
 * ---------------------------------------------------------------------------- *
 * Initialises the IIC driver by looking up the configuration in the config
 * table and then initialising it. Also sets the IIC serial clock rate.
 * ---------------------------------------------------------------------------- */
int Initialize_Sound_IPs(void) {
	//Configure the IIC data structure
	IicConfig(XPAR_XIICPS_0_DEVICE_ID);

	//Configure the Audio Codec's PLL
	AudioPllConfig();

	xil_printf("SSM2603 configured\n\r");

	// Initialise GPIO
	int Status;

	Status = XGpio_Initialize(&Gpio_audio_enable, AUDIO_ENABLE_ID);
	if(Status != XST_SUCCESS) return XST_FAILURE;

	XGpio_SetDataDirection(&Gpio_audio_enable, 1, 0x00);

	//return XST_SUCCESS;

	nco_init(&Nco);

	xil_printf("GPIO and NCO peripheral configured\r\n");

	/* Display interactive menu interface via terminal */
	//menu(); - Stream Audio
	u32 sound;
	XGpio_DiscreteWrite(&Gpio_audio_enable, 1, 1);
	while(1) {
			for(int i = 0; i <= sizeof(deathSound); i++){
				sound = deathSound[i];
				Xil_Out32(I2S_DATA_TX_L_REG, sound);
				Xil_Out32(I2S_DATA_TX_R_REG, sound);
			}
			sleep(1);
	}
	while(1) {
		xil_printf("STREAMING AUDIO\r\n");
		xil_printf("Press 'q' to return to the main menu\r\n");
		XGpio_DiscreteWrite(&Gpio_audio_enable, 1, 1);
		audio_stream();
	}

    return XST_SUCCESS;
}

/* ---------------------------------------------------------------------------- *
 * 								nco_initi()									*
 * ---------------------------------------------------------------------------- *
 * Initialises the NCO driver by looking up the configuration in the config
 * table and then initialising it.
 * ---------------------------------------------------------------------------- */
void nco_init(void *InstancePtr) {
	XNco_Config *cfgPtr;
	int status;

	/* Initialise the NCO driver so that it's ready to use */

	// Look up the configuration in the config table
	cfgPtr = XNco_LookupConfig(NCO_ID);
	if (!cfgPtr) {
		print("ERROR: Lookup of NCO configuration failed.\n\r");
	}

	// Initialise the NCO driver configuration
	status = XNco_CfgInitialize(InstancePtr, cfgPtr);
	if (status != XST_SUCCESS) {
		print("ERROR: Could not initialise NCO.\n\r");
	}
}

/* ---------------------------------------------------------------------------- *
 * 								AudioPllConfig()								*
 * ---------------------------------------------------------------------------- *
 * Reset CODEC Software and power-up. Set default ADC and DAC volumes and enable
 * the correct signal path to the DSP unit. Set a 48 kHz sampling rate and
 * activate the SSM CODEC.
 * ---------------------------------------------------------------------------- */
void AudioPllConfig() {
	AudioWriteToReg(R15_SOFTWARE_RESET, 				0b000000000); //Perform Reset
	usleep(75000);
	AudioWriteToReg(R6_POWER_MANAGEMENT, 				0b000110000); //Power Up
	AudioWriteToReg(R0_LEFT_CHANNEL_ADC_INPUT_VOLUME, 	0b000010111); //Default Volume
	AudioWriteToReg(R1_RIGHT_CHANNEL_ADC_INPUT_VOLUME, 	0b000010111); //Default Volume
	AudioWriteToReg(R2_LEFT_CHANNEL_DAC_VOLUME, 		0b001111001);
	AudioWriteToReg(R3_RIGHT_CHANNEL_DAC_VOLUME, 		0b001111001);
	AudioWriteToReg(R4_ANALOG_AUDIO_PATH, 				0b000010010); //Allow Mixed DAC, Mute MIC
	AudioWriteToReg(R5_DIGITAL_AUDIO_PATH, 				0b000000111); //48 kHz Sampling Rate emphasis, no high pass
	AudioWriteToReg(R7_DIGITAL_AUDIO_I_F, 				0b000001110); //I2S Mode, set-up 32 bits
	AudioWriteToReg(R8_SAMPLING_RATE, 					0b000000000);
	usleep(75000);
	AudioWriteToReg(R9_ACTIVE, 							0b000000001); //Activate digital core
	AudioWriteToReg(R6_POWER_MANAGEMENT, 				0b000100010); //Output Power Up
}

/* ---------------------------------------------------------------------------- *
 * 									IicConfig()									*
 * ---------------------------------------------------------------------------- *
 * Initialises the IIC driver by looking up the configuration in the config
 * table and then initialising it. Also sets the IIC serial clock rate.
 * ---------------------------------------------------------------------------- */
unsigned char IicConfig(unsigned int DeviceIdPS) {
	XIicPs_Config *Config;
	int Status;

	/* Initialise the IIC driver so that it's ready to use */

	// Look up the configuration in the config table
	Config = XIicPs_LookupConfig(DeviceIdPS);
	if(NULL == Config) {
		return XST_FAILURE;
	}

	// Initialise the IIC driver configuration
	Status = XIicPs_CfgInitialize(&Iic, Config, Config->BaseAddress);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XIicPs_SelfTest(&Iic);
	if (Status != XST_SUCCESS) {
		xil_printf("IIC FAILED \r\n");
		return XST_FAILURE;

	}
	xil_printf("IIC Passed\r\n");


	//Set the IIC serial clock rate.
	Status = XIicPs_SetSClk(&Iic, IIC_SCLK_RATE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/* ---------------------------------------------------------------------------- *
 * 								AudioWriteToReg									*
 * ---------------------------------------------------------------------------- *
 * Function to write to one of the registers from the audio
 * controller.
 * ---------------------------------------------------------------------------- */
void AudioWriteToReg(u8 u8RegAddr, u16 u16Data) {

	unsigned char u8TxData[2];

	u8TxData[0] = u8RegAddr << 1;
	u8TxData[0] = u8TxData[0] | ((u16Data >> 8) & 0b1);

	u8TxData[1] = u16Data & 0xFF;

	XIicPs_MasterSendPolled(&Iic, u8TxData, 2, IIC_SLAVE_ADDR);
	while(XIicPs_BusIsBusy(&Iic));
}

/* ---------------------------------------------------------------------------- *
 * 								audio_stream()									*
 * ---------------------------------------------------------------------------- *
 * This function performs audio loopback streaming by sampling the input audio
 * from the codec and then immediately passing the sample to the output of the
 * codec.
 *
 * The main menu can be accessed by entering 'q' on the keyboard.
 * ---------------------------------------------------------------------------- */
void audio_stream(){

	//int index = 0;
	u32  in_left, in_right;

	while (!XUartPs_IsReceiveData(UART_BASEADDR)){
		// Read audio input from codec
		in_left = Xil_In32(I2S_DATA_RX_L_REG);
		in_right = Xil_In32(I2S_DATA_RX_R_REG);

		// Write audio input to codec
		Xil_Out32(I2S_DATA_TX_L_REG, in_left);
		Xil_Out32(I2S_DATA_TX_R_REG, in_right);
	}

	/* If input from the terminal is 'q', then return to menu.
	 * Else, continue streaming. */
	if(XUartPs_ReadReg(UART_BASEADDR, XUARTPS_FIFO_OFFSET) == 'q') menu();
	else audio_stream();
} // audio_stream()

void menu(){
	u8 inp = 0x00;
	u32 CntrlRegister;

	XGpio_DiscreteWrite(&Gpio_audio_enable, 1, 0);

	CntrlRegister = XUartPs_ReadReg(UART_BASEADDR, XUARTPS_CR_OFFSET);

	XUartPs_WriteReg(UART_BASEADDR, XUARTPS_CR_OFFSET,
				  ((CntrlRegister & ~XUARTPS_CR_EN_DIS_MASK) |
				   XUARTPS_CR_TX_EN | XUARTPS_CR_RX_EN));

	xil_printf("\r\n\r\n");
	xil_printf("Embedded LMS Filtering Demo\r\n");
	xil_printf("Enter 's' to stream pure audio, 'n' to add tonal noise and 'f' to adaptively filter\r\n");
	xil_printf("----------------------------------------\r\n");

	// Wait for input from UART via the terminal
	while (!XUartPs_IsReceiveData(UART_BASEADDR));
				inp = XUartPs_ReadReg(UART_BASEADDR, XUARTPS_FIFO_OFFSET);
	// Select function based on UART input
	switch(inp){
	case 's':
		xil_printf("STREAMING AUDIO\r\n");
		xil_printf("Press 'q' to return to the main menu\r\n");
		XGpio_DiscreteWrite(&Gpio_audio_enable, 1, 1);
		audio_stream();
		break;
	case 'n':
		xil_printf("ENTERING NOISE GENERATION OPERATION\r\n");
		xil_printf("Select step size via the DIP switches...\r\n\n");
		xil_printf("Press 'q' to return to the main menu\r\n");
		XGpio_DiscreteWrite(&Gpio_audio_enable, 1, 1);
		//tonal_noise();
		break;
	case 'f':
		xil_printf("ENTERING LMS FILTERING OPERATION\r\n");
		xil_printf("Press 'q' to return to the main menu\r\n");
		XGpio_DiscreteWrite(&Gpio_audio_enable, 1, 1);
		//lms_filter();
		break;
	default:
		menu();
		break;
	} // switch
} // menu()
