<p align="center">
  <a href="." title="Sigma eLabs">
    <img src="/assets/SigmaeLabsBannerv2.png" title="Sigma eLabs" style="width: 100vw; min-width: 200px"/>
  </a>
</p>

# Tiva C Series Launchpad Board Support Package
This is an example of Board Support Package implementation. BSP is one of the common design and implementation approach which is wisely used in the embedded software programming. In embedded software programming world, most of the products more or less have the dependencies with the specific hardware, where software is running on.
<p align="center">
  <a href="." title="Tiva C Series Launchpad Board Support Package">
    <img src="/Board_Support_Packages/tivac-bsp-example/images/HardwareSetup.png" title="Tiva C Series Launchpad Board Support Package" style="min-width: 200px"/>
  </a>
</p>

# General
## Problem
Looking back to the example [GPIO Interrupt with on-board button pressed](https://github.com/Sigma-eLabs/uc-programming/tree/main/gpio/gpio-interrupt-button-pressed), where I demonstrate the implementation of GPIO interrupt, I'm facing some overhead problems:
* Firstly, even the software is very simple but all of the code (interrupt handling, GPIO configuration, hardware specific) is implemented inside of main.c file. This is very difficult to read and maintain the source code, especially in case the software becomes bigger.
* Secondly, most parts of the code are hardware specific. They support for Arm Cortex and Tiva C Launchpad hardware design only.
* Finally, the application code inside main() function can not be reused anymore if the hardware design is changed. Also, the efforts to adapt the changes of hardware is quite big for developer.

## Solution
With the identified problems above, I see that there is a chance to optimize the implementation and make the software is more robustness. By using the common approach used by many developers, the Board Support Package, I can resolve the problems and achieve the goal to make the software is simplier and maintainable.

I reorganize the software into modules: "main" and "BSP":
* BSP module implements the hardware specific APIs, directly interact with registers, interrupt, on-boards devices. And, bsp module provides usefull APIs to the user (in this case is the application) to read and control the system and hardware devices on the board.
* main module implements the application which uses the APIs provided by bsp module.
That is enough for this example.

# Requisitions
## Hardware
* [Tiva C Series TM4C123G LaunchPad Evaluation Kit - EK-TM4C123GXL](https://www.ti.com/tool/EK-TM4C123GXL)
* Logic Analyzer

## Software
* [Keil C](https://www.keil.com/demo/eval/arm.htm)
* [Logic 2](https://www.saleae.com/pages/downloads)

Note: For this project, you can choose your own hardware and software. It doesn't matter, because the selection is personal choice. The main purpose needs to be focused is resolving the problem in [General Requirements](#general-requirements) and the knowledge you get after this lesson. You can try to do the similar thing on the different hardware and software.

## Documents
* [Tiva™ C Series TM4C123G LaunchPad Evaluation Board User's Guide](https://github.com/Sigma-eLabs/technical-resources/blob/main/tiva-c-series/spmu296.pdf)
* [Tiva™ TM4C123GH6PM Microcontroller Datasheet](https://github.com/Sigma-eLabs/technical-resources/blob/main/tiva-c-series/tm4c123gh6pm.pdf)
* [Cortex Microcontroller Software Interface Standard (CMSIS)](https://github.com/Sigma-eLabs/technical-resources/tree/main/tiva-c-series/CMSIS)
* [Tiva™ TM4C123GH6PM Microcontroller Software Support Package](https://github.com/Sigma-eLabs/technical-resources/tree/main/tiva-c-series/ek-tm4c123gxl)

# Implementation
I place the source code of this example here: :rocket: [Tiva C BSP Example](/Board_Support_Packages/tivac-bsp-example/tivac-bsp-example/) :rocket:

* I put the configuration and initialization related to hardware/registers inside the BSP_init() function. From user point of view, it very useful and easy to use, especially, application developer, he just needs to know What the BSP_init() API do, but not How it do.
  ```C
  void BSP_init(void) {
    /* configuration to enable GPIOF module */
    SYSCTL->RCGCGPIO  |= (1U << 5);	/* enable Run mode for GPIOF */
    SYSCTL->GPIOHBCTL |= (1U << 5);	/* enable AHB bus for GPIOF */
    
    /* even PF2, PF3 pins unused, but explicitly configured them as output pins to prevent unwanted behavior */
    GPIOF_AHB->DIR |= (LED_RED | LED_BLUE | LED_GREEN); /* configure PF1, PF2, PF3 pins - red, blue, green LEDs as output pin */
    GPIOF_AHB->DEN |= (LED_RED | LED_BLUE | LED_GREEN); /* configure PF1, PF2, PF3 pins - red, blue, green LEDs with digital mode */
    
    /* configure User button switch 1 - PF4 pin as digital input pin with pull-up resistor */
    GPIOF_AHB->DIR &= ~USR_SW1;	/* configure PF4 pin - red LED as input pin */
    GPIOF_AHB->DEN |= USR_SW1;	/* configure PF4 pin - red LED with digital mode */
    GPIOF_AHB->PUR |= USR_SW1;	/* confgiure PF4 pin with pull-up resistor */
    
    /* Interrupt setup for User button switch 1 - PF4 pin: falling edge interrupt detection */
    GPIOF_AHB->IS &= ~USR_SW1;	/* edge-sensitive interrupt detection */
    GPIOF_AHB->IBE &= ~USR_SW1;	/* interrupt event is controller by GPIOIEV */
    GPIOF_AHB->IEV &= ~USR_SW1;	/* falling edge interrupt trigger */
    GPIOF_AHB->IM |= USR_SW1;	/* enable GPIOF interrupt for this pin */
    
    /* set interrupt priority for GPIOF */
    NVIC_SetPriority(GPIOF_IRQn, 1U); /* for now, stick to 1 but avoid 0 (highest priority), the reason will be explained in another lesson */
    
    /* enable GPIOF_IRQn in NVIC */
    NVIC_EnableIRQ(GPIOF_IRQn);
  }
  ```

* Example, to control the on-boards red LED state, the BSP module provides the below API. To use this API, user need to provide only the state of the LED he would like.
  ```C
  void BSP_setRedledState(LedState ledState) {
    if (ledState == LED_ON) {
      GPIOF_AHB->DATA_Bits[LED_RED] = LED_RED;
    }
    else {
      GPIOF_AHB->DATA_Bits[LED_RED] = 0U;
    }
  }
  ```

* BSP module also can provides the delay function for the application:
  ```C
  void BSP_delay(uint32_t volatile iter) {
    while(iter > 0U){
      --iter;
    }
  }
  ```
* With the help of BSP module's APIs, the application is abstracted with the hardware specification. Now, it simply call the APIs provided from BSP module and build its own functions.
  ```C
  /* inside of main() function */
  while (1) {
    if (UsrSw1_State == PRESSED) {
      /* Blink red LED for a while */
      Led_Blinky();
      
      /* reset the state of User button switch 1 */
      UsrSw1_State = RELEASED;
    }
  }
  ```
* Now, your application in main module is very simple.
  ```C
  /* main() function */
  int main(void) {
    
    BSP_init();
    
    /* endless loop */
    while (1) {
      if (usrSw1State == BUTTON_PRESSED) {	/* UsrSw1 button was pressed */
        /* Blink red LED for a while */	
        for (uint8_t iter = 5U; iter != 0U; --iter) {
          BSP_setRedledState(LED_ON); 	/* turn red LED on */
          BSP_delay(2000000U); 	/* delay for 2000000 interations */
          BSP_setRedledState(LED_OFF);	/* turn red LED off */
          BSP_delay(1000000U); 	/* delay for 1000000 interations */
        }
        
        /* reset the state of UsrSw1 button */
        usrSw1State = BUTTON_RELEASED;
      }
    }

    return 0;
  }
  ```
By organizing the software into modules and using BSP approach, the software is more robustness, reusable, simple and easy to maintain.

* To implement this software, you should read [Tiva™ TM4C123GH6PM Microcontroller Datasheet](https://github.com/Sigma-eLabs/technical-resources/blob/main/tiva-c-series/tm4c123gh6pm.pdf), the chapters related to GPIO, System Control modules to know about the functionality of common registers. Especially, the you should read about the interrupt related registers.

* You also should read [Tiva™ C Series TM4C123G LaunchPad Evaluation Board User's Guide](https://github.com/Sigma-eLabs/technical-resources/blob/main/tiva-c-series/spmu296.pdf) to know about the hardware design of the development kit which is being used.

* Additionally, you understand and know how to use the provided standard libraries like [Cortex Microcontroller Software Interface Standard (CMSIS)](https://github.com/Sigma-eLabs/technical-resources/tree/main/tiva-c-series/CMSIS), [Tiva™ TM4C123GH6PM Microcontroller Software Support Package](https://github.com/Sigma-eLabs/technical-resources/tree/main/tiva-c-series/ek-tm4c123gxl), which support Arm-Cortex architecture processor and TM4C123GHPM controller.

# Deployment & Verification
I do not intent to mention the steps to deploy and test the software here again because it's already done in [GPIO Interrupt with on-board button pressed](https://github.com/Sigma-eLabs/uc-programming/tree/main/gpio/gpio-interrupt-button-pressed), you can prefer to.
<p align="center">
  <a href="." title="Code">
    <img src="/Board_Support_Packages/tivac-bsp-example/images/Code.PNG" title="Code" style="min-width: 200px"/>
  </a>
</p>

# My Recommendations:
- [🔨 Development Boards](https://github.com/ai-ho/embedded-launchpad/blob/main/development-boards)
- [🎓 Training Courses](https://github.com/ai-ho/embedded-launchpad/blob/main/training-courses)
- [📚 Suggestion Books](https://github.com/ai-ho/embedded-launchpad/blob/main/suggested-books)
- [🔍 Reference Mterials](https://github.com/ai-ho/embedded-launchpad/blob/main/referrence-materials)

# Embedded Insights
[🚀 Embedded Insights](/)

# Repositories
[🏠 My Repositories](https://github.com/ai-ho)

# Contact & Discussion
If you have any thing would like to discuss or cooperate with me, please don't hesitate to contact me via:
- 📧 Email [Ho Thien Ai](mailto:thienaiho95@gmail.com)
- 💼 LinkedIn [Thien Ai Ho](https://www.linkedin.com/in/thien-ai-ho/)
