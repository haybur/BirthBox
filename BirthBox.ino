#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#include <Arduino.h>
//#include "Animations.h"
#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>
#include <Statistic.h>
#include <Button.h>
//CRGB leds[NUM_LEDS];
//#define switchPin 4
//#define brightnessPot A1
#define buttonPin 0
Button button(buttonPin);

// Pin layouts on the teensy 3:
// OctoWS2811: 2,14,7,8,6,20,21,5

#include <Audio.h>
//#include <Wire.h>
//#include <SPI.h>
//#include <SerialFlash.h>
// GUItool: begin automatically generated code
AudioInputI2S i2s1; //xy=219.1999969482422,183
AudioMixer4 mixer1; //xy=403.1999969482422,203.1999969482422
AudioAnalyzeFFT1024 fft1024; //xy=547.1999969482422,239
AudioConnection patchCord1(i2s1, 0, mixer1, 0);
AudioConnection patchCord2(i2s1, 1, mixer1, 1);
AudioConnection patchCord3(mixer1, fft1024);
AudioControlSGTL5000 audioShield; //xy=446.1999969482422,317
// GUItool: end automatically generated code
const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// These variables are required for audio detection, changing patterns based on music detection, and the visualizing patterns
	// These variables are needed by both MusicAnimationsBB.h and PatternListsBB.h
	// The variables used to play a function within the lists above
	uint8_t musicWithNoBeatPatternNumber = 0;
	uint8_t currentLowBeatPatternNumber = 0;
	uint8_t currentSpectrumPatternNumber = 0;
	uint8_t currentPatternNumber = 0;
	uint8_t currentAmbPatternNumber = 0;
	uint8_t musicWithConstBeatPatternNumber = 0;
	uint8_t allPatternsNumber = 0;

	const int numFFTBins = 14;
	int spectrumValue[numFFTBins];
	int spectrumValueOld[numFFTBins];

	double average[14];
	double stDev[14];
	long variance[14];
	int volume;
	int volumeAverage;

	Statistic freq[14];
	Statistic vol;

	uint8_t beatDetected[14] = {};
	elapsedMillis beatTimer[14] = {};
	int beatInterval[14];
	int beatIntervalOld[14];
	int constantBeatCounter[14];
	int beatPeak[14];
	int beatPeakOld[14];

	elapsedMillis silenceDuration;
	elapsedMillis songDuration;

	// Automation
	bool lowBeats = false;
	bool highBeats = false;
	bool midBeats = false;
	bool constBeats = false;
	bool silence = false;
	uint8_t lowBeatCounter = 0;
	uint8_t midBeatCounter = 0;
	uint8_t highBeatCounter = 0;
	uint8_t musicState = 0;
	uint8_t musicStateOld = 0;
	bool strongLow = false;
	bool strongMid = false;
	bool strongHigh = false;

// Mode Initiation Variables
bool initiate = true;
bool changingHue = false;
double fadeValue;

CRGBPalette16 gPal;
uint8_t brightness = 255;

elapsedMillis runTimer;

#define NUM_LEDS 1000
#define NUM_LEDS_PER_STRIP 200
#define ACT_LED_PER_STRIP 78
#define NUM_STRIPS 5
#define MILLI_AMPS 30000     // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
CHSV holder[NUM_STRIPS * NUM_LEDS_PER_STRIP];

TBlendType currentBlending = LINEARBLEND;
//CRGBPalette16 currentPalette(CRGB::Black);


int drips[4][10];

bool pressed;
int boxState = 2;
int buttonTimer;

#include "MapBB.h"
#include "DrawingBB.h"
#include "Ducenti.h"
#include "PatternListsBB.h"
#include "audioDetectionBB.h"
#include "MusicAnimationsBB.h"
#include "GradientPalettes.h"
#include "AutoPlay.h"

CRGBPalette16 currentPalette = Colorfull_gp;

#include "testingCode.h"

void setup() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			drips[i][j] = ACT_LED_PER_STRIP;
		}
	}

	button.begin();

	//pinMode(switchPin, INPUT_PULLUP);

	// Set input, aux or mic, and print to serial monitor
	audioSetup();

	Serial.begin(9600);
	LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);

	for (int i = 0; i < NUM_LEDS; i++) leds[i] = 0;

	runTimer = 0;
}

