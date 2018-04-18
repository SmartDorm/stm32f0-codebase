# stm32f0-codebase
Code for the SM32F0Discovery


Working as a "state machine" concept. "smart_dorm.h" will contain all function declarations and type definitions for all app-related content. Each state will have its own .c source file to keep track of them, and to keep problems isolated from one another. HAL functions will be incorporated in to wrappers to add extra layer of abstraction (i.e. lcd_write(char * message) vs. HAL_SPI_Transmit(...)). 

Raspberry Pi slave code to come shortly.
