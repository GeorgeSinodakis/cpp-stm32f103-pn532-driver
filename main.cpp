#include <stdint.h>
#include <string>
#include "nvic.h"
#include "tim2345.h"
#include "delay.h"
#include "rcc.h"
#include "gpio.h"
#include "st7735.h"
#include "pn532.h"

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

template <typename I> std::string toHexString(I value)
{
	std::string output = "0x";
	for (int nibble = sizeof(I) * 2 - 1; nibble >= 0; nibble--)
	{
		output += "0123456789ABCDEF"[(value >> (nibble * 4)) & 0x0f];
	}
	return output;
}

int main(void)
{
	clock_72Mhz();

	st7735_init(4);
	st7735_fill(BLACK);

	pn532_begin();

	while (1)
	{
		u32 id = pn532_readPassiveTargetID(PN532_MIFARE_ISO14443A);
		if(id)
		{
			st7735_fill(BLACK);
			st7735_print_str(5, 5, 2, WHITE, BLACK, &toHexString(id));
		}
		ms(2000);
	}
}