int earthVar;
uint8_t hue;
void loop() {
	LEDS.setBrightness(100);

	buttonFunctionality();

	switch (boxState) {
	case 1:
		BirthBoxShort();
		break;
	case 2:
		synesthesia();
		break;
	}

	//BirthBoxShort();
	
	//sun();
	//meltingIce();
	//growingGrass();
	//solarPanels();
	//colorWheel();
	//earthDucenti(true);
	//identifyPillars();
	//endSparkles();
	//forTesting();

	//hue++;

	//fillStats();
	//beatDetection(false);
	//lightning();
	//rain();
	//fadeRange(8, 0, 78);
	//explosions(2, 25, 53);
	//explosions(12, 103, 131);

	//holder[20] = CHSV(0, 0, 30);
	//holderToLEDs(255);
	//fadeRange(7, 0, 800);
	//plasma();


	//Serial.print("Runtime:   ");
	//Serial.println(holder[877].h);
	//Serial.println(runTimer);
	//FastLED.delay(30);
	//printSpectrum();
	//Serial.println(beatDetected[1]);

	LEDS.show();
}



void synesthesia() {
	fillStats();
	beatDetection(false);

	//ducentiSparkles();
	//synSparklesFading();
	//newChasing();
	//synSparklesNoFade();
	radialToBeat();
	highFreqDucentiSparkles();
	//chasing();
	//ducentiBlack();
}

void radialToBeat() {
	const uint8_t numOfBeatRadii = 10;
	static uint8_t beatRadiiHues[numOfBeatRadii];
	static uint8_t beatRadii[numOfBeatRadii];
	static int beatRadiiIndex;
	static uint8_t maxRadii = 160;
	static uint8_t lastHue;

	if (beatDetected[2] == 2) {  // if beat is detected
		for (int i = 0; i < numOfBeatRadii; i++) { // start a for loop to find first open value in beatRadii[]
			if (beatRadii[i] > maxRadii - 1 ) { // If it it has found a value that's above 200 (done)
				beatRadii[i] = 0;  // choose it and
				beatRadiiHues[i] = lastHue + random8(30, 150); // find a new hue
				lastHue = beatRadiiHues[i];
				break;				  // move on
			}

		}
	}

	for (int i = 0; i < numOfBeatRadii; i++) { // start a for loop to find what beatRadii are still growing
		if (beatRadii[i] < maxRadii) {              //
			uint8_t j = physicalToFibonacciOrder[beatRadii[i]];
			holder[800 + j] = CHSV(beatRadiiHues[i], random8(100, 255), 200 - beatRadii[i]);
		}
	}
	//Serial
	//Serial.println(beatRadii[0]);


	//EVERY_N_MILLIS(1) {
		for (int i = 0; i < numOfBeatRadii; i++) {
			if (beatRadii[i] < maxRadii)
				beatRadii[i]++;
		}
	//}

	holderToLEDsDucenti(255);
	fadeRange(2, 800, 1000);
}

void synSparklesFading() {
	maxNumOfSparkles = 10;
	brightness = 255;
	pillarSparkles(0, 312, 2, .25, -300, 0, false);
	pillarSparkles(0, 312, 6, .25, -300, 64, false);
	pillarSparkles(0, 312, 9, .25, -300, 96, false);
	pillarSparkles(0, 312, 12, .25, -300, 160, false);
	//pillarSparkles(0, 312, 14, .1, -100, 196, false
	fadeValue = constrain(fadeValue - 1, 1, 3);
	fadeRange(fadeValue, 0, 800);

	holderToLEDs(255);
}

void synSparklesNoFade() {
	maxNumOfSparkles = 25;
	brightness = 255;
	pillarSparkles(0, 312, 2, .2, -50, 0, false);
	pillarSparkles(0, 312, 6, .15, -50, 64, false);
	pillarSparkles(0, 312, 9, .15, -50, 96, false);
	pillarSparkles(0, 312, 12, .15, -50, 160, false);
	//pillarSparkles(0, 312, 14, .1, -100, 196, false
	fadeValue = constrain(fadeValue - 1, 0, 3);
	fadeRange(fadeValue, 0, 800);

	holderToLEDs(255);
}

