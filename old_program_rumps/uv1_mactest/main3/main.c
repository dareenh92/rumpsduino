//------------------------------------------------------------------------------
//
// Main Program
// Application		: mactest
// Core						: DSP
// Purpose
//	- test MAC functionality on DSP Core

#include "main.h"

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
	char ch, op_type;
	int i,j;
	
	//unsigned results
	uint32_t temp_u32b;
	uint64_t temp_u64b;
	uint32_t* temp_u32b_arr;
	//signed results
	int32_t temp_s32b;
	int64_t temp_s64b;
	int32_t* temp_s32b_arr;
	//operand
	uint32_t a,b;
	int32_t a_s, b_s;
	
	//initialize uart
	uart_hd_init_uart(0x1); // init GPIO and Timer
	
	while(1){
		// receive command
		ch = uart_hd_getchar();
		uart_hd_putchar(ch);
		
		//*****
		// Operation parameters
		//*****
		if(ch=='x'){
			mac_clear_mul();
			op_type = 0;
		}
		// op_type = 1 /////////////
		else if(ch=='a'){
			a = 1; b = 9;
			op_type = 1;
		}
		else if(ch=='b'){
			a = 182; b = 75;
			op_type = 1;
		}
		else if(ch=='c'){
			a = 14210; b = 2065;
			op_type = 1;
		}
		// op_type = 2 /////////////
		else if(ch=='d'){
			a = 2; b = 9;
			op_type = 2;
		}
		else if(ch=='e'){
			a = 65600; b = 25;
			op_type = 2;
		}
		else if(ch=='f'){
			a = 789500; b = 41700;
			op_type = 2;
		}
		// op_type = 3 /////////////
		else if(ch=='g'){
			a = 52; b = 133;
			op_type = 3;
		}
		else if(ch=='h'){
			a = 10; b = 1289;
			op_type = 3;
		}
		else if(ch=='i'){
			a = 780;b = 900;
			op_type = 3;
		}
		// op_type = 4 /////////////
		else if(ch=='j'){
			a_s = 10; b_s = -213;
			op_type = 4;
		}
		else if(ch=='k'){
			a_s = -2; b_s = -5;
			op_type = 4;
		}
		else if(ch=='l'){
			a_s = -213; b_s = 10;
			op_type = 4;
		}
		// op_type = 5 /////////////
		else if(ch=='m'){
			a_s = -4; b_s = 8;
			op_type = 5;
		}
		else if(ch=='n'){
			a_s = 77718; b_s = -612;
			op_type = 5;
		}
		else if(ch=='o'){
			a_s = -9110; b_s = -6243;
			op_type = 5;
		}
		// op_type = 6 /////////////
		else if(ch=='p'){
			a_s = 4; b_s = -67;
			op_type = 6;
		}
		else if(ch=='q'){
			a_s = 770; b_s = -112;
			op_type = 6;
		}
		else if(ch=='r'){
			a_s = -91; b_s = -640;
			op_type = 6;
		}
		
		//*****
		// Execute operation
		//*****
		if(op_type==1){ //16-bit unsigned multiplication
			temp_u32b = mac_umul_16(a, b);
			for(i=3; i>=0; i--)
				uart_hd_putchar( (temp_u32b >> (i*8)) & 0xff);
		}
		
		if(op_type==2){ //32-bit unsigned multiplication
			temp_u64b = mac_umul_32(a, b);
			for(i=7; i>=0; i--)
				uart_hd_putchar( (temp_u64b >> (i*8)) & 0xff);
		}
		
		if(op_type==3){ //32-bit unsigned multiplication accumulate
			temp_u32b_arr = mac_umul_acc(a, b);
			uart_hd_putchar( temp_u32b_arr[2] & 0xff);
			for(i=7; i>=0; i--)
				uart_hd_putchar( (temp_u32b_arr[1] >> (i*8)) & 0xff);
			for(i=7; i>=0; i--)
				uart_hd_putchar( (temp_u32b_arr[0] >> (i*8)) & 0xff);
		}
		
		if(op_type==4){ //16-bit signed multiplication
			temp_s32b = mac_smul_16(a_s, b_s);
			for(i=3; i>=0; i--)
				uart_hd_putchar( (temp_s32b >> (i*8)) & 0xff);
		}
		
		if(op_type==5){ //32-bit signed multiplication
			temp_s64b = mac_smul_32(a_s, b_s);
			for(i=7; i>=0; i--)
				uart_hd_putchar( (temp_s64b >> (i*8)) & 0xff);
		}
		
		if(op_type==6){ //32-bit signed multiplication accumulate
			temp_s32b_arr = mac_smul_acc(a_s, b_s);
			uart_hd_putchar( temp_s32b_arr[2] & 0xff);
			for(i=7; i>=0; i--)
				uart_hd_putchar( (temp_s32b_arr[1] >> (i*8)) & 0xff);
			for(i=7; i>=0; i--)
				uart_hd_putchar( (temp_s32b_arr[0] >> (i*8)) & 0xff);
		}
		
		
	}
	
	return 0;
}
