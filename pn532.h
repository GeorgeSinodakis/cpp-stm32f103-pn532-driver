#include <cstring>
#include "spi.h"

#define PN532_PACKBUFFSIZ					64
#define PN532_PREAMBLE                      (0x00)
#define PN532_STARTCODE1                    (0x00)
#define PN532_STARTCODE2                    (0xFF)
#define PN532_POSTAMBLE                     (0x00)
#define PN532_HOSTTOPN532                   (0xD4)
#define PN532_PN532TOHOST                   (0xD5)
#define PN532_COMMAND_DIAGNOSE              (0x00)
#define PN532_COMMAND_GETFIRMWAREVERSION    (0x02)
#define PN532_COMMAND_GETGENERALSTATUS      (0x04)
#define PN532_COMMAND_READREGISTER          (0x06)
#define PN532_COMMAND_WRITEREGISTER         (0x08)
#define PN532_COMMAND_READGPIO              (0x0C)
#define PN532_COMMAND_WRITEGPIO             (0x0E)
#define PN532_COMMAND_SETSERIALBAUDRATE     (0x10)
#define PN532_COMMAND_SETPARAMETERS         (0x12)
#define PN532_COMMAND_SAMCONFIGURATION      (0x14)
#define PN532_COMMAND_POWERDOWN             (0x16)
#define PN532_COMMAND_RFCONFIGURATION       (0x32)
#define PN532_COMMAND_RFREGULATIONTEST      (0x58)
#define PN532_COMMAND_INJUMPFORDEP          (0x56)
#define PN532_COMMAND_INJUMPFORPSL          (0x46)
#define PN532_COMMAND_INLISTPASSIVETARGET   (0x4A)
#define PN532_COMMAND_INATR                 (0x50)
#define PN532_COMMAND_INPSL                 (0x4E)
#define PN532_COMMAND_INDATAEXCHANGE        (0x40)
#define PN532_COMMAND_INCOMMUNICATETHRU     (0x42)
#define PN532_COMMAND_INDESELECT            (0x44)
#define PN532_COMMAND_INRELEASE             (0x52)
#define PN532_COMMAND_INSELECT              (0x54)
#define PN532_COMMAND_INAUTOPOLL            (0x60)
#define PN532_COMMAND_TGINITASTARGET        (0x8C)
#define PN532_COMMAND_TGSETGENERALBYTES     (0x92)
#define PN532_COMMAND_TGGETDATA             (0x86)
#define PN532_COMMAND_TGSETDATA             (0x8E)
#define PN532_COMMAND_TGSETMETADATA         (0x94)
#define PN532_COMMAND_TGGETINITIATORCOMMAND (0x88)
#define PN532_COMMAND_TGRESPONSETOINITIATOR (0x90)
#define PN532_COMMAND_TGGETTARGETSTATUS     (0x8A)
#define PN532_RESPONSE_INDATAEXCHANGE       (0x41)
#define PN532_RESPONSE_INLISTPASSIVETARGET  (0x4B)
#define PN532_WAKEUP                        (0x55)
#define PN532_SPI_STATREAD                  (0x02)
#define PN532_SPI_DATAWRITE                 (0x01)
#define PN532_SPI_DATAREAD                  (0x03)
#define PN532_SPI_READY                     (0x01)
#define PN532_I2C_ADDRESS                   (0x48 >> 1)
#define PN532_I2C_READBIT                   (0x01)
#define PN532_I2C_BUSY                      (0x00)
#define PN532_I2C_READY                     (0x01)
#define PN532_I2C_READYTIMEOUT              (20)
#define PN532_MIFARE_ISO14443A              (0x00)
#define MIFARE_CMD_AUTH_A                   (0x60)
#define MIFARE_CMD_AUTH_B                   (0x61)
#define MIFARE_CMD_READ                     (0x30)
#define MIFARE_CMD_WRITE                    (0xA0)
#define MIFARE_CMD_TRANSFER                 (0xB0)
#define MIFARE_CMD_DECREMENT                (0xC0)
#define MIFARE_CMD_INCREMENT                (0xC1)
#define MIFARE_CMD_STORE                    (0xC2)
#define MIFARE_ULTRALIGHT_CMD_WRITE         (0xA2)
#define NDEF_URIPREFIX_NONE                 (0x00)
#define NDEF_URIPREFIX_HTTP_WWWDOT          (0x01)
#define NDEF_URIPREFIX_HTTPS_WWWDOT         (0x02)
#define NDEF_URIPREFIX_HTTP                 (0x03)
#define NDEF_URIPREFIX_HTTPS                (0x04)
#define NDEF_URIPREFIX_TEL                  (0x05)
#define NDEF_URIPREFIX_MAILTO               (0x06)
#define NDEF_URIPREFIX_FTP_ANONAT           (0x07)
#define NDEF_URIPREFIX_FTP_FTPDOT           (0x08)
#define NDEF_URIPREFIX_FTPS                 (0x09)
#define NDEF_URIPREFIX_SFTP                 (0x0A)
#define NDEF_URIPREFIX_SMB                  (0x0B)
#define NDEF_URIPREFIX_NFS                  (0x0C)
#define NDEF_URIPREFIX_FTP                  (0x0D)
#define NDEF_URIPREFIX_DAV                  (0x0E)
#define NDEF_URIPREFIX_NEWS                 (0x0F)
#define NDEF_URIPREFIX_TELNET               (0x10)
#define NDEF_URIPREFIX_IMAP                 (0x11)
#define NDEF_URIPREFIX_RTSP                 (0x12)
#define NDEF_URIPREFIX_URN                  (0x13)
#define NDEF_URIPREFIX_POP                  (0x14)
#define NDEF_URIPREFIX_SIP                  (0x15)
#define NDEF_URIPREFIX_SIPS                 (0x16)
#define NDEF_URIPREFIX_TFTP                 (0x17)
#define NDEF_URIPREFIX_BTSPP                (0x18)
#define NDEF_URIPREFIX_BTL2CAP              (0x19)
#define NDEF_URIPREFIX_BTGOEP               (0x1A)
#define NDEF_URIPREFIX_TCPOBEX              (0x1B)
#define NDEF_URIPREFIX_IRDAOBEX             (0x1C)
#define NDEF_URIPREFIX_FILE                 (0x1D)
#define NDEF_URIPREFIX_URN_EPC_ID           (0x1E)
#define NDEF_URIPREFIX_URN_EPC_TAG          (0x1F)
#define NDEF_URIPREFIX_URN_EPC_PAT          (0x20)
#define NDEF_URIPREFIX_URN_EPC_RAW          (0x21)
#define NDEF_URIPREFIX_URN_EPC              (0x22)
#define NDEF_URIPREFIX_URN_NFC              (0x23)
#define PN532_GPIO_P30                      (0)
#define PN532_GPIO_VALIDATIONBIT            (0x80)
#define PN532_GPIO_P31                      (1)
#define PN532_GPIO_P32                      (2)
#define PN532_GPIO_P33                      (3)
#define PN532_GPIO_P35                      (5)
#define PN532_GPIO_P34                      (4)

