/**
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * |
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32f4_ili9341.h"

/**
 * @brief  Orientation
 * @note   Used private
 */
typedef enum {
	TM_ILI9341_Landscape,
	TM_ILI9341_Portrait
} TM_ILI9341_Orientation;

/**
 * @brief  LCD options
 * @note   Used private
 */
typedef struct {
	uint16_t width;
	uint16_t height;
	TM_ILI9341_Orientation orientation; // 1 = portrait; 0 = landscape
} TM_ILI931_Options_t;

/* Pin definitions */
#define ILI9341_RST_SET				GPIO_SetBits(ILI9341_RST_PORT, ILI9341_RST_PIN)
#define ILI9341_RST_RESET			GPIO_ResetBits(ILI9341_RST_PORT, ILI9341_RST_PIN)
#define ILI9341_CS_SET				GPIO_SetBits(ILI9341_CS_PORT, ILI9341_CS_PIN)
#define ILI9341_CS_RESET			GPIO_ResetBits(ILI9341_CS_PORT, ILI9341_CS_PIN)
#define ILI9341_WRX_SET				GPIO_SetBits(ILI9341_WRX_PORT, ILI9341_WRX_PIN)
#define ILI9341_WRX_RESET			GPIO_ResetBits(ILI9341_WRX_PORT, ILI9341_WRX_PIN)

/* Private defines */
#define ILI9341_RESET				0x01
#define ILI9341_SLEEP_OUT			0x11
#define ILI9341_GAMMA				0x26
#define ILI9341_DISPLAY_OFF			0x28
#define ILI9341_DISPLAY_ON			0x29
#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_MAC					0x36
#define ILI9341_PIXEL_FORMAT		0x3A
#define ILI9341_WDB					0x51
#define ILI9341_WCD					0x53
#define ILI9341_RGB_INTERFACE		0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC					0xB6
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1				0xC5
#define ILI9341_VCOM2				0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA				0xE8
#define ILI9341_DTCB				0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC					0xF7

extern void Delay(__IO uint32_t nTime);

/* Pin functions */
uint16_t ILI9341_x;
uint16_t ILI9341_y;
TM_ILI931_Options_t ILI9341_Opts;
uint8_t ILI9341_INT_CalledFromPuts = 0;

/* Private functions */
void TM_ILI9341_InitLCD(void);
void TM_ILI9341_SendData(uint8_t data);
void TM_ILI9341_SendCommand(uint8_t data);
void TM_ILI9341_Delay(volatile unsigned int delay);
void TM_ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void TM_ILI9341_INT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

void TM_Test()
{
	/* Init WRX pin */
	TM_GPIO_Init(ILI9341_WRX_PORT, ILI9341_WRX_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Medium);

	/* Init CS pin */
	TM_GPIO_Init(ILI9341_CS_PORT, ILI9341_CS_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Medium);

	/* Init RST pin */
	TM_GPIO_Init(ILI9341_RST_PORT, ILI9341_RST_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low);

        TM_ILI9341_Delay(20000);

        while (0) {
          ILI9341_CS_SET;
          ILI9341_RST_SET;
          ILI9341_WRX_SET;
        TM_ILI9341_Delay(20000);
        ILI9341_CS_RESET;
          ILI9341_RST_RESET;
          ILI9341_WRX_RESET;
        TM_ILI9341_Delay(20000);
        }

}

void TM_ILI9341_Init() {
	/* Init WRX pin */
	TM_GPIO_Init(ILI9341_WRX_PORT, ILI9341_WRX_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Medium);

	/* Init CS pin */
	TM_GPIO_Init(ILI9341_CS_PORT, ILI9341_CS_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Medium);

	/* Init RST pin */
	TM_GPIO_Init(ILI9341_RST_PORT, ILI9341_RST_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Low);

	/* CS high */
	ILI9341_CS_SET;

	/* Init SPI */
	TM_SPI_Init(ILI9341_SPI, ILI9341_SPI_PINS);

	/* Init DMA for SPI */
	TM_SPI_DMA_Init(ILI9341_SPI);

	/* Init LCD */
	TM_ILI9341_InitLCD();

	/* Set default settings */
	ILI9341_x = ILI9341_y = 0;
	ILI9341_Opts.width = ILI9341_WIDTH;
	ILI9341_Opts.height = ILI9341_HEIGHT;
	ILI9341_Opts.orientation = TM_ILI9341_Portrait;
//	ILI9341_Opts.orientation = TM_ILI9341_Landscape;

	/* Fill with white color */
	TM_ILI9341_Fill(ILI9341_COLOR_CYAN);
//        TM_ILI9341_INT_Fill(0, 0, 320 - 1, 480, ILI9341_COLOR_RED);
}

