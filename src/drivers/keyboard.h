#ifndef KEYBOARD_H
#define KEYBOARD_H


#define KEYBOARD_PORT	(0x60)

/**
 * @brief      Inits the keyboard by registering the interrupt handler to IRQ1
 */
void init_keyboard();

/**
 * @brief      Unregisters the interrupt handler
 */
void destroy_keyboard();


#endif