#include "spi.h"
#include "gpio.h"
#define SPI_CS_BIT      13
#define SPI_MOSI_BIT    12
#define SPI_MISO_BIT    11 //in
#define SPI_SCK_BIT     10


static void init(uint32_t sck,uint32_t miso,uint32_t mosi) {
gpio_init(sck, GPIO_FUNC_SIO);
gpio_dir(sck, 1);

gpio_init(mosi, GPIO_FUNC_SIO);
gpio_dir(mosi, 1);

gpio_init(miso, GPIO_FUNC_SIO);
gpio_dir(miso, 0);

}

unsigned char transfer(unsigned char data) {
   unsigned char rd = 0;
   gpio_set(SPI_SCK_BIT,0);// nok_sclk=0;
   gpio_set(SPI_MOSI_BIT,0);//nok_sda=0;

   for (int i = 0; i < 8; i++){
        rd =  rd <<1;
        if ((data << i) & 0x80){
             gpio_set(SPI_MOSI_BIT,1);//nok_sda=1;
        }else{
             gpio_set(SPI_MOSI_BIT,0);//nok_sda=0;
        }

        if(gpio_get(SPI_MISO_BIT))  rd |= 1;
        //asm("nop");asm("nop");
        gpio_set(SPI_SCK_BIT,1);//nok_sclk=1;
        //asm("nop");asm("nop");
        gpio_set(SPI_SCK_BIT,0);// nok_sclk=0;
   }
   
   return rd;

}
//-----------------------------------------------------------------
// spi_init: Initialise SPI master
//-----------------------------------------------------------------
void spi_init(void)           
{         
     init(SPI_SCK_BIT,SPI_MISO_BIT,SPI_MOSI_BIT);
     gpio_init(SPI_CS_BIT, GPIO_FUNC_SIO);
     gpio_dir(SPI_CS_BIT, 1);
}
//-----------------------------------------------------------------
// spi_cs: Set chip select
//-----------------------------------------------------------------
void spi_cs(uint32_t value)
{
     gpio_set(SPI_CS_BIT,value);// nok_sclk=0;
}
