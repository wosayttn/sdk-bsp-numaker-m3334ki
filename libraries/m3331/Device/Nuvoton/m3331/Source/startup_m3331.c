/**************************************************************************//**
 * @file     startup_m3331.c
 * @version  V3.00
 * @brief    CMSIS Device Startup File for NuMicro M3331
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2023 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#include <inttypes.h>
#include <stdio.h>
#include "NuMicro.h"

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void(*VECTOR_TABLE_Type)(void);

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern __NO_RETURN void __PROGRAM_START(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void);
void Default_Handler(void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* External Interrupts */
void BOD_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 0: Brown Out detection
void IRC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 1: Internal RC
void PWRWU_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 2: Power down wake up
void RAMPE_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 3: RAM parity error
void CLKFAIL_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 4: Clock detection fail
void ISP_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 5: ISP
void RTC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 6: Real Time Clock
void WDT0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 8: Watchdog timer
void WWDT0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 9: Window watchdog timer
void EINT0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 10: External Input 0
void EINT1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 11: External Input 1
void EINT2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 12: External Input 2
void EINT3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 13: External Input 3
void EINT4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 14: External Input 4
void EINT5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 15: External Input 5
void GPA_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 16: GPIO Port A
void GPB_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 17: GPIO Port B
void GPC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 18: GPIO Port C
void GPD_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 19: GPIO Port D
void GPE_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 20: GPIO Port E
void GPF_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 21: GPIO Port F
void QSPI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 22: QSPI0
void SPI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 23: SPI0
void EBRAKE0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 24: EBRAKE0
void EPWM0P0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 25: EPWM0P0
void EPWM0P1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 26: EPWM0P1
void EPWM0P2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 27: EPWM0P2
void EBRAKE1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 28: EBRAKE1
void EPWM1P0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 29: EPWM1P0
void EPWM1P1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 30: EPWM1P1
void EPWM1P2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 31: EPWM1P2
void TMR0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 32: Timer 0
void TMR1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 33: Timer 1
void TMR2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 34: Timer 2
void TMR3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 35: Timer 3
void UART0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 36: UART0
void UART1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 37: UART1
void I2C0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 38: I2C0
void I2C1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 39: I2C1
void PDMA0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 40: Peripheral DMA 0
void EADC00_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));     // 42: EADC0 interrupt source 0
void EADC01_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));     // 43: EADC0 interrupt source 1
void ACMP01_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));     // 44: ACMP0 and ACMP1
void EADC02_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));     // 46: EADC0 interrupt source 2
void EADC03_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));     // 47: EADC0 interrupt source 3
void UART2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 48: UART2
void UART3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 49: UART3
void SPI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 51: SPI1
void SPI2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 52: SPI2
void HSUSBH_OHCI_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 54: HSUSBH_OHCI
void ETI_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 56: ETI
void CRC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 57: CRC0
void NS_ISP_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));     // 59: NS ISP
void SCU_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 60: SCU
void SDH0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 64: SDH0
void HSUSBD_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));     // 65: HSUSBD
void WDT1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 67: WDT1
void I2S0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 68: I2S0
void GPG_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 72: GPIO Port G
void EINT6_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 73: External Input 6
void UART4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 74: UART4
void USCI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 76: USCI0
void USCI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 77: USCI1
void BPWM0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 78: BPWM0
void BPWM1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 79: BPWM1
void I2C2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 82: I2C2
void EQEI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 84: EQEI0
void ECAP0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 86: ECAP0
void GPH_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));        // 88: GPIO Port H
void EINT7_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 89: External Input 7
void WWDT1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 91: WWDT1
void HSUSBH_EHCI_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 92: HSUSBH_EHCI
void HSOTG_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 93: HSOTG
void NS_RAMPE_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));   // 96: NS RAMPE
void PDMA1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 98: PDMA1
void I3C0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 108: I3C0
void CANFD00_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 112: CAN FD 00
void CANFD01_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 113: CAN FD 01
void CANFD10_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 114: CAN FD 10
void CANFD11_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));    // 115: CAN FD 11
void PDCI_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));       // 119: PDCI
void LLSI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 128: LLSI0
void LLSI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 129: LLSI1
void LLSI2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 130: LLSI1
void LLSI3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 131: PWM0P2
void LLSI4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 132: BRAKE1
void LLSI5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 133: PWM1P0
void LLSI6_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 134: PWM1P1
void LLSI7_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 135: PWM1P2
void LLSI8_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 136: LPADC0
void LLSI9_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 137: LPUART0
void ELLSI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));     // 138: LPI2C0
void BPWM2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 139: LPSPI0
void BPWM3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 140: LPTMR0
void BPWM4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 141: LPTMR1
void BPWM5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));      // 142: TTMR0

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
#if defined ( __GNUC__ )
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
#endif

