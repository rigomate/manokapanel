#include "fwlib/f1/cmsis/CM3/DeviceSupport/ST/STM32F10x/stm32f10x.h"
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;

void __attribute__((weak)) SystemInit(void);
extern int main(void);

void __attribute__((weak)) _estack(void);
void Reset_Handler(void);
void __attribute__((weak)) NMI_Handler(void);
void __attribute__((weak)) HardFault_Handler(void);
void __attribute__((weak)) MemManage_Handler(void);
void __attribute__((weak)) BusFault_Handler(void);
void __attribute__((weak)) UsageFault_Handler(void);
void __attribute__((weak)) SVC_Handler(void);
void __attribute__((weak)) DebugMon_Handler(void);
void __attribute__((weak)) PendSV_Handler(void);
void __attribute__((weak)) SysTick_Handler(void);
void __attribute__((weak)) WWDG_IRQHandler(void);
void __attribute__((weak)) PVD_IRQHandler(void);
void __attribute__((weak)) TAMPER_IRQHandler(void);
void __attribute__((weak)) RTC_IRQHandler(void);
void __attribute__((weak)) FLASH_IRQHandler(void);
void __attribute__((weak)) RCC_IRQHandler(void);
void __attribute__((weak)) EXTI0_IRQHandler(void);
void __attribute__((weak)) EXTI1_IRQHandler(void);
void __attribute__((weak)) EXTI2_IRQHandler(void);
void __attribute__((weak)) EXTI3_IRQHandler(void);
void __attribute__((weak)) EXTI4_IRQHandler(void);
void __attribute__((weak)) DMA1_Channel1_IRQHandler(void);
void __attribute__((weak)) DMA1_Channel2_IRQHandler(void);
void __attribute__((weak)) DMA1_Channel3_IRQHandler(void);
void __attribute__((weak)) DMA1_Channel4_IRQHandler(void);
void __attribute__((weak)) DMA1_Channel5_IRQHandler(void);
void __attribute__((weak)) DMA1_Channel6_IRQHandler(void);
void __attribute__((weak)) DMA1_Channel7_IRQHandler(void);
void __attribute__((weak)) ADC1_IRQHandler(void);
void __attribute__((weak)) EXTI9_5_IRQHandler(void);
void __attribute__((weak)) TIM1_BRK_TIM15_IRQHandler(void);
void __attribute__((weak)) TIM1_UP_TIM16_IRQHandler(void);
void __attribute__((weak)) TIM1_TRG_COM_TIM17_IRQHandler(void);
void __attribute__((weak)) TIM1_CC_IRQHandler(void);
void __attribute__((weak)) TIM2_IRQHandler(void);
void __attribute__((weak)) TIM3_IRQHandler(void);
void __attribute__((weak)) TIM4_IRQHandler(void);
void __attribute__((weak)) I2C1_EV_IRQHandler(void);
void __attribute__((weak)) I2C1_ER_IRQHandler(void);
void __attribute__((weak)) I2C2_EV_IRQHandler(void);
void __attribute__((weak)) I2C2_ER_IRQHandler(void);
void __attribute__((weak)) SPI1_IRQHandler(void);
void __attribute__((weak)) SPI2_IRQHandler(void);
void __attribute__((weak)) USART1_IRQHandler(void);
void __attribute__((weak)) USART2_IRQHandler(void);
void __attribute__((weak)) USART3_IRQHandler(void);
void __attribute__((weak)) EXTI15_10_IRQHandler(void);
void __attribute__((weak)) RTCAlarm_IRQHandler(void);
void __attribute__((weak)) CEC_IRQHandler(void);
void __attribute__((weak)) TIM6_DAC_IRQHandler(void);
void __attribute__((weak)) TIM7_IRQHandler(void);


void Default_Handler(void) {
    while (1) {
        // Infinite loop, preserve system state for examination by a debugger
    }
}

