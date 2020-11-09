int purpleCount()
{
	int purpleCount = 0;
	displayString(2, "Enter number of purple pills");
	while (!getButtonPress(buttonEnter))
	{
		while (!getButtonPress(buttonUp))
		{
			displayString(4, "Purple Count: %d", purpleCount);
			if (getButtonPress(buttonEnter))
			{
				return purpleCount;
			}
		}
		while (getButtonPress(buttonUp))
		{	}
		purpleCount++;
	}
	return purpleCount;
}

void purpleCheck(int &purplecount)
{
	while(!getButtonPress(buttonRight))
	{
		displayString(8, "Total Purple Count: %d", purplecount);
		displayString(10, "Is this correct? Right = Yes");
		displayString(12, "Left = No");
		if(getButtonPress(buttonLeft))
		{
			eraseDisplay();
			purplecount = purpleCount();
		}
	}
}

int redCount()
{
	int redCount = 0;
	displayString(2, "Enter number of red pills");
	while (!getButtonPress(buttonEnter))
	{
		while (!getButtonPress(buttonUp))
		{
			displayString(4, "Red Count: %d", redCount);
			if (getButtonPress(buttonEnter))
			{
				return redCount;
			}
		}
		while (getButtonPress(buttonUp))
		{	}
		redCount++;
	}
	return redCount;
}

void redCheck(int &redcount)
{
	while(!getButtonPress(buttonRight))
	{
		displayString(8, "Total Red Count: %d", redcount);
		displayString(10, "Is this correct? Right = Yes");
		displayString(12, "Left = No");
		if(getButtonPress(buttonLeft))
		{
			eraseDisplay();
			redcount = redCount();
		}
	}
}

int greenCount()
{
	int greenCount = 0;
	displayString(2, "Enter number of green pills");
	while (!getButtonPress(buttonEnter))
	{
		while (!getButtonPress(buttonUp))
		{
			displayString(4, "Green Count: %d", greenCount);
			if (getButtonPress(buttonEnter))
			{
				return greenCount;
			}
		}
		while (getButtonPress(buttonUp))
		{	}
		greenCount++;
	}
	return greenCount;
}

void greenCheck(int &greencount)
{
	while(!getButtonPress(buttonRight))
	{
		displayString(8, "Total Green Count: %d", greencount);
		displayString(10, "Is this correct? Right = Yes");
		displayString(12, "Left = No");
		if(getButtonPress(buttonLeft))
		{
			eraseDisplay();
			greencount = greenCount();
		}
	}
}

int yellowCount()
{
	int yellowCount = 0;
	displayString(2, "Enter number of yellow pills");
	while (!getButtonPress(buttonEnter))
	{
		while (!getButtonPress(buttonUp))
		{
			displayString(4, "Yellow Count: %d", yellowCount);
			if (getButtonPress(buttonEnter))
			{
				return yellowCount;
			}
		}
		while (getButtonPress(buttonUp))
		{	}
		yellowCount++;
	}
	return yellowCount;
}
void yellowCheck(int &yellowcount)
{
	while(!getButtonPress(buttonRight))
	{
		displayString(8, "Total Yellow Count: %d", yellowcount);
		displayString(10, "Is this correct? Right = Yes");
		displayString(12, "Left = No");
		if(getButtonPress(buttonLeft))
		{
			eraseDisplay();
			yellowcount = yellowCount();
		}
	}
}

void actuateDispenser()
{
	motor[motorA] = 8;
}

float average()
{
	float sum = 0.0, average = 0.0;
	int colour = 0;
	for(int i = 0; i < 50; i++)
	{
		colour = SensorValue[S1];
		sum += colour;
		wait1Msec(10);
	}
	average = sum/50.0;
	return average;
}

bool isPillNeeded(int &purplecount, int &redcount, int &greencount, int &yellowcount)
{
	float colour = 0;
	while(SensorValue[S1] >= 0 && SensorValue[S1] <= 2)
	{
		wait1Msec(10);
	}
	wait1Msec(100);
	colour = average();
	displayString(10, "colour: %f", colour);
	if (colour >=2.1 && colour <= 2.9 && purplecount > 0)
	{
		purplecount --;
		wait1Msec(100);
		return true;
	}
	else if (colour >= 3.5 && colour <= 4.5 && greencount > 0)
	{
		greencount --;
		wait1Msec(100);
		return true;
	}
	else if (colour >= 10 && yellowcount > 0)
	{
		yellowcount --;
		wait1Msec(100);
		return true;
	}
	else if (colour <= 8 && colour >= 6 && redcount > 0)
	{
		redcount --;
		wait1Msec(100);
		return true;
	}
	else
	{
		wait1Msec(600);
		return false;
	}
}

void actuatePlate()
{
	resetMotorEncoder(motorB);
	while(nMotorEncoder[motorB] > -90)
	{
		motor[motorB] = -10;
	}
	motor[motorB] = 0;
	wait1Msec(1700);
	resetMotorEncoder(motorB);
	while(nMotorEncoder[motorB] < 90)
	{
		motor[motorB] = 10;
	}
	motor[motorB] = 0;
}

void recycle()
{
	resetMotorEncoder(motorC);
	motor[motorA] = 0;
	while(nMotorEncoder[motorC] < 780)
	{
		motor[motorC] = 10;
	}
	motor[motorC] = 0;
	wait1Msec(1000);

	resetMotorEncoder(motorC);
	while (nMotorEncoder[motorC] > -780)
	{
		motor[motorC] = -10;
	}
	motor[motorC] = 0;
	resetMotorEncoder(motorC);
}

void dispenserOff()
{
	motor[motorA] = 0;
}

task main()
{
	int purplecount = 0;
	int redcount = 0;
	int greencount = 0;
	int yellowcount = 0;
	int discardCount = 0;
	bool needPill = false;

	SensorType[S1] = sensorEV3_Color;
	SensorType[S3] = sensorEV3_Touch;
	SensorMode[S1] = modeEV3Color_Reflected;

	purplecount = purpleCount();
	purpleCheck(purplecount);
	eraseDisplay();

	redcount = redCount();
	redCheck(redcount);
	eraseDisplay();

	greencount = greenCount();
	greenCheck(greencount);
	eraseDisplay();

	yellowcount = yellowCount();
	yellowCheck(yellowcount);
	eraseDisplay();

	while (purplecount > 0 || redcount > 0 || greencount > 0 || yellowcount > 0)
	{
		needPill = false;
		actuateDispenser();
		needPill = isPillNeeded(purplecount, redcount, greencount, yellowcount);
		wait1Msec(400);
		if (needPill)
			actuatePlate();
		else if (!needPill)
			discardCount++;

		if (discardCount == 8)
		{
			wait1Msec(500);
			recycle();
			discardCount = 0;
		}
		eraseDisplay();
		displayString(4, "Needed: Purple: %d Red: %d", purplecount, redcount);
		displayString(6, "Green: %d Yellow: %d", greencount, yellowcount);
		displayString(8, "Discard: %d", discardCount);
	}
	wait1Msec(1000);
	dispenserOff();
	recycle();
	eraseDisplay();
	displayString(2, "Program Ended, pop your pills!");

	wait1Msec(3000);
}
