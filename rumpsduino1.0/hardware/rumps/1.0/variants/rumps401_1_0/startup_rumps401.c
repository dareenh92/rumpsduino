/* start_up.s */

/* Addresses pulled in from the linker script */
extern unsigned int _end_stack;
extern unsigned int _end_text;
extern unsigned int _start_data;
extern unsigned int _end_data;
extern unsigned int _start_bss;
extern unsigned int _end_bss;

/* Application main() called in reset handler */
extern int main(void);

#define WEAK_ALIAS(x) __attribute__ ((weak, alias(#x)))

/* Cortex M0 core interrupt handlers */
void Reset_Handler(void);
void NMI_Handler(void) WEAK_ALIAS(Dummy_Handler);
void HARD_FAULT_Handler(void) WEAK_ALIAS(Dummy_Handler);
void SVC_Handler(void) WEAK_ALIAS(Dummy_Handler);
void PendSV_Handler(void) WEAK_ALIAS(Dummy_Handler);
void SYSTICK_Handler(void) WEAK_ALIAS(Dummy_Handler);

/* Device specific interrupt handlers */
void EXTERNAL_0_Handler(void) WEAK_ALIAS(Dummy_Handler);
void EXTERNAL_1_Handler(void) WEAK_ALIAS(Dummy_Handler);
void EXTERNAL_2_Handler(void) WEAK_ALIAS(Dummy_Handler);
void EXTERNAL_3_Handler(void) WEAK_ALIAS(Dummy_Handler);

void EXTERNAL_4_Handler(void) WEAK_ALIAS(Dummy_Handler);
void EXTERNAL_5_Handler(void) WEAK_ALIAS(Dummy_Handler);
void EXTERNAL_6_Handler(void) WEAK_ALIAS(Dummy_Handler);
void EXTERNAL_7_Handler(void) WEAK_ALIAS(Dummy_Handler);

void EXTERNAL_8_Handler(void) WEAK_ALIAS(Dummy_Handler);
void EXTERNAL_9_Handler(void) WEAK_ALIAS(Dummy_Handler);
void EXTERNAL_10_Handler(void) WEAK_ALIAS(Dummy_Handler);
void EXTERNAL_11_Handler(void) WEAK_ALIAS(Dummy_Handler);

void EXTERNAL_12_Handler(void) WEAK_ALIAS(Dummy_Handler);
void EXTERNAL_13_Handler(void) WEAK_ALIAS(Dummy_Handler);
void EXTERNAL_14_Handler(void) WEAK_ALIAS(Dummy_Handler);
void EXTERNAL_15_Handler(void) WEAK_ALIAS(Dummy_Handler);

void Dummy_Handler(void);

/* Stack top and vector handler table */
void *vector_table[] __attribute__ ((section(".vectors"))) = {
    &_end_stack,
    Reset_Handler,
    NMI_Handler,
    HARD_FAULT_Handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    SVC_Handler,
    0,
    0,
    PendSV_Handler,
    SYSTICK_Handler,

    /* Device specific interrupt vectors */
    EXTERNAL_0_Handler,
    EXTERNAL_1_Handler,
    EXTERNAL_2_Handler,
    EXTERNAL_3_Handler,
	
    EXTERNAL_4_Handler,
    EXTERNAL_5_Handler,
    EXTERNAL_6_Handler,
    EXTERNAL_7_Handler,
	
    EXTERNAL_8_Handler,
    EXTERNAL_9_Handler,
    EXTERNAL_10_Handler,
    EXTERNAL_11_Handler,
	
    EXTERNAL_12_Handler,
    EXTERNAL_13_Handler,
    EXTERNAL_14_Handler,
    EXTERNAL_15_Handler,
	
	0,
	0,
	0,
	0,
	
	0,
	0,
	0,
	0,
	
	0,
	0,
	0,
	0,
	
	0,
	0,
	0,
	0,
};

void Reset_Handler(void) {
    unsigned int i;
	
	unsigned char *src, *dst;
  
    //----

    /* Copy with byte pointers to obviate unaligned access problems */

    /* Copy data section from Flash to RAM */
    src = (unsigned char *)&_end_text;
    dst = (unsigned char *)&_start_data;
    while (dst < (unsigned char *)&_end_data)
        *dst++ = *src++;

    /* Clear the bss section */
    dst = (unsigned char *)&_start_bss;
    while (dst < (unsigned char *)&_end_bss)
        *dst++ = 0;

    main();
}

void Dummy_Handler(void) {
  unsigned int i;
  
  while (1);
}