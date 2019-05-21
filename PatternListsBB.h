//#pragma once

// These are lists of functions that will be displayed during different types of music / no music.
// No music is playing, so display an ambient function
typedef void(*ambientPatternList[])();
ambientPatternList ambientPatterns = {
	colorWheel,
};

typedef void(*musicWithNoBeatPatternsList[])();
musicWithNoBeatPatternsList musicWithNoBeatPatterns = {
	colorWheel,
};
// Both high frequency beats and low frequency beats are being detected
typedef void(*spectrumPatternList[])();
spectrumPatternList spectrumPatterns = {
	colorWheel,
};
// Only beats in the low frequency are being detected
typedef void(*lowBeatPatternList[])();
lowBeatPatternList lowBeatPatterns = {
	colorWheel,
};

typedef void(*constBeatPatternList[])();
constBeatPatternList constBeatPatterns = {
	colorWheel,
};

typedef void(*allPatternsList[])();
allPatternsList allPatterns = {
	colorWheel,
};

void nextAllPatterns() {
	allPatternsNumber++;
	if (allPatternsNumber >= ARRAY_SIZE(allPatterns))
		allPatternsNumber = 0;
}

// These functions choose a random pattern within the lists above
void nextMusicWithConstBeatPattern() {
	// add one to the current pattern number, and wrap around at the end
	//musicWithNoBeatPatternNumber = (musicWithNoBeatPatternNumber + 1) % ARRAY_SIZE(musicWithNoBeatPatterns);
	musicWithConstBeatPatternNumber = random8(ARRAY_SIZE(constBeatPatterns));
}

void nextMusicWithNoBeatPattern() {
	// add one to the current pattern number, and wrap around at the end
	//musicWithNoBeatPatternNumber = (musicWithNoBeatPatternNumber + 1) % ARRAY_SIZE(musicWithNoBeatPatterns);
	musicWithNoBeatPatternNumber = random8(ARRAY_SIZE(musicWithNoBeatPatterns));
}

void nextAmbPattern() {
	//currentAmbPatternNumber = (currentAmbPatternNumber + 1) % ARRAY_SIZE(ambientPatterns);
	currentAmbPatternNumber = random8(ARRAY_SIZE(ambientPatterns));
}

void nextLowBeatPattern() {
	//currentLowBeatPatternNumber = (currentLowBeatPatternNumber + 1) % ARRAY_SIZE(lowBeatPatterns);
	currentLowBeatPatternNumber = random8(ARRAY_SIZE(lowBeatPatterns));
}

void nextSpectrumPattern() {
	//currentSpectrumPatternNumber = (currentSpectrumPatternNumber + 1) % ARRAY_SIZE(spectrumPatterns);
	currentSpectrumPatternNumber = random8(ARRAY_SIZE(spectrumPatterns));
}