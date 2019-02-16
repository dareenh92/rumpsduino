#ifndef __NOC_H_
#define __NOC_H_

#include <stdio.h>


#define IC_NOC_CSR0           ((*(volatile unsigned long *) (0x50040000)))
#define IC_NOC_CSR1           ((*(volatile unsigned long *) (0x50040004)))
#define IC_NOC_CSR2           ((*(volatile unsigned long *) (0x50040008)))
#define IC_NOC_RX_BUFF0       ((*(volatile unsigned long *) (0x5004000C)))
#define IC_NOC_RX_BUFF1       ((*(volatile unsigned long *) (0x50040010)))
#define IC_NOC_RX_BUFF2       ((*(volatile unsigned long *) (0x50040014)))
#define IC_NOC_RX_BUFF3       ((*(volatile unsigned long *) (0x50040018)))
#define IC_NOC_TX_BUFF0       ((*(volatile unsigned long *) (0x5004001C)))
#define IC_NOC_TX_BUFF1       ((*(volatile unsigned long *) (0x50040020)))
#define IC_NOC_TX_BUFF2       ((*(volatile unsigned long *) (0x50040024)))
#define IC_NOC_TX_BUFF3       ((*(volatile unsigned long *) (0x50040028)))
#define IC_NOC_RX_BD_BUFF0    ((*(volatile unsigned long *) (0x5004002C)))
#define IC_NOC_RX_BD_BUFF1    ((*(volatile unsigned long *) (0x50040030)))
#define IC_NOC_RX_BD_BUFF2    ((*(volatile unsigned long *) (0x50040034)))
#define IC_NOC_RX_BD_BUFF3    ((*(volatile unsigned long *) (0x50040038)))
#define IC_NOC_TX_BD_BUFF0    ((*(volatile unsigned long *) (0x5004003C)))
#define IC_NOC_TX_BD_BUFF1    ((*(volatile unsigned long *) (0x50040040)))
#define IC_NOC_TX_BD_BUFF2    ((*(volatile unsigned long *) (0x50040044)))
#define IC_NOC_TX_BD_BUFF3    ((*(volatile unsigned long *) (0x50040048)))
#define IC_NOC_BD_CSR         ((*(volatile unsigned long *) (0x5004004C)))

#define NC_NOC_CSR0           ((*(volatile unsigned long *) (0x50040000)))
#define NC_NOC_CSR1           ((*(volatile unsigned long *) (0x50040004)))
#define NC_NOC_CSR2           ((*(volatile unsigned long *) (0x50040008)))
#define NC_NOC_RX_BUFF0       ((*(volatile unsigned long *) (0x5004000C)))
#define NC_NOC_RX_BUFF1       ((*(volatile unsigned long *) (0x50040010)))
#define NC_NOC_RX_BUFF2       ((*(volatile unsigned long *) (0x50040014)))
#define NC_NOC_RX_BUFF3       ((*(volatile unsigned long *) (0x50040018)))
#define NC_NOC_TX_BUFF0       ((*(volatile unsigned long *) (0x5004001C)))
#define NC_NOC_TX_BUFF1       ((*(volatile unsigned long *) (0x50040020)))
#define NC_NOC_TX_BUFF2       ((*(volatile unsigned long *) (0x50040024)))
#define NC_NOC_TX_BUFF3       ((*(volatile unsigned long *) (0x50040028)))

//###
// macro functions
//###

// Check if there is flit in rx buffer, 1->yes, otherwise->no
#define noc_IC_rxbuff0_av ((IC_NOC_CSR0 & 0x1)==0x1)
#define noc_IC_rxbuff1_av ((IC_NOC_CSR0 & 0x4)==0x4)
#define noc_IC_rxbuff2_av ((IC_NOC_CSR0 & 0x10)==0x10)
#define noc_IC_rxbuff3_av ((IC_NOC_CSR0 & 0x40)==0x40)
#define noc_NC_rxbuff0_av ((NC_NOC_CSR0 & 0x1)==0x1)
#define noc_NC_rxbuff1_av ((NC_NOC_CSR0 & 0x4)==0x4)
#define noc_NC_rxbuff2_av ((NC_NOC_CSR0 & 0x10)==0x10)
#define noc_NC_rxbuff3_av ((NC_NOC_CSR0 & 0x40)==0x40)

// Get number of flits in rx buffer
#define noc_getIC_rxbuff0_count ((IC_NOC_CSR2 & 0xf0000)>>16)
#define noc_getIC_rxbuff1_count ((IC_NOC_CSR2 & 0xf00000)>>20)
#define noc_getIC_rxbuff2_count ((IC_NOC_CSR2 & 0xf000000)>>24)
#define noc_getIC_rxbuff3_count ((IC_NOC_CSR2 & 0xf0000000)>>28)
#define noc_getNC_rxbuff0_count ((NC_NOC_CSR2 & 0xf0000)>>16)
#define noc_getNC_rxbuff1_count ((NC_NOC_CSR2 & 0xf00000)>>20)
#define noc_getNC_rxbuff2_count ((NC_NOC_CSR2 & 0xf000000)>>24)
#define noc_getNC_rxbuff3_count ((NC_NOC_CSR2 & 0xf0000000)>>28)

// transmit buffer status, 1->full, otherwise->not full
#define noc_IC_txbuff_isfull ((IC_NOC_CSR1 & 0x2)==0x2)
#define noc_NC_txbuff_isfull ((NC_NOC_CSR1 & 0x2)==0x2)

#endif


