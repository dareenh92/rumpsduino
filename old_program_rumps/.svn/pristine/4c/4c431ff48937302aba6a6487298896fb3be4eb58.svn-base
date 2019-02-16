#ifndef __MAC_H_
#define __MAC_H_

#include <stdint.h>

#define MAC_CSR			((*(volatile unsigned long *) (0x50030000)))
#define MAC_A       ((*(volatile unsigned long *) (0x50030004)))
#define MAC_B       ((*(volatile unsigned long *) (0x50030008)))
#define MAC_MUL0    ((*(volatile unsigned long *) (0x5003000C)))
#define MAC_MUL1    ((*(volatile unsigned long *) (0x50030010)))
#define MAC_MUL2    ((*(volatile unsigned long *) (0x50030014)))

////////////////////////
// MAC Function Wrapper
////////////////////////
void mac_clear_mul();
uint32_t mac_get_mul0();
uint32_t* mac_umul_acc(uint32_t a, uint32_t b);
int32_t* mac_smul_acc(int32_t a, int32_t b);

// 16bits unsigned multiplication
inline uint32_t mac_umul_16(uint16_t a, uint16_t b){
	MAC_CSR = 0x01; //define operation type
	MAC_A = ((uint32_t)a << 16) | b;
	return MAC_MUL0;
}
// 32bits unsigned multiplication
inline uint64_t mac_umul_32(uint32_t a, uint32_t b){
	MAC_CSR = 0x00; //define operation type
	MAC_B = b;
	MAC_A = a; 
	return ((uint64_t)MAC_MUL1 << 32) | MAC_MUL0;
}
// 16bits signed multiplication
inline int32_t mac_smul_16(int16_t a, int16_t b){
	MAC_CSR = 0x05; //define operation type
	MAC_A = ((int32_t)a << 16) | (b & 0x0000ffff);
	return MAC_MUL0;
}
// 32bits signed multiplication
inline int64_t mac_smul_32(int32_t a, int32_t b){
	MAC_CSR = 0x04; //define operation type
	MAC_B = b;
	MAC_A = a; 
	return ((int64_t)MAC_MUL1 << 32) | MAC_MUL0;
}
#endif
