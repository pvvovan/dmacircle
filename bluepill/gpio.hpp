#ifndef GPIO_HPP_
#define GPIO_HPP_

#include <stm32f1xx.h>


class gpio {

public:
	enum class output : uint8_t {
		general_push_pull	= 0b00, // CNFy[1:0] -> 00b: General purpose output push-pull
		general_open_drain	= 0b01, // CNFy[1:0] -> 01b: General purpose output Open-drain
		alternate_push_pull	= 0b10, // CNFy[1:0] -> 10b: Alternate function output Push-pull
		alternate_open_drain	= 0b11  // CNFy[1:0] -> 11b: Alternate function output Open-drain
	};

	enum class speed : uint8_t {
		max10MHz	= 0b01, // MODEy[1:0] -> 01b: Output mode, max speed 10 MHz
		max2MHz		= 0b10, // MODEy[1:0] -> 10b: 10: Output mode, max speed 2 MHz
		max50MHz	= 0b11  // MODEy[1:0] -> 11b: Output mode, max speed 50 MHz
	};

	template<uint8_t ... pins>
	void Init(GPIO_TypeDef *port = GPIOA, output cnf = output::general_push_pull, speed mode = speed::max2MHz)
	{
		/* I/O port x clock enable */
		uint32_t RCC_APB2ENR_IOPxEN = 0u;

		if (port == GPIOA) {
			RCC_APB2ENR_IOPxEN = RCC_APB2ENR_IOPAEN;
		} else if (port == GPIOB) {
			RCC_APB2ENR_IOPxEN = RCC_APB2ENR_IOPBEN;
		} else if (port == GPIOC) {
			RCC_APB2ENR_IOPxEN = RCC_APB2ENR_IOPCEN;
		} else if (port == GPIOD) {
			RCC_APB2ENR_IOPxEN = RCC_APB2ENR_IOPDEN;
		} else if (port == GPIOE) {
			RCC_APB2ENR_IOPxEN = RCC_APB2ENR_IOPEEN;
		} else {
			// TODO: report error
		}

		RCC->APB2ENR |= RCC_APB2ENR_IOPxEN;
		__IO uint32_t tmpreg = RCC->APB2ENR & (~RCC_APB2ENR_IOPxEN);
		static_cast<void>(tmpreg);

		uint32_t reg_crl = port->CRL;
		uint32_t reg_crh = port->CRH;

		// clear CNF and MODE bits for 'pins'
		(((pins < 8 ? reg_crl : reg_crh) &= static_cast<uint32_t>(~(0b1111u << (4 * (pins < 8 ? pins : (pins - 8)))))), ... );

		// set MODE[1:0] -> output mode max speed MHz
		(((pins < 8 ? reg_crl : reg_crh) |= (static_cast<uint32_t>(mode) << (4 * (pins < 8 ? pins : (pins - 8))))), ... );

		// set CNFy[1:0] -> port configuration
		(((pins < 8 ? reg_crl : reg_crh) |= (static_cast<uint32_t>(cnf) << ((4 * (pins < 8 ? pins : (pins - 8))) + 2))), ... );

		port->CRL = reg_crl;
		port->CRH = reg_crh;
	}

	template<uint8_t ... pins>
	void Set(GPIO_TypeDef* port) {
		uint32_t temp = 0u;
		((temp |= (1u << pins)), ...);
		port->BSRR = temp;
	}

	template<uint8_t ... pins>
	void Toggle(GPIO_TypeDef* port) {
		uint32_t temp = 0u;
		((temp |= (1u << pins)), ...);
		port->ODR ^= temp;
	}

	template<uint8_t ... pins>
	void Reset(GPIO_TypeDef* port) {
		uint32_t temp = 0u;
		((temp |= (1u << pins)), ...);
		port->BRR = temp;
	}

};

#endif /* GPIO_HPP_ */
