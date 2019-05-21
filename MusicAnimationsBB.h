////// Animations.h

//void fadeall(int fadevar) {
//	for (int i = 0; i < NUM_STRIPS; i++) {
//		for (int j = 0; j < ACT_LED_PER_STRIP; j++) {
//			if (holder[ACT_LED_PER_STRIP * i + j].v > 0 && sparkleSat > 0)
//				holder[ACT_LED_PER_STRIP * i + j].v--;
//		}
//	}
//}

int sparkleNumber;
//uint8_t sparkleHue;
int freqfreq;
int maxNumOfSparkles = 25;
int ducentiSparkleHue;

void pillarSparkles(int sparkStart, int sparkEnd, uint8_t freq, double multiplierAdjustment, int additionAdjustment, uint8_t hue, bool white) {
	//static uint8_t sparkleSat = (white) ? 255 : 0;
	static int sparkleSat;
	if (white) sparkleSat = 0;
	else sparkleSat = 255;

	sparkleNumber = spectrumValue[freq] + additionAdjustment;
	sparkleNumber = constrain(sparkleNumber, 0, 500);
	sparkleNumber = map(sparkleNumber, 0, 500, 0, maxNumOfSparkles);
	sparkleNumber = sparkleNumber * multiplierAdjustment;
		
	for (int i = 0; i < sparkleNumber; i++) {
		if (sparkStart >= 800) 
			holder[random16(sparkStart, sparkEnd)] = CHSV(hue, sparkleSat, 100);
		else
			holder[random16(sparkStart, sparkEnd)] = CHSV(hue, sparkleSat, brightness);
	}

	fadeValue = fadeValue + sparkleNumber * .5;
}

void highFreqDucentiSparkles() {
	pillarSparkles(940, 1000, 12, .2, -50, 0, true);
}

void ducentiSparkles() {
	// For the Ducenti
	pillarSparkles(800, 1000, 2, .1, -50, 96, false);
	pillarSparkles(800, 1000, 6, .1, -50, 64, false);
	pillarSparkles(800, 1000, 9, .1, -100, 0, false);
	pillarSparkles(800, 1000, 12, .1, -50, 160, false);
	//pillarSparkles(800, 1000, 14, .2, -50, 196, false);
	holderToLEDsDucenti(255);
	fadeValue = constrain(fadeValue - 1, 1, 3);
	fadeRange(fadeValue, 800, 1000);
}

const int numOfChasers = 20;
int chaserPos[numOfChasers];
int chaserHue[numOfChasers];
int chaserPillar[numOfChasers];
int assignPillar;
double speedLows = 15;
double speedMids = 15;
double speedHighs = 15;
elapsedMillis timerLows;
elapsedMillis timerMids;
elapsedMillis timerHighs;