const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE =
{
    (VECTOR_TABLE_Type)(&__INITIAL_SP),       /*       Initial Stack Pointer                             */
    Reset_Handler,                            /*       Reset Handler                                     */
    NMI_Handler,                              /*   -14 NMI Handler                                       */
    HardFault_Handler,                        /*   -13 Hard Fault Handler                                */
    MemManage_Handler,                        /*   -12 MPU Fault Handler                                 */
    BusFault_Handler,                         /*   -11 Bus Fault Handler                                 */
    UsageFault_Handler,                       /*   -10 Usage Fault Handler                               */
    0,                                        /*    -9 Reserved                                          */
    0,                                        /*    -8 Reserved                                          */
    0,                                        /*    -7 Reserved                                          */
    0,                                        /*    -6 Reserved                                          */
    SVC_Handler,                              /*    -5 SVC Handler                                       */
    DebugMon_Handler,                         /*    -4 Debug Monitor Handler                             */
    0,                                        /*    -3 Reserved                                          */
    PendSV_Handler,                           /*    -2 PendSV Handler Handler                            */
    SysTick_Handler,                          /*    -1 SysTick Handler                                   */

    /* Interrupts */
    BOD_IRQHandler,                           /*    0: Brown Out detection                               */
    IRC_IRQHandler,                           /*    1: Internal RC                                       */
    PWRWU_IRQHandler,                         /*    2: Power down wake up                                */
    RAMPE_IRQHandler,                         /*    3: RAM parity error                                  */
    CLKFAIL_IRQHandler,                       /*    4: Clock detection fail                              */
    ISP_IRQHandler,                           /*    5: ISP                                               */
    RTC_IRQHandler,                           /*    6: Real Time Clock                                   */
    0,                                        /*    7: Reserved                                          */
    WDT0_IRQHandler,                          /*    8: Watchdog timer                                    */
    WWDT0_IRQHandler,                         /*    9: Window watchdog timer                             */
    EINT0_IRQHandler,                         /*    10: External Input 0                                 */
    EINT1_IRQHandler,                         /*    11: External Input 1                                 */
    EINT2_IRQHandler,                         /*    12: External Input 2                                 */
    EINT3_IRQHandler,                         /*    13: External Input 3                                 */
    EINT4_IRQHandler,                         /*    14: External Input 4                                 */
    EINT5_IRQHandler,                         /*    15: External Input 5                                 */
    GPA_IRQHandler,                           /*    16: GPIO Port A                                      */
    GPB_IRQHandler,                           /*    17: GPIO Port B                                      */
    GPC_IRQHandler,                           /*    18: GPIO Port C                                      */
    GPD_IRQHandler,                           /*    19: GPIO Port D                                      */
    GPE_IRQHandler,                           /*    20: GPIO Port E                                      */
    GPF_IRQHandler,                           /*    21: GPIO Port F                                      */
    QSPI0_IRQHandler,                         /*    22: QSPI0                                            */
    SPI0_IRQHandler,                          /*    23: SPI0                                             */
    EBRAKE0_IRQHandler,                       /*    24: EBRAKE0                                          */
    EPWM0P0_IRQHandler,                       /*    25: EPWM0P0                                          */
    EPWM0P1_IRQHandler,                       /*    26: EPWM0P1                                          */
    EPWM0P2_IRQHandler,                       /*    27: EPWM0P2                                          */
    EBRAKE1_IRQHandler,                       /*    28: EBRAKE1                                          */
    EPWM1P0_IRQHandler,                       /*    29: EPWM1P0                                          */
    EPWM1P1_IRQHandler,                       /*    30: EPWM1P1                                          */
    EPWM1P2_IRQHandler,                       /*    31: EPWM1P2                                          */
    TMR0_IRQHandler,                          /*    32: Timer 0                                          */
    TMR1_IRQHandler,                          /*    33: Timer 1                                          */
    TMR2_IRQHandler,                          /*    34: Timer 2                                          */
    TMR3_IRQHandler,                          /*    35: Timer 3                                          */
    UART0_IRQHandler,                         /*    36: UART0                                            */
    UART1_IRQHandler,                         /*    37: UART1                                            */
    I2C0_IRQHandler,                          /*    38: I2C0                                             */
    I2C1_IRQHandler,                          /*    39: I2C1                                             */
    PDMA0_IRQHandler,                         /*    40: Peripheral DMA 0                                 */
    0,                                        /*    41: Reserved                                         */
    EADC00_IRQHandler,                        /*    42: EADC0 interrupt source 0                         */
    EADC01_IRQHandler,                        /*    43: EADC0 interrupt source 1                         */
    ACMP01_IRQHandler,                        /*    44: ACMP0 and ACMP1                                  */
    0,                                        /*    45: Reserved                                         */
    EADC02_IRQHandler,                        /*    46: EADC0 interrupt source 2                         */
    EADC03_IRQHandler,                        /*    47: EADC0 interrupt source 3                         */
    UART2_IRQHandler,                         /*    48: UART2                                            */
    UART3_IRQHandler,                         /*    49: UART3                                            */
    0,                                        /*    50: Reserved                                         */
    SPI1_IRQHandler,                          /*    51: SPI1                                             */
    SPI2_IRQHandler,                          /*    52: SPI2                                             */
    0,                                        /*    53: Reserved                                         */
    HSUSBH_OHCI_IRQHandler,                   /*    54: HSUSBH_OHCI                                      */
    0,                                        /*    55: Reserved                                         */
    ETI_IRQHandler,                           /*    56: ETI                                              */
    CRC_IRQHandler,                           /*    57: CRC0                                             */
    0,                                        /*    58: Reserved                                         */
    NS_ISP_IRQHandler,                        /*    59: NS ISP                                           */
    SCU_IRQHandler,                           /*    60: SCU                                              */
    0,                                        /*    61: Reserved                                         */
    0,                                        /*    62: Reserved                                         */
    0,                                        /*    63: Reserved                                         */
    SDH0_IRQHandler,                          /*    64: SDH0                                             */
    HSUSBD_IRQHandler,                        /*    65: HSUSBD                                           */
    0,                                        /*    66: Reserved                                         */
    WDT1_IRQHandler,                          /*    67: WDT1                                             */
    I2S0_IRQHandler,                          /*    68: I2S0                                             */
    0,                                        /*    69: Reserved                                         */
    0,                                        /*    70: OPA012                                           */
    0,                                        /*    71: CRPT                                             */
    GPG_IRQHandler,                           /*    72: GPIO Port G                                      */
    EINT6_IRQHandler,                         /*    73: External Input 6                                 */
    UART4_IRQHandler,                         /*    74: UART4                                            */
    0,                                        /*    75: Reserved                                         */
    USCI0_IRQHandler,                         /*    76: USCI0                                            */
    USCI1_IRQHandler,                         /*    77: USCI1                                            */
    BPWM0_IRQHandler,                         /*    78: BPWM0                                            */
    BPWM1_IRQHandler,                         /*    79: BPWM1                                            */
    0,                                        /*    80: Reserved                                         */
    0,                                        /*    81: Reserved                                         */
    I2C2_IRQHandler,                          /*    82: I2C2                                             */
    0,                                        /*    83: Reserved                                         */
    EQEI0_IRQHandler,                         /*    84: EQEI0                                            */
    0,                                        /*    85: Reserved                                         */
    ECAP0_IRQHandler,                         /*    86: ECAP0                                            */
    0,                                        /*    87: Reserved                                         */
    GPH_IRQHandler,                           /*    88: GPIO Port H                                      */
    EINT7_IRQHandler,                         /*    89: External Input 7                                 */
    0,                                        /*    90: Reserved                                         */
    WWDT1_IRQHandler,                         /*    91: WWDT1                                            */
    HSUSBH_EHCI_IRQHandler,                   /*    92: HSUSBH_EHCI                                      */
    HSOTG_IRQHandler,                         /*    93: HSOTG                                            */
    0,                                        /*    94: Reserved                                         */
    0,                                        /*    95: Reserved                                         */
    NS_RAMPE_IRQHandler,                      /*    96: NS RAMPE                                         */
    0,                                        /*    97: Reserved                                         */
    PDMA1_IRQHandler,                         /*    98: Peripheral DMA 1                                 */
    0,                                        /*    99: Reserved                                         */
    0,                                        /*    100: Reserved                                        */
    0,                                        /*    101: Reserved                                        */
    0,                                        /*    102: Reserved                                        */
    0,                                        /*    103: Reserved                                        */
    0,                                        /*    104: Reserved                                        */
    0,                                        /*    105: Reserved                                        */
    0,                                        /*    106: Reserved                                        */
    0,                                        /*    107: Reserved                                        */
    I3C0_IRQHandler,                          /*    108: I3C0                                            */
    0,                                        /*    109: Reserved                                        */
    0,                                        /*    110: Reserved                                        */
    0,                                        /*    110: Reserved                                        */
    CANFD00_IRQHandler,                       /*    112: CAN FD 00                                       */
    CANFD01_IRQHandler,                       /*    113: CAN FD 01                                       */
    CANFD10_IRQHandler,                       /*    114: CAN FD 10                                       */
    CANFD11_IRQHandler,                       /*    115: CAN FD 11                                       */
    0,                                        /*    116: Reserved                                        */
    0,                                        /*    117: Reserved                                        */
    0,                                        /*    118: Reserved                                        */
    PDCI_IRQHandler,                          /*    119: PDCI                                            */
    0,                                        /*    120: Reserved                                        */
    0,                                        /*    121: Reserved                                        */
    0,                                        /*    122: Reserved                                        */
    0,                                        /*    123: Reserved                                        */
    0,                                        /*    124: Reserved                                        */
    0,                                        /*    125: Reserved                                        */
    0,                                        /*    126: Reserved                                        */
    0,                                        /*    127: Reserved                                        */
    LLSI0_IRQHandler,                         /*    128: LLSI0                                           */
    LLSI1_IRQHandler,                         /*    129: LLSI1                                           */
    LLSI2_IRQHandler,                         /*    130: LLSI2                                           */
    LLSI3_IRQHandler,                         /*    131: LLSI3                                           */
    LLSI4_IRQHandler,                         /*    132: LLSI4                                           */
    LLSI5_IRQHandler,                         /*    133: LLSI5                                           */
    LLSI6_IRQHandler,                         /*    134: LLSI6                                           */
    LLSI7_IRQHandler,                         /*    135: LLSI7                                           */
    LLSI8_IRQHandler,                         /*    136: LLSI8                                           */
    LLSI9_IRQHandler,                         /*    137: LLSI9                                           */
    ELLSI0_IRQHandler,                        /*    138: ELLSI0                                          */
    BPWM2_IRQHandler,                         /*    139: BPWM2                                           */
    BPWM3_IRQHandler,                         /*    140: BPWM3                                           */
    BPWM4_IRQHandler,                         /*    141: BPWM4                                           */
    BPWM5_IRQHandler,                         /*    142: BPWM5                                           */
};

