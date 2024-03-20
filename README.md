A simple 16-bit bootloader that is run using the qemu systems emulator. It's written in C with inline assembly to invoke the facilities of the BIOS and communicate with the I/O in the absence of the strandard C library. The program is limited by the size of the boot sector (512 bytes). The BIOS loads a 512-byte boot sector into memory location “0x7c00” and begins executing it.

Read Makefile to run and compile.

The program prints out the welcome string and awaits the user to input their password. The password can be no lobger than 8 characters. On each keystroke on the input a dot is printed out to imitate password input and display. The actual password string is displayed on pressing the Enter. If the first character in the password is Enter, initiate a reboot.
