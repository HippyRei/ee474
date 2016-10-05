# 474lab1

This program counts to 7 using 3 LEDs and uses a buzzer to generate different tones as the LEDs count up.

The program uses GPIO 67, 68, and 44 to accomplish this with the LEDs, and also the PWM port EHRPWM1A for the buzzer.
The PWM port's period increases by 200000 each step and the duty is always 50% of the period.
