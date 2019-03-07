#ifndef TIMER_H
#define TIMER_H

#define CLOCK_FREQUENCY (1193180) 	//Hz units
#define TIMER_COMMAND_PORT (0x43)
#define TIMER_CHANNEL_PORT (0x40)
/**
 * @brief      Set the frequency of the timer and init it
 *
 * @param[in]  frequency  The frequency to set
 */
void init_timer(const int frequency);

#endif