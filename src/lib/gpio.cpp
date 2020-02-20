/*

Copyright (C) 2017-2020 Ivan Kniazkov

This file is part of interpreter of programming language
codenamed "Goat" ("Goat interpreter").

Goat interpreter is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Goat interpreter is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with Goat interpreter.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "settings.h"
#include "gpio.h"

#ifdef GPIO_DEBUG
#include "utils.h"
#include <iostream>
#endif

#ifdef GPIO_RASPBERRY_PI
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

namespace g0at
{
    namespace lib
    {
#ifdef GPIO_ENABLE

#ifdef GPIO_EMULATION
#define GPIO_IMPLEMENTED
        static const unsigned int gpio_port_numbers[] = {0, 1, 2, 3, 4, 5, 6, 7};
        static const unsigned int gpio_count = sizeof(gpio_port_numbers) / sizeof(unsigned int);
#ifdef GPIO_DEBUG
        static int64_t gpio_time_start;
#endif

        bool gpio_values[gpio_count];

        bool gpio_init()
        {
            for (unsigned int i = 0; i < gpio_count; i++)
                gpio_values[i] = false;
#ifdef GPIO_DEBUG
            gpio_time_start = get_time_ns();
#endif
            return true;
        }

        gpio_info gpio_get_info()
        {
            return gpio_info(gpio_count, gpio_port_numbers);
        }

        void gpio_set_value(unsigned int port, bool value)
        {
            if (port < gpio_count)
            {
#ifdef GPIO_DEBUG
                if (gpio_values[port] != value)
                {
                    int64_t gpio_time = get_time_ns() - gpio_time_start;
                    std::cout << "\n*** gpio " << port << '\t' << (value ? '1' : '0') << '\t' << gpio_time << std::flush;
                }
#endif
                gpio_values[port] = value;
            }
        }

        bool gpio_get_value(unsigned int port)
        {
            return port < gpio_count ? gpio_values[port] : false;
        }
#endif // GPIO_EMULATION

#ifdef GPIO_RASPBERRY_PI
#define GPIO_IMPLEMENTED
        static const unsigned int gpio_port_numbers[] = {27, 4, 18, 23, 25, 12, 6, 13, 5};
        static const unsigned int gpio_count = sizeof(gpio_port_numbers) / sizeof(unsigned int);

        gpio_info gpio_get_info()
        {
            return gpio_info(gpio_count, gpio_port_numbers);
        }

static volatile uint32_t pi_model = 1;

static volatile uint32_t pi_periph_base = 0x20000000;
static volatile uint32_t pi_bus_address = 0x40000000;

#define SYST_BASE  (pi_periph_base + 0x003000)
#define DMA_BASE   (pi_periph_base + 0x007000)
#define CLK_BASE   (pi_periph_base + 0x101000)
#define GPIO_BASE  (pi_periph_base + 0x200000)
#define UART0_BASE (pi_periph_base + 0x201000)
#define PCM_BASE   (pi_periph_base + 0x203000)
#define SPI0_BASE  (pi_periph_base + 0x204000)
#define I2C0_BASE  (pi_periph_base + 0x205000)
#define PWM_BASE   (pi_periph_base + 0x20C000)
#define BSCS_BASE  (pi_periph_base + 0x214000)
#define UART1_BASE (pi_periph_base + 0x215000)
#define I2C1_BASE  (pi_periph_base + 0x804000)
#define I2C2_BASE  (pi_periph_base + 0x805000)
#define DMA15_BASE (pi_periph_base + 0xE05000)

#define DMA_LEN   0x1000 /* allow access to all channels */
#define CLK_LEN   0xA8
#define GPIO_LEN  0xB4
//#define GPIO_LEN  0xFF
#define SYST_LEN  0x1C
#define PCM_LEN   0x24
#define PWM_LEN   0x28
#define I2C_LEN   0x1C
#define BSCS_LEN  0x40

#define GPSET0 7
#define GPSET1 8

#define GPCLR0 10
#define GPCLR1 11

#define GPLEV0 13
#define GPLEV1 14

#define GPPUD     37
#define GPPUDCLK0 38
#define GPPUDCLK1 39

#define SYST_CS  0
#define SYST_CLO 1
#define SYST_CHI 2

static volatile uint32_t  *gpio_register = (uint32_t  *)MAP_FAILED;
static volatile uint32_t  *system_register = (uint32_t  *)MAP_FAILED;
static volatile uint32_t  *bcst_register = (uint32_t  *)MAP_FAILED;

#define PI_BANK (gpio>>5)
#define PI_BIT  (1<<(gpio&0x1F))

/* gpio modes. */

#define PI_INPUT  0
#define PI_OUTPUT 1
#define PI_ALT0   4
#define PI_ALT1   5
#define PI_ALT2   6
#define PI_ALT3   7
#define PI_ALT4   3
#define PI_ALT5   2

void gpio_set_mode(unsigned gpio, unsigned mode)
{
   int reg, shift;

   reg   =  gpio/10;
   shift = (gpio%10) * 3;

   gpio_register[reg] = (gpio_register[reg] & ~(7<<shift)) | (mode<<shift);
}

int gpio_get_mode(unsigned gpio)
{
   int reg, shift;

   reg   =  gpio/10;
   shift = (gpio%10) * 3;

   return (*(gpio_register + reg) >> shift) & 7;
}

/* Values for pull-ups/downs off, pull-down and pull-up. */

