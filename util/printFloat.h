#ifndef print_float_h
#define print_float_h

/**
 * Routine to print floating point numbers over serial.
 * @param f          the number to print
 * @param num_digits decimal precision
 */
void print_float(float f, int num_digits)
{
    int pows_of_ten[4] = {1, 10, 100, 1000};
    int multiplier, whole, fract, d, n;

    multiplier = pows_of_ten[num_digits];
    if (f < 0.0) {
        f = -f;
        Serial.print("-");
    }
    whole = (int) f;
    fract = (int) (multiplier * (f - (float)whole));

    Serial.print(whole);
    Serial.print(".");

    // print each digit with leading zeros
    for (n=num_digits-1; n>=0; n--) {
         d = fract / pows_of_ten[n];
         Serial.print(d);
         fract = fract % pows_of_ten[n];
    }
}

#endif
