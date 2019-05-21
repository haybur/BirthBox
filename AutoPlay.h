#pragma once

typedef void(*SimplePattern)();
typedef SimplePattern SimplePatternList[];
typedef struct { SimplePattern mPattern;  uint16_t mTime; } PatternAndTime;
typedef PatternAndTime PatternAndTimeList[];

bool gLoopPlaylist = false;

uint8_t gCurrentTrackNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

bool gRestartPlaylistFlag = false;

uint8_t pillarBrightness;
int poleRadius = 45;

elapsedMillis earthDelay;

void initialize() {
	initializeModes();
	pillarBrightness = 110;
	//heightOfIce = 0;
	Serial.println("initialization");
	//gCurrentTrackNumber = 1;
}

void intro() { // not spinng earth, white pillars
	earthDucenti(false, 170, poleRadius);
	// White pillars
	EVERY_N_MILLIS(50)
		pillarBrightness = qadd8(pillarBrightness, 1);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < ACT_LED_PER_STRIP; j++) {
			holder[ACT_LED_PER_STRIP * i + j] = CHSV(0, 0, pillarBrightness);
		}
	}
}

void seq1() { // Ice melts on the pillars and Ducenti poles
	EVERY_N_MILLIS(250)
		if (poleRadius > 14)
			poleRadius--;
	earthDucenti(false, 130, poleRadius);
	meltingIce();
}

void seq2() { // Upbeat now, sun shines and grass grows
	sun();
	growingGrass();
}

uint8_t earthBrightness;

void seq3() { // Solar panels generating power under the sun
	sun();
	solarPanels();
	earthBrightness = 100;
}

void seq4() { // sparkles at the end with the Earth fading in
	if (earthDelay > 3000) {
		EVERY_N_MILLIS(100) {
			if (earthBrightness < 255)
				earthBrightness++;
		}
	}
	endSparkles();

	earthDucenti(true, earthBrightness, 14);

}

void end() {
	earthDucenti(false, 130, 45);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < ACT_LED_PER_STRIP; j++)
			holder[ACT_LED_PER_STRIP * i + j] = CHSV(0, 0, 0);
	}
	earthDelay = 0;
}

void allBlack() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < ACT_LED_PER_STRIP; j++) {
			holder[ACT_LED_PER_STRIP * i + j] = CHSV(0, 0, 0);
		}
	}
	ducentiBlack();
	resetGrass();
	earthDelay = 0;
}

const PatternAndTimeList gPlaylist = {
	{ initialize,					10  },
	{ intro,						8000  },
	{ seq1,							12000 },
	{ seq2,							16000 }, // was 18
	{ seq3,							8000 }, // was 6
	{ allBlack,						2000  },
	{ seq4,							35000 },
	{ end,							1 }
};

void nextPattern()
{
	// add one to the current pattern number
	gCurrentTrackNumber = gCurrentTrackNumber + 1;

	// If we've come to the end of the playlist, we can either 
	// automatically restart it at the beginning, or just stay at the end.
	if (gCurrentTrackNumber == ARRAY_SIZE(gPlaylist)) {
		if (gLoopPlaylist == true) {
			// restart at beginning
			gCurrentTrackNumber = 0;
		}
		else {
			// stay on the last track
			gCurrentTrackNumber--;
		}
	}
}