/* Vector Table */
__attribute__((section(".isr_vector")))
void (*const g_pfnVectors[])(void) = {
    (void (*)(void))&_estack,   // The initial stack pointer
    Reset_Handler,              // The reset handler
    NMI_Handler,                // NMI handler
    HardFault_Handler,          // Hard Fault handler
    MemManage_Handler,          // Memory Manage Fault handler
    BusFault_Handler,           // Bus Fault handler
    UsageFault_Handler,         // Usage Fault handler
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    SVC_Handler,                // SVCall handler
    DebugMon_Handler,           // Debug Monitor handler
    0,                          // Reserved
    PendSV_Handler,             // PendSV handler
    SysTick_Handler,            // SysTick handler
    WWDG_IRQHandler,            // Window Watchdog
    PVD_IRQHandler,             // PVD through EXTI Line detection
    TAMPER_IRQHandler,          // Tamper
    RTC_IRQHandler,             // RTC
    FLASH_IRQHandler,           // Flash
    RCC_IRQHandler,             // RCC
    EXTI0_IRQHandler,           // EXTI Line 0
    EXTI1_IRQHandler,           // EXTI Line 1
    EXTI2_IRQHandler,           // EXTI Line 2
    EXTI3_IRQHandler,           // EXTI Line 3
    EXTI4_IRQHandler,           // EXTI Line 4
    DMA1_Channel1_IRQHandler,   // DMA1 Channel 1
    DMA1_Channel2_IRQHandler,   // DMA1 Channel 2
    DMA1_Channel3_IRQHandler,   // DMA1 Channel 3
    DMA1_Channel4_IRQHandler,   // DMA1 Channel 4
    DMA1_Channel5_IRQHandler,   // DMA1 Channel 5
    DMA1_Channel6_IRQHandler,   // DMA1 Channel 6
    DMA1_Channel7_IRQHandler,   // DMA1 Channel 7
    ADC1_IRQHandler,            // ADC1
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    EXTI9_5_IRQHandler,         // EXTI Line 9..5
    TIM1_BRK_TIM15_IRQHandler,  // TIM1 Break and TIM15
    TIM1_UP_TIM16_IRQHandler,   // TIM1 Update and TIM16
    TIM1_TRG_COM_TIM17_IRQHandler, // TIM1 Trigger and Commutation and TIM17
    TIM1_CC_IRQHandler,         // TIM1 Capture Compare
    TIM2_IRQHandler,            // TIM2
    TIM3_IRQHandler,            // TIM3
    TIM4_IRQHandler,            // TIM4
    I2C1_EV_IRQHandler,         // I2C1 Event
    I2C1_ER_IRQHandler,         // I2C1 Error
    I2C2_EV_IRQHandler,         // I2C2 Event
    I2C2_ER_IRQHandler,         // I2C2 Error
    SPI1_IRQHandler,            // SPI1
    SPI2_IRQHandler,            // SPI2
    USART1_IRQHandler,          // USART1
    USART2_IRQHandler,          // USART2
    USART3_IRQHandler,          // USART3
    EXTI15_10_IRQHandler,       // EXTI Line 15..10
    RTCAlarm_IRQHandler,        // RTC Alarm through EXTI Line
    CEC_IRQHandler,             // CEC
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    0,                          // Reserved
    TIM6_DAC_IRQHandler,        // TIM6 and DAC underrun
    TIM7_IRQHandler             // TIM7
};

void Reset_Handler(void) {
    uint32_t *src, *dest;

    // Copy the data segment initializers from flash to SRAM
    src = &_sidata;
    dest = &_sdata;
    while (dest < &_edata) {
        *dest++ = *src++;
    }

    // Zero fill the bss segment
    dest = &_sbss;
    while (dest < &_ebss) {
        *dest++ = 0;
    }

    // Call the clock system initialization function
    SystemInit();

    // Call the application's entry point
    main();

    // We should never reach here
    while (1);
}
