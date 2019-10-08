#include <asf.h>
#include <board.h>
#include <gpio.h>
#include <sysclk.h>
#include "busy_delay.h"

#define CONFIG_USART_IF (AVR32_USART2)

// defines for BRTT interface
#define TEST_A      AVR32_PIN_PA31
#define RESPONSE_A  AVR32_PIN_PA30
#define TEST_B      AVR32_PIN_PA29
#define RESPONSE_B  AVR32_PIN_PA28
#define TEST_C      AVR32_PIN_PA27
#define RESPONSE_C  AVR32_PIN_PB00
volatile int flagA = 0;
volatile int flagB = 0;
volatile int flagC = 0;

__attribute__((__interrupt__)) static void interrupt_J3(void);

void init(){
    sysclk_init();
    board_init();
    busy_delay_init(BOARD_OSC0_HZ);
    
    cpu_irq_disable();
    INTC_init_interrupts();
    INTC_register_interrupt(&interrupt_J3, AVR32_GPIO_IRQ_3, AVR32_INTC_INT1);
    cpu_irq_enable();
    
    stdio_usb_init(&CONFIG_USART_IF);

    #if defined(__GNUC__) && defined(__AVR32__)
        setbuf(stdout, NULL);
        setbuf(stdin,  NULL);
    #endif
}

// TASK_C (and D 1) with expensive B)
/*__attribute__((__interrupt__)) static void interrupt_J3(void){ 
	if (gpio_get_pin_interrupt_flag(TEST_A)){
		gpio_clr_gpio_pin(RESPONSE_A);
		busy_delay_us(5);
		gpio_set_gpio_pin(RESPONSE_A);
		gpio_clear_pin_interrupt_flag(TEST_A);
	}
	if (gpio_get_pin_interrupt_flag(TEST_B)){
		gpio_clr_gpio_pin(RESPONSE_B);
		//fast B event
		//busy_delay_us(5);
		// expensive B event
		busy_delay_us(100);
		gpio_set_gpio_pin(RESPONSE_B);
		gpio_clear_pin_interrupt_flag(TEST_B);
	}
	if (gpio_get_pin_interrupt_flag(TEST_C)){
		gpio_clr_gpio_pin(RESPONSE_C);
		busy_delay_us(5);
		gpio_set_gpio_pin(RESPONSE_C);
		gpio_clear_pin_interrupt_flag(TEST_C);
	}
}*/

// The variance is much lower but the response time is a bit slower. This can be explained by the use of interrupts. Tests are done one sequentially. 
// Tests A and B look the same whereas C is different (2 pikes)

// TASK_D 2) deferred interrupt handling
__attribute__((__interrupt__)) static void interrupt_J3(void){
	if (gpio_get_pin_interrupt_flag(TEST_A)){
		flagA = 1;
		gpio_clear_pin_interrupt_flag(TEST_A);
	}
	if (gpio_get_pin_interrupt_flag(TEST_B)){
		flagB = 1;
		gpio_clear_pin_interrupt_flag(TEST_B);
	}
	if (gpio_get_pin_interrupt_flag(TEST_C)){
		flagC = 1;
		gpio_clear_pin_interrupt_flag(TEST_C);
	}
}


int main (void){
    init();
    //Initialisation : responses actives
	gpio_configure_pin(TEST_A, GPIO_DIR_INPUT);
	gpio_configure_pin(TEST_B, GPIO_DIR_INPUT);
	gpio_configure_pin(TEST_C, GPIO_DIR_INPUT);
	
	gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT|GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_B, GPIO_DIR_OUTPUT|GPIO_INIT_HIGH);
	gpio_configure_pin(RESPONSE_C, GPIO_DIR_OUTPUT|GPIO_INIT_HIGH);
	
	/*gpio_set_gpio_pin(RESPONSE_A);
	gpio_set_gpio_pin(RESPONSE_B);
	gpio_set_gpio_pin(RESPONSE_C);*/
	
	// TASK_C (or D): everything is done into the interrupt J3  (just flags are set in the interrupt)
	gpio_enable_pin_interrupt(TEST_A, GPIO_FALLING_EDGE); //falling edge because the butterfly is set like this
	gpio_enable_pin_interrupt(TEST_B, GPIO_FALLING_EDGE);
	gpio_enable_pin_interrupt(TEST_C, GPIO_FALLING_EDGE);
	

	
    while(1){
        //gpio_toggle_pin(LED0_GPIO);
		//printf("tick\n");
		//busy_delay_ms(500);
		
		//TASK_A and B
		/*if(gpio_get_pin_value(TEST_A)==0){
			gpio_clr_gpio_pin(RESPONSE_A);
			//printf("A should be 0 %u\n", gpio_get_pin_value(RESPONSE_A));
			busy_delay_us(5);
			gpio_set_gpio_pin(RESPONSE_A);
			//printf("A should be 1 : %u\n", gpio_get_pin_value(RESPONSE_A));
		}
		
		if(gpio_get_pin_value(TEST_B)==0){
			gpio_clr_gpio_pin(RESPONSE_B);
			//printf("B should be 0 %u\n", gpio_get_pin_value(RESPONSE_B));
			busy_delay_us(5);
			gpio_set_gpio_pin(RESPONSE_B);
			//printf("B should be 1 %u\n", gpio_get_pin_value(RESPONSE_B));
		}
		if(gpio_get_pin_value(TEST_C)==0){
			gpio_set_pin_low(RESPONSE_C);
			//printf("C should be 0 %u\n", gpio_get_pin_value(RESPONSE_C));
			busy_delay_us(5);
			gpio_set_gpio_pin(RESPONSE_C);
			//printf("C should be 1 %u\n", gpio_get_pin_value(RESPONSE_C));
		}*/
		
		
		// The 	Task A has lowest variance and lowest maximum. Task B needs to access three different pins and maybe has to wait inbetween. 
		// The response for A,B and C looks similar
		
		
		
		// TASK_D2
		
		if (flagA){
			gpio_clr_gpio_pin(RESPONSE_A);
			busy_delay_us(5);
			gpio_set_gpio_pin(RESPONSE_A);
			//gpio_clear_pin_interrupt_flag(TEST_A);
			flagA = 0;
		}
		if (flagB){
			gpio_clr_gpio_pin(RESPONSE_B);
			//expensive event B
			busy_delay_us(100);
			//fast event B
			//busy_delay_us(5);
			gpio_set_gpio_pin(RESPONSE_B);
			//gpio_clear_pin_interrupt_flag(TEST_B);
			flagB = 0;
		}
		if (flagC){
			gpio_clr_gpio_pin(RESPONSE_C);
			busy_delay_us(5);
			gpio_set_gpio_pin(RESPONSE_C);
			//gpio_clear_pin_interrupt_flag(TEST_C);
			flagC = 0;
		}
		
	
    }
}
