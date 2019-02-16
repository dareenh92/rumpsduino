//------------------------------------------------------------------------------
//
// Test for Systick, interrupt and SEV
//
// Change priority level of Systick to 0 (highest)
// Change priority level of IRQ_0 to 3 (lowest)

#include "main.h"

volatile unsigned int plain_distributed = 0;
volatile unsigned int plain_recv_count = 0;
volatile unsigned int cipher_send_count = 0;
volatile unsigned int cipher_word_send_count = 0;

volatile unsigned int pp_data_ready = 0;
volatile unsigned int receive_noc	 = 0;

volatile unsigned int plain[4]	 	= {0, 0, 0, 0};
volatile unsigned int cipher	 	= 0;

void main_scheduler (void);
void blink(void);

//------------------------------------------------------------------------------
int main(void)
{
	int i = 0;
	
	unsigned int sleep_count = 0;
		NVIC_SetPriority(4, 0);	 // set Ext INT 4 (noc rx) priority 
	
	NVIC_EnableIRQ(4);				// enable Ext INT 4 (noc rx) 
	
	IC_NOC_CSR2 = 0x0000f44f; // set NOC RX interrupt to be generated after receive 4 flits
	IC_NOC_CSR1 = 0x14000000; // set NOC RX interrupt and set RX IM

	GPIO_DATAOUT = 0xffffffff;
  
  	GPIO_OEN_SET = 0xffffffff;	// set all output

	blink();
	blink();
	blink();

	//initialize uart
	init_uart();
	
	main_scheduler();
	 
	return 0;
}
	

void main_scheduler(void)
{
	int i;

	while(1)
	{
		blink();
		blink();
		blink();

		for (i = 0; i < 4; i++)
		{
			plain[0] = (unsigned int) _getchar(); // get byte from UART
			_putchar((unsigned char) plain[0] & 0xff);	// return for checking
			plain[1] = (unsigned int) _getchar(); // get byte from UART
			_putchar((unsigned char) plain[1] & 0xff);	// return for checking
			plain[2] = (unsigned int) _getchar(); // get byte from UART
			_putchar((unsigned char) plain[2] & 0xff);	// return for checking
			plain[3] = (unsigned int) _getchar(); // get byte from UART
			_putchar((unsigned char) plain[3] & 0xff);	// return for checking

			IC_NOC_TX_BUFF1 = ((plain[3] << 24) | 
											 (plain[2] << 16) | 
											 (plain[1] <<  8) | 
											  plain[0]); // send through NoC	
			blink();
		}

		for (i = 0; i < 4; i++)
		{
			plain[0] = (unsigned int) _getchar(); // get byte from UART
			_putchar((unsigned char) plain[0] & 0xff);	// return for checking
			plain[1] = (unsigned int) _getchar(); // get byte from UART
			_putchar((unsigned char) plain[1] & 0xff);	// return for checking
			plain[2] = (unsigned int) _getchar(); // get byte from UART
			_putchar((unsigned char) plain[2] & 0xff);	// return for checking
			plain[3] = (unsigned int) _getchar(); // get byte from UART
			_putchar((unsigned char) plain[3] & 0xff);	// return for checking

			IC_NOC_TX_BUFF2 = ((plain[3] << 24) | 
											 (plain[2] << 16) | 
											 (plain[1] <<  8) | 
											  plain[0]); // send through NoC	
			blink();
		}

		while (receive_noc != 1); 

		for (i = 0; i < 4; i++)
		{
			cipher = IC_NOC_RX_BUFF1;
			_putchar((unsigned char) cipher & 0xff); // send back through UART
			_putchar((unsigned char) (cipher >>  8) & 0xff);
			_putchar((unsigned char) (cipher >> 16) & 0xff);
			_putchar((unsigned char) (cipher >> 24) & 0xff);	
		}

		receive_noc = 0;
			
		NVIC_ClearPendingIRQ(4);
		
		NVIC_EnableIRQ(4);		 // enable NOC RX interrupt

		while (receive_noc != 1); 

		for (i = 0; i < 4; i++)
		{
			cipher = IC_NOC_RX_BUFF2;
			_putchar((unsigned char) cipher & 0xff); // send back through UART
			_putchar((unsigned char) (cipher >>  8) & 0xff);
			_putchar((unsigned char) (cipher >> 16) & 0xff);
			_putchar((unsigned char) (cipher >> 24) & 0xff);	
		}

		receive_noc = 0;
			
		NVIC_ClearPendingIRQ(4);
		
		NVIC_EnableIRQ(4);		 // enable NOC RX interrupt
			
	}
}


void blink(void)
{
	int i;

	GPIO_BTGL = 0x00000001;

	for (i = 0; i < 10000; i++)
		__NOP();

	GPIO_BTGL = 0x00000001;

	for (i = 0; i < 10000; i++)
		__NOP();
}