void TM_ILI9341_InitLCD(void) {
	/* Force reset */
	ILI9341_RST_RESET;
	TM_ILI9341_Delay(20000);
	ILI9341_RST_SET;

	/* Delay for RST response */
	TM_ILI9341_Delay(20000);

	/* Software reset */
	TM_ILI9341_SendCommand(ILI9341_RESET);
	TM_ILI9341_Delay(50000);
        while (0) {
	TM_ILI9341_SendCommand(ILI9341_POWERA);
	TM_ILI9341_Delay(20000);
        }
#if 0
	TM_ILI9341_SendCommand(ILI9341_POWERA);
	TM_ILI9341_SendData(0x39);
	TM_ILI9341_SendData(0x2C);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x34);
	TM_ILI9341_SendData(0x02);
	TM_ILI9341_SendCommand(ILI9341_POWERB);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0xC1);
	TM_ILI9341_SendData(0x30);
	TM_ILI9341_SendCommand(ILI9341_DTCA);
	TM_ILI9341_SendData(0x85);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x78);
	TM_ILI9341_SendCommand(ILI9341_DTCB);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendCommand(ILI9341_POWER_SEQ);
	TM_ILI9341_SendData(0x64);
	TM_ILI9341_SendData(0x03);
	TM_ILI9341_SendData(0x12);
	TM_ILI9341_SendData(0x81);
	TM_ILI9341_SendCommand(ILI9341_PRC);
	TM_ILI9341_SendData(0x20);
	TM_ILI9341_SendCommand(ILI9341_POWER1);
	TM_ILI9341_SendData(0x23);
	TM_ILI9341_SendCommand(ILI9341_POWER2);
	TM_ILI9341_SendData(0x10);
	TM_ILI9341_SendCommand(ILI9341_VCOM1);
	TM_ILI9341_SendData(0x3E);
	TM_ILI9341_SendData(0x28);
	TM_ILI9341_SendCommand(ILI9341_VCOM2);
	TM_ILI9341_SendData(0x86);
	TM_ILI9341_SendCommand(ILI9341_MAC);
	TM_ILI9341_SendData(0x48);
	TM_ILI9341_SendCommand(ILI9341_PIXEL_FORMAT);
	TM_ILI9341_SendData(0x55);
	TM_ILI9341_SendCommand(ILI9341_FRC);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x18);
	TM_ILI9341_SendCommand(ILI9341_DFC);
	TM_ILI9341_SendData(0x08);
	TM_ILI9341_SendData(0x82);
	TM_ILI9341_SendData(0x27);
	TM_ILI9341_SendCommand(ILI9341_3GAMMA_EN);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0xEF);
	TM_ILI9341_SendCommand(ILI9341_PAGE_ADDR);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x01);
	TM_ILI9341_SendData(0x3F);
	TM_ILI9341_SendCommand(ILI9341_GAMMA);
	TM_ILI9341_SendData(0x01);
	TM_ILI9341_SendCommand(ILI9341_PGAMMA);
	TM_ILI9341_SendData(0x0F);
	TM_ILI9341_SendData(0x31);
	TM_ILI9341_SendData(0x2B);
	TM_ILI9341_SendData(0x0C);
	TM_ILI9341_SendData(0x0E);
	TM_ILI9341_SendData(0x08);
	TM_ILI9341_SendData(0x4E);
	TM_ILI9341_SendData(0xF1);
	TM_ILI9341_SendData(0x37);
	TM_ILI9341_SendData(0x07);
	TM_ILI9341_SendData(0x10);
	TM_ILI9341_SendData(0x03);
	TM_ILI9341_SendData(0x0E);
	TM_ILI9341_SendData(0x09);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendCommand(ILI9341_NGAMMA);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x0E);
	TM_ILI9341_SendData(0x14);
	TM_ILI9341_SendData(0x03);
	TM_ILI9341_SendData(0x11);
	TM_ILI9341_SendData(0x07);
	TM_ILI9341_SendData(0x31);
	TM_ILI9341_SendData(0xC1);
	TM_ILI9341_SendData(0x48);
	TM_ILI9341_SendData(0x08);
	TM_ILI9341_SendData(0x0F);
	TM_ILI9341_SendData(0x0C);
	TM_ILI9341_SendData(0x31);
	TM_ILI9341_SendData(0x36);
	TM_ILI9341_SendData(0x0F);
	TM_ILI9341_SendCommand(ILI9341_SLEEP_OUT);

	TM_ILI9341_Delay(1000000);

	TM_ILI9341_SendCommand(ILI9341_DISPLAY_ON);
	TM_ILI9341_SendCommand(ILI9341_GRAM);


#elif 1         // works!


#define LCD_ILI9488_CMD         TM_ILI9341_SendCommand
#define LCD_ILI9488_Parameter   TM_ILI9341_SendData

//-----------------CMI------------
LCD_ILI9488_CMD(0xE0);
LCD_ILI9488_Parameter(0x00);
LCD_ILI9488_Parameter(0x13);
LCD_ILI9488_Parameter(0x18);
LCD_ILI9488_Parameter(0x04);
LCD_ILI9488_Parameter(0x0F);
LCD_ILI9488_Parameter(0x06);
LCD_ILI9488_Parameter(0x3A);
LCD_ILI9488_Parameter(0x56);
LCD_ILI9488_Parameter(0x4D);
LCD_ILI9488_Parameter(0x03);
LCD_ILI9488_Parameter(0x0A);
LCD_ILI9488_Parameter(0x06);
LCD_ILI9488_Parameter(0x30);
LCD_ILI9488_Parameter(0x3E);
LCD_ILI9488_Parameter(0x0F);

LCD_ILI9488_CMD(0xE1);
LCD_ILI9488_Parameter(0x00);
LCD_ILI9488_Parameter(0x13);
LCD_ILI9488_Parameter(0x18);
LCD_ILI9488_Parameter(0x01);
LCD_ILI9488_Parameter(0x11);
LCD_ILI9488_Parameter(0x06);
LCD_ILI9488_Parameter(0x38);
LCD_ILI9488_Parameter(0x34);
LCD_ILI9488_Parameter(0x4D);
LCD_ILI9488_Parameter(0x06);
LCD_ILI9488_Parameter(0x0D);
LCD_ILI9488_Parameter(0x0B);
LCD_ILI9488_Parameter(0x31);
LCD_ILI9488_Parameter(0x37);
LCD_ILI9488_Parameter(0x0f);

LCD_ILI9488_CMD(0xC0);
LCD_ILI9488_Parameter(0x10);
LCD_ILI9488_Parameter(0x10);

LCD_ILI9488_CMD(0xC1);
LCD_ILI9488_Parameter(0x41);

LCD_ILI9488_CMD(0xC5);
LCD_ILI9488_Parameter(0x00);
LCD_ILI9488_Parameter(0x2c); // VCOM
LCD_ILI9488_Parameter(0x80);


LCD_ILI9488_CMD(0x36);
LCD_ILI9488_Parameter(0xC8);

LCD_ILI9488_CMD(0x3A);  //Interface Mode Control
LCD_ILI9488_Parameter(0x51);

LCD_ILI9488_CMD(0XB0);  //Interface Mode Control
LCD_ILI9488_Parameter(0x00);
LCD_ILI9488_CMD(0xB1);   //Frame rate 70HZ
LCD_ILI9488_Parameter(0xB0);
LCD_ILI9488_Parameter(0x11);

LCD_ILI9488_CMD(0xB4);
LCD_ILI9488_Parameter(0x02);

LCD_ILI9488_CMD(0xB6);  //RGB/MCU Interface Control
LCD_ILI9488_Parameter(0x00);
LCD_ILI9488_Parameter(0x22);
LCD_ILI9488_Parameter(0x3B);

LCD_ILI9488_CMD(0xE9);
LCD_ILI9488_Parameter(0x00);

LCD_ILI9488_CMD(0XF7);
LCD_ILI9488_Parameter(0xA9);
LCD_ILI9488_Parameter(0x51);
LCD_ILI9488_Parameter(0x2C);
LCD_ILI9488_Parameter(0x82);

LCD_ILI9488_CMD(0x11);
	Delay(15);
LCD_ILI9488_CMD(0x29);