void BirthBoxShort() {
	//Serial.println(gCurrentTrackNumber);

	gPlaylist[gCurrentTrackNumber].mPattern();

	{
		EVERY_N_MILLISECONDS_I(patternTimer, gPlaylist[gCurrentTrackNumber].mTime) {
			nextPattern();
			patternTimer.setPeriod(gPlaylist[gCurrentTrackNumber].mTime);
		}
		if (gRestartPlaylistFlag) {

			// Set the 'current pattern number' back to zero
			gCurrentTrackNumber = 0;

			// Set the playback duration for this patter to it's correct time
			patternTimer.setPeriod(gPlaylist[gCurrentTrackNumber].mTime);
			// Reset the pattern timer so that we start marking time from right now
			patternTimer.reset();

			// Finally, clear the gRestartPlaylistFlag flag
			gRestartPlaylistFlag = false;
		}
	}
	holderToLEDs(4);
	holderToLEDsDucenti(4);
}

uint8_t sparkleSat = 255;
int pillar;
int sparkles;
CHSV sparkleColor;
uint8_t sparkleHue;
int amountOfSparkles = 50; // was 30
int sparkleCounter;

void buttonFunctionality() {
	if (button.pressed()) {
		pressed = true;
	}
	if (pressed) buttonTimer++;
	if (button.released()) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < ACT_LED_PER_STRIP; j++) {
				holder[ACT_LED_PER_STRIP * i + j] = CHSV(0, 0, 0);
			}
		}
		if (buttonTimer > 100) {
			boxState = 2;
		}
		else {
			boxState = 1;
			reset();
		}
		pressed = false;
		buttonTimer = 0;
	}
}

void endSparkles() {
	//Serial.print(sparkleCounter);
	//Serial.print("\t");
	//Serial.println(amountOfSparkles);

	sparkleCounter++;

	EVERY_N_MILLIS(700) {
		amountOfSparkles = constrain(amountOfSparkles - 1, 0, 1000);
	}

	EVERY_N_MILLIS(60) {
		if (sparkleSat > 0)
			sparkleSat--;
		//else
		//	sparkleSat = 255;
	}	
	sparkleHue = sparkleHue + 3;

	if (sparkleCounter > amountOfSparkles) {
		sparkleColor = CHSV(sparkleHue, sparkleSat, 200);
		pillar = random8(5);
		sparkles = random8(ACT_LED_PER_STRIP);
		holder[ACT_LED_PER_STRIP * pillar + sparkles] = sparkleColor;
		sparkleCounter = 0;
	}

	for (int i = 0; i < NUM_STRIPS; i++) {
		for (int j = 0; j < ACT_LED_PER_STRIP; j++) {
			if (holder[ACT_LED_PER_STRIP * i + j].v > 0 && sparkleSat > 0)
				holder[ACT_LED_PER_STRIP * i + j].v--;
		}
	}
}

void holderToLEDsDucenti(int blend) {
	for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
		if (blend < 255)
			nblend(leds[800 + i], holder[800 + i], blend);
		else
			leds[800 + i] = holder[800 + i];
	}
}

void holderToLEDs(int blend) {
	for (int i = 0; i < 4; i++) {
		if (blend < 255) {
			//if (i != 4) {
				for (int j = 0; j < ACT_LED_PER_STRIP; j++) {
					nblend(leds[200 * i + j], holder[78 * i + j], blend);
				}
			//}
			//else {
			//	for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
			//		//leds[i * NUM_LEDS_PER_STRIP + j] = holder[i * NUM_LEDS_PER_STRIP + j];
			//		nblend(leds[i * NUM_LEDS_PER_STRIP + j], holder[i * NUM_LEDS_PER_STRIP + j], blend);
			//	}
			//}
		}
		else {
			for (int j = 0; j < ACT_LED_PER_STRIP; j++) {
				leds[i * NUM_LEDS_PER_STRIP + j] = holder[i * ACT_LED_PER_STRIP + j];
			}
		}
	}
}