volatile uint8_t pn532_packetbuffer[PN532_PACKBUFFSIZ];
uint8_t pn532ack[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
uint8_t pn532response_firmwarevers[] = {0x00, 0xFF, 0x06, 0xFA, 0xD5, 0x03};
volatile uint32_t cards[9] = {0x50c72d52, 0x80f03752, 0x200e2052, 0x9392d66a, 0x839fde6a, 0x53a6dd6a, 0x70862e52, 0x101b2752, 0x12592a13}; //ta id twn kartwn

void pn532_readspidata(uint8_t* buff, uint8_t n)
{
	SPI2_assert(); // write LOW to CS pin
	us(2000);
	SPI2_tranceiver(PN532_SPI_DATAREAD);

	for (uint8_t i = 0; i < n; i++)
	{
		us(1000);
		buff[i] = SPI2_tranceiver(0);
	}

	SPI2_deassert(); // write HIGH to CS pin
}

uint8_t pn532_spi_readack(void)
{
	uint8_t ackbuff[6];

	pn532_readspidata(ackbuff, 6);

	return (0 == strncmp((char*) ackbuff, (char*) pn532ack, 6)) ? 1 : 0;
}

uint8_t pn532_readspistatus(void)
{
	SPI2_assert(); // write LOW to CS pin
	us(2000);

	SPI2_tranceiver(PN532_SPI_STATREAD);

	uint8_t s = SPI2_tranceiver(0);

	SPI2_deassert(); // write HIGH to CS pin
	return s;
}

void pn532_spiwritecommand(uint8_t* cmd, uint8_t cmdlen)
{
	uint8_t checksum;
	cmdlen++;
	SPI2_assert(); // write LOW to CS pin
	us(2000);

	SPI2_tranceiver(PN532_SPI_DATAWRITE);
	checksum = PN532_PREAMBLE + PN532_PREAMBLE + PN532_STARTCODE2;
	SPI2_tranceiver(PN532_PREAMBLE);
	SPI2_tranceiver(PN532_PREAMBLE);
	SPI2_tranceiver(PN532_STARTCODE2);
	SPI2_tranceiver(cmdlen);
	SPI2_tranceiver(~cmdlen + 1);
	SPI2_tranceiver(PN532_HOSTTOPN532);

	checksum += PN532_HOSTTOPN532;
	for (uint8_t i = 0; i < cmdlen - 1; i++)
	{
		SPI2_tranceiver(cmd[i]);
		checksum += cmd[i];
	}

	SPI2_tranceiver(~checksum);
	SPI2_tranceiver(PN532_POSTAMBLE);
	SPI2_deassert(); // write HIGH to CS pin
}

uint8_t pn532_sendCommandCheckAck(uint8_t *cmd, uint8_t cmdlen, uint16_t timeout)
{
	uint16_t timer = 0;

	// write the command
	pn532_spiwritecommand(cmd, cmdlen);

	// wait for the chip to say it's ready
	while (pn532_readspistatus() != PN532_SPI_READY)
	{
		if (timeout != 0)
		{
			timer += 10;
			if (timer > timeout)
			{
				return 0;
			}
		}
		ms(10);
	}

	// read acknowledgement
	if (0 == pn532_spi_readack())
	{
		return 0;
	}

	timer = 0;
	// wait for the chip to say it's ready
	while (pn532_readspistatus() != PN532_SPI_READY)
	{
		if (timeout != 0)
		{
			timer += 10;
			if (timer > timeout)
			{
				return 0;
			}
		}
		ms(10);
	}

	return 1;
}

uint8_t pn532_getFirmwareVersion(void)
{
	pn532_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;

	if (0 == pn532_sendCommandCheckAck(pn532_packetbuffer, 1, 1000)) return 0;

	// read data packet
	pn532_readspidata(pn532_packetbuffer, 12);

	if (pn532_packetbuffer[6] != 50) return 0;
	if (pn532_packetbuffer[7] != 1) return 0;
	if (pn532_packetbuffer[8] != 6) return 0;
	if (pn532_packetbuffer[9] != 7) return 0;

	return 1;
}

uint8_t pn532_SAMConfig(void) {
	pn532_packetbuffer[0] = PN532_COMMAND_SAMCONFIGURATION;
	pn532_packetbuffer[1] = 0x01; // normal mode
	pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
	pn532_packetbuffer[3] = 0x01; // use IRQ pin!

	if (0 == pn532_sendCommandCheckAck(pn532_packetbuffer, 4, 1000)) return 0;

	// read data packet
	pn532_readspidata(pn532_packetbuffer, 8);

	return (pn532_packetbuffer[5] == 0x15);
}

uint32_t pn532_readPassiveTargetID(uint8_t cardbaudrate)
{
	uint32_t i = 0;
	pn532_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
	pn532_packetbuffer[1] = 1; // max 1 card at once
	pn532_packetbuffer[2] = cardbaudrate;

	if (0 == pn532_sendCommandCheckAck(pn532_packetbuffer, 3, 1000)) return 0x00; // no cards read

	// read data packet
	pn532_readspidata(pn532_packetbuffer, 20);

	if (pn532_packetbuffer[7] != 1) return 0;

	i = pn532_packetbuffer[13];
	i <<= 8;
	i |= pn532_packetbuffer[14];
	i <<= 8;
	i |= pn532_packetbuffer[15];
	i <<= 8;
	i |= pn532_packetbuffer[16];

	return i;
}

uint8_t pn532_authenticateBlock(uint8_t cardnumber, uint32_t cid, uint8_t blockaddress, uint8_t authtype, uint8_t* keys)
{
	pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
	pn532_packetbuffer[1] = cardnumber;

	if (authtype == MIFARE_CMD_AUTH_A)
	{
		pn532_packetbuffer[2] = MIFARE_CMD_AUTH_A;
	}
	else
	{
		pn532_packetbuffer[2] = MIFARE_CMD_AUTH_B;
	}
	pn532_packetbuffer[3] = blockaddress; // Address can be 0-63 for MIFARE 1K card

	pn532_packetbuffer[4] = keys[0];
	pn532_packetbuffer[5] = keys[1];
	pn532_packetbuffer[6] = keys[2];
	pn532_packetbuffer[7] = keys[3];
	pn532_packetbuffer[8] = keys[4];
	pn532_packetbuffer[9] = keys[5];

	pn532_packetbuffer[10] = ((cid >> 24) & 0xFF);
	pn532_packetbuffer[11] = ((cid >> 16) & 0xFF);
	pn532_packetbuffer[12] = ((cid >> 8) & 0xFF);
	pn532_packetbuffer[13] = ((cid >> 0) & 0xFF);

	if (0 == pn532_sendCommandCheckAck(pn532_packetbuffer, 14, 1000)) return 0;

	// read data packet
	pn532_readspidata(pn532_packetbuffer, 2 + 6);

	if ((pn532_packetbuffer[6] = 0x41) && (pn532_packetbuffer[7] == 0x00))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t pn532_readMemoryBlock(uint8_t cardnumber, uint8_t blockaddress, uint8_t* block)
{
	pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
	pn532_packetbuffer[1] = cardnumber;
	pn532_packetbuffer[2] = MIFARE_CMD_READ;
	pn532_packetbuffer[3] = blockaddress;

	if (0 == pn532_sendCommandCheckAck(pn532_packetbuffer, 4, 1000)) return 0;

	// read data packet
	pn532_readspidata(pn532_packetbuffer, 18 + 6);
	for (uint8_t i = 8; i < 18 + 6; i++)
	{
		block[i - 8] = pn532_packetbuffer[i];
	}

	if ((pn532_packetbuffer[6] == 0x41) && (pn532_packetbuffer[7] == 0x00))
	{
		return 1; // read successful
	}
	else
	{
		return 0;
	}
}

uint8_t pn532_writeMemoryBlock(uint8_t cardnumber, uint8_t blockaddress, uint8_t* block)
{
	pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
	pn532_packetbuffer[1] = cardnumber;
	pn532_packetbuffer[2] = MIFARE_CMD_WRITE;
	pn532_packetbuffer[3] = blockaddress;

	for (uint8_t byte = 0; byte < 16; byte++)
	{
		pn532_packetbuffer[4 + byte] = block[byte];
	}

	if (0 == pn532_sendCommandCheckAck(pn532_packetbuffer, 4 + 16, 1000)) return 0;

	// read data packet
	pn532_readspidata(pn532_packetbuffer, 2 + 6);

	if ((pn532_packetbuffer[6] == 0x41) && (pn532_packetbuffer[7] == 0x00))
	{
		return 1; // write successful
	}
	else
	{
		return 0;
	}
}

uint8_t pn532_begin(void)
{
    SPI2_enable(true, false, false, 16);
	SPI2_assert();
	ms(1000);
	pn532_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;
	pn532_sendCommandCheckAck(pn532_packetbuffer, 1, 1000);
	SPI2_deassert();
	if (1 != pn532_getFirmwareVersion())
	{
		return 1; //getFirmwareVersion error
	}
	if (1 != pn532_SAMConfig())
	{
		return 2; //SAMConfig error
	}
	return 0;
}