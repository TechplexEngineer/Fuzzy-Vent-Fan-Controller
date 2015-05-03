// HIH_6130_1  - Arduino
//
// Arduino                HIH-6130
// SCL (Analog 5) ------- SCL (term 3)
// SDA (Analog 4) ------- SDA (term 4)
//
// Note 2.2K pullups to 5 VDC on both SDA and SCL
//
// Pin4 ----------------- Vdd (term 8)
//


#include <Wire.h> //I2C library

#include "temp.h"
#include "printFloat.h"
#include "trapmf.h"

byte fetch_humidity_temperature(unsigned int *p_Hum, unsigned int *p_Temp);
void print_float(float f, int num_digits);
// float c2f(float f);

#define TRUE 1
#define FALSE 0

void setup(void)
{
	Serial.begin(9600);	// start serial output
	Wire.begin();		// start i2c communication

	delay(5000);		// wait for init to complete
	Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>");
}

void loop(void)
{
	byte _status;
	unsigned int H_dat, T_dat;
	float RH, temp;

	float out_off[8] 		= {0};
	float out_slow[4] 		= {0};
	float out_medium[2] 	= {0};
	float out_mediumFast[4] = {0};
	float out_fast[12] 		= {0};

	float cool            = 0;
	float somewhatCool    = 0;
	float comfortable     = 0;
	float somewhatHot     = 0;
	float hot             = 0;
	float low             = 0;
	float lowBorderline   = 0;
	float medium          = 0;
	float highBorderline  = 0;
	float high            = 0;

	float out_off_max         = 0;
	float out_slow_max        = 0;
	float out_medium_max      = 0;
	float out_mediumFast_max  = 0;
	float out_fast_max        = 0;

	while(1)
	{
		_status = fetch_humidity_temperature(&H_dat, &T_dat);
		if(_status != 0){
			 Serial.println("Abnormal state");
		}
		RH = (float) H_dat * 6.10e-3;
		temp = (float) T_dat * 1.007e-2 - 40.0;
		temp = c2f(temp);

		// evaluate the membership functions at the measured temperature
		cool            = trapmf(temp, 0,  0,  59, 62);
		somewhatCool    = trapmf(temp, 59, 62, 62, 72);
		comfortable     = trapmf(temp, 62, 72, 72, 82);
		somewhatHot     = trapmf(temp, 72, 82, 82, 85);
		hot             = trapmf(temp, 82, 85, 90, 90);

		// evaluate the membership functions at the measured relative humidity
		low             = trapmf(RH, 0,  0,  50,  60);
		lowBorderline   = trapmf(RH, 50, 60, 60,  70);
		medium          = trapmf(RH, 60, 70, 70,  80);
		highBorderline  = trapmf(RH, 70, 80, 80,  90);
		high            = trapmf(RH, 80, 90, 100, 100);

		//row1 (of rule table)
		out_off[0] = AND(low, cool);
		out_off[1] = AND(low, somewhatCool);
		out_off[2] = AND(low, comfortable);
		out_off[3] = AND(low, somewhatHot);
		out_off[4] = AND(low, hot);
		//row2 (of rule table)
		out_off[5] 	= AND(lowBorderline, cool);
		out_off[6] 	= AND(lowBorderline, somewhatCool);
		out_slow[0] = AND(lowBorderline, comfortable);
		out_slow[1] = AND(lowBorderline, somewhatHot);
		out_slow[2] = AND(lowBorderline, hot);
		//row3 (of rule table)
		out_slow[3]   = AND(medium, cool);
		out_medium[0] = AND(medium, somewhatCool);
		out_medium[1] = AND(medium, comfortable);
		out_fast[0]   = AND(medium, somewhatHot);
		out_fast[1]   = AND(medium, hot);
		//row4 (of rule table)
		out_mediumFast[0] = AND(highBorderline, cool);
		out_mediumFast[1] = AND(highBorderline, somewhatCool);
		out_fast[2] 	  = AND(highBorderline, comfortable);
		out_fast[3] 	  = AND(highBorderline, somewhatHot);
		out_fast[4] 	  = AND(highBorderline, hot);
		//row5 (of rule table)
		out_mediumFast[2] = AND(high, cool);
		out_fast[5] 	  = AND(high, somewhatCool);
		out_fast[6] 	  = AND(high, comfortable);
		out_fast[7] 	  = AND(high, somewhatHot);
		out_fast[8] 	  = AND(high, hot);

		// now that we have anded the D.o.T. we need to find the OR(max) of each
		out_off_max         = ArrMax(out_off, 8);
		out_slow_max        = ArrMax(out_slow, 4);
		out_medium_max      = ArrMax(out_medium, 2);
		out_mediumFast_max  = ArrMax(out_mediumFast, 4);
		out_fast_max        = ArrMax(out_fast, 12);

		// Calculate the weighted average using the crisp consequent
		float out = (out_off_max*0 + out_slow_max*40 + out_medium_max*60 + out_mediumFast_max*80 + out_fast_max*100);
		out /= (out_off_max+ out_slow_max+ out_medium_max+ out_mediumFast_max+ out_fast_max);

		print_float(RH, 1);
		Serial.print("  ");
		print_float(temp, 2);
		Serial.print("  ");
		print_float(out, 2);
		Serial.println();

		delay(1000);
	}
}

/**
 * Read the temperature and humidity from the sensor over the i2c bus.
 * based on code by Peter H Anderson, Baltimore, MD, Nov, '11
 * @param  p_H_dat [out] Relative Humidity
 * @param  p_T_dat [out] Temperature data. In celsius
 * @return         status. 1 indicates normal operation
 */
byte fetch_humidity_temperature(unsigned int *p_H_dat, unsigned int *p_T_dat)
{
		byte address, Hum_H, Hum_L, Temp_H, Temp_L, _status;
		unsigned int H_dat, T_dat;
		address = 0x27;;
		Wire.beginTransmission(address);
		Wire.endTransmission();
		delay(100);

		Wire.requestFrom((int)address, (int) 4);
		Hum_H = Wire.read();
		Hum_L = Wire.read();
		Temp_H = Wire.read();
		Temp_L = Wire.read();
		Wire.endTransmission();

		_status = (Hum_H >> 6) & 0x03;
		Hum_H = Hum_H & 0x3f;
		H_dat = (((unsigned int)Hum_H) << 8) | Hum_L;
		T_dat = (((unsigned int)Temp_H) << 8) | Temp_L;
		T_dat = T_dat / 4;
		*p_H_dat = H_dat;
		*p_T_dat = T_dat;
		return(_status);
}



