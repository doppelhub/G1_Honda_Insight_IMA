//Copyright 2026(c) John Sullivan

//G1 Insight Hall Sensor Tester

#include "hst.h"

//To update firmware (if using Nano with LiControl bootloader):
	//1 : Apply short from A3 to GND (2p header on PCB)
	//2 : Upload firmware
	//3 : Remove short (328p not boot with jumper installed)

uint8_t getHallState(void)
{
	uint8_t hallState = 0b000;

	if (digitalRead(PIN_HALL_A)) { hallState |= 0b100; }
	if (digitalRead(PIN_HALL_B)) { hallState |= 0b010; }
	if (digitalRead(PIN_HALL_C)) { hallState |= 0b001; }

	return hallState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t getHallIndex(uint8_t state)
{
	static const uint8_t hallSequence[] = { 0b001, 0b101, 0b100, 0b110, 0b010, 0b011 };
	static const uint8_t numArrayElements = uint8_t(sizeof(hallSequence) / sizeof(hallSequence[0]));

	for (uint8_t ii=0; ii<numArrayElements; ii++)
	{
		if (hallSequence[ii] == state) { return ii; }
	}

	Serial.print(F("\nError: index not found"));
	return 127;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool isStateTransitionValid (uint8_t stateNow, uint8_t previousState)
{
	if ((stateNow      == 0b000) ||
	    (stateNow      == 0b111) ||
	    (previousState == 0b000) ||
	    (previousState == 0b111)  ) { return NO; }

	uint8_t indexPrevious = getHallIndex(previousState);
	uint8_t indexNow      = getHallIndex(stateNow);

	if (((indexPrevious - indexNow) == 1)         ||
		((indexNow - indexPrevious) == 1)         ||
		((indexPrevious == 0) && (indexNow == 5)) || //wrap around
		((indexPrevious == 5) && (indexNow == 0))  ) { return YES; }
	else                                             { return  NO; }
}

void printHallState_binary(uint8_t state)
{
	const uint8_t binaryDigitsToPrint = 3;

	for (int8_t ii=(binaryDigitsToPrint-1); ii>=0; --ii)
	{
		Serial.print((state>>ii) & 1);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()  
{
	Serial.begin(115200); //USB
	Serial.print(F("\n\nHall Sensor Tester v" FW_VERSION ", " BUILD_DATE "\nKEY MUST BE OFF FOR SAFETY!"));

	Serial.print(F("\nInitial Hall State :"));
	printHallState_binary(getHallState());

    Serial.print(F("\n"
    	"\nRotate crankshaft by hand at least one complete mechanical revolution"
    	"\nyou should see QTY48 Hall transitions per mechanical revolution"
    	"\n'1r' should print every QTY6 state changes (one electrical revolution)"
    	"\n'1r' should print QTY8 times per mechanical revolution"
        ));

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
	uint8_t stateNow = getHallState();
	static uint8_t previousState = stateNow;
	static uint8_t powerupState = stateNow;

	if (stateNow != previousState)
	{
		Serial.print(F("\n0b"));
		printHallState_binary(stateNow);

		if (isStateTransitionValid(stateNow, previousState)) { Serial.print(F(" OK"   )); }
		else                                                 { Serial.print(F(" Invalid!")); }

		if (stateNow == powerupState) { Serial.print(F(" 1r")); }

		previousState = stateNow;
	}
}