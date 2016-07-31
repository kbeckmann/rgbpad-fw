#ifndef _BOARD_H
#define _BOARD_H

#define ENABLE_UART 1
#define ENABLE_UART_PRINTF 1

extern UART_HandleTypeDef huart1;

#if ENABLE_UART && ENABLE_UART_PRINTF
    void uart_printf(const char* format, ...);
#else
#   define uart_printf(...)
#endif

#endif // _BOARD_H