void newChasing() {
	// Assign new chasers
	if (beatDetected[2] == 2) {  // if beat is detected
		speedLows -= .85*speedLows;
		for (int i = 0; i < numOfChasers; i++) { // start a for loop to find first open value in beatRadii[]
			if (chaserPos[i] == 0) { // If it it has found a value that's above 200 (done)
				chaserPos[i] = 78;  // choose it and
				chaserHue[i] = random8(25);
				if (assignPillar < 3) assignPillar++;
				else assignPillar = 0;
				chaserPillar[i] = assignPillar;
				//chaserPillar[i] = random(4);
				break;				  // move on
			}
		}
	}
	if (beatDetected[8] == 2) {  // if beat is detected
		speedMids -= .85*speedMids;
		for (int i = 0; i < numOfChasers; i++) { // start a for loop to find first open value in beatRadii[]
			if (chaserPos[i] == 0) { // If it it has found a value that's above 200 (done)
				chaserPos[i] = 78;  // choose it and
				chaserHue[i] = random8(130, 190);
				if (assignPillar < 3) assignPillar++;
				else assignPillar = 0;
				chaserPillar[i] = assignPillar;
				//chaserPillar[i] = random(4);
				break;				  // move on
			}
		}
	}
	if (beatDetected[12] == 2) {  // if beat is detected
		speedHighs -= .85*speedHighs;
		for (int i = 0; i < numOfChasers; i++) { // start a for loop to find first open value in beatRadii[]
			if (chaserPos[i] == 0) { // If it it has found a value that's above 200 (done)
				chaserPos[i] = 78;  // choose it and
				chaserHue[i] = random8(66, 126);
				if (assignPillar < 3) assignPillar++;
				else assignPillar = 0;
				chaserPillar[i] = assignPillar;
				//chaserPillar[i] = random8(4);
				break;				  // move on
			}
		}
	}

	//Move all chasers
	if (timerLows > speedLows) {
		for (int i = 0; i < numOfChasers; i++) {
			if (chaserPos[i] > 0) {
				if (chaserHue[i] > 0 && chaserHue[i] < 26)
					chaserPos[i]--;
			}
		}
		timerLows = 0;
	}
	if (timerMids > speedMids) {
		for (int i = 0; i < numOfChasers; i++) {
			if (chaserPos[i] > 0) {
				if (chaserHue[i] > 129 && chaserHue[i] < 191)
					chaserPos[i]--;
			}
		}
		timerMids = 0;
	}
	if (timerHighs > speedHighs) {
		for (int i = 0; i < numOfChasers; i++) {
			if (chaserPos[i] > 0) {
				if (chaserHue[i] > 65 && chaserHue[i] < 127)
					chaserPos[i]--;
			}
		}
		timerHighs = 0;
	}

	// Color all chasers
	for (int i = 0; i < numOfChasers; i++) {
		//Serial.print(chaserPos[i]);
		//Serial.print("  ");
		if (chaserPos[i] > 1) {
			holder[ACT_LED_PER_STRIP * (chaserPillar[i] + 1) - chaserPos[i]] = CHSV(chaserHue[i], 255, 255);
		}
	}
	//Serial.println();

	//Serial.print(speedLows);
	//Serial.print("\t");
	//Serial.print(speedMids);
	//Serial.print("\t");
	//Serial.println(speedHighs);

	speedLows = constrain(speedLows + .1 + .01*speedLows, 1, 35);
	//speedLows = constrain(speedLows + .3 + .01*speedLows, 15, 200);
	speedMids = constrain(speedMids + .1 + .01*speedMids, 1, 35);
	speedHighs = constrain(speedHighs + .1 + .01*speedHighs, 1, 35);

	holderToLEDs(255);
	fadeRange(2, 0, 800);
}


int chasersr[20] = { 78 };
int chasersg[20] = { 78 };
int chasersb[20] = { 78 };

void chasing() {

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 20; i++) {
			holder[NUM_LEDS_PER_STRIP * j + chasersr[i]] = CHSV(0, 255, 255);
			holder[NUM_LEDS_PER_STRIP * j + chasersg[i]] = CHSV(96, 255, 255);
			holder[NUM_LEDS_PER_STRIP * j + chasersb[i]] = CHSV(160, 255, 255);
		}
	}

	EVERY_N_MILLIS(10) {
		for (int i = 0; i < 20; i++) {
			if (chasersr[i] < 78) 
				chasersr[i]++;
			if (chasersb[i] < 78)
				chasersb[i]++;
			if (chasersg[i] < 78)
				chasersg[i]++;
		}
	}

	if (beatDetected[2] == 2) {
		for (int i = 0; i < 20; i++) {
			if (chasersr[i] > 77) {
				chasersr[i] = 0;
				break;
			}
		}
	}
	if (beatDetected[8] == 2) {
		for (int i = 0; i < 20; i++) {
			if (chasersb[i] > 77) {
				chasersb[i] = 0;
				break;
			}
		}
	}
	if (beatDetected[12] == 2) {
		for (int i = 0; i < 20; i++) {
			if (chasersg[i] > 77) {
				chasersg[i] = 0;
				break;
			}
		}
	}
	holderToLEDs(255);
	fadeRange(10, 0, 800);
	//Serial.println(chasersb[0]);
}

void explosions(int freq, int begin, int end) {                                                                 // Display ripples triggered by peaks.
	static const int maxsteps = 25;                                               // Maximum number of steps.
	static uint8_t colour;                                                        // Ripple colour is based on samples.
	static uint16_t center = 0;                                                   // Center of current ripple.
	static int8_t step = -1;                                                      // Phase of the ripple as it spreads out.

	if (beatDetected[freq] == 2)
		step = -1;														// Trigger a new ripple if we have a peak

	//fadeToBlackBy(leds, begin, end, 7);                                            // Fade the strand, where 1 = slow, 255 = fast
	//fadeRange(7, begin, end);

	switch (step) {

	case -1:                                                                    // Initialize ripple variables. By Andrew Tuline.
		center = random16(begin, end);
		colour = (freq - 2) * 17;
		step = 0;
		break;

	case 0:																		// Display the first pixel of the ripple.
		holder[center] = CHSV(colour, 125, 255);
		step++;
		break;

	case maxsteps:                                                              // At the end of the ripples.
	  // step = -1;
		break;

	default:                                                                    // Middle of the ripples.
		holder[center + step] = CHSV(colour + step * 2, 255, 255 - step * 6);
		holder[center - step] = CHSV(colour + step * 2, 255, 255 - step * 6);
		step++;                                                                  // Next step.
		break;
	} // switch step
} // ripple()

