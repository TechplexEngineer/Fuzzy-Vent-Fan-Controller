
#ifndef trapmf_h
#define trapmf_h

/**
 * Evaluate a trapezoidal membership function at point n given corner points:
 * a b, c, d.
 *         b         c
 *         +---------+
 *        /           \
 *       /             \
 *      /               \
 * ----+                 +----
 *     a                 d
 * @param  n the x-value at which to evaluate the membership function.
 * @param  a the corner a value
 * @param  b the corner b value
 * @param  c the corner c value
 * @param  d the corner d value
 * @return   the degree of truth of the membership function at the value n
 */
float trapmf (int n, int a, int b, int c, int d )
{
    float out = 0;
    if (n > a && n < b) {
        float m=(0-1)/(a-b);
        out = m*(n-a)+0;
    } else if (n >= b && n <= c ) {
        out = 1;
    } else if (n > c && n < d) {
        float m=(1-0)/(c-d);
        out = m*(n-c)+1;
    } else if (n <= a || n >= d ) {
        out = 0;
    }

    return out;
}

/**
 * find the max value in an array
 * @param  arr    array to search
 * @param  arrlen length of array
 * @return        maximum value
 */
float ArrMax(float arr[], int arrlen) {
    float max = arr[0];
    for (int i = 1; i < arrlen; i++) {
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}
/**
 * Compute the Fuzzy AND of two numbers
 * @param  a first number
 * @param  b second number
 * @return   the AND value (min)
 */
float AND(float a, float b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }

}

/**
 * Compute the Fuzzy and of two numbers
 * @param  a first number
 * @param  b second number
 * @return   the OR value (max)
 */
float OR(float a, float b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

#endif