#elif 1 //

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

	TM_ILI9341_SendCommand(0xB0); //Interface Mode Control
	TM_ILI9341_SendData(0x00);

	TM_ILI9341_SendCommand(0xB1); //Frame Rate
	TM_ILI9341_SendData(0xA0);

	TM_ILI9341_SendCommand(0xB4); //Display Inversion Control , 2 Dot
        TM_ILI9341_SendData(0x02);

	TM_ILI9341_SendCommand(0xB6); //RGB/MCU Interface Control
	TM_ILI9341_SendData(0x02);
	TM_ILI9341_SendData(0x02);
	TM_ILI9341_SendData(0x3B);

	TM_ILI9341_SendCommand(0xB7); //EntryMode
	TM_ILI9341_SendData(0xC6);


	TM_ILI9341_SendCommand(0xC0);//Power Control 1
	TM_ILI9341_SendData(0x17);
	TM_ILI9341_SendData(0x15);

	TM_ILI9341_SendCommand(0xC1);//Power Control 2
	TM_ILI9341_SendData(0x41);

	/*data[0] = 0x00;		//Power Control 3
	data[1] = 0x4D;
	data[2] = 0x80;
	TM_ILI9341_SendCommand(0xC5, data, 3);*/

	//VCOM Control
	TM_ILI9341_SendCommand(0xC5);
	TM_ILI9341_SendData(0x00);
	TM_ILI9341_SendData(0x12);
	TM_ILI9341_SendData(0x80);

	TM_ILI9341_SendCommand(0x36);  //Memory Access
	TM_ILI9341_SendData(0xC8);



	//16bpp DPI and DBI and
	TM_ILI9341_SendCommand(0x3A);  //Interface Pixel Format
	TM_ILI9341_SendData(0x51);



/*	data[0] = 0x18; //default gamma
	data[1] = 0x16;
	TM_ILI9341_SendCommand(0xC0, data, 2);
	*/


	/*data[0] = 0x00;
	data[1] = 0x04;
	TM_ILI9341_SendCommand(0xBE, data, 2); //*/


	TM_ILI9341_SendCommand(0xE0);
  TM_ILI9341_SendData(0x00);			//P-Gamma
  TM_ILI9341_SendData(0x03);
  TM_ILI9341_SendData(0x09);
  TM_ILI9341_SendData(0x08);
  TM_ILI9341_SendData(0x16);
  TM_ILI9341_SendData(0x0A);
  TM_ILI9341_SendData(0x3F);
  TM_ILI9341_SendData(0x78);
  TM_ILI9341_SendData(0x4C);
  TM_ILI9341_SendData(0x09);
  TM_ILI9341_SendData(0x0A);
  TM_ILI9341_SendData(0x08);
  TM_ILI9341_SendData(0x16);
  TM_ILI9341_SendData(0x1A);
  TM_ILI9341_SendData(0x0F);

	TM_ILI9341_SendCommand(0xE1);
  TM_ILI9341_SendData(0x00);		//N-Gamma
  TM_ILI9341_SendData(0X16);
  TM_ILI9341_SendData(0X19);
  TM_ILI9341_SendData(0x03);
  TM_ILI9341_SendData(0x0F);
  TM_ILI9341_SendData(0x05);
  TM_ILI9341_SendData(0x32);
  TM_ILI9341_SendData(0x45);
  TM_ILI9341_SendData(0x46);
  TM_ILI9341_SendData(0x04);
  TM_ILI9341_SendData(0x0E);
  TM_ILI9341_SendData(0x0D);
  TM_ILI9341_SendData(0x35);
  TM_ILI9341_SendData(0x37);
  TM_ILI9341_SendData(0x0F);


	TM_ILI9341_SendCommand(0xE9); //Set Image Func
  TM_ILI9341_SendData(0x00);

	TM_ILI9341_SendCommand(0x51); //Set Brightness to Max
  TM_ILI9341_SendData(0xFF);

	TM_ILI9341_SendCommand(0xF7); //Adjust Control
  TM_ILI9341_SendData(0xA9);
  TM_ILI9341_SendData(0x51);
  TM_ILI9341_SendData(0x2C);
  TM_ILI9341_SendData(0x82);

	TM_ILI9341_SendCommand(0x36); //set default rotation to 0
//  TM_ILI9341_SendData(MADCTL_MX | MADCTL_BGR);
  TM_ILI9341_SendData(0xC8);

	TM_ILI9341_SendCommand(0x11);    	//Exit Sleep
	TM_ILI9341_Delay(120000);

	TM_ILI9341_SendCommand(0x29);    //Display on

#elif 0

#define ILI9488WriteReg         TM_ILI9341_SendCommand
#define ILI9488WriteData        TM_ILI9341_SendData
	/* PGAMCTRL (Positive Gamma Control) (E0h) */
	ILI9488WriteReg(0xE0);
	ILI9488WriteData(0x00);
	ILI9488WriteData(0x07);
	ILI9488WriteData(0x0f);
	ILI9488WriteData(0x0D);
	ILI9488WriteData(0x1B);
	ILI9488WriteData(0x0A);
	ILI9488WriteData(0x3c);
	ILI9488WriteData(0x78);
	ILI9488WriteData(0x4A);
	ILI9488WriteData(0x07);
	ILI9488WriteData(0x0E);
	ILI9488WriteData(0x09);
	ILI9488WriteData(0x1B);
	ILI9488WriteData(0x1e);
	ILI9488WriteData(0x0f);

	/* NGAMCTRL (Negative Gamma Control) (E1h)  */
	ILI9488WriteReg(0xE1);
	ILI9488WriteData(0x00);
	ILI9488WriteData(0x22);
	ILI9488WriteData(0x24);
	ILI9488WriteData(0x06);
	ILI9488WriteData(0x12);
	ILI9488WriteData(0x07);
	ILI9488WriteData(0x36);
	ILI9488WriteData(0x47);
	ILI9488WriteData(0x47);
	ILI9488WriteData(0x06);
	ILI9488WriteData(0x0a);
	ILI9488WriteData(0x07);
	ILI9488WriteData(0x30);
	ILI9488WriteData(0x37);
	ILI9488WriteData(0x0f);

	/* Power Control 1 (C0h)  */
	ILI9488WriteReg(0xC0);
	ILI9488WriteData(0x10);
	ILI9488WriteData(0x10);

	/* Power Control 2 (C1h)  */
	ILI9488WriteReg(0xC1);
	ILI9488WriteData(0x41);

	/* VCOM Control (C5h)  */
	ILI9488WriteReg(0xC5);
	ILI9488WriteData(0x00);
	ILI9488WriteData(0x2c); // VCOM
	ILI9488WriteData(0x80);

	/* Memory Access Control (36h)  */
	ILI9488WriteReg(0x36);
	ILI9488WriteData(0x08);

	/* Interface Pixel Format (3Ah) */
	ILI9488WriteReg(0x3A);  //Interface Mode Control
	ILI9488WriteData(0x55);

	ILI9488WriteReg(0xB0);  //Interface Mode Control
	ILI9488WriteData(0x00);
	ILI9488WriteReg(0xB1);   //Frame rate 70HZ
	ILI9488WriteData(0xB0);

	/* Display Inversion Control (B4h) */
	ILI9488WriteReg(0xB4);
	ILI9488WriteData(0x02);

	/* Display Function Control (B6h)  */
	ILI9488WriteReg(0xB6);  //RGB/MCU Interface Control
	ILI9488WriteData(0x02);
	ILI9488WriteData(0x22);

	/* Set Image Function (E9h)  */
	ILI9488WriteReg(0xE9);
	ILI9488WriteData(0x00);

	/* Adjust Control 3 (F7h)  */
	ILI9488WriteReg(0XF7);
	ILI9488WriteData(0xA9);
	ILI9488WriteData(0x51);
	ILI9488WriteData(0x2C);
	ILI9488WriteData(0x82);

	/* Sleep Out (11h) */
	ILI9488WriteReg(0x11);
	Delay(15);

	/* Display ON (29h) */
	ILI9488WriteReg(0x29);

