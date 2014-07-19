#ifndef RANDOM_INCLUDE
#define RANDOM_INCLUDE

/**
 *  These functions are used for random number gerneration.
 *  Iternally a std::mt19937 is used. By default it is seeded with the
 *  current system time.
 */


/**
 *  This functions returns a random int in the given interval [min, max]
 */
int random(int min, int max);


/**
 *  This functions returns a random unsigned int in the given interval [min, max]
 */
unsigned int random(unsigned int min, unsigned int max);


/**
 *  This functions returns a random float in the given interval [min, max]
 */
float random(float min, float max);


/**
 *  This function returns a random float in the interval [middle - deviation, middle + deviation]
 */
float randomDev(float middle, float deviation);


/**
 *  This function sets a new seed.
 */
void setRandomSeed(unsigned long seed);

#endif //RANDOM_INCLUDE
