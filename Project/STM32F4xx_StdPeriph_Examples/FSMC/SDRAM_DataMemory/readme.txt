/**
  @page FMC_SDRAM_DataMemory FMC SDRAM data memory example
  
  @verbatim
  ******************* (C) COPYRIGHT 2013 STMicroelectronics ********************
  * @file    FMC/FMC_SDRAM_DataMemory/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0RC2
  * @date    20-February-2013
  * @brief   Description of the FMC SDRAM_DataMemory example.
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

  This example shows how to use the MT48LC2M3B2B5-7E SDRAM mounted on STM324x9I-EVAL 
  as dta memory (including heap and stack).
  
  The example scenario does not reflect a real application case, its purpose is to
  provide only the procedure to follow to use the external SDRAM as data memory.

  This example does not use the default library startup file. It uses a modified 
  startup file provided with the example. While startup, the SDRAM memory is 
  configured and initialized to be ready to contain data.
  
  The user has to configure his preferred toolchain using the provided linker file.
  The RAM zone is modified in order to use the external memory as a RAM. 
 
@par Directory contents
                       
 - FMC/FMC_SDRAM_DataMemory/system_stm32f4xx.c          STM32F4xx system clock configuration file 
 - FMC/FMC_SDRAM_DataMemory/stm32f4xx_conf.h            Library Configuration file
 - FMC/FMC_SDRAM_DataMemory/stm32f4xx_it.c              Interrupt handlers
 - FMC/FMC_SDRAM_DataMemory/stm32f4xx_it.h              Header for stm32f4xx_it.c
 - FMC/FMC_SDRAM_DataMemory/main.c                      Main program   
 - FMC/FMC_SDRAM_DataMemory/main.h                      Header for main.c module 
 - FMC/FMC_SDRAM_DataMemory/startup                     Directory containing startup file for each toolchain               


@note The "system_stm32f4xx.c" is generated by an automatic clock configuration 
      tool and can be easily customized to your own configuration. 
      To select different clock setup, use the "STM32F4xx_Clock_Configuration_V1.1.0.xls" 
      provided with the AN3988 package available on <a href="http://www.st.com/internet/mcu/family/141.jsp">  ST Microcontrollers </a>

      
@par Hardware and Software environment 

  - This example runs on STM32F429x/STM32F439x Devices.
    
  - This example has been tested with STMicroelectronics STM32439I-EVAL (STM32F429x/ 
    STM32F439x Devices) evaluation boards and can be easily tailored to any other  
    supported device and development board.


@par How to use it ? 

 In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F4xx_StdPeriph_Templates
 - Open your preferred toolchain 
 - Update your project settings as follows:
<ul>
 <li> MDK-ARM 
    - in Project->Options for Linker window, uncheck the option "Use Memory Layout
      from Target Dialog". You can then import the scatter file dedicated for this 
      example.
    - uncomment "#define DATA_IN_ExtSDRAM " in the "system_stm32f4xx.c" file               

 <li> EWARM 
    - use "stm32f4xx_flash_extsdram.icf" as linker file (under Project\STM32F4xx_StdPeriph_Templates\EWARM)
    - uncomment "#define DATA_IN_ExtSDRAM " in the "system_stm32f4xx.c" file

 <li> RIDE 
    - In the Application options -> script menu, set "Use Default Script File" 
      to "No" and use "stm32f4xx_flash_extsdram.ld" as Script File.
    - uncomment "#define DATA_IN_ExtSDRAM " in the "system_stm32f4xx.c" file

<li> TASKING 
    -In the project properties window, select C/C++ Build->settings node then 
         the  "Linker->Script file"  and use "stm32f4xx_extsdram.lsl" as Script File.
    - uncomment "#define DATA_IN_ExtSDRAM " in the "system_stm32f4xx.c" file
  
 <li> TrueSTUDIO 
    - In the project properties window, select 'C/C++ Build'->settings node then 
      the 'C Linker'->General node and use "stm32f4xx_flash_extsdram.ld" as Script File
      (under Project\STM32F4xx_StdPeriph_Templates\TrueSTUDIO).
    - uncomment "#define DATA_IN_ExtSDRAM " in the "system_stm32f4xx.c" file
      
</ul>

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
