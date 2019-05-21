#pragma once

void colorWheel() {
	for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
		uint8_t j = physicalToFibonacciOrder[i];
		uint8_t angle = ledAngles[j];
		//if (angle > 0 && angle < 255)
			holder[800 + i] = CHSV(angle, 255, 255);
	}
}

void earthDucenti(bool spinning, int brightness, int poleRadius) {

	static uint8_t offset = 0;

	for (uint16_t i = 0; i < NUM_LEDS_PER_STRIP; i++) {

		holder[800 + i] = CHSV(0, 0, 0);

		uint8_t j = physicalToFibonacciOrder[i];
		uint8_t angle = ledAngles[j] + offset;

		// Africa / Europe/ asia
		if (angle < 130 && angle > 70 && ledRadii[j] > 30)
			holder[800 + i] = CHSV(1, 255, brightness);
		if (angle < 90 && angle  > 70 && ledRadii[j] > 160)
			holder[800 + i].h = 0;
		if (angle < 130 && angle  > 100 && ledRadii[j] < 60)
			holder[800 + i].h = 0;

		// Japan?
		if (angle > 140 && angle < 160 && ledRadii[j] > 80)
			holder[800 + i] = CHSV(1, 255, brightness);
		
		// Pacific Ocean
		if (angle > 175 && angle < 205 && ledRadii[i] > 17) {
			holder[800 + i] = CHSV(160, 255, brightness);
			//holder[800 + i] = CHSV(160 - 60*(abs(angle - 195) / 195), 255, 220);
		}

		// Atlantic Ocean
		if (angle > 25 && angle < 55 && ledRadii[i] > 17) {
			holder[800 + i] = CHSV(160, 255, brightness);
		}

		// The Americas
		if (angle > 230 && angle <= 255 && ledRadii[j] > 65)
			holder[800 + i] = CHSV(1, 255, brightness);
		if (angle < 20 && angle >= 0 && ledRadii[j] > 20)
			holder[800 + i] = CHSV(1, 255, brightness);

		// Antarctica
		if (ledRadii[j] < poleRadius)
			holder[800 + i] = CHSV(160, 10, brightness);

		// The Oceans
		if (holder[800 + i].h == 0)
			holder[800 + i] = CHSV(160 + ledRadii[j] / 10, 200, brightness);

		//nblend(leds[i], holder[800 + i], 35);
	}

	if (spinning) {
		EVERY_N_MILLIS(50)
			offset++;
	}
}

// based on FastLED example Fire2012WithPalette: https://github.com/FastLED/FastLED/blob/master/examples/Fire2012WithPalette/Fire2012WithPalette.ino
void sun()
{
	EVERY_N_MILLIS(130)
	{
	/*	for (int i = 0; i < NUM_LEDS_PER_STRIP; i++)
			leds[800 + i] = CRGB::Black;*/
		//fill_solid(leds, NUM_LEDS, CRGB::Black);

		// Add entropy to random number generator; we use a lot of it.
		random16_add_entropy(random(256));

		// COOLING: How much does the air cool as it rises?
		// Less cooling = taller flames.  More cooling = shorter flames.
		// Default 55, suggested range 20-100
		uint8_t cooling = 1;

		// SPARKING: What chance (out of 255) is there that a new spark will be lit?
		// Higher chance = more roaring fire.  Lower chance = more flickery fire.
		// Default 120, suggested range 50-200.
		uint8_t sparking = 3;

		// Array of temperature readings at each simulation cell
		//static byte heat[kMatrixWidth + 3][kMatrixHeight + 3];
		CHSV heat[kMatrixWidth][kMatrixHeight];

		for (int x = 0; x < 10; x++)
		{
			// Step 1.  Cool down every cell a little
			for (int y = 0; y < 10; y++)
			{
				heat[x][y].hue = random(70, 94);
				heat[x][y].v = 255;
				heat[x][y].sat = qadd8(heat[x][y].sat, random8(0, cooling));
			}

			// Step 2.  Heat from each cell diffuses a little
			for (int y = 0; y < kMatrixHeight; y++)
			{
				heat[x][y].sat = qadd8((heat[x + 1][y + 1].sat + heat[x - 1][y - 1].sat + heat[x - 1][y + 1].sat + heat[x + 1][y - 1].sat)/4, heat[x][y].sat);
			}

			// Step 2.  Randomly ignite new 'sparks' of heat
			for (int y = 0; y < 10; y++) {
				if (random8() < sparking)
				{
					heat[x][y].sat = qsub8(heat[x][y].sat, random8(0, 250));
					//heat[x][y].sat = qsub8(random8(0, 20), heat[x][y].sat);
				}
			}
			// Step 4.  Map from heat cells to LED colors
			for (int y = 0; y < kMatrixHeight; y++)
					setPixelXY10(x, y, heat[x][y]);
	
		}
	}
}

void ducentiBlack() {
	for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
		holder[800 + i] = CHSV(0, 0, 0);
		leds[800 + i] = CHSV(0, 0, 0);
	}
}