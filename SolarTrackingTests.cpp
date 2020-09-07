#include <iostream>
#include "E101.h"
using namespace std;

int count = 0;

//numbers of the motors, so it can be varied later on (this depends on the sockets being used)
int xMotor = 1;
int yMotor = 5;

//angles of the motors, and the max and min angles they can have
int defaultXSetting = 46;
int defaultYSetting = 54;
int xMotorSetting = 49;
int yMotorSetting = 48;
int motorsMin = 32;
int motorsMax = 65;
double anglePerSetting = 180.0/33.0;
double angleSizeX, angleSizeY;

bool sunIsThere = true;

int mostRedRow = -1;
int mostRedCol = -1;

//boundaries of the middle box
int middleBoxLeft = 140;
int middleBoxRight = 180;
int middleBoxTop = 102;
int middleBoxBottom = 138;

void getInfo() {
	int redThreshold = 40;
	 
	 //int redpix[320][240];
	 int greatestRedRow = 0;
	 int whichRow = -1;
	 int greatestRedCol = 0;
	 int whichCol = -1;
	 
	 for (int i = 0; i < 240; i++) { //for all cols
		 int rowSum = 0;
		for (int j = 0; j < 320; j++) { //rows
			int red = get_pixel(i, j, 0);
			int green = get_pixel(i, j, 1);
			int blue = get_pixel(i, j, 2);
			if (red > green+redThreshold && red > blue+redThreshold && red > 80) {
				//cout << "row hi";
				//redpix[i][j] = red;
				rowSum += 1;
			}
		}
		if (rowSum > greatestRedRow) {
			greatestRedRow = rowSum;
			whichRow = i;
		}
	 }
	 
	 for (int j = 0; j < 320; j++) { //for all rows
		 int colSum = 0;
		 for (int i = 0; i < 240; i++) { //cols
			int red = get_pixel(i, j, 0);
			int green = get_pixel(i, j, 1);
			int blue = get_pixel(i, j, 2);
			 if (red > green+redThreshold && red > blue+redThreshold && red > 80) {
				//cout << "col hi";
				colSum += 1;
			}
		 }
		 if (colSum > greatestRedCol) {
			 greatestRedCol = colSum;
			 whichCol = j;
		 }
	 }
	 
	 //cout << "Greatest row: " << greatestRedRow << "/n";
	 //cout <<  "Which row: " << whichRow << "/n";
	 //cout <<  "Greatest col: " << greatestRedCol << "/n";
	 //cout <<  "Which col: " << whichCol << "/n";
	 set_pixel(whichRow, whichCol, 255, 255, 0);
     set_pixel(whichRow-1, whichCol, 255,255, 0);
     set_pixel(whichRow, whichCol-1, 255,255, 0);
     set_pixel(whichRow+1, whichCol, 255,255, 0);
     set_pixel(whichRow, whichCol+1, 255,255, 0);
	 
	 int sunExistsThreshold = 25; //for checking if the number of red pixels in the greatestRedRow and greatestRedCol is sufficient enough to consider that the sun exists there
	 
	 if (greatestRedRow < sunExistsThreshold || greatestRedCol < sunExistsThreshold) {
		sunIsThere = false;
	 } else if (greatestRedRow >= (greatestRedCol*2)/3 && greatestRedCol >= (greatestRedRow*2)/3) { //conditions 3 and 5
		mostRedRow = whichRow;
		mostRedCol = whichCol;
		sunIsThere = true;
	 } else {
		sunIsThere = false;
	 }
	 
	 update_screen();
}

