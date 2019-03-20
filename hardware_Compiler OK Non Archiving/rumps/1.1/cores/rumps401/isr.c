//------------------------------------------------------------------------------
//

// cannot and should not use printf(), data sharing problem
// message() should be reentrant (by default) 

unsigned int systick_cnt = 0;
unsigned int ext_0_cnt = 0;
unsigned int ext_1_cnt = 0;
unsigned int ext_2_cnt = 0;
unsigned int ext_3_cnt = 0;
unsigned int ext_4_cnt = 0;
unsigned int ext_5_cnt = 0;
unsigned int ext_6_cnt = 0;
unsigned int ext_7_cnt = 0;
unsigned int ext_8_cnt = 0;
unsigned int ext_9_cnt = 0;
unsigned int ext_10_cnt = 0;
unsigned int ext_11_cnt = 0;
unsigned int ext_12_cnt = 0;
unsigned int ext_13_cnt = 0;
unsigned int ext_14_cnt = 0;
unsigned int ext_15_cnt = 0;

//-----------------------------------------------------------------------------------
/*void NMI_Handler(void)
{
	__SEV();
	while(1);
}
*/

//-----------------------------------------------------------------------------------
void HARD_FAULT_Handler(void)
{
	__SEV();
	while(1);
}

//-----------------------------------------------------------------------------------
void SYSTICK_Handler(void)
{
	__SEV();
  	systick_cnt++;
  	while(1);
}

//-----------------------------------------------------------------------------------
void EXTERNAL_0_Handler(void)
{
	__SEV();
	ext_0_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_1_Handler(void)
{
	__SEV();
	ext_1_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_2_Handler(void)
{
	__SEV();
	ext_2_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_3_Handler(void)
{
	__SEV();
	ext_3_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_4_Handler(void)
{
	__SEV();
	ext_4_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_5_Handler(void)
{
	__SEV();
	ext_5_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_6_Handler(void)
{
	__SEV();
	ext_6_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_7_Handler(void)
{
	__SEV();
	ext_7_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_8_Handler(void)
{
	__SEV();
	ext_8_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_9_Handler(void)
{
	__SEV();
	ext_9_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_10_Handler(void)
{
	__SEV();
	ext_10_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_11_Handler(void)
{
	__SEV();
	ext_11_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_12_Handler(void)
{
	__SEV();
	ext_12_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_13_Handler(void)
{
	__SEV();
	ext_13_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_14_Handler(void)
{
	__SEV();
	ext_14_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_15_Handler(void)
{
	__SEV();
	ext_15_cnt++;
}
