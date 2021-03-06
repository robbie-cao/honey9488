/**
  @page NVIC_DMA_WFIMode NVIC DMA in WFI mode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    NVIC/NVIC_WFIMode/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0RC2
  * @date    20-February-2013
  * @brief   Description of the NVIC DMA in WFI mode example.
  ******************************************************************************
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
  @endverbatim

@par Example Description 

  This example shows how to enter the system to WFI mode with DMA transfer enabled
  and wake-up from this mode by the DMA End of Transfer interrupt.

  In the associated software, the system clock is set to 168 MHz, the DMA1 Stream1
  channel4 is configured to transfer 10 data from the EVAL_COM1 (USART3 for 
  STM32F40xx/STM32F41xx and STM32F427x/STM32F437x devices, and USART1 for 
  STM32F429x/STM32F439x devices) data register to a predefined buffer "DST_Buffer", 
  and to generate an interrupt at the end of the transfer. The EVAL_COM1 (USART3/1) 
  receives data from Hyperterminal.
  LED1 is toggled with a frequency depending on the system clock, this is used 
  to indicate whether the MCU is in WFI or RUN mode. 

  Pressing Tamper/key push button (generates rising edge on EXTI Line15/13) will 
  put the core in the WFI mode, causing the LED1 to stop toggling.
  To wake-up from WFI mode you have to send the sequence (0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
  from the Hyperterminal to the EVAL_COM1 (USART3/1). These bytes will be transferred 
  by the DMA from the EVAL_COM1 (USART3/1) receive data register to the predefined 
  buffer, then generates an interrupt which exits the system from WFI mode. 
  The LED1 restarts toggling and a LED2 will toggle if the buffer is correctly 
  received else a LED3 is toggled.


@par Directory contents 

  - NVIC/NVIC_WFIMode/stm32f4xx_conf.h     Library Configuration file
  - NVIC/NVIC_WFIMode/stm32f4xx_it.c       Interrupt handlers
  - NVIC/NVIC_WFIMode/stm32f4xx_it.h       Interrupt handlers header file
  - NVIC/NVIC_WFIMode/main.h               Header for main.c module    
  - NVIC/NVIC_WFIMode/main.c               Main program
  - NVIC/NVIC_WFIMode/system_stm32f4xx.c   STM32F4xx system source file
  
@note The "system_stm32f4xx.c" is generated by an automatic clock configuration 
      tool and can be easily customized to your own configuration. 
      To select different clock setup, use the "STM32F4xx_Clock_Configuration_V1.1.0.xls" 
      provided with the AN3988 package available on <a href="http://www.st.com/internet/mcu/family/141.jsp">  ST Microcontrollers </a>

         
@par Hardware and Software environment

  - This example runs on STM32F40xx/STM32F41xx, STM32F427x/STM32F437x, 
    STM32F429x/STM32F439x Devices.
  
  - This example has been tested with STMicroelectronics STM324xG-EVAL (STM32F40xx/
    STM32F41xx Devices), STM32437I-EVAL (STM32F427x/STM32F437x Devices) and STM324x9I-EVAL 
    (STM32F429x/STM32F439x Devices) evaluation boards and can be easily tailored to any 
    other supported device and development board.

  - STM324xG-EVAL/STM32437I-EVAL Set-up
    - Use the Key push-button connected to pin PG.15 (EXTI Line15)
    - Use LED1, LED2 and LED3 connected respectively to PG.06, PG.08 and PC.07 pins    
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN16 (USART3) and PC serial port if you want to display data on the HyperTerminal.
      @note Make sure that jumper JP22 is in position 1-2.

  - STM324x9I-EVAL Set-up
    - Use the Tamper/Key push-button connected to pin PC.13 (EXTI Line13)
    - Use LED1, LED2, LED3 and LED4 connected respectively to PG.06, PG.07, PG.10
      and PG.12 pins 
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN8 (USART1) and PC serial port if you want to display data on the HyperTerminal.
      @note Make sure that jumper JP7 is in position 1-2.  
            
  - Hyperterminal configuration:
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: None   
 
            
@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F4xx_StdPeriph_Templates
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

   
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
