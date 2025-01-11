#ifndef SYSDEF_H
#define SYSDEF_H
/*
 *** Try Kernel v2 システム（ハードウェア）関連共通定義
 */

/* メモリ関連 */
#define SRAM_START              (0x20000000)
#define SRAM_SIZE               (256*1024)

#define	INITIAL_SP              (SRAM_START + SRAM_SIZE)

/* APB ペリフェラル */
/* Clocks */
#define CLOCKS_BASE             0x40008000
#define CLK_GPOUT0              (CLOCKS_BASE+0x00)
#define CLK_GPOUT1              (CLOCKS_BASE+0x0C)
#define CLK_GPOUT2              (CLOCKS_BASE+0x18)
#define CLK_GPOUT3              (CLOCKS_BASE+0x24)
#define CLK_REF	                (CLOCKS_BASE+0x30)
#define CLK_SYS	                (CLOCKS_BASE+0x3C)
#define CLK_PERI                (CLOCKS_BASE+0x48)
#define CLK_USB	                (CLOCKS_BASE+0x54)
#define CLK_ADC	                (CLOCKS_BASE+0x60)
#define CLK_RTC	                (CLOCKS_BASE+0x6C)
#define CLK_SYS_RESUS_CTRL      (CLOCKS_BASE+0x78)
#define CLK_SYS_RESUS_STATUS    (CLOCKS_BASE+0x7C)

#define	CLK_x_CTRL              (0x00)
#define	CLK_x_DIV               (0x04)
#define	CLK_x_SELECTED          (0x08)

#define CLK_SYS_CTRL_AUXSRC     (0x000000e0)
#define CLK_SYS_CTRL_SRC        (0x00000001)
#define CLK_REF_CTRL_SRC        (0x00000003)
#define CLK_CTRL_ENABLE	        (0x00000800)

#define CLK_SYS_CTRL_SRC_AUX	(0x1)

#define CLK_KIND_GPOUT0         0
#define CLK_KIND_GPOUT1         1
#define CLK_KIND_GPOUT2         2
#define CLK_KIND_GPOUT3         3
#define CLK_KIND_REF            4
#define CLK_KIND_SYS            5
#define CLK_KIND_PERI           6
#define CLK_KIND_USB            7
#define CLK_KIND_ADC            8
#define CLK_KIND_RTC            9

/* Reset Controler */
#define RESETS_BASE             0x4000C000
#define RESETS_RESET            (RESETS_BASE+0x0)
#define RESETS_WDSEL            (RESETS_BASE+0x4)
#define RESETS_RESET_DONE       (RESETS_BASE+0x8)

#define RESETS_RESET_ADC        (0x00000001)
#define RESETS_RESET_I2C0       (0x00000008)
#define RESETS_RESET_I2C1       (0x00000010)

/* GPIO */
#define IO_BANK0_BASE           0x40014000

#define	GPIO_CTRL(n)            (IO_BANK0_BASE+0x04+(n*8))
#define	GPIO_CTRL_FUNCSEL_I2C   3
#define	GPIO_CTRL_FUNCSEL_NULL  31

#define INTR(n)             	(IO_BANK0_BASE+0x0F0+(n*4))

#define PROC0_INTE(n)       	(IO_BANK0_BASE+0x100+((n>>3)*4))
#define INTE_MODE_EDGE_HIGH     0x08
#define INTE_MODE_EDGE_LOW      0x04
#define INTE_MODE_LEVEL_HIGH    0x02
#define INTE_MODE_LEVEL_LOW     0x01
#define INTE_MODE_NON           0x00

#define PADS_BANK0_BASE         0x4001c000
#define	GPIO(n)                 (PADS_BANK0_BASE+0x4+(n*4))

#define	GPIO_OD                 (1<<7)
#define	GPIO_IE                 (1<<6)
#define	GPIO_DRIVE_2MA          (0<<4)
#define	GPIO_DRIVE_4MA          (1<<4)
#define	GPIO_DRIVE_8MA          (2<<4)
#define	GPIO_DRIVE_12MA         (3<<4)
#define	GPIO_PUE                (1<<3)
#define	GPIO_PDE                (1<<2)
#define	GPIO_SHEMITT            (1<<1)
#define	GPIO_SLEWDAST           (1<<0)

/* Crystal Oscillator(XOSC) */
#define XOSC_BASE               0x40024000
#define XOSC_CTRL               (XOSC_BASE+0x00)
#define XOSC_STATUS             (XOSC_BASE+0x04)
#define XOSC_STARTUP            (XOSC_BASE+0x0C)

#define	XOSC_CTRL_ENABLE        (0x00FAB000)
#define	XOSC_CTRL_DISABLE       (0x00D1E000)
#define	XOSC_CTRL_FRANG_1_15MHZ (0x00000AA0)

#define	XOSC_STATUS_STABLE      (0x80000000)

/* PLL */
#define PLL_SYS_BASE            (0x40028000)
#define PLL_USB_BASE            (0x4002C000)

#define PLL_CS                  (0x00)
#define	PLL_PWR                 (0x04)
#define	PLL_FBDIV_INT           (0x08)
#define	PLL_PRIM                (0x0C)

#define	PLL_CS_LOCK             (1<<31)
#define	PLL_PWR_PD              (1<<0)
#define	PLL_PWR_VCOPD           (1<<5)
#define	PLL_PWR_POSTDIVPD       (1<<3)
#define PLL_PRIM_POSTDIV1_LSB   (16)
#define PLL_PRIM_POSTDIV2_LSB   (12)

/* UART */
#define UART0_BASE              0x40034000
#define UART1_BASE              0x40038000

