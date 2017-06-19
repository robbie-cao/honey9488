/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c
  * @author  MCD Application Team
  * @version V1.2.0RC2
  * @date    20-February-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tm_stm32f4_fonts.h"
#include "tm_stm32f4_ili9341.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#if defined (USE_STM324xG_EVAL)
  #define MESSAGE1   "     STM32F40xx     "
  #define MESSAGE2   " Device running on  "
  #define MESSAGE3   "   STM324xG-EVAL    "

#elif defined (USE_STM324x7I_EVAL)
  #define MESSAGE1   "     STM32F427x     "
  #define MESSAGE2   " Device running on  "
  #define MESSAGE3   "  STM324x7I-EVAL    "

#else
  #define MESSAGE1   "          STM32F429X          "
  #define MESSAGE2   "      Device running on       "
  #define MESSAGE3   "        STM324x9I-EVAL        "
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);

extern const uint16_t TM_Font11x18 [];
extern TM_FontDef_t TM_Font_7x10;
extern TM_FontDef_t TM_Font_11x18;
extern TM_FontDef_t TM_Font_16x26;

// Color definitions
#define ILI9488_BLACK       0x00000000      /*   0,   0,   0 */
#define ILI9488_NAVY        0x00097194      /*   0,   0, 128 */
#define ILI9488_DARKGREEN   0x001F7300      /*   0, 128,   0 */
#define ILI9488_DARKCYAN    0x0001857E      /*   0, 128, 128 */
#define ILI9488_MAROON      0x00E770FF      /* 128,   0,   0 */
#define ILI9488_PURPLE      0x00710087      /* 128,   0, 128 */
#define ILI9488_OLIVE       0x00007BE0      /* 128, 128,   0 */
#define ILI9488_LIGHTGREY   0x0000C618      /* 192, 192, 192 */
#define ILI9488_DARKGREY    0x00007BEF      /* 128, 128, 128 */
#define ILI9488_BLUE        0x000D15FF      /*   0,   0, 255 */
#define ILI9488_GREEN       0x0031FF0D      /*   0, 255,   0 */
#define ILI9488_CYAN        0x000007FF      /*   0, 255, 255 */
#define ILI9488_RED         0x00FF1919      /* 255,   0,   0 */
#define ILI9488_MAGENTA     0x00FF5CDE      /* 255,   0, 255 */
#define ILI9488_YELLOW      0x00FAFF5C      /* 255, 255,   0 */
#define ILI9488_WHITE       0x00FFFFFE      /* 255, 255, 255 */
#define ILI9488_ORANGE      0x00DB7100      /* 255, 165,   0 */
#define ILI9488_GREENYELLOW 0x0000AFE5      /* 173, 255,  47 */
#define ILI9488_PINK        0x0000F81F


/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40xx.s/startup_stm32f427x.s/startup_stm32f429x.s)
       before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
#if 0
    TM_Test3();

  TM_ILI9488_Init();
  ILI9488_Control_init();
  while (1);

  TM_Test();
#endif
  TM_ILI9341_Init();

  {
    TM_FontDef_t font;

    font.FontWidth = 11;
    font.FontHeight = 18;
    font.data = TM_Font11x18;
    TM_ILI9341_Puts(0, 0, "Honeywell Connected Air Stat", &TM_Font_16x26, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE);
    TM_ILI9341_Puts(160, 240, "Hello 9488", &TM_Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE);
    TM_ILI9341_Puts(0, 400, "320RGB x 480 Resolution and 16.7M-color", &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
  }
    TM_ILI9341_Puts(0, 60, "Honeywell", &TM_Font_16x26, 0xFF0000, 0xFFFFFF);
    TM_ILI9341_Puts(0, 90, "Honeywell", &TM_Font_16x26, 0x00FF00, 0x0007FF);
    TM_ILI9341_Puts(0, 120, "Honeywell", &TM_Font_16x26, 0x0000FF, 0x000000);

  TM_ILI9341_DrawPixel(10, 100, ILI9341_COLOR_RED);
  TM_ILI9341_DrawPixel(10, 102, ILI9341_COLOR_BLUE);
  TM_ILI9341_DrawPixel(10, 104, ILI9341_COLOR_GREEN);

  for (int i = 10; i < 30; i++) {
    TM_ILI9341_DrawLine(i, 150, i, 250, 0xFF0000);
  }
  for (int i = 30; i < 50; i++) {
    TM_ILI9341_DrawLine(i, 150, i, 250, 0x00FF00);
  }
  for (int i = 50; i < 70; i++) {
    TM_ILI9341_DrawLine(i, 150, i, 250, 0x0000FF);
  }

  for (int i = 100; i < 120; i++) {
    TM_ILI9341_DrawLine(150, i, 250, i, 0xFF0000);
  }
  for (int i = 120; i < 140; i++) {
    TM_ILI9341_DrawLine(150, i, 250, i, 0x00FF00);
  }
  for (int i = 140; i < 160; i++) {
    TM_ILI9341_DrawLine(150, i, 250, i, 0x0000FF);
  }


  TM_ILI9341_DrawFilledRectangle(20, 200, 200, 250, 0xFF0000);
  TM_ILI9341_DrawFilledCircle(100, 300, 50, 0x000000);

  while (1);

  while (0) {
    TM_ILI9341_SendCommand(0x21);
    Delay(100);
    TM_ILI9341_SendCommand(0x20);
    Delay(100);
  }

  while (1) {
    TM_ILI9341_SendCommand(0x23);
    Delay(100);
    TM_ILI9341_SendCommand(0x22);
    Delay(100);
  }

  /* Initialize LEDs and LCD available on EVAL board **************************/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  /* Initialize the LCD */
  LCD_Init();

  /* Display message on LCD ***************************************************/
#if defined (USE_STM324x9I_EVAL)
  /* Enable The Display */
  LCD_DisplayOn();
  /* Set LCD Background Layer  */
  LCD_SetLayer(LCD_BACKGROUND_LAYER);
  /* Clear the Background Layer */
  LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Foreground Layer  */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);

  /* Configure the transparency for foreground */
  LCD_SetTransparency(100);
#endif /* USE_STM324x9I_EVAL */

  /* Clear the Foreground Layer */
  LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);

  /* Display LCD messages */
  LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)MESSAGE1);
  LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)MESSAGE2);
  LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)MESSAGE3);
  LCD_DisplayStringLine(LCD_LINE_15, (uint8_t *)MESSAGE3);
  LCD_DisplayStringLine(LCD_LINE_16, (uint8_t *)MESSAGE3);
  LCD_DisplayStringLine(LCD_LINE_17, (uint8_t *)MESSAGE3);
  LCD_DisplayStringLine(LCD_LINE_18, (uint8_t *)MESSAGE3);
  LCD_DisplayStringLine(LCD_LINE_19, (uint8_t *)MESSAGE3);

  /* Turn on LEDs *************************************************************/
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);

  /* Add your application code here */

  /* Infinite loop */
  while (1)
  {
    /* Toggle LD1 */
    STM_EVAL_LEDToggle(LED1);
    /* Insert 50 ms delay */
    Delay(5);

    /* Toggle LD2 */
    STM_EVAL_LEDToggle(LED2);
    /* Insert 50 ms delay */
    Delay(5);

    /* Toggle LD3 */
    STM_EVAL_LEDToggle(LED3);
    /* Insert 50 ms delay */
    Delay(5);

    /* Toggle LD4 */
    STM_EVAL_LEDToggle(LED4);
    /* Insert 50 ms delay */
    Delay(5);
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  {
    uwTimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
