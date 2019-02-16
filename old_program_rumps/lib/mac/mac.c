//------------------------------------------------------------------------------
//
#include "main.h"

// clear MUL register
void mac_clear_mul(){
	
	MAC_CSR = 0x08; // set MAC_CLR
}

uint32_t mac_get_mul0(){
	return MAC_MUL0;
}


// 32bits unsigned multiply accumulate
uint32_t* mac_umul_acc(uint32_t a, uint32_t b){
	static uint32_t mul_reg[3];
	
	MAC_CSR = 0x02; //define operation type
	
	MAC_B = b;
	MAC_A = a;
	
	mul_reg[0] = MAC_MUL0;
	mul_reg[1] = MAC_MUL1;
	mul_reg[2] = MAC_MUL2;
	
	return mul_reg;
}

// 32bits unsigned multiply accumulate
int32_t* mac_smul_acc(int32_t a, int32_t b){
	static int32_t mul_reg[3];
	
	MAC_CSR = 0x06; //define operation type
	
	MAC_B = b;
	MAC_A = a;
	
	mul_reg[0] = MAC_MUL0;
	mul_reg[1] = MAC_MUL1;
	mul_reg[2] = MAC_MUL2;
	
	return mul_reg;
}