#elif 0         // works!

/* Level 1 Commands (from the display Datasheet) */
#define ILI9488_CMD_NOP                             0x00
#define ILI9488_CMD_SOFTWARE_RESET                  0x01
#define ILI9488_CMD_READ_DISP_ID                    0x04
#define ILI9488_CMD_READ_DSI_ERRORS                 0x05
#define ILI9488_CMD_READ_DISP_STATUS                0x09
#define ILI9488_CMD_READ_DISP_POWER_MODE            0x0A
#define ILI9488_CMD_READ_DISP_MADCTRL               0x0B
#define ILI9488_CMD_READ_DISP_PIXEL_FORMAT          0x0C
#define ILI9488_CMD_READ_DISP_IMAGE_MODE            0x0D
#define ILI9488_CMD_READ_DISP_SIGNAL_MODE           0x0E
#define ILI9488_CMD_READ_DISP_SELF_DIAGNOSTIC       0x0F
#define ILI9488_CMD_SLEEP_IN                        0x10
#define ILI9488_CMD_SLEEP_OUT                       0x11
#define ILI9488_CMD_PARTIAL_MODE_ON                 0x12
#define ILI9488_CMD_NORMAL_DISP_MODE_ON             0x13
#define ILI9488_CMD_DISP_INVERSION_OFF              0x20
#define ILI9488_CMD_DISP_INVERSION_ON               0x21
#define ILI9488_CMD_ALL_PIXELS_OFF                  0x22
#define ILI9488_CMD_ALL_PIXELS_ON                   0x23
#define ILI9488_CMD_DISPLAY_OFF                     0x28
#define ILI9488_CMD_DISPLAY_ON                      0x29
#define ILI9488_CMD_COLUMN_ADDRESS_SET              0x2A
#define ILI9488_CMD_PAGE_ADDRESS_SET                0x2B
#define ILI9488_CMD_MEMORY_WRITE                    0x2C
#define ILI9488_CMD_MEMORY_READ                     0x2E
#define ILI9488_CMD_PARTIAL_AREA                    0x30
#define ILI9488_CMD_VERT_SCROLL_DEFINITION          0x33
#define ILI9488_CMD_TEARING_EFFECT_LINE_OFF         0x34
#define ILI9488_CMD_TEARING_EFFECT_LINE_ON          0x35
#define ILI9488_CMD_MEMORY_ACCESS_CONTROL           0x36
#define ILI9488_CMD_VERT_SCROLL_START_ADDRESS       0x37
#define ILI9488_CMD_IDLE_MODE_OFF                   0x38
#define ILI9488_CMD_IDLE_MODE_ON                    0x39
#define ILI9488_CMD_INTERFACE_PIXEL_FORMAT          0x3A
#define ILI9488_CMD_WRITE_MEMORY_CONTINUE           0x3C
#define ILI9488_CMD_READ_MEMORY_CONTINUE            0x3E
#define ILI9488_CMD_WRITE_TEAR_SCANLINE             0x44
#define ILI9488_CMD_READ_TEAR_SCANLINE              0x45
#define ILI9488_CMD_WRITE_DISPLAY_BRIGHTNESS        0x51
#define ILI9488_CMD_READ_DISPLAY_BRIGHTNESS         0x52
#define ILI9488_CMD_WRITE_CTRL_DISPLAY              0x53
#define ILI9488_CMD_READ_CTRL_DISPLAY               0x54
#define ILI9488_CMD_WRITE_CONTENT_ADAPT_BRIGHTNESS  0x55
#define ILI9488_CMD_READ_CONTENT_ADAPT_BRIGHTNESS   0x56
#define ILI9488_CMD_WRITE_MIN_CAB_LEVEL             0x5E
#define ILI9488_CMD_READ_MIN_CAB_LEVEL              0x5F
#define ILI9488_CMD_READ_BRIGHTNESS_DIAG_RESULT     0x68
#define ILI9488_CMD_READ_ID1                        0xDA
#define ILI9488_CMD_READ_ID2                        0xDB
#define ILI9488_CMD_READ_ID3                        0xDC

