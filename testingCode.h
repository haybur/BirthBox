#pragma once

void plasma() {
	// Persistent local variables
	static int16_t thisphase = 0;                                                 // Phase of a cubicwave8.
	static int16_t thatphase = 0;                                                 // Phase of the cos8.

	// Temporary local variables
	uint16_t thisbright;
	uint16_t colorIndex;

	thisphase += beatsin8(6, -4, 4);                                                // You can change direction and speed individually.
	thatphase += beatsin8(7, -4, 4);                                                // Two phase values to make a complex pattern. By Andrew Tuline.

	for (int k = 0; k < 78; k++) {                                              // For each of the LED's in the strand, set a brightness based on a wave as follows.
		thisbright = cubicwave8((k * 8) + thisphase) / 2;
		thisbright += cos8((k * 10) + thatphase) / 2;                                     // Let's munge the brightness a bit and animate it all with the phases.
		colorIndex = thisbright;
		thisbright = constrain(thisbright - (255 - average[2] * 2), 0, 255);             // qsuba chops off values below a threshold defined by sampleavg. Gives a cool effect.
		//leds[k] = ColorFromPalette(currentPalette, colorIndex, thisbright, currentBlending);   // Let's now add the foreground colour.
		leds[k] = CHSV(colorIndex, 255, thisbright);
	}
} // plasma()

void noisepal() {                                                              // Create fire based on noise and sampleavg. 

// Local definitions
#define xscale 20                                                             // How far apart they are
#define yscale 3                                                              // How fast they move

// Persistent local variables

// Temporary local variables
	static uint16_t dist;                                                     // A random number for our noise generator.


	for (int i = 0; i < 78; i++) {                                       // Just ONE loop to fill up the LED array as all of the pixels change.
		uint8_t index = inoise8(i*xscale, dist + i * yscale);                         // Get a value from the noise function. I'm using both x and y axis.
		//leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);    // With that value, look up the 8 bit colour palette value and assign it to the current LED.
		leds[i] = CHSV(index, 255, 255);
	}
	dist += beatsin8(10, 1, 4);                                                // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
																			  // In some sketches, I've used millis() instead of an incremented counter. Works a treat.
} // noisepal()


void rain() {                                                  // A 'Matrix' like display using sampleavg for brightness.
	static int rainCounter = 0;

	if (rainCounter == 10) {
		for (int i = 0; i < 4; i++)
			holder[ACT_LED_PER_STRIP * i] = CHSV(160, random8(100, 220), 150);

		rainCounter = 0;
	}
	else {
		for (int i = 0; i < 4; i++)
			holder[ACT_LED_PER_STRIP * i] = CHSV(0, 0, 0);
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < ACT_LED_PER_STRIP; j++)
			holder[ACT_LED_PER_STRIP * i + j] = holder[ACT_LED_PER_STRIP * i + j - 1];
	}

	leds[20] = CRGB::Blue;

	rainCounter++;

	delay(10);
} // matrix()

void lightning() {
	static int dimmer = 1;

	for (int flashCounter = 0; flashCounter < random8(3, 8); flashCounter++) {
		if (flashCounter == 0) dimmer = 5;                         // the brightness of the leader is scaled down by a factor of 5
		else dimmer = random8(1, 3);                               // return strokes are brighter than the leader

		fill_solid(holder, 78, CHSV(255, 0, 100 / dimmer));
		FastLED.show();                       // Show a section of LED's
		delay(random8(4, 10));                                     // each flash only lasts 4-10 milliseconds
		fill_solid(holder, 78, CHSV(0, 0, 0));           // Clear the section of LED's
		FastLED.show();

		if (flashCounter == 0) delay(150);                       // longer delay until next flash after the leader

		delay(50 + random8(100));                                   // shorter delay between strokes  
	} // for()
	delay(2000);
}