//int maxNumOfSparkles = 25;
//int sparkleNumber;
//void addSparkles(int sparkStart, int sparkEnd, uint8_t freq, double multiplierAdjustment, int additionAdjustment, int hue, bool white) {
//	// If sparkles are being added to react to music, do this:
//
//	// If the freq argument is a valid FFT bin, 0 - 15
//	if (freq > 0 && freq < 15) {
//		sparkleNumber = spectrumValue[freq] + additionAdjustment;
//		sparkleNumber = constrain(sparkleNumber, 0, 500);
//		sparkleNumber = map(sparkleNumber, 0, 500, 0, maxNumOfSparkles);
//		sparkleNumber = sparkleNumber * multiplierAdjustment;
//
//		for (int i = 0; i < sparkleNumber; i++) {
//			//leds[getRandomPixel(sparkStart, sparkEnd)] = sparkleColor;
//			int pos = getRandomPixel(sparkStart, sparkEnd);
//			testVar = map(ledx[pos] - ledy[pos], -31, 31, 0, 255);
//			leds[pos] = ColorFromPalette(currentPalette, testVar+hue1, brightness, LINEARBLEND);
//			if (white) leds[pos] = CHSV(0, 0, brightness);
//			fadeValue = fadeValue + sparkleNumber*.5;
//		}
//	}
//	// Else, if sparkles are being added as an ambient effect, do this:
//	else {
//		EVERY_N_MILLISECONDS(additionAdjustment) {
//			int pos = getRandomPixel(sparkStart, sparkEnd);
//			leds[pos] = CHSV(hue, 255, brightness);
//			if (white) leds[pos] = CHSV(0, 0, brightness);
//		}
//	}
//}