void identifyPillars() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < ACT_LED_PER_STRIP; j++) {
			holder[200 * i + j] = CHSV(i*60, 255, 255);

			/*switch (i) {
			case 0:
				holder[200 * i + j] = CHSV(0, 255, 255);
				break;
			case 1:
				holder[200 * i + j] = CHSV(60, 255, 255);
				break;
			case 2:
				holder[200 * i + j] = CHSV(i * 60, 255, 255);
				break;
			case 3:
				holder[200 * i + j] = CHSV(i * 60, 255, 255);
				break;
			}*/


			//leds[200 * i + j] = CHSV(2, 255, 255);
			/*leds[200 * j + i] = CRGB::Red;
			leds[200 * j + i] = CRGB::Green;
			leds[200 * j + i] = CRGB::Blue;
			leds[200 * j + i] = CRGB::Cyan;*/
		}
	}
}

#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 160

bool gReverseDirection = false;

//void Fire2012()
//{
//	// Array of temperature readings at each simulation cell
//	static byte heat[ACT_LED_PER_STRIP * NUM_STRIPS];
//
//	// Step 1.  Cool down every cell a little
//	for (int i = 0; i < ACT_LED_PER_STRIP * NUM_STRIPS; i++) {
//		heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
//	}
//
//	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
//	for (int k = NUM_LEDS - 1; k >= 2; k--) {
//		double x = masterBrightness - 510;
//		x = x / -255;
//		heat[k] = ((heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3) / x;
//	}
//
//	// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
//	if (random8() < SPARKING) {
//		int y = random8(7);
//		heat[y] = qadd8(heat[y], random8(50, 120));
//	}
//
//	// Step 4.  Map from heat cells to LED colors
//	for (int j = 0; j < NUM_LEDS; j++) {
//		CRGB color = HeatColor(heat[j]);
//		int pixelnumber;
//		if (gReverseDirection) {
//			pixelnumber = (NUM_LEDS - 1) - j;
//		}
//		else {
//			pixelnumber = j;
//		}
//		leds[pixelnumber] = color;
//	}
//}

CHSV pillarColors[NUM_LEDS] = { CHSV(0, 0, 0) };
uint8_t counter = 0;
bool nextDripBin = true;

int heightOfIce;

void meltingIce() {
	//static int drips[4][10] = { ACT_LED_PER_STRIP };

	// Slowly change the height of the white portion with the blue tip
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < ACT_LED_PER_STRIP; j++) {
			if (j > heightOfIce)
				holder[i * ACT_LED_PER_STRIP + j] = CHSV(0, 0, 180);
			else if (j > heightOfIce - 2) // add the blue top
				holder[i * ACT_LED_PER_STRIP + j] = CHSV(160, 200, 255);
			else
				holder[i * ACT_LED_PER_STRIP + j] = CHSV(0, 0, 0);
		}
	}

	// Creates the drips randomly on every pillar
	EVERY_N_MILLIS(140) {
		heightOfIce++;
		for (int i = 0; i < 4; i++) {
			if (random8(5) < 3) {
				nextDripBin = true;
				for (int j = 0; j < 10; j++) {
					if (nextDripBin) {
						if (drips[i][j] == ACT_LED_PER_STRIP) {
							drips[i][j] = heightOfIce;
							nextDripBin = false;
						}
					}
					//if (i == 0) {
					//	Serial.print(drips[i][j]);
					//	Serial.print("\t");
					//	if (j == 9)
					//		Serial.println();
					//}
				}
			}
		}
	}

	

	EVERY_N_MILLIS(40) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 10; j++) {
				if (drips[i][j] < ACT_LED_PER_STRIP) {
					drips[i][j]++;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			if (drips[i][j] < ACT_LED_PER_STRIP) {
				holder[ACT_LED_PER_STRIP * i + drips[i][j] + 0] = CHSV(160, 255, 255);
				if (heightOfIce > 2) {
					holder[ACT_LED_PER_STRIP * i + drips[i][j] - 1] = CHSV(160, 255, 255);
					holder[ACT_LED_PER_STRIP * i + drips[i][j] - 2] = CHSV(160, 30, 200);
				}
			}
		}
	}
	//if (heightOfIce >= ACT_LED_PER_STRIP)
	//	heightOfIce = 0;
}

