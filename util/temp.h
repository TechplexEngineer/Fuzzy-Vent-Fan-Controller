
#ifndef Temp_h
#define Temp_h

/**
 * Celsius to Fahrenheit
 * @param  f Celsius temperature to convert
 * @return   Fahrenheit temperature
 */
float c2f(float f)
{
  float _temp = f*9.0;
  _temp /= 5;
  _temp += 32;
  return _temp;
}

#endif
