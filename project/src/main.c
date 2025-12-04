#include "toy.h"

int main(void)
{
    init_hardware();

    while(1) {
        __low_power_mode_0(); // Enter LPM0, wake on timer or button interrupt
    }
}
