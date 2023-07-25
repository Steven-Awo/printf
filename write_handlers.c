#include "main.h"

/**
 * handle_write_char - Prints a string
 * @c: char types.
 * @bufferz: The Buffer array to handle print
 * @flagsz:  It Calculates active flags.
 * @widthz: get the width.
 * @precisionz: The precision of specifier
 * @sizez: The Size of specifier
 *
 * Return:The Number of chars printed.
 */
int handle_write_char(char c, char bufferz[],
	int flagsz, int widthz, int precisionz, int sizez)
{ /* char is stored at left and paddind at buffer's right */
	int i = 0;
	char bed = ' ';

	UNUSED(precisionz);
	UNUSED(sizez);

	if (flagsz & F_ZERO)
		bed = '0';

	bufferz[i++] = c;
	bufferz[i] = '\0';

	if (widthz > 1)
	{
		bufferz[BUFF_SIZE - 1] = '\0';
		for (i = 0; i < widthz - 1; i++)
			bufferz[BUFF_SIZE - i - 2] = bed;

		if (flagsz & F_MINUS)
			return (write(1, &bufferz[0], 1) +
					write(1, &bufferz[BUFF_SIZE - i - 1], widthz - 1));
		else
			return (write(1, &bufferz[BUFF_SIZE - i - 1], widthz - 1) +
					write(1, &bufferz[0], 1));
	}

	return (write(1, &bufferz[0], 1));
}


/**
 * write_number -This Prints a string
 * @is_negativez: List of arguments
 * @ind: char types.
 * @bufferz: Buffer array to handle print
 * @flagsz:  Calculates the active flags
 * @widthz: The get width.
 * @precisionz: The precision of specifier
 * @sizez:The Size of specifier
 *
 * Return: Number of chars printed.
 */
int write_number(int is_negativez, int ind, char bufferz[],
	int flagsz, int widthz, int precisionz, int sizez)
{
	int length = BUFF_SIZE - ind - 1;
	char bed = ' ', extra_ch = 0;

	UNUSED(sizez);

	if ((flagsz & F_ZERO) && !(flagsz & F_MINUS))
		bed = '0';
	if (is_negativez)
		extra_ch = '-';
	else if (flagsz & F_PLUS)
		extra_ch = '+';
	else if (flagsz & F_SPACE)
		extra_ch = ' ';

	return (write_num(ind, bufferz, flagsz, widthz, precisionz,
		length, bed, extra_ch));
}

/**
 * write_num - This Write a number using a bufffer
 * @ind: Number at which the number starts on the buffer
 * @bufferz: This is the Buffer
 * @flagsz: This is Flags
 * @widthz: This is the width
 * @precz: This is the Precision specifier
 * @lengthz: This is the Number length
 * @paddz: This is the Pading char
 * @extra_c: This is the Extra char
 *
 * Return: Number of printed chars.
 */
int write_num(int ind, char bufferz[],
	int flagsz, int widthz, int precz,
	int lengthz, char paddz, char extra_c)
{
	int i, paddz_start = 1;

	if (precz == 0 && ind == BUFF_SIZE - 2 && bufferz[ind] == '0' && widthz == 0)
		return (0); /* printf(".0d", 0)  no char is printed */
	if (precz == 0 && ind == BUFF_SIZE - 2 && bufferz[ind] == '0')
		bufferz[ind] = paddz = ' '; /* width is displayed with padding ' ' */
	if (precz > 0 && precz < lengthz)
		paddz = ' ';
	while (precz > lengthz)
		bufferz[--ind] = '0', lengthz++;
	if (extra_c != 0)
		lengthz++;
	if (widthz > lengthz)
	{
		for (i = 1; i < widthz - lengthz + 1; i++)
			bufferz[i] = paddz;
		bufferz[i] = '\0';
		if (flagsz & F_MINUS && paddz == ' ')/* Asign extra char to left of buffer */
		{
			if (extra_c)
				bufferz[--ind] = extra_c;
			return (write(1, &bufferz[ind], lengthz) + write(1, &bufferz[1], i - 1));
		}
		else if (!(flagsz & F_MINUS) && paddz == ' ')/* extra char to left of buff */
		{
			if (extra_c)
				bufferz[--ind] = extra_c;
			return (write(1, &bufferz[1], i - 1) + write(1, &bufferz[ind], lengthz));
		}
		else if (!(flagsz & F_MINUS) && paddz == '0')/* extra char to left of padd */
		{
			if (extra_c)
				bufferz[--paddz_start] = extra_c;
			return (write(1, &bufferz[paddz_start], i - paddz_start) +
				write(1, &bufferz[ind], lengthz - (1 - paddz_start)));
		}
	}
	if (extra_c)
		bufferz[--ind] = extra_c;
	return (write(1, &bufferz[ind], lengthz));
}

