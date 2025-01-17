/*
 * Copyright (C) 2015 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    cpu_msp430fxyz TI MSP430F
 * @ingroup     cpu
 * @brief       Texas Instruments MSP430F family specific code
 * @{
 *
 * @file
 * @brief           CPU specific definitions for internal peripheral handling
 *
 * @author          Hauke Petersen <hauke.petersen@fu-berlin.de>
 */

#ifndef PERIPH_CPU_H
#define PERIPH_CPU_H

#include <stdbool.h>

#include "bitarithm.h"
#include "cpu.h"
#include "msp430_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Define a custom type for GPIO pins
 * @{
 */
#define HAVE_GPIO_T
typedef uint16_t gpio_t;
/** @} */

/**
 * @brief   Definition of a fitting UNDEF value
 */
#define GPIO_UNDEF          (0xffff)

/**
 * @brief   Mandatory function for defining a GPIO pins
 */
#define GPIO_PIN(x, y)      ((gpio_t)(((x & 0xff) << 8) | (1 << (y & 0x07))))

/**
 * @brief   No support for HW chip select...
 */
#define SPI_HWCS(x)         (SPI_CS_UNDEF)

#ifndef DOXYGEN
/**
 * @brief   Override flank selection values
 * @{
 */
#define HAVE_GPIO_FLANK_T
typedef enum {
    GPIO_FALLING = 0xff,        /**< emit interrupt on falling flank */
    GPIO_RISING  = 0x00,        /**< emit interrupt on rising flank */
    GPIO_BOTH    = 0xab         /**< not supported -> random value*/
} gpio_flank_t;
/** @} */

/**
 * @brief   Override SPI mode selection values
 * @{
 */
#define HAVE_SPI_MODE_T
#ifndef SPI_USE_USCI
typedef enum {
    SPI_MODE_0 = (USART_TCTL_CKPH),                         /**< CPOL=0, CPHA=0 */
    SPI_MODE_1 = 0,                                         /**< CPOL=0, CPHA=1 */
    SPI_MODE_2 = (USART_TCTL_CKPL | USART_TCTL_CKPH),       /**< CPOL=1, CPHA=0 */
    SPI_MODE_3 = (USART_TCTL_CKPL)                          /**< CPOL=1, CPHA=1 */
} spi_mode_t;
#else
typedef enum {
    SPI_MODE_0 = (USCI_SPI_CTL0_CKPH),                      /**< CPOL=0, CPHA=0 */
    SPI_MODE_1 = 0,                                         /**< CPOL=0, CPHA=1 */
    SPI_MODE_2 = (USCI_SPI_CTL0_CKPL | USCI_SPI_CTL0_CKPH), /**< CPOL=1, CPHA=0 */
    SPI_MODE_3 = (USCI_SPI_CTL0_CKPL)                       /**< CPOL=1, CPHA=1 */
} spi_mode_t;
#endif
/** @} */

/**
 * @brief   Override SPI clock speed selection values
 * @{
 */
#define HAVE_SPI_CLK_T
typedef enum {
    SPI_CLK_100KHZ = 100000,    /**< 100KHz */
    SPI_CLK_400KHZ = 400000,    /**< 400KHz */
    SPI_CLK_1MHZ   = 1000000,   /**< 1MHz */
    SPI_CLK_5MHZ   = 5000000,   /**< 5MHz */
    SPI_CLK_10MHZ  = 0,         /**< not supported */
} spi_clk_t;
/** @} */
#endif /* ndef DOXYGEN */

/**
 * @brief   Available ports on MSP430 platforms
 */
enum {
    P1 = 1,                 /**< PORT 1 */
    P2 = 2,                 /**< PORT 2 */
    P3 = 3,                 /**< PORT 3 */
    P4 = 4,                 /**< PORT 4 */
    P5 = 5,                 /**< PORT 5 */
    P6 = 6,                 /**< PORT 6 */
};

/**
 * @brief   Enable or disable a pin to be used by peripheral modules
 *
 * @param[in] pin       pin to (de-)select
 * @param[in] enable    true for enabling peripheral use, false for disabling it
 */
void gpio_periph_mode(gpio_t pin, bool enable);

/**
 * @brief declare needed generic SPI functions
 * @{
 */
#define PERIPH_SPI_NEEDS_INIT_CS
#define PERIPH_SPI_NEEDS_TRANSFER_BYTE
#define PERIPH_SPI_NEEDS_TRANSFER_REG
#define PERIPH_SPI_NEEDS_TRANSFER_REGS
/** @} */

/**
 * @brief   The measured DCO frequency
 *
 * @warning This will have a value of 0 if the DCO is not enabled in the boards
 *          `periph_conf.h`
 */
extern uint32_t msp430_fxyz_dco_freq;

/**
 * @brief   Possible clock sources to generate the main clock from
 */
typedef enum {
    /**
     * @brief   Internal digitally controlled oscillator (DCO) with RC-type
     *          characteristics.
     */
    MAIN_CLOCK_SOURCE_DCOCLK       = SELM_0,
    /**
     * @brief   High frequency crystal between 450 kHz and 8 MHz
     *
     * @note    If XT2 is not enabled, this will behave identical to
     *          @ref MAIN_CLOCK_SOURCE_LFXT1CLK
     */
    MAIN_CLOCK_SOURCE_XT2CLK       = SELM_2,
    /**
     * @brief   Low frequency 32.768 kHz or high frequency crystal
     *          between 450 kHz and 8 MHz
     */
    MAIN_CLOCK_SOURCE_LFXT1CLK     = SELM_3,
} msp430_fxyz_main_clock_source_t;

/**
 * @brief   Possible clock sources to generate the submain clock from
 */
typedef enum {
    /**
     * @brief   Internal digitally controlled oscillator (DCO) with RC-type
     *          characteristics.
     */
    SUBMAIN_CLOCK_SOURCE_DCOCLK        = 0,
    /**
     * @brief   High frequency crystal between 450 kHz and 8 MHz
     *
     * @note    If XT2 is not enabled, this will behave identical to
     *          @ref SUBMAIN_CLOCK_SOURCE_LFXT1CLK
     */
    SUBMAIN_CLOCK_SOURCE_XT2CLK        = SELS,
    /**
     * @brief   Low frequency 32.768 kHz or high frequency crystal
     *          between 450 kHz and 8 MHz
     *
     * @note    If XT2 is enabled, this will behave identical to
     *          @ref SUBMAIN_CLOCK_SOURCE_XT2CLK
     *
     * It is recommended to use @ref SUBMAIN_CLOCK_SOURCE_XT2CLK when XT2 is present
     * in the config to avoid confusion.
     */
    SUBMAIN_CLOCK_SOURCE_LFXT1CLK      = SELS,
} msp430_fxyz_submain_clock_source_t;

/**
 * @brief   Clock dividers for the main clock
 */
typedef enum {
    /**
     * @brief   Divide the main clock by 1
     */
    MAIN_CLOCK_DIVIDE_BY_1      = DIVM_0,
    /**
     * @brief   Divide the main clock by 2
     */
    MAIN_CLOCK_DIVIDE_BY_2      = DIVM_1,
    /**
     * @brief   Divide the main clock by 4
     */
    MAIN_CLOCK_DIVIDE_BY_4      = DIVM_2,
    /**
     * @brief   Divide the main clock by 8
     */
    MAIN_CLOCK_DIVIDE_BY_8      = DIVM_3,
} msp430_fxyz_main_clock_divider_t;

/**
 * @brief   Clock dividers for the submain clock
 */
typedef enum {
    /**
     * @brief   Divide the main clock by 1
     */
    SUBMAIN_CLOCK_DIVIDE_BY_1   = DIVS_0,
    /**
     * @brief   Divide the main clock by 2
     */
    SUBMAIN_CLOCK_DIVIDE_BY_2   = DIVS_1,
    /**
     * @brief   Divide the main clock by 4
     */
    SUBMAIN_CLOCK_DIVIDE_BY_4   = DIVS_2,
    /**
     * @brief   Divide the main clock by 8
     */
    SUBMAIN_CLOCK_DIVIDE_BY_8   = DIVS_3,
} msp430_fxyz_submain_clock_divider_t;

/**
 * @brief   Clock dividers for the auxiliary clock
 */
typedef enum {
    /**
     * @brief   Divide the main clock by 1
     */
    AUXILIARY_CLOCK_DIVIDE_BY_1   = DIVA_0,
    /**
     * @brief   Divide the main clock by 2
     */
    AUXILIARY_CLOCK_DIVIDE_BY_2   = DIVA_1,
    /**
     * @brief   Divide the main clock by 4
     */
    AUXILIARY_CLOCK_DIVIDE_BY_4   = DIVA_2,
    /**
     * @brief   Divide the main clock by 8
     */
    AUXILIARY_CLOCK_DIVIDE_BY_8   = DIVA_3,
} msp430_fxyz_auxiliary_clock_divider_t;

/**
 * @brief   MSP430Fxzy Basic Clock System Parameters
 *
 * @details This structure is optimized for readability under the assumption
 *          that its values are readily available compile time constants.
 *          Hence, accesses to the structure will be fully optimized out be
 *          a decent optimizing compiler.
 *
 * MSP430Fxzy boards are expected to declare the parameters to configure the
 * Basic Clock System in their `periph_conf.h` as
 * `static const msp430_fxzy_clock_params_t clock_params;`.
 */
typedef struct {
    /**
     * @brief   The target frequency to run the DCO at in Hz
     *
     * A frequency between 160 kHz and 4 MHz is enforced by compile time tests
     * to make sure DCO calibration will succeed reliable.
     */
    uint32_t target_dco_frequency;
    /**
     * @brief   The frequency of the LFXT1 crystal in Hz
     *
     * This should be either 32768 Hz watch crystal or a high frequency
     * crystal anywhere between 450 kHz and 8 MHz. The clock driver will
     * automatically configure high frequency mode of the LFXT1 clock source
     * when the frequency is >= 450 kHz.
     */
    uint32_t lfxt1_frequency;
    /**
     * @brief   The frequency of the XT2 crystal in Hz
     *
     * A value of 0 Hz means no XT2 crystal is present. Otherwise the frequency
     * must be anywhere between 450 kHz and 8 MHz.
     */
    uint32_t xt2_frequency;
    /**
     * @brief   The clock source to select for the main clock
     *
     * @details This is the clock the CPU will run at
     */
    msp430_fxyz_main_clock_source_t main_clock_source;
    /**
     * @brief   The clock source to select for the submain CPU clock
     *
     * @details This is the clock high speed peripherals will run at
     */
    msp430_fxyz_submain_clock_source_t submain_clock_source;
    /**
     * @brief   Divider of the main clock
     */
    msp430_fxyz_main_clock_divider_t main_clock_divier;
    /**
     * @brief   Divider of the submain clock
     */
    msp430_fxyz_submain_clock_divider_t submain_clock_divier;
    /**
     * @brief   Divider of the auxiliary clock
     */
    msp430_fxyz_auxiliary_clock_divider_t auxiliary_clock_divier;
    /**
     * @brief   An external resistor connected to source the current for the DCO
     *
     * From the datasheet:
     *
     * > The DCO temperature coefficient can be reduced by using an external
     * > resistor R_OSC tied to D_VCC to source the current for the DC generator.
     */
    bool has_r_osc;
    /**
     * @brief   A high frequency crystal (e.g. 8 MHz) is connected between
     *          XT2IN and XT2OUT
     */
    bool has_xt2;
} msp430_fxyz_clock_params_t;

/**
 * @brief   Initialize the basic clock system to provide the main clock,
 *          the subsystem clock, and the auxiliary clock.
 *
 * The main clock, the subsystem clock, and the auxiliary clock will be set up
 * as specified by `static const msp430_fxyz_clock_params_t clock_params` in
 * the `periph_conf.h` of the board.
 *
 * @note    This function takes some time and typically is only called during
 *          boot.
 *
 * @post    The main clock, the subsystem clock and the auxiliary clock are
 *          are set up and stable
 */
void default_clock_init(void);

/**
 * @brief   Call during boot up process to initialize the clock
 *
 * @note    This is a weak alias for @ref default_clock_init so that this can
 *          be overwritten with a strong symbol from the board in case some
 *          special handling is required. The boards custom `clock_init()` can
 *          still call @ref default_clock_init if that is useful; or it could
 *          do everything from scratch.
 */
void clock_init(void);

/**
 * @brief   Get the configured submain clock frequency
 *
 * @note    This is only useful when implementing MSP430 peripheral drivers
 */
uint32_t msp430_fxyz_submain_clock_freq(void);

/**
 * @brief   Get the configured auxiliary clock frequency
 *
 * @note    This is only useful when implementing MSP430 peripheral drivers
 */
uint32_t msp430_fxyz_auxiliary_clock_freq(void);

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_CPU_H */
/** @} */