//
//// Six zones of glitter reacting to only 3 frequencies. 
//void sixFreqGlitter() {
//	if (initiate) {
//		brightness = 255;
//		//currentPalette = justWhite_p;
//		currentPalette = gGradientPalettes[random8(gGradientPaletteCount)];
//		changingHue = random8(2);
//		initiate = false;
//	}
//
//	if (changingHue)
//		hue1++;
//
//	addSparkles(cor9, cor13, 12, .15, -50, 0, false);
//	//brightness = spectrumValue[8] * 2;
//	addSparkles(cor1, cor9, 8, .15, -50, 64, false);
//	//brightness = spectrumValue[2] * 2;
//	addSparkles(cor13, cor15, 2, .2, -50, 128, false);
//	addSparkles(cor27, cor28, 2, .2, -50, 128, false);
//	addSparkles(cor0,   cor1, 2, .2, -50, 128, false);
//	//brightness = spectrumValue[8] * 2;
//	addSparkles(cor19, cor27, 8, .15, -50, 64, false);
//	//brightness = spectrumValue[12] * 2;
//	addSparkles(cor15, cor19, 12, .15, -50, 0, false);
//
//	fadeAll(254 - fadeValue);
//	fadeValue = constrain(fadeValue - 1.5, 0, 15);
//}
//
//// Same thing but this establishes a color palette
//void sixFreqGlitter_usa() {
//	if (initiate) {
//		brightness = 255;
//		currentPalette = usa_p;
//		changingHue = false;
//		initiate = false;
//	}
//	sixFreqGlitter();
//}
//
//// Ambient Confetti over the entire strip.
//void ambientConfetti() {
//	if (initiate) {
//		changingHue = true;
//		initiate = false;
//	}
//
//	addSparkles(cor0, cor28, 25, 1.9, 100, hue, false);
//
//	fadeAll(250);
//}
//
//// Put in the direction as an input and this will return the slope and starting y-int of the line that is sweeping across design
//void sweeps(int coordMode) {
//	switch (coordMode) {
//	case 1:                ////// SWEEPING UP TO THE RIGHT
//		slope = -1;
//		yint = ledy[cor0] + ledx[cor0] + var;
//		delay(16);
//		if (yint > (ledy[cor14] + ledx[cor14])) sweepStatus = false;
//		else sweepStatus = true;
//		break;
//	case 2:              ////// SWEEPING DOWN TO THE LEFT
//		slope = -1;
//		yint = ledy[cor14] + ledx[cor14] - var;
//		delay(16);
//		if (yint < (ledy[cor0] + ledx[cor0])) sweepStatus = false;
//		else sweepStatus = true;
//		break;
//	case 3:              ////// SWEEPING UP FROM THE BOTTOM
//		slope = 0;
//		yint = ledy[cor10] + var;
//		delay(16);            /// Delay to make it take about the same time to sweep up/down as it takes to go diagonal
//		if (yint > ledy[cor16]) sweepStatus = false;
//		else sweepStatus = true;
//		break;
//	case 4:              ////// SWEEPING DOWN FROM THE TOP
//		slope = 0;
//		yint = ledy[cor16] - var;
//		delay(16);  // OLD DELAY VALUE IS 8, FOR THE TEENSY 3.2
//		if (yint < ledy[cor10]) sweepStatus = false;
//		else sweepStatus = true;
//		break;
//	case 5:              ////// SWEEPING LEFT TO RIGHT
//		slope = 0;
//		yint = ledx[cor17] + var;
//		delay(16);
//		if (yint > ledx[cor11]) sweepStatus = false;
//		else sweepStatus = true;
//		break;
//	case 6:       ////// SWEEPING RIGHT TO LEFT
//		slope = 0;
//		yint = ledx[cor11] - var;
//		delay(16);
//		if (yint < ledx[cor17]) sweepStatus = false;
//		else sweepStatus = true;
//		break;
//	case 7:     ////// SWEEPING UP TO THE LEFT
//		slope = 1;
//		yint = ledy[cor10] - ledx[cor11] + var;
//		if (yint > (ledy[cor17] - ledx[cor17])) sweepStatus = false;
//		else sweepStatus = true;
//		break;
//	case 8:     ////// SWEEPING DOWN TO THE RIGHT
//		slope = 1;
//		yint = ledy[cor17 -1] - ledx[cor17 - 1] - var;
//		if (yint < (ledy[cor10] - ledx[cor11 - 1])) sweepStatus = false;
//		else sweepStatus = true;
//		break;
//	}
//}
//
//// Depending on the coordModeMain variable set in initiate(), this function switches coordMode between 2 values to make it go back and fourth
//void pingPongingSweeps() {
//	pingPong = !pingPong;
//
//	switch (coordModeMain) {
//	default:
//		coordMode = random8(1, 9);
//		break;
//	case 1:
//		coordMode = (pingPong) ? 1 : 2;
//		break;
//	case 2:
//		coordMode = (pingPong) ? 3 : 4;
//		break;
//	case 3:
//		coordMode = (pingPong) ? 5 : 6;
//		break;
//	case 4:
//		coordMode = (pingPong) ? 7 : 8;
//		break;
//	case 5:
//		coordMode = random8(1, 9);
//		break;
//	}
//}
//
//void sweepsToBeat() {
//	if (initiate) {
//		coordModeMain = random8(1, 6); // random8(n,m) goes from n to m-1
//		changingHue = false;
//		initiate = false;
//		brightness = 255;
//		setCoordinates();
//	}
//
//	if (beatDetected[2] == 2) {
//		// Sweep from the opposite side. "coordModeMain" sets which direction it's pingponging
//		pingPongingSweeps();
//		// Decrease the speed variable, which increases the speed of the sweep. (less time passing before it continues)
//		speed -= .85*speed;
//		// Reset the sweep variable
//		var = 0;
//		brightness = 255;
//	}
//
//	// Sweeps() sets the slope, y intercept, and increases "var" until it completes the sweep
//	sweeps(coordMode);
//
//	// This function calculates which LEDs should light up as the line sweeps across the mirror
//	color = CHSV(hue, 255, 255);
//	linearSweepMath(coordMode);
//
//	//// slow down the sweep by a little bit
//	speed = constrain(speed + .1 + .02*speed, 0, 80);
//	fadeAll(250);
//
//	//// When sweepStatus = true, the sweep is done. If done, display glitter on small squares
//	if (!sweepStatus) {
//		sixFreqGlitter();
//
//		// 0 keeps any color from being displayed in the big middle square
//		fadeRange(0, cor0, cor1);
//		fadeRange(0, cor27, cor28);
//		fadeRange(0, cor13, cor15);
//	}
//
//	//// If enough time has passed, increase the sweep by 1
//	if (time > speed) {
//		var++;
//		hue++;
//		time = 0;
//	}
//}
//
//void ambientSweeps() {
//	if (initiate) {
//		changingHue = true;
//		initiate = false;
//		coordMode = random8(1, 9);
//	}
//
//	Serial.print(sweepStatus);
//	Serial.print("\t");
//	Serial.print(var);
//	Serial.print("\t");
//	Serial.print(coordMode);
//	Serial.print("\t");
//	Serial.println(time);
//
//	color = CHSV(hue, 255, 255);
//
//	if (!sweepStatus) {
//		var = 0;
//		coordMode = random8(1, 9);
//	}
//
//	if (time > var) {
//		var++;
//		time = 0;
//	}
//
//	sweeps(coordMode);
//
//	linearSweepMath(coordMode);
//	
//	fadeAll(253);
//}
//
//void ambientRainbow() {
//	if (initiate) {
//		changingHue = false;
//		initiate = false;
//	}
//
//	for (int i = 0; i < NUM_LEDS; i++) {
//		int temp = -1 * ledx[i] + ledy[i];
//		leds[i] = CHSV(temp + hue, 255, 255);
//	}
//	hue = hue + 2;
//	delay(50);
//
//	leds[0] = leds[1];
//}
//
//bool circleStatus1 = false;
//bool circleStatus2 = false;
//
//void circleDouble(bool sparkles) {   // This mode shows 2 circles expanding from the middle of the smallest squares
//
//	if (beatDetected[2] == 2) {
//		brightness = 255;
//		hue = random8(255);
//		sat = random8(150, 255);
//		radius = 0;
//		pingPong = !pingPong;
//		if (pingPong)
//			circleStatus1 = true;
//		else
//			circleStatus2 = true;
//	}
//
//	color = CHSV(hue, sat, brightness);
//
//	// Circle on right 
//	if (pingPong) {
//		if (circleStatus1) {
//			propagateCircle(color, ((ledx[cor4] + ledx[cor1]) / 2), ((ledy[cor4] + ledy[cor2]) / 2));
//			// This for loop keeps lights on the opposite small square from lighting
//			for (int i = 0; i < 3; i++) {
//				leds[cor22 - i] = 0;
//				leds[cor22 + i] = 0;
//			}
//		}
//		if (time > radius && circleStatus1) {
//			radius = radius + .5;
//			time = 0;
//		}
//	}
//	// Circle on left
//	else {
//		if (circleStatus2) {
//			propagateCircle(color, ((ledx[cor21] + ledx[cor19]) / 2), ((ledy[cor22] + ledy[cor20]) / 2));
//			// This for loop keeps lights on the opposite small square from lighting
//			for (int i = 0; i < 3; i++) {
//				leds[cor2 - i] = 0;
//				leds[cor2 + i] = 0;
//			}
//		}
//		if (time > radius && circleStatus2) {
//			radius = radius + .5;
//			time = 0;
//		}
//	}
//
//	// Thse 2 lines allow the radius to grow until it's at corners 0/28 and 14, aka the big corners in the middle
//	uint8_t pixelBrightness = leds[1].getLuma();
//	if (pixelBrightness > 0) circleStatus1 = false;
//	pixelBrightness = leds[cor28 - 1].getLuma();
//	if (pixelBrightness > 0) circleStatus2 = false;
//
//	if (sparkles) {
//		if (!circleStatus1) {
//			addSparkles(cor9, cor13, 12, .15, -50, 0, false);
//			addSparkles(cor1, cor9, 8, .15, -50, 64, false);
//		}
//		if (!circleStatus2) {
//			addSparkles(cor19, cor27, 8, .15, -50, 64, false);
//			addSparkles(cor15, cor19, 12, .15, -50, 0, false);
//		}
//	}
//
//	fadeAll(248);
//	brightness = brightness - 2;
//}
//
//void circleDoubleNoSparkles() {
//	if (initiate) {
//		changingHue = false;
//		circleStatus1 = false;
//		circleStatus1 = false;
//		initiate = false;
//	}
//	circleDouble(false);
//}
//
//void circleDoubleSparkles() {
//	if (initiate) {
//		changingHue = false;
//		initiate = false;
//	}
//	circleDouble(true);
//}
//
//void travelingDotsToLowBeats() {
//	if (initiate) {
//		color = CHSV(random(255), random(180, 255), 255);
//		changingHue = false;
//		initiate = false;
//	}
//
//	if (beatDetected[2] == 2) {
//		bottomDotSpeed -= .85*bottomDotSpeed;
//		topDotSpeed -= .85*topDotSpeed;
//		travelingDotBigSquareDesigns(1, 0);
//	}
//
//	travelingBottomDot(bottomDotSpeed);
//	travelingTopDot(topDotSpeed);
//
//	bottomDotSpeed = constrain(bottomDotSpeed + .2 + .01*bottomDotSpeed, 0, 200);
//	topDotSpeed = constrain(topDotSpeed + .2 + .01*topDotSpeed, 0, 200);
//
//	fadeAll(250);
//}
//
//void travelingDotsToHighAndLow() {
//	if (initiate) {
//		color = CHSV(random(255), random(180, 255), 255);
//		changingHue = false;
//		initiate = false;
//	}
//
//	if (beatDetected[2] == 2) {
//		bottomDotSpeed -= .85*bottomDotSpeed;
//		travelingDotBigSquareDesigns(2, 0);
//	}
//	if (beatDetected[12] == 2) {
//		topDotSpeed -= .85*topDotSpeed;
//		travelingDotBigSquareDesigns(2, 1);
//	}
//
//	travelingBottomDot(bottomDotSpeed);
//	travelingTopDot(topDotSpeed);
//
//	bottomDotSpeed = constrain(bottomDotSpeed + .2 + .01*bottomDotSpeed, 0, 200);
//	topDotSpeed = constrain(topDotSpeed + .2 + .01*topDotSpeed, 0, 200);
//
//	fadeAll(250);
//}
//
//void beatMeter() {
//	slope = 1;
//
//	if (beatDetected[2] == 2) {
//		yintLow = ledy[cor11 - 1] - ledx[cor11] - 1; // Regarding the - 1, see below
//		lowSpeed -= .85*lowSpeed;
//		for (int i = 0; i < cor14; i++) leds[i] = CHSV(hue1, 255, 180);
//	}
//	if (beatDetected[12] == 2) {
//		yintHigh = ledy[cor17 - 1] - ledx[cor17] + 1;  // Without the + 1, 1 LED stays on in the corner. 
//		highSpeed -= .85*highSpeed;
//		for (int i = cor14; i < cor28; i++) leds[i] = CHSV(hue2, 255, 180);
//	}
//
//	if (time > highSpeed) {
//		if (yintHigh > 6) yintHigh--;
//		time = 0;
//	}
//	if (time1 > lowSpeed) {
//		if (yintLow < -6) yintLow++;
//		time1 = 0;
//	}
//	if (yintHigh == 8) hue2 = random8(255);
//	if (yintLow == -8) hue1 = random8(255);
//
//	for (int i = cor0; i < cor14 - 6; i++) {
//		if (slope * ledx[i] + yintLow - ledy[i] < 1 && slope *ledx[i] + yintLow - ledy[i] > -1) leds[i] = CRGB::Black;
//	}
//	for (int i = cor14 + 5; i < cor28; i++) {
//		if (slope * ledx[i] + yintHigh - ledy[i] < 1 && slope *ledx[i] + yintHigh - ledy[i] > -1) leds[i] = CRGB::Black;
//	}
//
//	if (yintLow > -7 || !leds[cor0 + 7]) {
//		for (int i = 0; i < spectrumValue[2] - 10; i++) {  // For as loud as the low frequency is, add that many sparkles to bottom left
//			int pos = random16(cor0 + 7, cor14);
//			//brightness = map(spectrumValue[2], 0, 14, 0, 255);
//			leds[pos] = CHSV(hue1, random8(200, 255), random(30, 130));
//		}
//	}
//	if (yintHigh < 7 || !leds[cor28 - 7]) {
//		for (int i = 0; i < spectrumValue[12] - 10; i++) {  // For as loud as the high frequency is, add that many sparkles to top right
//			int pos = random16(cor14, cor28 - 7);
//			//brightness = map(spectrumValue[12], 0, 14, 0, 255);
//			leds[pos] = CHSV(hue2, random8(200, 255), random8(30, 130));
//		}
//	}
//
//	// Creates white corners on top right and bottom left
//	for (int i = 0; i < 4; i++) {
//		leds[i + 1] = CHSV(0, 0, 255);
//		leds[cor14 + i] = CHSV(0, 0, 255);
//		leds[cor14 - i - 1] = CHSV(0, 0, 255);
//		leds[cor28 - i - 1] = CHSV(0, 0, 255);
//	}
//
//	highSpeed = constrain(highSpeed + .2 + .01*highSpeed, 0, 200);
//	lowSpeed = constrain(lowSpeed + .2 + .01*lowSpeed, 0, 200);
//	fadeAll(254);
//	LEDS.show();
//}
//
//void lowBeatWithFadeAndSparkles() {
//	if (initiate) {
//		//color = CHSV(random(255), random(180, 255), 255);
//		//currentPalette = justWhite_p;
//		changingHue = false;
//		brightness = 255;
//		initiate = false;
//	}
//
//	EVERY_N_BSECONDS(10) 
//		var = random8(255);
//
//	color = CHSV(var, 255, 255);
//
//	if (beatDetected[2] == 2) {
//		travelingDotBigSquareDesigns(1, 0);
//		lowSpeed -= .85*lowSpeed; // In this case, lowSpeed is the speed of the fade
//	}
//	lowSpeed = constrain(lowSpeed + .2 + .01*lowSpeed, 240, 254);
//	fadeRange(lowSpeed, cor13, cor15);
//	fadeRange(lowSpeed, cor0, cor3);
//	fadeRange(lowSpeed, cor21, cor23);
//	fadeRange(lowSpeed, cor27, cor28);
//
//	currentPalette = justWhite_p;
//
//	addSparkles(cor3, cor5, 12, .15, -50, 0, false);
//	addSparkles(cor9, cor13, 12, .15, -50, 0, false);
//	addSparkles(cor5, cor9, 8, .15, -50, 64, false);
//	addSparkles(cor23, cor27, 8, .15, -50, 64, false);
//	addSparkles(cor15, cor21, 12, .15, -50, 0, false);
//
//	fadeRange(254 - fadeValue, cor3, cor13);
//	fadeRange(254 - fadeValue, cor15, cor21);
//	fadeRange(254 - fadeValue, cor23, cor27);
//	fadeValue = constrain(fadeValue - 1.5, 0, 15);
//}
//
//void lowBeatNoFadeAndSparkles() {
//	if (initiate) {
//		color = CHSV(random(255), random(180, 255), 255);
//		//currentPalette = justWhite_p;
//		changingHue = false;
//		brightness = 255;
//		initiate = false;
//	}
//
//	EVERY_N_MILLIS(20) {
//		fadeRange(254, cor0, cor3);
//		fadeRange(254, cor13, cor15);
//		fadeRange(254, cor27, cor28);
//		fadeRange(254, cor21, cor23);
//	}
//	//	var = random8(255);
//
//	if (beatDetected[2] == 2) {
//		if (var < 180) {
//			lightSide(cor0, cor3, 0, color);
//			lightSide(cor13, cor15 - 2, 0, color);
//			lightSide(cor27, cor28, 0, color);
//			lightSide(cor21, cor23, 0, color);
//			color = CHSV(color.h + random8(50, 200), 255, 255);
//		}
//
//		var = 255;
//	}
//	var--;
//
//	currentPalette = justWhite_p;
//
//	addSparkles(cor3, cor5, 12, .15, -50, 0, false);
//	addSparkles(cor9, cor13, 12, .15, -50, 0, false);
//	addSparkles(cor5, cor9, 8, .15, -50, 64, false);
//	addSparkles(cor23, cor27, 8, .15, -50, 64, false);
//	addSparkles(cor15, cor21, 12, .15, -50, 0, false);
//
//	fadeRange(254 - fadeValue, cor3, cor13);
//	fadeRange(254 - fadeValue, cor15, cor21);
//	fadeRange(254 - fadeValue, cor23, cor27);
//	fadeValue = constrain(fadeValue - 1.5, 0, 15);
//}
//
//void circle_midOut(int mode, int brightnessPlay) {   // This mode shows a circle expanding from the middle
//	if (initiate) {
//		changingHue = false;
//		initiate = false;
//		length = 1.1 * sqrt(pow(ledx[cor11], 2) + pow(ledy[cor11], 2));
//	}
//
//	if (beatDetected[2] == 2) {
//		radius = sqrt(pow(ledx[cor1], 2) + pow(ledy[cor2], 2)); // Works on both size mirrors
//		time = 0;
//		hue = hue + random8(50, 150);
//		pingPong = !pingPong;
//		speed -= .50*speed;
//	}
//	
//	switch (brightnessPlay) {
//	case 0:
//		brightness = 255;
//		break;
//	case 1:
//		if (pingPong) brightness = 255;
//		else brightness = 0;
//		break;
//	case 2: 	// Make the center of the circle brighter than the outsides
//		brightness = 255 - 200 * (radius / length);
//		break;
//	}
//
//	//if (time > pow(radius - sqrt(pow(ledx[cor1], 1.3) + pow(ledy[cor2], 2)), 1)) { // 1.3 for small, 
//	if (time > speed) {
//		if (radius < length + 2 )
//				radius = radius + .5;
//		time = 0;
//		}
//
//	// create a color gradient as the circle expands
//	//if (radius < 30) {
//	//  hue++;
//	//}
//
//	switch (mode) {
//	case 1:
//		if (pingPong) color = CHSV(hue, random8(130, 255), brightness); // random color
//		else color = CHSV(hue, 0, 0); // black
//		break;
//	case 2:
//		color = CHSV(hue, random8(130, 255), brightness); // random color
//		fadeAll(map(speed, 0, 60, 251, 255));
//		break;
//	}
//
//	propagateCircle(color, 0, 0);
//	speed = constrain(speed + .12, 0, 50);  // old is .1 and adding .15
//	leds[0] = leds[1]; // for an unknown reason, leds[0] was not being set by this mode
//	//speed = constrain(speed + .05 + .02*speed, 0, 70);  // old is .1 and adding .15
//}
//
//void circle_midOut_alternatingBlack() {
//	circle_midOut(1, 2);
//}
//
//void circle_midOut_fading() {
//	circle_midOut(2, 0);
//}
//
//void bouncingDot(int fade, double friction) {
//	if (initiate) {
//		changingHue = false;
//		initiate = false;
//	}
//
//	if (beatDetected[2] == 2) {
//		speed1 -= .85*speed1; // old is .85
//	}
//
//	if (time1 > speed1) { // If enough time has passed (time1), then move the light
//		movingLightPosition++;
//		topSmallSqVar++;
//		botSmallSqVar++;
//		hue1 = hue1 + 4;
//		time1 = 0;
//	}
//	switch (z) {
//	case 1: // Move left on bottom and up on right
//			// dot on side without beginning and end of strip
//		leds[cor13 + movingLightPosition] = CHSV(hue1, 255, 255);
//
//		// dot on side with beginning and end of strip
//		if (movingLightPosition < (cor1 - cor0 + 1))
//			leds[cor1 - movingLightPosition - 1] = CHSV(hue1, 255, 255);
//		else
//			leds[cor28 - movingLightPosition + (cor1 - cor0 + 1)] = CHSV(hue1, 255, 255);
//
//		if (cor28 - movingLightPosition + (cor1 - cor0 + 1) == cor27) {
//			//if (cor13 + movingLightPosition == cor15 - 5) {
//			topSmallSqVar = 0;
//			movingLightPosition = 0;
//			topSquareColor = hue1;
//			z = 2;
//		}
//		break;
//	case 2: // Move right on top and down on left
//			// dot on side without beginning and end of strip
//		leds[cor15 - movingLightPosition - 4] = CHSV(hue1, 255, 255);
//
//		// // dot on side with beginning and end of strip
//		if (cor27 + movingLightPosition < NUM_LEDS) leds[cor27 + movingLightPosition] = CHSV(hue1, 255, 255);
//		else leds[cor0 + movingLightPosition - (cor28 - cor27)] = CHSV(hue1, 255, 255);
//		//(leds[cor0 + movingLightPosition - 10] = CHSV(hue1, 255, 255);
//
//		if (cor15 - movingLightPosition - 4 == cor13) {
//			botSmallSqVar = 0;
//			movingLightPosition = 0;
//			botSquareColor = hue1;
//			z = 1;
//		}
//		break;
//	}
//
//	bouncingDotSmallSquares(botSquareColor, topSquareColor);
//
//	speed1 = constrain(speed1 + friction + .01*speed1, 15, 200);  // old is .01
//																  //speed1 = constrain(speed1 + friction + .01*speed1, 0, 200);  // old is .01
//																  //var = map(speed1, 0, 200, 244, 250);
//	fade_Inside_Big_Square(250);
//	fadeRange(fade, cor1, cor13); // Bottom right square fade
//	fadeRange(fade, cor15, cor27); // Top left sqaure fade
//}
//
//void bouncingDot_withFade() {
//	bouncingDot(254, .5);
//	fadeRange(254, cor1, cor5);
//	fadeRange(254, cor19, cor23);
//}
//
//void bouncingDot_withoutFade() {
//	bouncingDot(255, .2);
//	//fadeRange(254, cor1, cor5);
//	//fadeRange(254, cor19, cor23);
//
//	fadeRange(254, cor3, cor7);
//	fadeRange(254, cor19, cor21);
//	fadeRange(254, cor25, cor27);
//}