#if defined ( __GNUC__ )
    #pragma GCC diagnostic pop
#endif

__WEAK void Reset_Handler_PreInit(void)
{
    // Empty function
}

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void)
{
    // Copy __set_PSP/__set_MSPLIM/__set_PSPLIM from cmsis_armclang.h
    __ASM volatile("MSR psp, %0" : : "r"((uint32_t)(&__INITIAL_SP)) :);
    __ASM volatile("MSR msplim, %0" : : "r"((uint32_t)(&__STACK_LIMIT)));
    __ASM volatile("MSR psplim, %0" : : "r"((uint32_t)(&__STACK_LIMIT)));

#if defined(BSP_USING_CUSTOM_LOADER)
    uint32_t custom_loader_exec_last(void);
    if (custom_loader_exec_last())
    {
        __ASM volatile("MSR psp, %0" : : "r"((uint32_t)(&__INITIAL_SP)) :);
        __ASM volatile("MSR msplim, %0" : : "r"((uint32_t)(&__STACK_LIMIT)));
        __ASM volatile("MSR psplim, %0" : : "r"((uint32_t)(&__STACK_LIMIT)));
    }
#endif

#if defined(__SECURE_CODE) || !defined(__NONSECURE_CODE)

    SYS_UnlockReg();

    /* Keep Cortex-M33 CONTROL register reset value */
    __set_CONTROL(0);
    __ISB();

    /* Enable CACHE. NOTE: Data coherence must be care about when using FMC ISP. */
    CACHE_S->CTL |= CACHE_CTL_CACHEEN_Msk;

    /* HXT filter select */
    outpw(0x400002D4, 0x00FF8800);

    /* Initial default LXT Gain mode */
    RTC_LXTGainInit();
#endif
	
    Reset_Handler_PreInit();

    SystemInit();

    __PROGRAM_START();      // Enter PreMain (C library entry point)
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void)
{
    while (1);
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic pop
#endif