#define UARTx_DR                (0x000)
#define UARTx_FR                (0x018)
#define UARTx_IBRD              (0x024)
#define UARTx_FBRD              (0x028)
#define UARTx_LCR_H             (0x02C)
#define UARTx_CR                (0x030)

#define UART_CR_RXE             (1<<9)
#define UART_CR_TXE             (1<<8)
#define UART_CR_EN              (1<<0)
#define UART_FR_TXFF            (1<<5)

/* IOPORT レジスタ */
#define SIO_BASE                0xD0000000
#define	GPIO_IN                 (SIO_BASE+0x04)
#define GPIO_OUT                (SIO_BASE+0x10)
#define GPIO_OUT_SET            (SIO_BASE+0x14)
#define GPIO_OUT_CLR            (SIO_BASE+0x18)
#define GPIO_OUT_XOR            (SIO_BASE+0x1C)
#define GPIO_OE_SET             (SIO_BASE+0x24)
#define GPIO_OE_CLR             (SIO_BASE+0x28)
#define GPIO_OE_XOR             (SIO_BASE+0x2C)

/* PWM レジスタ  */
#define	PWM_BASE                0x40050000

#define PWM_CHx_CSR             (0x00)
#define PWM_CHx_DIV             (0x04)
#define PWM_CHx_CTR             (0x08)
#define PWM_CHx_CC              (0x0C)
#define PWM_CHx_TOP             (0x10)

#define PWM_CH_CSR(n)           (PWM_BASE + PWM_CHx_CSR + (n*0x14))
#define PWM_CH_DIV(n)           (PWM_BASE + PWM_CHx_DIV + (n*0x14))
#define PWM_CH_CTR(n)           (PWM_BASE + PWM_CHx_CTR + (n*0x14))
#define PWM_CH_CC(n)            (PWM_BASE + PWM_CHx_CC  + (n*0x14))
#define PWM_CH_TOP(n)           (PWM_BASE + PWM_CHx_TOP + (n*0x14))

#define PWM_CH_CSR_EN           (1<<0)

/* Timer レジスタ */
#define TIMER_BASE              (0x40054000)
#define TIMER_TIMEHR            (TIMER_BASE + 0x08)
#define TIMER_TIMELR            (TIMER_BASE + 0x0C)

/* Watchdog timer レジスタ */
#define	WDT_BASE                0x40058000
#define WDT_CTRL                (WDT_BASE+0x00)
#define WDT_LOAD                (WDT_BASE+0x04)
#define WDT_REASON              (WDT_BASE+0x80)
#define WDT_SCRATCH(n)          (WDT_BASE+0x0C+(n<<2))
#define WDT_TICK                (WDT_BASE+0x2C)

#define WDT_TICK_CYCLES         (0x000000FF)
#define WDT_TICK_ENABLE         (1<<9)

/* SysTick レジスタ */
#define SYST_CSR                (0xE000E010)
#define SYST_RVR                (0xE000E014)
#define SYST_CVR                (0xE000E018)

#define SYST_CSR_COUNTFLAG      (1<<16)
#define SYST_CSR_CLKSOURCE      (1<<2)
#define SYST_CSR_TICKINT        (1<<1)
#define SYST_CSR_ENABLE	        (1<<0)

/* クロック周波数 */
#define	CLOCK_XOSC              (12000000UL)
#define	CLOCK_REF               (CLOCK_XOSC)
#define	CLOCK_PERI              (CLOCK_SYS)

#define	XOSC_MHz                (12)
#define	XOSC_KHz                (XOSC_MHz*1000)

#define	TMCLK_MHz               (125)
#define	TMCLK_KHz               (TMCLK_MHz*1000)
#define	TIMER_PERIOD            (10)

#define	KHz                     (1000)
#define	MHz                     (KHz*1000)

/* 例外・割込み関連 */
/* SCB レジスタ */
#define SCB_VTOR                (0xE000ED08)
#define SCB_SHPR3               (0xE000ED20)

#define	INTLEVEL_0              (0x00)
#define	INTLEVEL_1              (0x40)
#define	INTLEVEL_2              (0x80)
#define	INTLEVEL_3              (0xC0)

/* NVIC レジスタ */
#define NVIC_ISER(x)            (0xE000E100 + (((x) / 32) << 2))
#define NVIC_ICER(x)            (0xE000E180 + (((x) / 32) << 2))
#define NVIC_ISPR(x)            (0xE000E200 + (((x) / 32) << 2))
#define NVIC_ICPR(x)            (0xE000E280 + (((x) / 32) << 2))
#define NVIC_IPR(x)             (0xE000E400 + (x))

#define	N_SYSVEC                16	/* 例外ベクタ数 */
#define N_INTVEC                32	/* 割込みベクタ数 */
#define INTPRI_BITWIDTH	        2

#define IRQ_TIMER0              0
#define IRQ_TIMER1              1
#define IRQ_TIMER2              2
#define IRQ_TIMER3              3
#define IRQ_PWM                 4
#define IRQ_USBCTRL             5
#define IRQ_XIP                 6
#define IRQ_PIO00               7
#define IRQ_PIO01               8
#define IRQ_PIO10               9
#define IRQ_PIO11               10
#define IRQ_DMA0                11
#define IRQ_DMA1                12
#define IRQ_BANK0               13
#define IRQ_QSPI                14
#define IRQ_SIOPR0              15
#define IRQ_SIOPR1              16
#define IRQ_CLOCKS              17
#define IRQ_SPI0                18
#define IRQ_SPI1                19
#define IRQ_UART0               20
#define IRQ_UART1               21
#define IRQ_ADC                 22
#define IRQ_I2C0                23
#define IRQ_I2C1                24
#define IRQ_RTC                 25

#endif  /* SYSDEF_H */