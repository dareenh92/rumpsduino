//------------------------------------------------------------------------------
//
// Test for Systick, interrupt and SEV
//
// Change priority level of Systick to 0 (highest)
// Change priority level of IRQ_0 to 3 (lowest)

#include "main.h"

volatile unsigned int plain_word_recv_count = 0;
volatile unsigned int cipher_send_count = 0;

volatile unsigned int receive_noc	 = 0;

volatile unsigned int key[4] = { 	0x00000000,
													0x00000000,
													0x00000000,
													0x00000000};

volatile unsigned int iv_cipher[4] = { 	0xffffffff,
															0xffffffff,
															0xffffffff,
															0xffffffff};

void main_scheduler (void);
void blink(void);

//------------------------------------------------------------------------------
int main(void)
{
	int i = 0;
	
	unsigned int sleep_count = 0;
	
	NVIC_SetPriority(2, 0);	 // set Ext INT 2 (noc rx) priority 
	//NVIC_SetPriority(3, 0);	 // set Ext INT 2 (aes) priority 
	
	NVIC_EnableIRQ(2);				// enable Ext INT 2 (noc rx) 
	//NVIC_EnableIRQ(3);				// enable Ext INT 2 (aes) 
	
	IC_NOC_CSR2 = 0x0000fff4; // set NOC RX interrupt to be generated after receive 4 flits
	NC_NOC_CSR1 = 0x14000000; // set NOC RX interrupt and set RX IM
	
	//AES_CSR = 0x1;	// enable the AES interrupt

	GPIO_DATAOUT = 0xff;
  
  	GPIO_OEN_SET = 0xff;	// set all output
	
	main_scheduler();
	 
	return 0;
}
	

void main_scheduler(void)
{
	int result;

	AES_KEY0 =  key[0];
	AES_KEY1 =  key[1];
	AES_KEY2 =  key[2];
	AES_KEY3 =  key[3];

	while(1)
	{
		if (receive_noc == 1) {
			blink();

			AES_PLAIN0 = NC_NOC_RX_BUFF0 ^ iv_cipher[0];
			AES_PLAIN1 = NC_NOC_RX_BUFF0 ^ iv_cipher[1];
			AES_PLAIN2 = NC_NOC_RX_BUFF0 ^ iv_cipher[2];
			AES_PLAIN3 = NC_NOC_RX_BUFF0 ^ iv_cipher[3];
			
			receive_noc = 0;
			
			NVIC_ClearPendingIRQ(2);
			
			NVIC_EnableIRQ(2);		 // enable NOC RX interrupt
		}
			
		if ((AES_CSR & 0x100) == 0x100){
			iv_cipher[0] = AES_CIPHER0;
			iv_cipher[1] = AES_CIPHER1;
			iv_cipher[2] = AES_CIPHER2;
			iv_cipher[3] = AES_CIPHER3;

			NC_NOC_TX_BUFF0 =	iv_cipher[0];
			NC_NOC_TX_BUFF0 =	iv_cipher[1];
			NC_NOC_TX_BUFF0 =	iv_cipher[2];
			NC_NOC_TX_BUFF0 =	iv_cipher[3];

			cipher_send_count++;
		}
	}
}


void blink(void)
{
	int i;

	GPIO_BTGL = 0x01;

	for (i = 0; i < 100000; i++)
		__NOP();

	GPIO_BTGL = 0x01;

	for (i = 0; i < 100000; i++)
		__NOP();
}