void turnModule() {
	if (!sunIsThere) {//move Sun to default position
		//movements
		xMotorSetting = defaultXSetting; //default x
		yMotorSetting = defaultYSetting; //default y
		set_motors(xMotor, xMotorSetting);
		set_motors(yMotor, yMotorSetting);
		
		//display
		angleSizeX = 0.0;
		angleSizeY = 0.0;
		cout << "Horizontal angle: " << angleSizeX << " degrees" << "\n";
		cout << "Vertical angle: " << angleSizeY << " degrees" << "\n";
		cout << "\n";
		sleep1(50);
	} else { //the range of set_motors() is 32-65. So 65-32 = 33. With this, we use the angle to 
		while (sunIsThere && (mostRedCol < middleBoxLeft || mostRedCol > middleBoxRight || mostRedRow < middleBoxTop || mostRedRow > middleBoxBottom)) {
			//if statements for determining where the yellow dot is and changing the motors accordingly
			if (mostRedRow > middleBoxTop && mostRedRow < middleBoxBottom) {
				if (mostRedCol < middleBoxLeft) { //yellow point is directly left of the box
					if (xMotorSetting != motorsMin) { xMotorSetting--; }
					set_motors(xMotor, xMotorSetting);
				} else if (mostRedCol > middleBoxRight) { //yellow point is directly right of the box
					if (xMotorSetting != motorsMax) { xMotorSetting++; }
					set_motors(xMotor, xMotorSetting);
				}
			} else if (mostRedCol > middleBoxLeft && mostRedCol < middleBoxRight) {
				if (mostRedRow < middleBoxTop) { //yellow point is directly above the box
					if (yMotorSetting != motorsMin) { yMotorSetting--; }
					set_motors(yMotor, yMotorSetting);
				} else if (mostRedRow > middleBoxBottom) { //yellow point is directly below the box
					if (yMotorSetting != motorsMax) { yMotorSetting++; }
					set_motors(yMotor, yMotorSetting);
				}
			} else if (mostRedCol < middleBoxLeft && mostRedRow < middleBoxTop) { //yellow point is left and above the box
				if (xMotorSetting != motorsMin && yMotorSetting != motorsMin) {
					xMotorSetting--;
					yMotorSetting--;
				}
				set_motors(xMotor, xMotorSetting);
				set_motors(yMotor, yMotorSetting);
			} else if (mostRedCol > middleBoxRight && mostRedRow < middleBoxTop) { //yellow point is right and above the box
				if (xMotorSetting != motorsMax && yMotorSetting != motorsMin) {
					xMotorSetting++;
					yMotorSetting--;
				}
				set_motors(xMotor, xMotorSetting);
				set_motors(yMotor, yMotorSetting);
			} else if (mostRedCol < middleBoxLeft && mostRedRow > middleBoxBottom) { //yellow point is left and below the box
				if (xMotorSetting != motorsMin && yMotorSetting != motorsMax) {
					xMotorSetting--;
					yMotorSetting++;
				}
				set_motors(xMotor, xMotorSetting);
				set_motors(yMotor, yMotorSetting);
			} else if (mostRedCol > middleBoxRight && mostRedRow > middleBoxBottom) { //yellow point is right and below the box
				if (xMotorSetting != motorsMax && yMotorSetting != motorsMax) {
					xMotorSetting++;
					yMotorSetting++;
				}
				set_motors(xMotor, xMotorSetting);
				set_motors(yMotor, yMotorSetting);
			}
			
			//for drawing the middle box boundaries, comment out for hand-in to increase speed
			for (int i = 0; i < 240; i++) {
				set_pixel(i, middleBoxLeft, 255, 255,255);
				set_pixel(i, middleBoxRight, 255, 255,255);
			}
			for (int i = 0; i < 320; i++) {
				set_pixel(middleBoxTop, i, 255, 255, 255);
				set_pixel(middleBoxBottom, i, 255, 255, 255);
			}
			
			update_screen();
			
			angleSizeX = ((double)xMotorSetting)-defaultXSetting;
			if (angleSizeX != 0.0) {
				angleSizeX = angleSizeX*anglePerSetting;
			} else {
				angleSizeX = 0.0;
			}
			angleSizeY = ((double)yMotorSetting)-defaultYSetting;
			if (angleSizeY != 0.0) {
				angleSizeY = angleSizeY*anglePerSetting;
			} else {
				angleSizeY = 0.0;
			}
			cout << "Horizontal angle: " << angleSizeX << " degrees" << "\n";
			cout << "Vertical angle: " << angleSizeY << " degrees" << "\n";
			cout << "\n";
			
			hardware_exchange();
			take_picture();
			getInfo();
		}
	}
	
	//for drawing the middle box boundaries, comment out for hand-in to increase speed
	for (int i = 0; i < 240; i++) {
		set_pixel(i, middleBoxLeft, 255, 255,255);
		set_pixel(i, middleBoxRight, 255, 255,255);
	}
	for (int i = 0; i < 320; i++) {
		set_pixel(middleBoxTop, i, 255, 255, 255);
		set_pixel(middleBoxBottom, i, 255, 255, 255);
	}
	update_screen();
}

int main()
{
	init(0);

	cout << "For horizontal direction, 0 degrees is considered to be directly in front" << "\n";
	cout << "For vertical direction, 0 degrees is upward" << "\n";
	
	open_screen_stream();
	
	while (count < 100000) {
		take_picture();
		 
		getInfo();
		turnModule();
		 
		hardware_exchange();
		
		count++;
	}
	close_screen_stream();
    stoph();
	
	return 0;
}
