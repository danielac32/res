/* Xinu for STM32
 *
 * Original license applies
 * Modifications for STM32 by Robin Krens
 * Please see LICENSE and AUTHORS 
 * 
 * $LOG$
 * 2019/11/11 - ROBIN KRENS
 * Initial version 
 * 
 * $DESCRIPTION$
 *
 * */

/* Control register */
#define UART_EN		13 
#define UART_INTR_RX	5
#define UART_INTRCIE	6
#define UART_INTRX	7
#define UARTX_EN 	3
#define UART_RX_EN 	2

#define PARITY		12

/* Status register */
#define UART_RXNE	0x20
#define UARTC		0x40
#define UARTCXE	0xC0

#define UARTEST	6

#define UART_FIFO_SIZE	1
#define UARTBUF 256
#define ECHO 1

void uart_init(uint32 id, uint32 baudrate);
uint32 uart_read(uint32 id, char *buff, uint32 len);
uint32 uart_write(uint32 id, char *buff, uint32 len);
void uart_intr_enable(uint32 id, uint32 rx, uint32 tx);
void uart_flush(uint32 id);
void uart_deinit(uint32 id);
#define UART_RX0_BUFFER_SIZE 64

#define UART_RX0_BUFFER_MASK (UART_RX0_BUFFER_SIZE - 1)
#define UART_FRAME_ERROR      0x0800             
#define UART_OVERRUN_ERROR    0x0400            
#define UART_BUFFER_OVERFLOW  0x0200           
#define UART_NO_DATA          0x0100   

uint8 UART_RxBuf[UART_RX0_BUFFER_SIZE];

uint8 UART_TxHead;
uint8 UART_TxTail;
uint8 UART_RxHead;
uint8 UART_RxTail;
uint8 UART_LastRxError;
uint16 uart0_available(void);
uint16 uart0_getc(void);
void irq_uart0();
uint8 readBytes(char *buffer,uint8 length);

struct uart_csreg {
	volatile uint32 sr;
	volatile uint32 dr;
	volatile uint32 brr;
	volatile uint32 cr1;
	volatile uint32 cr2;
	volatile uint32 cr3;
	volatile uint32 gtr;
};
