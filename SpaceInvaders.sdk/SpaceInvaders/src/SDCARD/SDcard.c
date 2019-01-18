#include "SDcard.h"


/************ SD card parameters ************/
static FATFS FS;						// File System instance

//FRESULT result;									// FRESULT variable
//char FileName[32];								// File name
//static char *Log_File; 							// pointer to the log
//static const char *RootPath = "0:/";			//  string pointer to the logical drive number
unsigned int BytesWr;							// Bytes written
/************ SD card parameters ************/


PlayerEntity players[MAX_SCORES];


void insertScore(int newScore, char * name) {
	// Read Scores
	PlayerEntity * scores = readScores();
	#if debug
		for(int i = 0; i <= MAX_SCORES; i++) xil_printf("Orig: %d, %s , %d\n", i, scores[i].playerName, scores[i].playerScore);
	#endif

	// If new score is high enough add it to list
	if(scores[MAX_SCORES - 1].playerScore < newScore) {
		// Create new player list and populate with old players
		PlayerEntity scoreList[MAX_SCORES+1];
		for(int i = 0; i < MAX_SCORES; i++) {
				scoreList[i] = scores[i];
		}

		// Create new player entity and add to list
		PlayerEntity newPlayer;
		newPlayer.playerScore = newScore;
		strcpy(newPlayer.playerName, name);
		scoreList[MAX_SCORES] = newPlayer;

		#if debug
			for(int i = 0; i <= MAX_SCORES; i++) xil_printf("New: %d, %s , %d\n", i, scoreList[i].playerName, scoreList[i].playerScore);
		#endif

		// Sort scores and save in MAX_SCORES long array
		scores = bubbleSort(scoreList);

		#if debug
			for(int i = 0; i <= MAX_SCORES; i++) xil_printf("%d, %s , %d\n", i, scores[i].playerName, scores[i].playerScore);
		#endif

		// Save scores to SD
		//saveScores(scores);
	}

}

void saveScores(PlayerEntity *scoreList) {
	// Create char array which can contain all elements of the scoreList
//	int arraySize = 0;
//	int digitLengths[MAX_SCORES];
//
//	// Go through all players
//	for(int i = 0; i < MAX_SCORES; i++) {
//		// Find length of player name + separation char
//		digitLengths[i] = strlen(scoreList[i].playerName) + 1;
//
//		// Find length of score
//		if(scoreList[i].playerScore == 0) {
//			// add places for 0, \r and \n
//			digitLengths[i] += 3;
//		} else {
//			// add places for the digits and the \r \n
//			digitLengths[i] += (floor(log10(abs(scoreList[i].playerScore))) + 1) + 2;
//		}
//		arraySize += digitLengths[i];
//
//	}
//
//	// Convert scores to strings
//	// Go through all elements of char score list
//	for(int i = 0; i < )
//	for(int i = 0; i < arraySize; i++) {
//
//		// Loop through all numbers
//		for(int j = 0; j < MAX_SCORES; j++) {
//
//			// Loop once for each digit of number
//			int addition = scoreList[j];
//			int k;
//			for(k = digitLengths[j] - 2; k >= 0; k--) {
//				int temp = addition % 10;
//				addition /= 10;
//				i+j+k
//
//				//newScoreList[i+j] = scoreList[]
//			}
//		}
//
//	}

//	// Concat all strings
//	char *newScoreList;
//	newScoreList[0] = '\0';
//	for(int i = 0; i < MAX_SCORES; i++) {
//		//strcat(newScoreList,scoreList[i].playerScore);
//
//		strcat(newScoreList,scoreList[i].playerName);
//		char end[3] = {13, 10, 0};
//		strcat(newScoreList, end);
//	}
//
//	// Populate list //
//	sprintf(newScoreList, "%d\r\n", scoreList[0]);
//	for(int i = 1; i < MAX_SCORES; i++) {
//		sprintf(newScoreList, "%s%d\r\n", newScoreList, scoreList[i]);
//	}
//	if(debug) xil_printf("ScoreLIST!!!: \n%s", newScoreList);
//	return;


    // Initializes the SD Card
	initializeSD();
	if(debug) xil_printf("SD Initialized\n");

	// Mount root path, with forced imidiate mount
	f_mount(&FS,/*"0:/"*/"", 1);
	if(debug) xil_printf("FS Mounted\n");

	// File instance
	FIL file;
	if(debug) xil_printf("File opened\n");

	// Open file if exists, create if not
	FRESULT result = f_open(&file, "Scores.TXT", FA_CREATE_ALWAYS | FA_WRITE);
	// If file is non existing create it
	if (result) {
		if(debug) xil_printf("Returning ... \n");
	    f_close(&file);
		return;
	}


}