void meltingIceBackwards() {
	static int heightOfIce;
	static int drips[4][10];

	// Slowly change the height of the white portion with the blue tip
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < ACT_LED_PER_STRIP; j++) {
			if (j < heightOfIce)
				holder[i * ACT_LED_PER_STRIP + j] = CHSV(0, 0, 180);
			else if (j < heightOfIce + 2) // add the blue top
				holder[i * ACT_LED_PER_STRIP + j] = CHSV(160, 200, 255);
			else
				holder[i * ACT_LED_PER_STRIP + j] = CHSV(0, 0, 0);
		}
	}

	// Creates the drips randomly on every pillar
	EVERY_N_MILLIS(300) {
		heightOfIce--;
		for (int i = 0; i < 4; i++) {
			if (random8(5) < 3) {
				nextDripBin = true;
				for (int j = 0; j < 10; j++) {
					if (nextDripBin) {
						if (drips[i][j] == 0) {
							drips[i][j] = heightOfIce;
							nextDripBin = false;
						}
					}
				}
			}
		}
	}

	EVERY_N_MILLIS(50) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 10; j++) {
				if (drips[i][j] > 0) {
					drips[i][j]--;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			if (drips[i][j] > 0) {
				holder[ACT_LED_PER_STRIP * i + drips[i][j] + 0] = CHSV(160, 255, 255);
							if (heightOfIce < 75) {
								holder[ACT_LED_PER_STRIP * i + drips[i][j] + 1] = CHSV(160, 255, 255);
								holder[ACT_LED_PER_STRIP * i + drips[i][j] + 2] = CHSV(160, 30, 200);
							}
			}
			if (heightOfIce <= 0) heightOfIce = ACT_LED_PER_STRIP - 1;
		}
	}
}

int heightOfGrass = 0;
int heightOfSun = 0;
int heightOfDarkness;
int grassStage = 1;
uint8_t grassUpperBrightness;
uint8_t grassLowerBrightness;
uint8_t sunLength = 15;

void growingGrass() {
	
	switch (grassStage) {
	case 1: // Sun starts growing first
		pillarSun();
		if (heightOfSun > 5)
			grassStage = 2;
		grassUpperBrightness = 255;
		grassLowerBrightness = 120;
		break;
	case 2: // Sun continues, grass grows
		grassGrows();
		pillarSun();
		if (heightOfGrass > 40)
			grassStage = 3;
		break;
	case 3: // Sun stops growing, grass keeps growing
		grassGrows();
		pillarSun();
		break;
	//case 4:
	//	//grassGrows(); // to keep it sparkling
	//	//darknessGrows();
	//	pillarSun();
	//	fadeGrass();
	//	if (grassUpperBrightness == 0)
	//		grassStage = 5;
	//	break;
	//case 5:
	//	heightOfGrass = 0;
	//	heightOfSun = 0;
	//	heightOfDarkness = 0;
	//	grassStage = 1;
	//	break;
	}

	//holderToLEDs(2);
}

void darknessGrows() {
	EVERY_N_MILLIS(100) {
		heightOfDarkness++;
	}
	for (int i = 0; i < NUM_STRIPS; i++) {
		for (int j = 0; j < heightOfDarkness; j++)
			holder[ACT_LED_PER_STRIP * (i + 1) - j] = CHSV(0, 0, 0);
	}
}

void grassGrows() {
	EVERY_N_MILLIS(100) {
		if (heightOfGrass < 60)
			heightOfGrass++;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < heightOfGrass; j++)
				holder[ACT_LED_PER_STRIP * (i + 1) - j - 1] = CHSV(random8(86, 106), random8(180, 255), random8(grassLowerBrightness, grassUpperBrightness)); // -1 to keep it from showing up on wrong side
			}
		}
}

void fadeGrass() {
	if (grassLowerBrightness > 0)
		grassLowerBrightness--;
	if (grassUpperBrightness > 0)
		grassUpperBrightness--;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < heightOfGrass; j++)
			holder[ACT_LED_PER_STRIP * (i + 1) - j - 1] = CHSV(random8(86, 106), random8(180, 255), random8(grassLowerBrightness, grassLowerBrightness)); // -1 to keep it from showing up on wrong side
	}

}

