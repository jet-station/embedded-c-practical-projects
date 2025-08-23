<p align="center">
  <a href="." title="Sigma eLabs">
    <img src="/assets/SigmaeLabsBannerv2.png" title="Sigma eLabs" style="width: 100vw; min-width: 200px"/>
  </a>
</p>

# Non-blocking Implementation For Time-Delay
In this topic I will talk about one of the solutions to implement the tradition time-dely function in the different way. The expectation is to remove the blocking waits in-line, efficiently utilize the CPU resource, and make the software is more responsive.
<p align="center">
  <a href="." title="Tiva C Series Launchpad Board Support Package">
    <img src="/Non_Blocking_Delay/Non_Blocking_Delay_Sample/images/non_blocking_delay.png" title="Tiva C Series Launchpad Board Support Package" style="min-width: 200px"/>
  </a>
</p>

# Problem With Traditional Time-Delay Function
When I looking back to the example [Board Support Package Example](https://github.com/Sigma-eLabs/embedded-software-programming-concepts/tree/main/Board_Support_Packages), where I demonstrate one implementation of Board Support Package for Tiva C Launch Pad, there are few things I'm not happy with:
* The first thing is the delay function. I used delay function to make the software wait for a while. After the waiting time is overed, the software continue to process the next instructions. The problem is the software do the for/while loop to increase/decrease a counter until the counter is reach the threshold (synchronous), but do nothing else. You can figure out that it's a wasteness, right? Maybe you can think that instead of sitting and waiting, the software use this time to process another job, then comeback and check the time later (asynchronous).
<p align="center">
  <a href="." title="Traditional Blocking Delay">
    <img src="/Non_Blocking_Delay/Non_Blocking_Delay_Sample/images/blocking_delay.png" title="Traditional Blocking Delay" style="min-width: 200px"/>
  </a>
</p>

* The second thing is I used the SW1 button state (pressed or released) to check whether the LED blinky should be peformed in the main() function. However, I think that it's may caused the misunderstanding for you because in reality the state pressed of SW1 button is changed quickly for a momment and not keep for a long time. So, I thing there is a chance to improve the implementation to make the software is more robustness.
<p align="center">
  <a href="." title="Uncleared Implementation">
    <img src="/Non_Blocking_Delay/Non_Blocking_Delay_Sample/images/uncleared_implementation.png" title="Uncleared Implementation" style="min-width: 200px"/>
  </a>
</p>

# Solution
* To resolve the problem with tradition delay function, I need the support of the "watch". In ARM-CortexM, we have System Tick Timer, which is suitable to support this case. I will use one counter to tracking the time-stamp when the software running to check if the delay time is elapsed. Beside of that, I need the state of blinky job to decide which actions should be done like turn LED on/off, check if the desired time-delay is elapsed, initialize/reset the job.
<p align="center">
  <a href="." title="Uncleared Implementation">
    <img src="/Non_Blocking_Delay/Non_Blocking_Delay_Sample/images/solution.png" title="Uncleared Implementation" style="min-width: 200px"/>
  </a>
</p>

* For the secode problem, instead of using the state of SW1 button, I use the state of the blinky job, which is "requested" in case the button is pressed and "finished" when the blinky job is finished or initialized.

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
I place the source code of this example here: :rocket: [Non-blocking delay Example](/Non_Blocking_Delay/Non_Blocking_Delay_Sample/non-blocking-delay-sample/) :rocket:

* Implemation of SysTick_Handler() function, which will update the tick counter every system tick is eslapsed.
  ```C
  void SysTick_Handler(void) {
    ++l_tickCounter;
  }
  ```

* Enable the use of System Tick:
  ```C
	/* interrupt set up for System Tick */
	SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / BSP_TICKS_PER_SEC); /* by this setting, the Sytem Tick will occured 100 (100Hz) times every second */
  NVIC_SetPriority(SysTick_IRQn, 0U); /* set the SysTick interrupt priority (highest) */
  ```
* To get the current time-stamp of the software, I use this function:
  ```C
  uint32_t BSP_getTickCounter(void) {
    uint32_t tickCounter;
    
    /* enter critical context here is to prevent
    interrupt occurs and modifies the value of l_tickCounter */
    __disable_irq();
    tickCounter = l_tickCounter;
    __enable_irq();
    
    return tickCounter;
  }
  ```
* LED blinky state is defined with 4 state here:
  ```C
  typedef enum {
    BLINKY_INITIAL,
    BLINKY_OFF,
    BLINKY_ON,
    BLINKY_STOP
  } BlinkyState;
  ```

* To check if the delay time of LED on/off state is overed, I use the combination of LED blinky state and logic to check:
  ```C
  case BLINKY_ON: {
    if ((BSP_getTickCounter() - start) > (BSP_TICKS_PER_SEC / 2U)) {
      BSP_setRedledState(LED_OFF);
      blinkyState = BLINKY_OFF;
      start = BSP_getTickCounter();
    }
    break;
  }
  ```

* The blinky job is known as Requested if the SW1 button is pressed, and reset back to Finished once the job is done.
  ```C
  /* interrupt function is invoked when GPIOF interrupt happened */
  void GPIOPortF_IRQHandler(void) {
    if ((GPIOF_AHB->RIS & USR_SW1) != 0U) { /* check if the interrupt caused by User button switch 1 - PF4 pin pressed */
      blinkyJobStat = JOB_REQUESTED;	/* blinky job requested */
    }
    
    GPIOF_AHB->ICR = USR_SW1;	/* clear the interrupt source caused by User button switch 1, otherwise it is remained forever */
  }
  ```
  ```C
  case BLINKY_STOP: {
      /* reset the state of UsrSw1 button */
      blinkyJobStat = JOB_FINISHED;	/* job is done */
      blinkyState = BLINKY_INITIAL;	/* reset the blinky state */
    break;
  }
  ```
* Now, the tradition BSP_delay() function is not needed anymore and the software can use the CPU resource to do another useful thing. Also, the software implementation is more readable than before.

* To implement this software, you should read [Tiva™ TM4C123GH6PM Microcontroller Datasheet](https://github.com/Sigma-eLabs/technical-resources/blob/main/tiva-c-series/tm4c123gh6pm.pdf), the chapters related to GPIO, System Control modules to know about the functionality of common registers. Especially, the you should read about the interrupt related registers.

* You also should read [Tiva™ C Series TM4C123G LaunchPad Evaluation Board User's Guide](https://github.com/Sigma-eLabs/technical-resources/blob/main/tiva-c-series/spmu296.pdf) to know about the hardware design of the development kit which is being used.

* Additionally, you understand and know how to use the provided standard libraries like [Cortex Microcontroller Software Interface Standard (CMSIS)](https://github.com/Sigma-eLabs/technical-resources/tree/main/tiva-c-series/CMSIS), [Tiva™ TM4C123GH6PM Microcontroller Software Support Package](https://github.com/Sigma-eLabs/technical-resources/tree/main/tiva-c-series/ek-tm4c123gxl), which support Arm-Cortex architecture processor and TM4C123GHPM controller.

# Deployment & Verification
The software is flashed inton Tiva C Launch Pad. I reset the software, pressed the SW1 button and recorded the result by Logic Analyzer:
<p align="center">
  <a href="." title="Demostrantion">
    <img src="/Non_Blocking_Delay/Non_Blocking_Delay_Sample/images/demonstration.png" title="Demontration" style="min-width: 200px"/>
  </a>
</p>


# Drawbacks
* Even though I have demonstrated one solution to elimate the blocking wait in-line and help the software can utilize the CPU to process another job to make it's more responsive. However, I need to enable System Tick, use more state variables to hold the information and the comlexity of logic is increased. Which means, I have to design and implement the software more carefully.

* Nothing is perfect and the software is also the same. I see that there are many chances to improve. Can you try it?

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
