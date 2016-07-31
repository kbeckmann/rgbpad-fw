#ifndef _btnS_H
#define _BUTTONS_H

#define BUTTONS_COLS (4)
#define BUTTONS_ROWS (4)
#define BUTTONS_COUNT (BUTTONS_COLS * BUTTONS_ROWS)

extern uint8_t g_buttons[BUTTONS_COUNT];

void read_buttons();

#endif // _BUTTONS_H