#include "stm32f1xx.h"
#include "adc_handle.hpp"


void adc_init()
{
	// ADC Prescaler = 6
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

	// Enable ADC clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	__IO uint32_t tmpreg = RCC->APB2ENR & (~RCC_APB2ENR_ADC1EN);

	// Enable ADC pin clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	tmpreg = RCC->APB2ENR & (~RCC_APB2ENR_IOPAEN);
	static_cast<void>(tmpreg);

	// Enable GPIO A pin 0 in analog mode
	GPIOA->CRL &= (~(GPIO_CRL_CNF0) & ~(GPIO_CRL_MODE0)); // CNFy[1:0]=00 Analog mode; input mode MODE[1:0]=00

	// ALIGN bit in the ADC_CR2
	ADC1->CR2 &= ~ADC_CR2_ALIGN; // right alignment

	// SMP[2:0] bits in the ADC_SMPR1 and ADC_SMPR2
	// Channel 0 Sample time selection
	ADC1->SMPR2 |= ADC_SMPR2_SMP0; // ADC sample time 0b111 = 239.5 cycles

	// EXTSEL[2:0] External event select for regular group: 0b111=SWSTART
	ADC1->CR2 |= ADC_CR2_EXTSEL;

	/* The regular channels and their order in the conversion sequence must be selected 
	in the ADC_SQRx registers. The total number of conversions in the regular group must 
	be written in the L[3:0] bits in the ADC_SQR1 register. */
	ADC1->SQR3 &= ~ADC_SQR3_SQ1; // regular channel 0 in SQ1[4:0] first conversion in regular sequence
	ADC1->SQR1 &= ~ADC_SQR1_L; // Regular channel sequence length L[3:0]=0000 -> 1 conversion

	/* Calibration is started by setting the CAL bit in the ADC_CR2 register. Once calibration is
	over, the CAL bit is reset by hardware and normal conversion can be performed.
	Before starting a calibration, the ADC must have been in power-on state (ADON bit = ‘1’) for
	at least two ADC clock cycles. */
	ADC1->CR2 |= ADC_CR2_ADON;
	for (long i = 0; i < 512L * 16 * 8 * 3; i++) { // wait max possible cycles needed
		asm volatile ("nop");
	}
	ADC1->CR2 |= ADC_CR2_CAL;
	while (ADC1->CR2 & ADC_CR2_CAL) { } // wait for CAL:0 Calibration completed
}

volatile float g_adc_voltage = 0u;

// Single conversion mode
void adc_convert()
{
	// Conversion starts when ADON bit is set for a second time by software after ADC power-up time (t_STAB)
	ADC1->CR2 |= ADC_CR2_ADON;

	// wait for EOC:1 Conversion complete
	while ((ADC1->SR & ADC_SR_EOC) == 0u) { }

	g_adc_voltage = static_cast<float>(ADC1->DR) * 3.3f / 0xFFF;
}

volatile uint32_t adc_dma_buffer[4][3] = { { 0u, }, };
__IO uint32_t dma1_status;

