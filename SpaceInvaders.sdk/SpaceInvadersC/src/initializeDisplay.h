#ifndef SRC_INITIALIZEDISPLAY_H_
#define SRC_INITIALIZEDISPLAY_H_

// Includes
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "display_ctrl/display_ctrl.h"

// Video defines
#define MAX_FRAME 	(1920*1080*3)
#define STRIDE 		(1920 * 3)

// XPAR defines
#define DYNCLK_BASEADDR XPAR_AXI_DYNCLK_0_BASEADDR
#define VGA_VDMA_ID XPAR_AXIVDMA_0_DEVICE_ID
#define DISP_VTC_ID XPAR_VTC_0_DEVICE_ID

// Video driver struct.
XAxiVdma vdma;

// Display controller struct.
DisplayCtrl controller;

// Create extern buffer pointers.
extern u8 buffer[DISPLAY_NUM_FRAMES][MAX_FRAME];
extern u8 *bufferPointer[DISPLAY_NUM_FRAMES];

/**
 * Initialize the display.
 */
void initializeDisplay()
{
	// Create status variable.
	int status;

	// Create VDMA config holder.
	XAxiVdma_Config *vdmaConfig;

	// Connect the buffer pointer.
	for (int i = 0; i < DISPLAY_NUM_FRAMES; i++)
	{
		bufferPointer[i] = buffer[i];
	}

	// Get configuration for VDMA device.
	vdmaConfig = XAxiVdma_LookupConfig(VGA_VDMA_ID);
	if (! vdmaConfig)
	{
		xil_printf("No video DMA found for ID %d\r\n", VGA_VDMA_ID);
		return;
	}

	// Initialize the VDMA device.
	status = XAxiVdma_CfgInitialize(&vdma, vdmaConfig, vdmaConfig->BaseAddress);
	if (status != XST_SUCCESS)
	{
		xil_printf("VDMA initialization failed %d\r\n", status);
		return;
	}

	// Initialize the display controller.
	status = DisplayInitialize(&controller, &vdma, DISP_VTC_ID, DYNCLK_BASEADDR, bufferPointer, STRIDE);
	if (status != XST_SUCCESS)
	{
		xil_printf("Display controller initialization failed %d\r\n", status);
		return;
	}

	// Start the display controller.
	status = DisplayStart(&controller);
	if (status != XST_SUCCESS)
	{
		xil_printf("Couldn't start display controller %d\r\n", status);
		return;
	}
}

#endif