/* Level 2 Commands (from the display Datasheet) */
#define ILI9488_CMD_INTERFACE_MODE_CONTROL          0xB0
#define ILI9488_CMD_FRAME_RATE_CONTROL_NORMAL       0xB1
#define ILI9488_CMD_FRAME_RATE_CONTROL_IDLE_8COLOR  0xB2
#define ILI9488_CMD_FRAME_RATE_CONTROL_PARTIAL      0xB3
#define ILI9488_CMD_DISPLAY_INVERSION_CONTROL       0xB4
#define ILI9488_CMD_BLANKING_PORCH_CONTROL          0xB5
#define ILI9488_CMD_DISPLAY_FUNCTION_CONTROL        0xB6
#define ILI9488_CMD_ENTRY_MODE_SET                  0xB7
#define ILI9488_CMD_COLOR_ENHANCEMENT_CTRL_1        0xB9
#define ILI9488_CMD_COLOR_ENHANCEMENT_CTRL_2        0xBA
#define ILI9488_CMD_HS_LANES_CONTROL                0xBE
#define ILI9488_CMD_POWER_CONTROL_1                 0xC0
#define ILI9488_CMD_POWER_CONTROL_2                 0xC1
#define ILI9488_CMD_POWER_CONTROL_3                 0xC2
#define ILI9488_CMD_POWER_CONTROL_4                 0xC3
#define ILI9488_CMD_POWER_CONTROL_5                 0xC4
#define ILI9488_CMD_VCOM_CONTROL_1                  0xC5
#define ILI9488_CMD_CABC_CONTROL_1                  0xC6
#define ILI9488_CMD_CABC_CONTROL_2                  0xC8
#define ILI9488_CMD_CABC_CONTROL_3                  0xC9
#define ILI9488_CMD_CABC_CONTROL_4                  0xCA
#define ILI9488_CMD_CABC_CONTROL_5                  0xCB
#define ILI9488_CMD_CABC_CONTROL_6                  0xCC
#define ILI9488_CMD_CABC_CONTROL_7                  0xCD
#define ILI9488_CMD_CABC_CONTROL_8                  0xCE
#define ILI9488_CMD_CABC_CONTROL_9                  0xCF
#define ILI9488_CMD_NVMEM_WRITE                     0xD0
#define ILI9488_CMD_NVMEM_PROTECTION_KEY            0xD1
#define ILI9488_CMD_NVMEM_STATUS_READ               0xD2
#define ILI9488_CMD_READ_ID4                        0xD3
#define ILI9488_CMD_ADJUST_CONTROL_1                0xD7
#define ILI9488_CMD_READ_ID4_CHECK                  0xD8
#define ILI9488_CMD_POSITIVE_GAMMA_CTRL             0xE0
#define ILI9488_CMD_NEGATIVE_GAMMA_CTRL             0xE1
#define ILI9488_CMD_DIGITAL_GAMMA_CONTROL_1         0xE2
#define ILI9488_CMD_DIGITAL_GAMMA_CONTROL_2         0xE3
#define ILI9488_CMD_SET_IMAGE_FUNCTION              0xE9
#define ILI9488_CMD_ADJUST_CONTROL_2                0xF2
#define ILI9488_CMD_ADJUST_CONTROL_3                0xF7
#define ILI9488_CMD_ADJUST_CONTROL_4                0xF8
#define ILI9488_CMD_ADJUST_CONTROL_5                0xF9
#define ILI9488_CMD_SPI_READ_CMD_SETTING            0xFB
#define ILI9488_CMD_ADJUST_CONTROL_6                0xFC
#define ILI9488_CMD_ADJUST_CONTROL_7                0xFF

#define ili9488_send_command         TM_ILI9341_SendCommand
#define ili9488_send_byte        TM_ILI9341_SendData
#define ili9488_wait_for_send_done()
#define ili9488_deselect_chip()

	// PGAMCTRL - Positive Gamma Control
	ili9488_send_command(ILI9488_CMD_POSITIVE_GAMMA_CTRL);
	ili9488_send_byte(0x00);
	ili9488_send_byte(0x07);
	ili9488_send_byte(0x0f);
	ili9488_send_byte(0x0D);
	ili9488_send_byte(0x1B);
	ili9488_send_byte(0x0A);
	ili9488_send_byte(0x3c);
	ili9488_send_byte(0x78);
	ili9488_send_byte(0x4A);
	ili9488_send_byte(0x07);
	ili9488_send_byte(0x0E);
	ili9488_send_byte(0x09);
	ili9488_send_byte(0x1B);
	ili9488_send_byte(0x1e);
	ili9488_send_byte(0x0f);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

	// NGAMCTRL - Negative Gamma Control
	ili9488_send_command(ILI9488_CMD_NEGATIVE_GAMMA_CTRL);
	ili9488_send_byte(0x00);
	ili9488_send_byte(0x22);
	ili9488_send_byte(0x24);
	ili9488_send_byte(0x06);
	ili9488_send_byte(0x12);
	ili9488_send_byte(0x07);
	ili9488_send_byte(0x36);
	ili9488_send_byte(0x47);
	ili9488_send_byte(0x47);
	ili9488_send_byte(0x06);
	ili9488_send_byte(0x0a);
	ili9488_send_byte(0x07);
	ili9488_send_byte(0x30);
	ili9488_send_byte(0x37);
	ili9488_send_byte(0x0f);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

	// Power Control 1
	ili9488_send_command(ILI9488_CMD_POWER_CONTROL_1);
	ili9488_send_byte(0x10);
	ili9488_send_byte(0x10);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

	// Power Control 2
	ili9488_send_command(ILI9488_CMD_POWER_CONTROL_2);
	ili9488_send_byte(0x41);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

	// VCOM Control
	ili9488_send_command(ILI9488_CMD_VCOM_CONTROL_1);
	ili9488_send_byte(0x00);
	ili9488_send_byte(0x22);
	ili9488_send_byte(0x80);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

	// Memory Access Control
	ili9488_send_command(ILI9488_CMD_MEMORY_ACCESS_CONTROL);
	ili9488_send_byte(0xC8);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();
//	        ILI9488_WriteParam(0xA8);	/* ∫·∆¡(≈≈œﬂ‘⁄◊Û±ﬂ) */
//		ILI9488_WriteParam(0x68);	/* ∫·∆¡ (≈≈œﬂ‘⁄”“±ﬂ) */
//		ILI9488_WriteParam(0xC8);	/*  ˙∆¡(≈≈œﬂ‘⁄…œ) */
//		ILI9488_WriteParam(0x08);	/*  ˙∆¡(≈≈œﬂ‘⁄œ¬) */

	//Interface Mode Control
	// Set up used SPI lines and RGB interface
	ili9488_send_command(ILI9488_CMD_INTERFACE_MODE_CONTROL);
	ili9488_send_byte(0x00);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

	//Frame rate 70HZ
	ili9488_send_command(ILI9488_CMD_FRAME_RATE_CONTROL_NORMAL);
	ili9488_send_byte(0xB0);
	ili9488_send_byte(0x11);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

	// Display Inversion
	ili9488_send_command(ILI9488_CMD_DISPLAY_INVERSION_CONTROL);
//	ili9488_send_byte(0x02);
	ili9488_send_byte(0x00);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

	// Entry Set Mode
	ili9488_send_command(ILI9488_CMD_ENTRY_MODE_SET);
	ili9488_send_byte(0xC6);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

