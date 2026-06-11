/**
 *******************************************************************************
 * @file 	main.c
 * @author 	jjo
 * @date 	Mar 29, 2024
 * @brief	Fichier principal de votre projet sur carte NuclÃ©o STM32G431KB
 *******************************************************************************
 */

#include "MAE.h"
#include "stm32g4_systick.h"
#include "stm32g4_gpio.h"
#include "stm32g4_uart.h"
#include "stm32g4_utils.h"
#include "stdio.h"

/**
  * @brief  Point d'entrÃ©e de votre application
  */
int main(void)
{
	/* Cette ligne doit rester la premiÃ¨re de votre main !
	 * Elle permet d'initialiser toutes les couches basses des drivers (Hardware Abstraction Layer),
	 * condition prÃ©alable indispensable Ã  l'exÃ©cution des lignes suivantes.
	 */
	HAL_Init();

	/* Initialisation des pÃ©riphÃ©riques utilisÃ©s dans votre programme */
	BSP_GPIO_enable();
	BSP_UART_init(UART2_ID, 115200); // Liaison série vers PC (Console de débogage)
	BSP_UART_init(UART1_ID, 9600);   // Liaison série vers Module Bluetooth HC-05
	BSP_ADC_init();

	/* Indique que les printf sont dirigÃ©s vers l'UART2 */
	BSP_SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	/* Initialisation du port de la led Verte (carte Nucleo) */
	BSP_GPIO_pin_config(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH,GPIO_NO_AF);

	/* Demarrage du systeme */
	printf("=== Systeme DECHUTE - Demarrage ===\n");

	/* Initialisation du systeme complet */
	printf("Initialisation MAE...\r\n");
	MAE_Init();

	/* TÃ¢che de fond, boucle infinie, Infinite loop,... quelque soit son nom vous n'en sortirez jamais */
	printf("=== Systeme Pret - Lancement de la boucle ===\r\n");
	while (1)
	{
		/* Exécution d'un cycle de la machine à états principale */
		system_state_machine();

		/* Cadencement de la boucle principale (10 ms) */
		HAL_Delay(10);

	}
}