#define PI_PUD_OFF  0
#define PI_PUD_DOWN 1
#define PI_PUD_UP   2

void gpio_set_pull_up_down(unsigned gpio, unsigned pud)
{
   *(gpio_register + GPPUD) = pud;

   usleep(20);

   *(gpio_register + GPPUDCLK0 + PI_BANK) = PI_BIT;

   usleep(20);
  
   *(gpio_register + GPPUD) = 0;

   *(gpio_register + GPPUDCLK0 + PI_BANK) = 0;
}

bool gpio_get_value(unsigned gpio)
{
   if ((*(gpio_register + GPLEV0 + PI_BANK) & PI_BIT) != 0) return true;
   else                                         return false;
}

void gpio_set_value(unsigned gpio, bool level)
{
   if (level == false) *(gpio_register + GPCLR0 + PI_BANK) = PI_BIT;
   else            *(gpio_register + GPSET0 + PI_BANK) = PI_BIT;
}

/*
void gpioTrigger(unsigned gpio, unsigned pulseLen, unsigned level)
{
   if (level == 0) *(gpio_register + GPCLR0 + PI_BANK) = PI_BIT;
   else            *(gpio_register + GPSET0 + PI_BANK) = PI_BIT;

   usleep(pulseLen);

   if (level != 0) *(gpio_register + GPCLR0 + PI_BANK) = PI_BIT;
   else            *(gpio_register + GPSET0 + PI_BANK) = PI_BIT;
}
*/

/* Bit (1<<x) will be set if gpio x is high. */

uint32_t gpio_read_bank_1(void) { return (*(gpio_register + GPLEV0)); }
uint32_t gpio_read_bank_2(void) { return (*(gpio_register + GPLEV1)); }

/* To clear gpio x bit or in (1<<x). */

void gpio_clear_bank_1(uint32_t bits) { *(gpio_register + GPCLR0) = bits; }
void gpio_clear_bank_2(uint32_t bits) { *(gpio_register + GPCLR1) = bits; }

/* To set gpio x bit or in (1<<x). */

void gpio_set_bank_1(uint32_t bits) { *(gpio_register + GPSET0) = bits; }
void gpio_set_bank_2(uint32_t bits) { *(gpio_register + GPSET1) = bits; }

unsigned gpio_hardware_revision(void)
{
   static unsigned rev = 0;

   FILE * filp;
   char buf[512];
   char term;
   int chars=4; /* number of chars in revision string */

   if (rev) return rev;

   pi_model = 0;

   filp = fopen ("/proc/cpuinfo", "r");

   if (filp != NULL)
   {
      while (fgets(buf, sizeof(buf), filp) != NULL)
      {
         if (pi_model == 0)
         {
            if (!strncasecmp("model name", buf, 10))
            {
               if (strstr (buf, "ARMv6") != NULL)
               {
                  pi_model = 1;
                  chars = 4;
                  pi_periph_base = 0x20000000;
                  pi_bus_address = 0x40000000;
               }
               else if (strstr (buf, "ARMv7") != NULL)
               {
                  pi_model = 2;
                  chars = 6;
                  pi_periph_base = 0x3F000000;
                  pi_bus_address = 0xC0000000;
               }
               else if (strstr (buf, "ARMv8") != NULL)
               {
                  pi_model = 2;
                  chars = 6;
                  pi_periph_base = 0x3F000000;
                  pi_bus_address = 0xC0000000;
               }
            }
         }

         if (!strncasecmp("revision", buf, 8))
         {
            if (sscanf(buf+strlen(buf)-(chars+1),
               "%x%c", &rev, &term) == 2)
            {
               if (term != '\n') rev = 0;
            }
         }
      }

      fclose(filp);
   }
   return rev;
}

/* Map in registers. */

static uint32_t * gpio_init_memory_map(int fd, uint32_t addr, uint32_t len)
{
    return (uint32_t *) mmap(0, len,
       PROT_READ|PROT_WRITE|PROT_EXEC,
       MAP_SHARED|MAP_LOCKED,
       fd, addr);
}

bool gpio_init(void)
{
   int fd;

   gpio_hardware_revision(); /* sets pi_model, needed for peripherals address */

   fd = open("/dev/mem", O_RDWR | O_SYNC) ;

   if (fd < 0)
   {
      fprintf(stderr,
         "This program needs root privileges.  Try using sudo\n");
      return false;
   }

   gpio_register  = gpio_init_memory_map(fd, GPIO_BASE,  GPIO_LEN);
   system_register  = gpio_init_memory_map(fd, SYST_BASE,  SYST_LEN);
   bcst_register  = gpio_init_memory_map(fd, BSCS_BASE,  BSCS_LEN);

   close(fd);

   if ((gpio_register == MAP_FAILED) ||
       (system_register == MAP_FAILED) ||
       (bcst_register == MAP_FAILED))
   {
      fprintf(stderr,
         "Bad, mmap failed\n");
      return false;
   }
   return true;
}        

#endif // GPIO_RASPBERRY_PI

#endif // GPIO_ENABLE

#ifndef GPIO_IMPLEMENTED
        bool gpio_init()
        {
            return true;
        }

        gpio_info gpio_get_info()
        {
            return gpio_info(0, nullptr);
        }

        void gpio_set_value(unsigned int port, bool value)
        {
        }

        bool gpio_get_value(unsigned int port)
        {
            return false;
        }
#endif
    };
};