#if 0//ndef CONF_ILI9488_SPI

	// Interface Pixel Format
	ili9488_send_command(ILI9488_CMD_INTERFACE_PIXEL_FORMAT);
	ili9488_send_byte(0x55);// DPI: 16bpp  DBI: 16bpp
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

	// Set Image Function
	// enable 24 bit data bus
	ili9488_send_command(ILI9488_CMD_SET_IMAGE_FUNCTION);
	ili9488_send_byte(0x01);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();
#else
	// Interface Pixel Format
	ili9488_send_command(ILI9488_CMD_INTERFACE_PIXEL_FORMAT);
	ili9488_send_byte(0x51);// DPI: 16bpp  DBI: 16bpp
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

	// Set Image Function
	ili9488_send_command(ILI9488_CMD_SET_IMAGE_FUNCTION);
	ili9488_send_byte(0x00);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();
#endif

	// Adjustment Control 3
	// fixed settings and DSI packet setup
	ili9488_send_command(ILI9488_CMD_ADJUST_CONTROL_3);
	ili9488_send_byte(0xA9);
	ili9488_send_byte(0x51);
	ili9488_send_byte(0x2C);
	ili9488_send_byte(0x82);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

	// Set Interface Mode
	ili9488_send_command(ILI9488_CMD_DISPLAY_FUNCTION_CONTROL);
	ili9488_send_byte(0x00);// RGB or other interface
	ili9488_send_byte(0x22);
	ili9488_send_byte(0x3B);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();

//	ili9488_set_orientation(0);
        {

/** Bit mask for flipping X for ili9488_set_orientation() */
#define ILI9488_FLIP_X 1
/** Bit mask for flipping Y for ili9488_set_orientation() */
#define ILI9488_FLIP_Y 2
/** Bit mask for swapping X and Y for ili9488_set_orientation() */
#define ILI9488_SWITCH_XY 4

/** @} */

/** Height of display using default orientation */
#define ILI9488_DEFAULT_HEIGHT   320

/** Width of display using default orientation */
#define ILI9488_DEFAULT_WIDTH    480

/** Height of display using swapped X/Y orientation */
#define ILI9488_SWITCH_XY_HEIGHT 480

/** Width of display using swapped X/Y orientation */
#define ILI9488_SWITCH_XY_WIDTH  320


        uint8_t flags = 0;
	/* Flip X/Y and reverse X orientation and set BGR mode*/
	uint8_t madctl = 0xC8;

	/* Pretend the display is in landscape mode by default to match other display drivers */
	//flags ^= ILI9488_SWITCH_XY | ILI9488_FLIP_X;

	if (flags & ILI9488_FLIP_X) {
		madctl &= ~(1 << 6);
	}

	if (flags & ILI9488_FLIP_Y) {
		madctl |= 1 << 7;
	}

	if (flags & ILI9488_SWITCH_XY) {
		madctl &= ~(1 << 5);
	}

	ili9488_send_command(ILI9488_CMD_MEMORY_ACCESS_CONTROL);
	ili9488_send_byte(madctl);
	ili9488_wait_for_send_done();
	ili9488_deselect_chip();
        }

//	ili9488_set_limits(0, 0, ILI9488_DEFAULT_WIDTH,
//			ILI9488_DEFAULT_HEIGHT);

	ili9488_send_command(ILI9488_CMD_SLEEP_OUT);
	ili9488_deselect_chip();
	Delay(15);
	ili9488_send_command(ILI9488_CMD_DISPLAY_ON);
	ili9488_deselect_chip();


#else   // works!

#define ILI9488_WriteCmd         TM_ILI9341_SendCommand
#define ILI9488_WriteParam        TM_ILI9341_SendData

	//************* Start Initial Sequence **********//
	/* Adjust Control 3 (F7h)  */
	ILI9488_WriteCmd(0XF7);
	ILI9488_WriteParam(0xA9);
	ILI9488_WriteParam(0x51);
	ILI9488_WriteParam(0x2C);
	ILI9488_WriteParam(0x82);	/* DSI write DCS command, use loose packet RGB 666 */

	/* Power Control 1 (C0h)  */
	ILI9488_WriteCmd(0xC0);
	ILI9488_WriteParam(0x11);
	ILI9488_WriteParam(0x09);

	/* Power Control 2 (C1h) */
	ILI9488_WriteCmd(0xC1);
	ILI9488_WriteParam(0x41);

	/* VCOM Control (C5h)  */
	ILI9488_WriteCmd(0XC5);
	ILI9488_WriteParam(0x00);
	ILI9488_WriteParam(0x0A);
	ILI9488_WriteParam(0x80);

	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	ILI9488_WriteCmd(0xB1);
	ILI9488_WriteParam(0xB0);
	ILI9488_WriteParam(0x11);

	/* Display Inversion Control (B4h) */
	ILI9488_WriteCmd(0xB4);
	ILI9488_WriteParam(0x02);

	/* Display Function Control (B6h)  */
	ILI9488_WriteCmd(0xB6);
	ILI9488_WriteParam(0x02);
	ILI9488_WriteParam(0x22);

	/* Entry Mode Set (B7h)  */
	ILI9488_WriteCmd(0xB7);
	ILI9488_WriteParam(0xc6);

	/* HS Lanes Control (BEh) */
	ILI9488_WriteCmd(0xBE);
	ILI9488_WriteParam(0x00);
	ILI9488_WriteParam(0x04);

	/* Set Image Function (E9h)  */
	ILI9488_WriteCmd(0xE9);
	ILI9488_WriteParam(0x00);

//	ILI9488_SetDirection(0);	/* ∫·∆¡(≈≈œﬂ‘⁄◊Û±ﬂ) */
        ILI9488_WriteCmd(0x36);
//		ILI9488_WriteParam(0xA8);	/* ∫·∆¡(≈≈œﬂ‘⁄◊Û±ﬂ) */
//		ILI9488_WriteParam(0x68);	/* ∫·∆¡ (≈≈œﬂ‘⁄”“±ﬂ) */
		ILI9488_WriteParam(0xC8);	/*  ˙∆¡(≈≈œﬂ‘⁄…œ) */
