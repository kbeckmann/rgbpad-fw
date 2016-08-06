#ifndef _BUTTONS_H
#define _BUTTONS_H

#define BUTTONS_COLS (4)
#define BUTTONS_ROWS (4)
#define BUTTONS_COUNT (BUTTONS_COLS * BUTTONS_ROWS)

extern uint8_t g_buttons[BUTTONS_COUNT];
extern uint16_t g_buttons_bits;

void read_buttons();
void read_buttons_bits();

#endif // _BUTTONS_H