void adc_start_dma()
{
	// Enable DMA clock
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	__IO uint32_t tmpreg = RCC->AHBENR & (~RCC_AHBENR_DMA1EN);

	// 1. Set the peripheral register address in the DMA_CPARx register. The data will be
	// moved from/ to this address to/ from the memory after the peripheral event.
	DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);

	// 2. Set the memory address in the DMA_CMARx register. The data will be written to or
	// read from this memory after the peripheral event.
	DMA1_Channel1->CMAR = (uint32_t)&adc_dma_buffer[0][0];

	// 3. Configure the total number of data to be transferred in the DMA_CNDTRx register.
	// After each peripheral event, this value will be decremented.
	DMA1_Channel1->CNDTR = sizeof(adc_dma_buffer) / sizeof(adc_dma_buffer[0][0]);

	// 4. Configure the channel priority using the PL[1:0] bits in the DMA_CCRx register
	DMA1_Channel1->CCR &= ~DMA_CCR_PL; // 00: Low

	// 5. Configure data transfer direction, circular mode, peripheral & memory incremented
	// mode, peripheral & memory data size, and interrupt after half and/or full transfer in the
	// DMA_CCRx register
	DMA1_Channel1->CCR &= ~DMA_CCR_MEM2MEM; // 0: Memory to memory mode disabled
	DMA1_Channel1->CCR |= DMA_CCR_MSIZE_1; // MSIZE[1:0] Memory size 0b10 32-bits
	DMA1_Channel1->CCR |= DMA_CCR_PSIZE_1; // PSIZE[1:0] Peripheral size 0b10 32-bits
	DMA1_Channel1->CCR |= DMA_CCR_MINC; // 1: Memory increment mode enabled
	DMA1_Channel1->CCR &= ~DMA_CCR_PINC; // 0: Peripheral increment mode disabled
	DMA1_Channel1->CCR |= DMA_CCR_CIRC; // 1: Circular mode enabled
	DMA1_Channel1->CCR &= ~DMA_CCR_DIR; // 0: Read from peripheral
	DMA1_Channel1->CCR &= ~DMA_CCR_TEIE; // 0: Transfer error interrupt disabled
	DMA1_Channel1->CCR &= ~DMA_CCR_HTIE; // 0: Half transfer interrupt disabled
	DMA1_Channel1->CCR &= ~DMA_CCR_TCIE; // 0: Transfer complete interrupt disabled

	// 6. Activate the channel by setting the ENABLE bit in the DMA_CCRx register.
	DMA1_Channel1->CCR |= DMA_CCR_EN; // 1: Channel enabled
	dma1_status = DMA1->ISR;





	// ADC Prescaler = 6
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

	// Enable ADC clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	tmpreg = RCC->APB2ENR & (~RCC_APB2ENR_ADC1EN);

	// Enable ADC pin clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	tmpreg = RCC->APB2ENR & (~RCC_APB2ENR_IOPAEN);
	static_cast<void>(tmpreg);

	// ALIGN bit in the ADC_CR2
	ADC1->CR2 &= ~ADC_CR2_ALIGN; // right alignment

	// Enable GPIO A pin 0 in analog mode
	GPIOA->CRL &= (~(GPIO_CRL_CNF0) & ~(GPIO_CRL_MODE0)); // CNFy[1:0]=00 Analog mode; input mode MODE[1:0]=00
	// Enable GPIO A pin 1 in analog mode
	GPIOA->CRL &= (~(GPIO_CRL_CNF1) & ~(GPIO_CRL_MODE1)); // CNFy[1:0]=00 Analog mode; input mode MODE[1:0]=00
	// Enable GPIO A pin 2 in analog mode
	GPIOA->CRL &= (~(GPIO_CRL_CNF2) & ~(GPIO_CRL_MODE2)); // CNFy[1:0]=00 Analog mode; input mode MODE[1:0]=00

	// SMP[2:0] bits in the ADC_SMPR1 and ADC_SMPR2
	// Channel 0 Sample time selection
	ADC1->SMPR2 |= ADC_SMPR2_SMP0; // ADC sample time 0b111 = 239.5 cycles
	// Channel 1 Sample time selection
	ADC1->SMPR2 |= ADC_SMPR2_SMP1; // ADC sample time 0b111 = 239.5 cycles
	// Channel 2 Sample time selection
	ADC1->SMPR2 |= ADC_SMPR2_SMP2; // ADC sample time 0b111 = 239.5 cycles

	// EXTSEL[2:0] External event select for regular group: 0b111=SWSTART
	ADC1->CR2 |= ADC_CR2_EXTSEL;

	/* The regular channels and their order in the conversion sequence must be selected 
	in the ADC_SQRx registers. The total number of conversions in the regular group must 
	be written in the L[3:0] bits in the ADC_SQR1 register. */
	ADC1->SQR3 &= ~ADC_SQR3_SQ1; // regular channel 0 in SQ1[4:0] first conversion in regular sequence
	ADC1->SQR3 |= ADC_SQR3_SQ2_0; // regular channel 1 in SQ1[4:0] first conversion in regular sequence
	ADC1->SQR3 |= ADC_SQR3_SQ3_1; // regular channel 2 in SQ1[4:0] first conversion in regular sequence
	ADC1->SQR1 |= ADC_SQR1_L_1; // Regular channel sequence length L[3:0]=0000 -> 3 conversions

	/* Scan mode can be selected by setting the SCAN bit in the ADC_CR1 register */
	ADC1->CR1 |= ADC_CR1_SCAN; // 1: Scan mode enabled

	/* If the CONT bit is set, conversion does not stop at the last selected
	group channel but continues again from the first selected group channel. */
	ADC1->CR2 |= ADC_CR2_CONT; // 1: Continuous conversion mode

	/* When using scan mode, DMA bit must be set and the direct memory access controller is
	used to transfer the converted data of regular group channels to SRAM after each update of
	the ADC_DR register. */
	ADC1->CR2 |= ADC_CR2_DMA; // 1: DMA mode enabled

	/* Calibration is started by setting the CAL bit in the ADC_CR2 register. Once calibration is
	over, the CAL bit is reset by hardware and normal conversion can be performed.
	Before starting a calibration, the ADC must have been in power-on state (ADON bit = ‘1’) for
	at least two ADC clock cycles. */
	ADC1->CR2 |= ADC_CR2_ADON;
	for (long i = 0; i < 512L * 16 * 8 * 3; i++) { // wait max possible cycles needed
		asm volatile ("nop");
	}
	ADC1->CR2 |= ADC_CR2_CAL;
	while (ADC1->CR2 & ADC_CR2_CAL) { } // wait for CAL:0 Calibration completed

	// Conversion starts when ADON bit is set for a second time by software after ADC power-up time (t_STAB)
	ADC1->CR2 |= ADC_CR2_ADON;
}
