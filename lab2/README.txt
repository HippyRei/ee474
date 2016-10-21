This is a collection of code for the Beaglebone that interacts with an LCD.

bbcommon.c - A library of functions that interact with the LCD that is used by
all of our processes. Important functions include initializeLCD, writeString,
writeCommand, activateGPIO, and setGPIO.

bbcommon.h - Includes constants that define paths to various directories in the
Beaglebone and defines the various commands.

getmoney.c - Uses functions in bbcommon to create an animation on the display
of the Beaglebone.

jitter.c - Uses GPIO-related functions from bbcommon to turn GPIO 20 on and off
rapidly to observe the effect of the scheduler on its timing.

lcdlistener.c - Creates 2 pipes (text_pipe and command_pipe), listens for things
to be written to them, and pushes whatever it reads to the LCD. For use with
other executables that push their output to the text and command pipes. When
using these executables, such as input.exe, start this process before them.

input.c - Gets user input from the command line and pushes it to the appropriate
pipe to be read by lcdlistener. Statements prefaced with "/" are interpreted as
commands, q is reserved for quit, and all other text is displayed on the LCD.

COMMANDS:
/clear  -Clear display
/b      -Backspace
/scl    -Shift cursor left
/scr    -Shift cursor right
/sdl    -Shift display left
/sdr    -Shift display right
/bl     -Move cursor to bottom line
/tl     -Move cursor to top line