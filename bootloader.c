/* A simple program to run a bootloader using the qemy systems emulator.
Note that the boot sector is 512 bytes in size.
See Makefile to compile and run. */

/* activate the 16-bit mode */
/* needs to stay the first line */
asm(".code16gcc\njmp $0, $main");

void print_str(char *str)
{
	char i = 0;
	while (str[i] != '\0')
	{
		asm volatile(
			"mov %0, %%al;"	  // move char to register AL (8 bit)
			"mov $0xE, %%ah;" // move E (in hex) to register AH
			"int $0x10"
			:
			: "r"(str[i]) // input in any register: char at idx i
			: "eax");	  // tell the compiler not to optimize the contents of reguster EAX
		i++;
	}
}

/* read a character from keystroke */
char read_char()
{
	char symbol;
	asm volatile(
		"mov $0x0, %%ah;" // move 0 to register AH
		"int $0x16;"	  // interrupt 16
		"mov %%al,%0;"	  // move the contents of register AL to the variable symbol
		: "=r"(symbol)	  // output: any register, store it in variable symbol
		:
		: "eax"); // tell the compiler not to optimize the contents of reguster EAX
	return symbol;
}

/* read a string */
char *read_str(char *str)
{
	char *buffer;
	unsigned short address = &buffer;
	char i = 0;
	while (str[i] != '\0')
	{
		unsigned short new_address = address + sizeof(char) * i;
		new_address = read_char(str[i]);
	}
	return buffer;
}

/* call reboot */
void reboot()
{
	asm volatile(
		"int $0x19;");
}

/* initialize a given array of char (buffer) to the provided char (symbol)
 given the length of the array */
void memset(char *buffer, char symbol, int length)
{
	while (length--)
	{
		*buffer++ = symbol;
	}
}

/* print string and puff it up with new line before and after */
void print_buffer(char *string, char *new_line)
{
	print_str(new_line);
	print_str(string);
	print_str(new_line);
}

void main(void)
{
	char *welcome_str = "Reboot!\n\r";
	int character;
	int idx = 0;
	char buffer[9] = "";
	char *dot = "*";
	char *new_line = "\n\r";

	print_str(welcome_str);

	while (1)
	{
		character = read_char();
		if (idx == 0 && character == '\r')
		{
			reboot();
		}
		if (idx <= 7)
		{
			if (character == '\r')
			{
				print_buffer(buffer, new_line);
				memset(buffer, 0, 8);
				idx = 0;
			}
			else
			{
				buffer[idx] = character;
				print_str(dot);
				idx++;
			}
		}
		else
		{
			if (character == '\r')
			{
				print_buffer(buffer, new_line);
				memset(buffer, 0, 8);
				idx = 0;
			}
		}
	}
}