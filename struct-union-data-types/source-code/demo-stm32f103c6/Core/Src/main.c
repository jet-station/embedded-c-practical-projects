
#include <stdbool.h> /* Standard bool data types */
#include <stdint.h> /* Standard integer data types */

/* Example of union data type for register access */
typedef struct {
	uint32_t bit0 : 1;
	uint32_t bit1 : 1;
	uint32_t bit2 : 1;
	uint32_t bit3 : 1;
	uint32_t reserved : 28;
} RegisterBits_st; /* Access individual bits */ 

typedef union {
	uint32_t register_u32; /* Access the entire register */
	RegisterBits_st bits_st; /* Access individual bits */
	uint8_t registerBytes_u8[4]; /* Access as bytes */
	uint16_t registerHalfWords_u16[2]; /* Access as half-words */
} RegisterType_un;

/* Example of struct data type for a rectangle shape */
typedef struct __attribute__((packed)) {
	uint16_t width_u16;
	uint32_t area_u32;
	uint16_t height_u16;
} Rectangle_st;

int main(void)
{

	/* Size of the struct in bytes */ 
	volatile uint8_t stDataSize_u8 = sizeof(Rectangle_st);

	/* Size of the union in bytes */
	volatile uint8_t unDataSize_u8 = sizeof(RegisterType_un);

	/* Struct initialization */
	Rectangle_st rect_st = {10U, 5U, 0U};

	/* Calculate area and update the struct element */
	rect_st.area_u32 = rect_st.width_u16 * rect_st.height_u16;


	/* Union declaration */
	RegisterType_un data_un;

	/* Set the entire register value */
	data_un.register_u32 = 0x12345678U;

	/* Access individual bits */
	data_un.bits_st.bit0 = 1;
	data_un.bits_st.bit1 = 0;
	data_un.bits_st.bit2 = 1;
	data_un.bits_st.bit3 = 0;

	/* Access and update as bytes */
	uint8_t byte2 = data_un.registerBytes_u8[2];
	data_un.registerBytes_u8[3] = 0xEF;

	/* Access and update as half-words */
	uint16_t halfWord0 = data_un.registerHalfWords_u16[0];
	data_un.registerHalfWords_u16[1] = 0xABCD;

	while (1)
	{
		/* do nothing here */
	}

	return 0;
}
