#include "hal_buttons.h"

struct button_s
{
    int id;
    int port;
    int pin;
};

typedef struct button_s button_t;

button_t buttons[] =
{
    { BTN_1,                PORT_A, 0 },
    { BTN_2,                PORT_A, 1 },
    { BTN_3,                PORT_B, 6 },
    { BTN_4,                PORT_B, 6 },
    { BTN_5,                PORT_B, 6 },
    { BTN_6,                PORT_B, 6 },
    { BTN_7,                PORT_B, 6 },
    { BTN_8,                PORT_B, 6 },
    { BTN_9,                PORT_B, 6 },
    { BTN_PROGRAM_MODE,     PORT_B, 6 },
    { BTN_ALWAYS_ON_LEDS,   PORT_B, 6 },
    { NULL,                 NULL,   NULL }
};