//		ILI9488_WriteParam(0x08);	/*  ˙∆¡(≈≈œﬂ‘⁄œ¬) */

        /* Interface Pixel Format (3Ah) */
	ILI9488_WriteCmd(0x3A);
	ILI9488_WriteParam(0x51);	/* 0x55 : 16 bits/pixel  */

	/* PGAMCTRL (Positive Gamma Control) (E0h) */
	ILI9488_WriteCmd(0xE0);
	ILI9488_WriteParam(0x00);
	ILI9488_WriteParam(0x07);
	ILI9488_WriteParam(0x10);
	ILI9488_WriteParam(0x09);
	ILI9488_WriteParam(0x17);
	ILI9488_WriteParam(0x0B);
	ILI9488_WriteParam(0x41);
	ILI9488_WriteParam(0x89);
	ILI9488_WriteParam(0x4B);
	ILI9488_WriteParam(0x0A);
	ILI9488_WriteParam(0x0C);
	ILI9488_WriteParam(0x0E);
	ILI9488_WriteParam(0x18);
	ILI9488_WriteParam(0x1B);
	ILI9488_WriteParam(0x0F);

	/* NGAMCTRL (Negative Gamma Control) (E1h)  */
	ILI9488_WriteCmd(0XE1);
	ILI9488_WriteParam(0x00);
	ILI9488_WriteParam(0x17);
	ILI9488_WriteParam(0x1A);
	ILI9488_WriteParam(0x04);
	ILI9488_WriteParam(0x0E);
	ILI9488_WriteParam(0x06);
	ILI9488_WriteParam(0x2F);
	ILI9488_WriteParam(0x45);
	ILI9488_WriteParam(0x43);
	ILI9488_WriteParam(0x02);
	ILI9488_WriteParam(0x0A);
	ILI9488_WriteParam(0x09);
	ILI9488_WriteParam(0x32);
	ILI9488_WriteParam(0x36);
	ILI9488_WriteParam(0x0F);

	/* Sleep Out (11h */
	ILI9488_WriteCmd(0x11);
	Delay(12);
	ILI9488_WriteCmd(0x29);	/* Display ON (29h) */


#endif
}

//void ili9488_set_orientation(uint8_t flags)
//{
//	/* Flip X/Y and reverse X orientation and set BGR mode*/
//	uint8_t madctl = 0x68;
//
//	/* Pretend the display is in landscape mode by default to match other display drivers */
//	//flags ^= ILI9488_SWITCH_XY | ILI9488_FLIP_X;
//
//	if (flags & ILI9488_FLIP_X) {
//		madctl &= ~(1 << 6);
//	}
//
//	if (flags & ILI9488_FLIP_Y) {
//		madctl |= 1 << 7;
//	}
//
//	if (flags & ILI9488_SWITCH_XY) {
//		madctl &= ~(1 << 5);
//	}
//
//	ili9488_send_command(ILI9488_CMD_MEMORY_ACCESS_CONTROL);
//	ili9488_send_byte(madctl);
//	ili9488_wait_for_send_done();
//	ili9488_deselect_chip();
//}

void TM_ILI9341_DisplayOn(void) {
	TM_ILI9341_SendCommand(ILI9341_DISPLAY_ON);
}

void TM_ILI9341_DisplayOff(void) {
	TM_ILI9341_SendCommand(ILI9341_DISPLAY_OFF);
}

void TM_ILI9341_SendCommand(uint8_t data) {
	ILI9341_WRX_RESET;
	ILI9341_CS_RESET;
	TM_SPI_Send(ILI9341_SPI, data);
	ILI9341_CS_SET;
}

void TM_ILI9341_SendData(uint8_t data) {
	ILI9341_WRX_SET;
	ILI9341_CS_RESET;
	TM_SPI_Send(ILI9341_SPI, data);
	ILI9341_CS_SET;
}

void TM_ILI9341_DrawPixel(uint16_t x, uint16_t y, uint32_t color) {
	TM_ILI9341_SetCursorPosition(x, y, x, y);

	TM_ILI9341_SendCommand(ILI9341_GRAM);
	TM_ILI9341_SendData(color >> 8);
	TM_ILI9341_SendData(color & 0xFF);
}


void TM_ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	TM_ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
	TM_ILI9341_SendData(x1 >> 8);
	TM_ILI9341_SendData(x1 & 0xFF);
	TM_ILI9341_SendData(x2 >> 8);
	TM_ILI9341_SendData(x2 & 0xFF);

	TM_ILI9341_SendCommand(ILI9341_PAGE_ADDR);
	TM_ILI9341_SendData(y1 >> 8);
	TM_ILI9341_SendData(y1 & 0xFF);
	TM_ILI9341_SendData(y2 >> 8);
	TM_ILI9341_SendData(y2 & 0xFF);
}

void TM_ILI9341_Fill(uint32_t color) {
	/* Fill entire screen */
	TM_ILI9341_INT_Fill(0, 0, ILI9341_Opts.width - 1, ILI9341_Opts.height, color);
}

void TM_ILI9341_INT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	uint32_t pixels_count;

	/* Set cursor position */
	TM_ILI9341_SetCursorPosition(x0, y0, x1, y1);

	/* Set command for GRAM data */
	TM_ILI9341_SendCommand(ILI9341_GRAM);

	/* Calculate pixels count */
	pixels_count = (x1 - x0 + 1) * (y1 - y0 + 1);

	/* Send everything */
	ILI9341_CS_RESET;
	ILI9341_WRX_SET;

	/* Go to 16-bit SPI mode */
	TM_SPI_SetDataSize(ILI9341_SPI, TM_SPI_DataSize_16b);

	/* Send first 65535 bytes, SPI MUST BE IN 16-bit MODE */
	TM_SPI_DMA_SendHalfWord(ILI9341_SPI, color, (pixels_count > 0xFFFF) ? 0xFFFF : pixels_count);
	/* Wait till done */
	while (TM_SPI_DMA_Working(ILI9341_SPI));

	/* Check again */
	if (pixels_count > 0xFFFF) {
		/* Send remaining data */
		TM_SPI_DMA_SendHalfWord(ILI9341_SPI, color, pixels_count - 0xFFFF);
		/* Wait till done */
		while (TM_SPI_DMA_Working(ILI9341_SPI));
	}

	ILI9341_CS_SET;

	/* Go back to 8-bit SPI mode */
	TM_SPI_SetDataSize(ILI9341_SPI, TM_SPI_DataSize_8b);
}

void TM_ILI9341_Delay(volatile unsigned int delay) {
	for (; delay != 0; delay--);
}

