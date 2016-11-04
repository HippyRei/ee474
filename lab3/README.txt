This project includes a kernel module that interacts with the LCD and 3 processes that use it
as a client.

lcd.c - This is a kernel module for the Beaglebone that interacts with an LCD. The source
code is set up to be cross-compiled on a computer with the beaglebone kernel; users can use
the module by using insmod to insert the module into the kernel. After this, the user can use
dmesg | tail to display the command that needs to be run to create the dev file. After this,
the dev file is added to the /dev folder and can opened, written to, and closed. Whatever
is written to the file will be displayed on the LCD, and there is a list of commands that
the kernel module looks out for that specify special actions:

/clear - clears the screen
/scl - shift cursor left
/scr - shift cursor right
/sdl - shift display left
/sdr - shift display right
/tl - move the cursor to the start of the top line
/bl - move the cursor to the start of the bottom line
/b - backspace


input.c - This program starts a user process that prompts the user for input and pushes them to
the screen. It uses the lcd's dev file to do this, so any commands that are in the list above will
be executed. Typing in "q" will end the process.

getmoney.c - This program displays a scrolling animation on the LCD. It is also a client of the
kernel module.

minesweeper.c - This program starts a game of Minesweeper on the 16x2 LCD. Rules for the game
can be found in the lab report. This game also depends on the kernel module. Here is a list
of possible commands:

w - move cursor up
a - move cursor left
s - move cursor down
d - move cursor right
e - select a tile
q- quit the game


NOTES ON BUILDING THE KERNEL:

In order to build the kernel, you need to clone the repository located at
https://github.com/RobertCNelson/bb-kernel.git and checkout the 3.8.13-bone79 branch.
Before you build the file, open up version.sh and comment out "toolchain="gcc_linaro_gnueabihf_4_7"
and uncomment "toolchain="gcc_linaro_gnueabihf_4_8." Then download the file at the following link,
and replace the tools/host_det.sh file with it:

https://raw.githubusercontent.com/RobertCNelson/stable-kernel/master/tools/host_det.sh

After performing these steps, run ./build_kernel.sh and let it run until a prompt appears. Press escape
twice at the prompt and let the rest of the build process run.
