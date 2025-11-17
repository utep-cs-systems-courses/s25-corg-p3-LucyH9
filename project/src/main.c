//Initialize program
#include "toy.h"

int main(void)
{
    init_hardware();

    while (1) {
        update_state();
        __low_power_mode_0(); // save power while waiting for button
    }
}
