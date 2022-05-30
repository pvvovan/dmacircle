#ifndef GPIO_HPP_
#define GPIO_HPP_

#include "stm32f1xx.h"


class Gpio {

public:
	enum class Mode:int {
		analog = 0b11, input = 0b00, output = 0b01, outputAF = 0b10
	};

	enum class Type:int {
		PP, OD
	};
	
	enum class Speed:int {
		low = 0, medium = 0b01, high = 0b10, veryHigh = 0b11
	};

	enum class Pupd:int {
		noPull = 0, pullUp = 0b01, pullDown = 0b10
	};

	enum class AF:int {
		af0, af1, af2, af3, af4, af5, af6, af7, af8, af9, af10, af11, af12, af13, af14, af15
	};

	template<uint8_t ... pins>
	void Init(GPIO_TypeDef *port = GPIOA, Mode m = Mode::input, Type t = Type::PP, Speed s = Speed::low, Pupd p = Pupd::noPull, AF a = AF::af0)
	{
		// RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
		__IO uint32_t tmpreg = RCC->APB2ENR & (~RCC_APB2ENR_AFIOEN);
		static_cast<void>(tmpreg);

		// if (port == GPIOA)
		// 	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
		// if (port == GPIOB)
		// 	RCC->AHBENR  |= RCC_AHBENR_GPIOBEN;
		// if (port == GPIOC)
			// RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
		// if (port == GPIOD)
		// 	RCC->AHBENR  |= RCC_AHBENR_GPIODEN;
		// if (port == GPIOF)
		// 	RCC->AHBENR  |= RCC_AHBENR_GPIOFEN;

		static_cast<void>(m);
		static_cast<void>(t);
		static_cast<void>(s);
		static_cast<void>(p);
		static_cast<void>(a);
		
		if (port == GPIOA) {
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
			tmpreg = RCC->APB2ENR & (~RCC_APB2ENR_IOPAEN);
		} else if (port == GPIOB) {
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
			tmpreg = RCC->APB2ENR & (~RCC_APB2ENR_IOPBEN);
		} else if (port == GPIOC) {
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
			tmpreg = RCC->APB2ENR & (~RCC_APB2ENR_IOPCEN);
		} else if (port == GPIOD) {
			RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
			tmpreg = RCC->APB2ENR & (~RCC_APB2ENR_IOPDEN);
		} else if (port == GPIOE) {
			RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
			tmpreg = RCC->APB2ENR & (~RCC_APB2ENR_IOPEEN);
		} else {
			// TODO: report error
		}

		port->CRH &= ~GPIO_CRH_MODE13_0; // MODE[1:0]
		port->CRH |= GPIO_CRH_MODE13_1; // {10}: Maximum output speed 2 MHz

		port->CRH &= ~GPIO_CRH_CNF13; // CNF1:0 CNF0:0 -> General purpose output Push-pull

		// uint32_t temp1 = 0, temp2 = 0;
		// ((temp1 |= (0b11 << (2*pins))), ... );
		// port->MODER &= ~(temp1);
		// temp1 = 0;
		// ((temp1 |= int(m) << (2*pins)), ... );
		// port->MODER |= temp1;

		// temp1 = 0;
		// ((temp1 |= (0b1 << (pins))), ... );
		// port->OTYPER &= ~(temp1);
		// temp1 = 0;
		// ((temp1 |= (int(t) << (pins))), ... );
		// port->OTYPER |= temp1;

		// temp1 = 0;
		// ((temp1|=(0b11 << (2*pins))), ... );
		// port->OSPEEDR &= ~(temp1);
		// temp1 = 0;
		// ((temp1 |= (int(s) << (2*pins))), ... );
		// port->OSPEEDR |= temp1;

		// temp1 = 0;
		// ((temp1 |= (0b11 << (2*pins))), ... );
		// port->PUPDR &= ~(temp1);
		// temp1 = 0;
		// ((temp1 |= (int(p) << (2*pins))), ... );
		// port->PUPDR |= temp1;

		// temp1 = 0;
		// (((pins < 8?temp1:temp2) |= (0b11 << (4*(pins-(pins<8?0:8))))), ... );
		// port->AFR[0] &= ~temp1;
		// port->AFR[1] &= ~temp2;

		// temp1 = 0;
		// temp2 = 0;
		// (((pins < 8?temp1:temp2) |= (int(a) << (4*(pins-(pins<8?0:8))))), ... );
		// port->AFR[0] |= temp1;
		// port->AFR[1] |= temp2;

	}

	template<uint32_t ... pins>
	void Set(GPIO_TypeDef* port) {
		uint32_t temp = 0u;
		((temp |= (1 << pins)), ...);
		port->BSRR = temp;
	}

	template<uint32_t ... pins>
	void Toggle(GPIO_TypeDef* port) {
		uint32_t temp = 0u;
		((temp |= (1 << pins)), ...);
		port->ODR ^= temp;
	}

	template<uint32_t ... pins>
	void Reset(GPIO_TypeDef* port) {
		uint32_t temp = 0;
		((temp |= (1 << pins)), ...);
		port->BRR = temp;
	}

};

#endif /* GPIO_HPP_ */