/**
 * write_unsgnd - This Writes an unsigned number
 * @is_negativez: Number indicating if the number is negative
 * @ind: Number at which the number starts in the buffer
 * @bufferz: Array of chars
 * @flagsz: The Flags of specifiers
 * @widthz: The Width of specifier
 * @precisionz: The Precision of specifier
 * @sizez:The Size of specifier
 *
 * Return: Number of written chars.
 */
int write_unsgnd(int is_negativez, int ind,
	char bufferz[],
	int flagsz, int widthz, int precisionz, int sizez)
{
	/* The number is stored at the bufer's right and starts at position i */
	int length = BUFF_SIZE - ind - 1, i = 0;
	char paddz = ' ';

	UNUSED(is_negativez);
	UNUSED(sizez);

	if (precisionz == 0 && ind == BUFF_SIZE - 2 && bufferz[ind] == '0')
		return (0); /* printf(".0d", 0)  no char is printed */

	if (precisionz > 0 && precisionz < length)
		paddz = ' ';

	while (precisionz > length)
	{
		bufferz[--ind] = '0';
		length++;
	}

	if ((flagsz & F_ZERO) && !(flagsz & F_MINUS))
		paddz = '0';

	if (widthz > length)
	{
		for (i = 0; i < widthz - length; i++)
			bufferz[i] = paddz;

		bufferz[i] = '\0';

		if (flagsz & F_MINUS) /* Asign extra char to left of buffer [buffer>padd]*/
		{
			return (write(1, &bufferz[ind], length) + write(1, &bufferz[0], i));
		}
		else /* Asign extra char to left of padding [padd>buffer]*/
		{
			return (write(1, &bufferz[0], i) + write(1, &bufferz[ind], length));
		}
	}

	return (write(1, &bufferz[ind], length));
}

/**
 * write_pointer - Write a memory address
 * @bufferz: The Arrays of chars
 * @ind: Number at which the number starts in the buffer
 * @lengthz: Length of number
 * @widthz: The Width of specifier
 * @flagsz: The Flags of specifier
 * @paddz: The Char representing the padding
 * @extra_c: The Char representing extra char
 * @paddz_start: Number where padding should start
 *
 * Return: Number of written chars.
 */
int write_pointer(char bufferz[], int ind, int lengthz,
	int widthz, int flagsz, char paddz, char extra_c, int paddz_start)
{
	int i;

	if (widthz > lengthz)
	{
	for (i = 3; i < widthz - lengthz + 3; i++)
		bufferz[i] = paddz;
	bufferz[i] = '\0';
	if (flagsz & F_MINUS && paddz == ' ')/* Asign extra char of buffer */
	{
	bufferz[--ind] = 'x';
	bufferz[--ind] = '0';
	if (extra_c)
		bufferz[--ind] = extra_c;
	return (write(1, &bufferz[ind], lengthz) + write(1, &bufferz[3], i - 3));
	}
	else if (!(flagsz & F_MINUS) && paddz == ' ')/* extra char of buffer */
	{
	bufferz[--ind] = 'x';
	bufferz[--ind] = '0';
	if (extra_c)
		bufferz[--ind] = extra_c;
	return (write(1, &bufferz[3], i - 3) + write(1, &bufferz[ind], lengthz));
	}
	else if (!(flagsz & F_MINUS) && paddz == '0')/* extra char to padd */
	{
	if (extra_c)
		bufferz[--paddz_start] = extra_c;
	bufferz[1] = '0';
	bufferz[2] = 'x';
	return (write(1, &bufferz[paddz_start], i - paddz_start) +
		write(1, &bufferz[ind], lengthz - (1 - paddz_start) - 2));
	}}
	bufferz[--ind] = 'x';
	bufferz[--ind] = '0';
	if (extra_c)
		bufferz[--ind] = extra_c;
	return (write(1, &bufferz[ind], BUFF_SIZE - ind - 1));
}
