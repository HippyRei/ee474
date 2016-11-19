This is a collection of programs to be used with a Beaglebone mounted on a drive base with a
distance sensor in front, as well as 3 LEDs, a buzzer, and a switch. Refer to the report for
information about how to wire everything.

After booting up the beaglebone with the correct configuration of peripherals, wait for the
green LED to turn on. This tells the user that the program is ready to be used. The user
then toggles the switch to make the drive base start moving forward. The drive base will
move forward until the distance sensor in front detects that something is too close to it.
It will then back up, turn to the left, and attempt to drive forward again. During this
process, the buzzer will emit 3 low beeps and the two red LEDs will turn on. When the user
wants to stop the process, they toggle the switch once more and the drive base will halt.
Further toggling of the switch can start and stop the process as many times as the user likes.

In order to ge the program to run on boot, a series of steps need to be taken. Refer to the
subsection titled "tank_entry.sh" for instructions on how to set this up.


tank_entry.c

