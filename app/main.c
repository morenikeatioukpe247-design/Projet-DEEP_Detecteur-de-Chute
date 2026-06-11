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

/*#define AT_CMD_SIZE 16
static uint8_t tab[AT_CMD_SIZE];*/



#define BLINK_DELAY		100	//ms

void write_LED(bool b)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}

bool char_received(uart_id_t uart_id)
{
	if( BSP_UART_data_ready(uart_id) )	/* Si un caractÃ¨re est reÃ§u sur l'UART 2*/
	{
		/* On "utilise" le caractÃ¨re pour vider le buffer de rÃ©ception */
		BSP_UART_get_next_byte(uart_id);
		return true;
	}
	else
		return false;
}
void heartbeat(void)
{
	while(! char_received(UART2_ID) )
	{
		write_LED(true);
		HAL_Delay(50);
		write_LED(false);
		HAL_Delay(1500);
	}
}
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
	BSP_UART_init(UART2_ID,115200);
	BSP_UART_init(UART1_ID, 9600); //Bluetooth HC-05
	BSP_ADC_init();

	/* Indique que les printf sont dirigÃ©s vers l'UART2 */
	BSP_SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	/* Initialisation du port de la led Verte (carte Nucleo) */
	BSP_GPIO_pin_config(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH,GPIO_NO_AF);



	/* Demarrage du systeme */
	printf("=== Systeme DECHUTE - Demarrage ===\n");
	
	/* Initialisation du systeme complet */
	MAE_Init();

	/*ILI9341_Init();
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	ILI9341_DrawCircle(20,20,5,ILI9341_COLOR_BLUE);
	ILI9341_DrawLine(20,20,100,20,ILI9341_COLOR_RED);
	ILI9341_DrawLine(20,20,20,100,ILI9341_COLOR_RED);
	ILI9341_Putc(110,11,'x',&Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	ILI9341_Putc(15,110,'y',&Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	ILI9341_Puts(200,200, "chaine", &Font_11x18, ILI9341_COLOR_BROWN, ILI9341_COLOR_WHITE);*/
	//process_test_photoresistor(ADC_2);

	//heartbeat();
	/* TÃ¢che de fond, boucle infinie, Infinite loop,... quelque soit son nom vous n'en sortirez jamais */
	while (1)
	{
		system_state_machine();
		
		HAL_Delay(10);

		/*
		************************CODE DE TESTS**********************************
		*/
		   //Bouton
		/*button_event_t evt = BUTTON_state_machine();
			if (evt == BUTTON_EVENT_PRESSED)
				printf("appui detecte\n");*/

		   //Capteurs de pouls
	   /* if (Pouls_ElectrodesConnectees())
	    {uint8_t bpm = Pouls_GetBPM();
	        printf("BPM = %d | RAW = %d\n", bpm, Pouls_GetRaw());}
	    else
	    { printf("Electrodes deconnectees\n");}
	    HAL_Delay(10);*/

		//ILI9341_demo(); Ecran

		   //Buzzer
		/*BUZZER_On();
		HAL_Delay(500);
		BUZZER_Off();
		HAL_Delay(500);
		BUZZER_On();
		HAL_Delay(500);
		BUZZER_Off();*/

	    //MPU6050_demo();

		   //Led
	    /* HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, GPIO_PIN_RESET);
	    HAL_Delay(1000);
	    HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_PIN, GPIO_PIN_SET);*/

		//void ILI9341_demo(void);

		   //Led nucleo
		//if( char_received(UART2_ID) )
		//{
		//write_LED(true);		/* write_LED? Faites un ctrl+clic dessus pour voir... */
		//HAL_Delay(BLINK_DELAY);	/* ... Ã§a fonctionne aussi avec les macros, les variables. C'est votre nouveau meilleur ami */
		//write_LED(false);
		//}

		   //Bluetooth
		/*printf("Hi <Student>, can you read me?\n");
		sprintf((char*)tab, "AT+NAME?\r\n");
		BSP_UART_puts(UART1_ID, tab, 0);
		HAL_Delay(2000); //ça c'est le bluetooth
		BT_SendAlert(ALERTE_CHUTE, 75, MODE_JOUR_BT);
		HAL_Delay(1000);*/

		   //Photoresistance
		/*uint16_t lumiere = BSP_ADC_getValue(ADC_2);
		printf("Luminosite : %d\n", lumiere);
		HAL_Delay(500);*/

	}
}