void TM_ILI9341_Rotate(TM_ILI9341_Orientation_t orientation) {
	TM_ILI9341_SendCommand(ILI9341_MAC);
	if (orientation == TM_ILI9341_Orientation_Portrait_1) {
		TM_ILI9341_SendData(0x58);
	} else if (orientation == TM_ILI9341_Orientation_Portrait_2) {
		TM_ILI9341_SendData(0x88);
	} else if (orientation == TM_ILI9341_Orientation_Landscape_1) {
		TM_ILI9341_SendData(0x28);
	} else if (orientation == TM_ILI9341_Orientation_Landscape_2) {
		TM_ILI9341_SendData(0xE8);
	}

	if (orientation == TM_ILI9341_Orientation_Portrait_1 || orientation == TM_ILI9341_Orientation_Portrait_2) {
		ILI9341_Opts.width = ILI9341_WIDTH;
		ILI9341_Opts.height = ILI9341_HEIGHT;
		ILI9341_Opts.orientation = TM_ILI9341_Portrait;
	} else {
		ILI9341_Opts.width = ILI9341_HEIGHT;
		ILI9341_Opts.height = ILI9341_WIDTH;
		ILI9341_Opts.orientation = TM_ILI9341_Landscape;
	}
}

void TM_ILI9341_Puts(uint16_t x, uint16_t y, char *str, TM_FontDef_t *font, uint32_t foreground, uint32_t background) {
	uint16_t startX = x;

	/* Set X and Y coordinates */
	ILI9341_x = x;
	ILI9341_y = y;

	while (*str) {
		/* New line */
		if (*str == '\n') {
			ILI9341_y += font->FontHeight + 1;
			/* if after \n is also \r, than go to the left of the screen */
			if (*(str + 1) == '\r') {
				ILI9341_x = 0;
				str++;
			} else {
				ILI9341_x = startX;
			}
			str++;
			continue;
		} else if (*str == '\r') {
			str++;
			continue;
		}

		/* Put character to LCD */
		TM_ILI9341_Putc(ILI9341_x, ILI9341_y, *str++, font, foreground, background);
	}
}

void TM_ILI9341_GetStringSize(char *str, TM_FontDef_t *font, uint16_t *width, uint16_t *height) {
	uint16_t w = 0;
	*height = font->FontHeight;
	while (*str++) {
		w += font->FontWidth;
	}
	*width = w;
}

void TM_ILI9341_Putc(uint16_t x, uint16_t y, char c, TM_FontDef_t *font, uint32_t foreground, uint32_t background) {
	uint32_t i, b, j;
	/* Set coordinates */
	ILI9341_x = x;
	ILI9341_y = y;

	if ((ILI9341_x + font->FontWidth) > ILI9341_Opts.width) {
		/* If at the end of a line of display, go to new line and set x to 0 position */
		ILI9341_y += font->FontHeight;
		ILI9341_x = 0;
	}

	/* Draw rectangle for background */
	TM_ILI9341_INT_Fill(ILI9341_x, ILI9341_y, ILI9341_x + font->FontWidth, ILI9341_y + font->FontHeight, background);

	/* Draw font data */
	for (i = 0; i < font->FontHeight; i++) {
		b = font->data[(c - 32) * font->FontHeight + i];
		for (j = 0; j < font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				TM_ILI9341_DrawPixel(ILI9341_x + j, (ILI9341_y + i), foreground);
			}
		}
	}

	/* Set new pointer */
	ILI9341_x += font->FontWidth;
}


void TM_ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color) {
	/* Code by dewoller: https://github.com/dewoller */

	int16_t dx, dy, sx, sy, err, e2;
	uint16_t tmp;

	/* Check for overflow */
	if (x0 >= ILI9341_Opts.width) {
		x0 = ILI9341_Opts.width - 1;
	}
	if (x1 >= ILI9341_Opts.width) {
		x1 = ILI9341_Opts.width - 1;
	}
	if (y0 >= ILI9341_Opts.height) {
		y0 = ILI9341_Opts.height - 1;
	}
	if (y1 >= ILI9341_Opts.height) {
		y1 = ILI9341_Opts.height - 1;
	}

	/* Check correction */
	if (x0 > x1) {
		tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	if (y0 > y1) {
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	dx = x1 - x0;
	dy = y1 - y0;

	/* Vertical or horizontal line */
	if (dx == 0 || dy == 0) {
		TM_ILI9341_INT_Fill(x0, y0, x1, y1, color);
		return;
	}

	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = ((dx > dy) ? dx : -dy) / 2;

	while (1) {
		TM_ILI9341_DrawPixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}

void TM_ILI9341_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color) {
	TM_ILI9341_DrawLine(x0, y0, x1, y0, color); //Top
	TM_ILI9341_DrawLine(x0, y0, x0, y1, color);	//Left
	TM_ILI9341_DrawLine(x1, y0, x1, y1, color);	//Right
	TM_ILI9341_DrawLine(x0, y1, x1, y1, color);	//Bottom
}

void TM_ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color) {
	uint16_t tmp;

	/* Check correction */
	if (x0 > x1) {
		tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	if (y0 > y1) {
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	/* Fill rectangle */
	TM_ILI9341_INT_Fill(x0, y0, x1, y1, color);

	/* CS HIGH back */
	ILI9341_CS_SET;
}

void TM_ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    TM_ILI9341_DrawPixel(x0, y0 + r, color);
    TM_ILI9341_DrawPixel(x0, y0 - r, color);
    TM_ILI9341_DrawPixel(x0 + r, y0, color);
    TM_ILI9341_DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        TM_ILI9341_DrawPixel(x0 + x, y0 + y, color);
        TM_ILI9341_DrawPixel(x0 - x, y0 + y, color);
        TM_ILI9341_DrawPixel(x0 + x, y0 - y, color);
        TM_ILI9341_DrawPixel(x0 - x, y0 - y, color);

        TM_ILI9341_DrawPixel(x0 + y, y0 + x, color);
        TM_ILI9341_DrawPixel(x0 - y, y0 + x, color);
        TM_ILI9341_DrawPixel(x0 + y, y0 - x, color);
        TM_ILI9341_DrawPixel(x0 - y, y0 - x, color);
    }
}

void TM_ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    TM_ILI9341_DrawPixel(x0, y0 + r, color);
    TM_ILI9341_DrawPixel(x0, y0 - r, color);
    TM_ILI9341_DrawPixel(x0 + r, y0, color);
    TM_ILI9341_DrawPixel(x0 - r, y0, color);
    TM_ILI9341_DrawLine(x0 - r, y0, x0 + r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        TM_ILI9341_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        TM_ILI9341_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

        TM_ILI9341_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
        TM_ILI9341_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
    }
}
