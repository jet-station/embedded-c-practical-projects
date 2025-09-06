#include <stdbool.h> /* Standard bool data types */
#include <stdint.h> /* Standard integer data types */

int main(void)
{
	/* C standard data types */
	const uint8_t C_DataSizeInBytes_u8[] = {
		sizeof(char),
		sizeof(unsigned char),
		sizeof(int),
		sizeof(short),
		sizeof(long),
		sizeof(float),
		sizeof(double),
		sizeof(unsigned int)
	};
	
	/* Embedded C standard data types */
	const uint8_t EmbeddedC_DataSizeInBytes_u8[] = {
		sizeof(bool),
		sizeof(int8_t),
		sizeof(uint8_t),
		sizeof(int16_t),
		sizeof(uint16_t),
		sizeof(int32_t),
		sizeof(uint32_t)
	};

	while (1)
	{

	}

	return 0;
}