PlayerEntity * bubbleSort(PlayerEntity *array) {
	for(int i = 0; i <= (MAX_SCORES); i++) {
		for(int j = 0; j < (MAX_SCORES - i); j++) {
			if (array[j].playerScore < array [j+1].playerScore) {
				swap(&array[j], &array[j+1]);
				#if debug
					xil_printf("%s, %d swapped with %s, %d\n", array[j].playerName, array[j].playerScore, array[j].playerName, array[j].playerScore);
				#endif
			}
		}
	}
	return array;
}

void swap(PlayerEntity *a, PlayerEntity *b) {
	PlayerEntity temp = *a;
	*a = *b;
	*b = temp;
}

PlayerEntity * readScores() {
    // Initializes the SD Card
	initializeSD();
	if(debug) xil_printf("SD Initialized\n");

	// Mount root path, with forced imidiate mount
	f_mount(&FS,/*"0:/"*/"", 1);
	if(debug) xil_printf("FS Mounted\n");

	// File instance
	FIL file;
	if(debug) xil_printf("File opened\n");

	// Open file if exists, create if not
	FRESULT result = f_open(&file, "Scores.TXT", FA_OPEN_ALWAYS | FA_READ);
	// If file is non existing create it
	if (result) {
		if(debug) xil_printf("Returning ... \n");
	    f_close(&file);
		return NULL;
	}
	if(debug) xil_printf("File found\n");

	// Buffer
	char buffer[SCORE_LIMIT*MAX_SCORES];
	if(debug) xil_printf("Scores Initialized (%d indices)\n", sizeof buffer);

	// Clear playerScores and create counter
	for(int i = 0; i < MAX_SCORES; i++) {
		players[i].playerScore = 0;
		strcpy(players[i].playerName, "");
	}
	int e = 0;
	// Read line
	while(f_gets(buffer, sizeof buffer, &file) ) {
		if(debug) xil_printf("\nScore (%d): %s", strlen(buffer), buffer);
		if(debug) xil_printf("%d gets: ",e);
		// instansiate variables for read mode (numbers/names) and indice in names
		int seperation = 0, n = 0;

		// Loop through the read buffer
		for(int i = 0; i <= sizeof buffer; i++) {
			// If \r \n \0 is found break out - and reset seperation var
			if( ((buffer[i] == 13) && (buffer[i+1] == 10) && (buffer[i+2] == 0)) || (n >= PLAYERNAME_LENGTH)) {
				seperation = 0;
				break;
			}

			// If score/name seperater is found set mode and continue
			if( buffer[i] == ';') {
				seperation = 1;
				continue;
			}

			// Seperation 0 = getting numbers
			if(seperation == 0) {
				// Read char as int
				int temp = charToInt(buffer[i]);
				if(debug) xil_printf("%d=%d, ",buffer[i], temp);
				// Multiply by 10 and add temp (current char in buffer)
				players[e].playerScore = temp == -1 ? players[e].playerScore : (players[e].playerScore << 3) + (players[e].playerScore << 1) + temp;
			} else { // Seperation = 1, get remaining non termination chars and save
				players[e].playerName[n] = buffer[i];
				n++;
			}
		}
        if(debug) xil_printf("\nPlayer: %s, Int: %d\n",players[e].playerName , players[e].playerScore);
        e++;
    }

    // Close the file and SD platform
    f_close(&file);
    cleanup_platform();
	if(debug) xil_printf("File Closed\n");
	return players;
}

int charToInt(char c) {
	int temp = -1;
	if( (c > 47)&&(c < 58) ) temp = c - 48;
	return temp;
}