void pillarSun() {
	EVERY_N_MILLIS(100) {
		//if (growing) {
			if (heightOfSun < sunLength)
				heightOfSun++;
		//}
		else {
			if (heightOfSun > 0)
				heightOfSun--;
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < heightOfSun; j++)
				holder[ACT_LED_PER_STRIP * i + j] = CHSV(random8(44, 64), random8(150, 250), random8(190, 255));
		}
	}
}

void resetGrass() {
	heightOfSun = 0;
	heightOfGrass = 0;
	grassStage = 1;
}

void growingGrassOnePillar() {
	if (heightOfDarkness == NUM_LEDS) {
		heightOfGrass = 0;
		heightOfSun = 0;
		heightOfDarkness = 0;
	}

	for (int i = 0; i < NUM_LEDS; i++)
		pillarColors[i] = CHSV(0, 0, 0);

	EVERY_N_MILLIS(200) {
		if (heightOfGrass < NUM_LEDS)
			heightOfGrass++;
		else
			heightOfDarkness = heightOfDarkness + 3;
	}

	EVERY_N_MILLIS(400) {
		if (heightOfGrass < 40)
			heightOfSun++;
		else
			heightOfSun--;
	}

	for (int i = 0; i < heightOfSun; i++) {
		//if (heightOfGrass < 30)
		pillarColors[NUM_LEDS - 1 - i] = CHSV(random8(60, 69), random8(150, 255), 255);
		//else 
		//	pillarColors[NUM_LEDS - (NUM_LEDS - heightOfGrass - 10)] = CHSV(random8(60, 69), random8(150, 255), 255);
	}

	for (int i = 0; i < heightOfGrass; i++) {
		pillarColors[i] = CHSV(random8(86, 106), random8(150, 255), 255);
	}

	for (int i = 0; i < heightOfDarkness; i++) {
		pillarColors[i] = CHSV(0, 0, 0);
	}

	for (int i = 0; i < NUM_LEDS; i++)
		nblend(leds[i], pillarColors[i], 20);
}

const int panelLength = 15;
uint8_t spBrightness = 0;
const int numOfElectrons = 3;
int electrons[numOfElectrons] = { 33, 49, 65 };

void solarPanels() {
	pillarSun();

	if (spBrightness < 255)
		spBrightness++;

	// Move electrons
	EVERY_N_MILLIS(30) {
		for (int i = 0; i < numOfElectrons; i++) {
			if (electrons[i] < ACT_LED_PER_STRIP - 1)
				electrons[i]++;
			else
				electrons[i] = 33;
		}
	}

	for (int i = 0; i < 4; i++) {
		// Turn everything below the panels black
		for (int j = 33; j < ACT_LED_PER_STRIP; j++) {
			holder[i * ACT_LED_PER_STRIP + j] = CHSV(0, 0, 0);
		}
		// Turn the panels blue
		for (int j = 0; j < panelLength; j++) {
			holder[i * ACT_LED_PER_STRIP + 18 + j] = CHSV(160, 255, spBrightness);
		}
		// Color electrons
		for (int j = 0; j < numOfElectrons; j++) {
			holder[i * ACT_LED_PER_STRIP + electrons[j]] = CHSV(0, 0, spBrightness);
			holder[i * ACT_LED_PER_STRIP + electrons[j] - 1] = CHSV(0, 0, spBrightness);
			holder[i * ACT_LED_PER_STRIP + electrons[j] - 2] = CHSV(0, 0, spBrightness);
		}
	}
}

void reset() {
	gRestartPlaylistFlag = true;
}

void initializeModes() {
	pillarBrightness = 110; // for beginning when it fades in

	// Melting Ice Init
	poleRadius = 45;
	heightOfIce = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			drips[i][j] = 0;
		}
	}

	// Growing Grass Init
	heightOfGrass = 0;
	heightOfSun = 0;
	heightOfDarkness;
	grassStage = 1;
	//sunLength = 15;

	// Solar Panel Init
	spBrightness = 0;
	//electrons[3] = { 33, 49, 65 };

	// Sparkles Init
	sparkleSat = 255;
	amountOfSparkles = 30;
}