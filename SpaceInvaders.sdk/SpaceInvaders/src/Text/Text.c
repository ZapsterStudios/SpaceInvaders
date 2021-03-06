// Header
#include "Text.h"

// Get the character image.
u8* textImage(char character) {
	// Check for digits.
	if (character >= 48 && character <= 57) {
		return CharTextImages[character - 48];
	}

	// Check for uppercase letters.
	if (character >= 65 && character <= 90) {
		return CharTextImages[(character - 65) + 10];
	}

	// Check for lowercase letters.
	if (character >= 97 && character <= 122) {
		return CharTextImages[(character - 97) + 10];
	}

	// Return null pointer.
	return NULL;
}

// Paint the text string.
void paintText(Text *text, u8 *frame) {
	// Generate the frame address for the starting position.
	int addr = (text->xPos * 3) + (text->yPos * STRIDE);

	// Loop through the height and set data.
	for (int ycoi = 0; ycoi < CHAR_TEXT_HEIGHT; ycoi++) {
		// Loop through all the chars.
		int offset = 0;
		for (int i = 0; text->text[i] != '\0'; i++) {
			// Find image for current char.
			char character = text->text[i];
			u8 *charImage = textImage(character);

			// Copy current char image to row.
			if (character != ' ' && charImage != NULL) {
				memcpy(
					&frame[offset + addr],
					&charImage[ycoi * CHAR_TEXT_WIDTH * 3],
					CHAR_TEXT_WIDTH * 3
				);
			}

			// Add offset based on char width.
			offset += (CHAR_TEXT_WIDTH * 3);
		}

		// Jump to next line.
		addr += STRIDE;
	}
}

// Depaint the text string.
void depaintText(Text *text, u8 *frame) {
	// Generate the frame address for the starting position.
	int addr = (text->xPos * 3) + (text->yPos * STRIDE);

	// Loop through the height and set data.
	for (int ycoi = 0; ycoi < CHAR_TEXT_HEIGHT; ycoi++) {
		// Draw the background color for the calculated width.
		memset(&frame[addr], BACKGROUND, calculateTextWidth(text) * 3);

		// Jump to next line.
		addr += STRIDE;
	}
}

// Calculate the width for the text.
int calculateTextWidth(Text *text) {
	// Calculate text length.
	int i; for (i = 0; text->text[i] != '\0'; i++);

	// Return length timed with char width.
	return i * CHAR_TEXT_WIDTH;
}
