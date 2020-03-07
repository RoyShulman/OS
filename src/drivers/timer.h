#ifndef TIMER_H
#define TIMER_H

/**
 * @brief      Set the frequency of the timer and init it
 *
 * @param[in]  frequency  The frequency to set
 */
void init_timer(const int frequency);

/**
 * @brief      Sets the frequency by writing to the PIC command port
 *
 * @param[in]  frequency  The frequency to set
 */

void set_frequency(const int frequency);

/**
 * @brief      Stop handling the timer interrupt with the handler
 */
void destroy_timer();

#endif