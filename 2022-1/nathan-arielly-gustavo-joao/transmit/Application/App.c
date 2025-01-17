/*
 * App.c
 *
 *  Created on: 22 de mar de 2022
 *      Author: nathan
 */

	#include "App.h"
	extern TIM_HandleTypeDef htim1;

	//keyboard
	uint8_t row[4] = {0}, col = 0;
	uint8_t x = 0, flagReceiver = 0, debug = 15;

	//senha
	uint8_t button = 0, i  = 0, validacao = 0;
	uint8_t senha[4] = {0}, senhaDefinida[4] = {1,2,3,4};
	uint8_t senhaDefinida2[4] = {1,1,2,3};
	uint8_t sendFlag = 0, tamFlag = 0;
	char text[16];

	//nrf
	uint8_t data_array[2], data_receive[2];
	uint8_t tx_address[5] = {0xA0,0xA0,0xA0,0xA0,0xA0};
	uint8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};

/*	uint8_t tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
	uint8_t rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};*/

	//lcd
	uint16_t app;
	uint16_t app_t;
	uint8_t tamanho = 0;

	//Interrupt with pull-up
	//interrupt pin keyboard
	void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){

			if (!(HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin))) {
				col = 1;

			}else if (!(HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin))) {
				col = 2;

			}else if (!(HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin))) {
				col = 3;

			}else if (!(HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin))) {
				col = 4;

			}
	}

	uint8_t autentica(uint8_t senha[4], uint8_t senhaCorreta[4]){

		uint8_t verifica[4] = {0};

		//roda por cada digito comparando com a senhaDefinida
		for (int cont = 0; cont < 4; cont++) {

			if (senha[cont] == senhaCorreta[cont]) {
				verifica[cont] = CORRETA;
			}else verifica[cont] = INCORRETA;
		}

		return (verifica[0]*verifica[1]*verifica[2]*verifica[3]);

	}


	void __init_application(){

		  //memset(&app, 0, sizeof(app_t));

		  HAL_TIM_Base_Start(&htim1);
		  lcd_attr_gpio(LCD_RS, LCD_RS_GPIO_Port, LCD_RS_Pin);
		  lcd_attr_gpio(LCD_RW, LCD_RW_GPIO_Port, LCD_RW_Pin);
		  lcd_attr_gpio(LCD_E, LCD_E_GPIO_Port, LCD_E_Pin);
		  lcd_attr_gpio(LCD_D4, LCD_D4_GPIO_Port, LCD_D4_Pin);
		  lcd_attr_gpio(LCD_D5, LCD_D5_GPIO_Port, LCD_D5_Pin);
		  lcd_attr_gpio(LCD_D6, LCD_D6_GPIO_Port, LCD_D6_Pin);
		  lcd_attr_gpio(LCD_D7, LCD_D7_GPIO_Port, LCD_D7_Pin);

		  lcd_init();
		  lcd_clear();
		  lcd_put_cur(0, 0);
		  lcd_send_string("Bem Vindo!");
		  lcd_put_cur(1, 0);
		  lcd_send_string("Digite a senha");
	}


	void app_init(){

		nrf24_init();
		nrf24_config(2, 2);
		nrf24_tx_address(tx_address);
		nrf24_rx_address(rx_address);
		__init_application();

	}

	void app_run(){

		while(1){

			//testa as coluna da linha selecionada;
			if(col == 1) {
				button = x*4;
				col = 0;
				senha[i] = button;
				i++;
			}
			else if(col == 2){
				button = 1+x*4;
				col = 0;
				senha[i] = button;
				i++;
			}
			else if(col == 3){
				button = 2+x*4;
				col = 0;
				senha[i] = button;
				i++;
			}
			else if(col == 4){

				if (x == 3) {
					i = 0;
				}else{
					button = 3+x*4;
					senha[i] = button;
					i++;
				}
				col = 0;
			}

			//incremento para alteração da linha;
			x++;
			if (x == 4) {
				x = 0;
			}

			//Switch da linha (x)
			switch(x){
				//verifica linha 0
				case 0:
					row[0] = 0;
					row[3] = 1;
					break;
				//verifica linha 1
				case 1:
					row[1] = 0;
					row[0] = 1;
					break;
				//verifica linha 2
				case 2:
					row[2] = 0;
					row[1] = 1;
					break;
				//verifica linha 3
				case 3:
					row[3] = 0;
					row[2] = 1;
					break;
				default:
				break;
			}


			//Ativa linha de acordo com o switch acima todas;
			HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, row[0]);
			HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, row[1]);
			HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, row[2]);
			HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, row[3]);

			HAL_Delay(75);

			//se tiver 4 digitos
			if(i == 4){

				//senha para desligar
				validacao = autentica(senha, senhaDefinida2);
				if(validacao == 1){
					sendFlag = 2;
					data_array[0] = 0;
				}

				//senha para ligar
				validacao = autentica(senha, senhaDefinida);
				if(validacao == 1){
					sendFlag = 1;
					data_array[0] = 2;
				}

				//limpa a senha digitada
				for (uint8_t j = 0; j < 4; j++) {

					senha[j] = 0;

				}

				if (sendFlag == 0) {
					  lcd_clear();
					  lcd_put_cur(0, 0);
					  lcd_send_string("Bem Vindo!");
					  lcd_put_cur(1, 0);
					  lcd_send_string("Senha incorreta!");
				}

				i = 0;

			}

			//exibe a senha que está sendo digitada
			if (i > 0 && sendFlag == 0) {
				lcd_clear();
 				sprintf(text, "Senha: %d%d%d%d", senha[0], senha[1], senha[2], senha[3]);
				lcd_put_cur(0, 0);
				lcd_send_string(text);
			}

			//exibe o tamanho
			if(sendFlag == 1) {

				lcd_clear();
				sprintf(text, "Tamanho: %d%d%d", senha[0], senha[1], senha[2]);
				lcd_put_cur(0, 0);
				lcd_send_string(text);
				lcd_put_cur(1, 3);
				lcd_send_string("De 0 a 243");

					//se tamanho for digitado
					if (i == 3) {

						uint16_t tam = ((senha[0]*100)+(senha[1]*10)+senha[2]);
						uint8_t lowbyte = tam;
						uint8_t highbyte = (tam>>8);

						if (tam <= 243) {

							data_array[1] = tam;
							tamFlag = 1;
							i = 0;

							//Limpa a senha
							for (uint8_t j = 0; j < 3; j++) {

								senha[j] = 0;
							}

						}else{
							lcd_put_cur(0, 0);
							lcd_send_string("digite novamente");
							lcd_put_cur(1, 0);
							lcd_send_string("tamanho invalido");
						}
					}

				}else if (sendFlag == 2) {

					tamFlag = 1;
				}


			//Se senha for correta
			if(sendFlag >= 1 && tamFlag == 1){

				tamFlag = 0;

				// 10 cm 2 segundos

				//NRF TRANSMIT -------------------
				HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);
				nrf24_send(data_array);
				sendFlag = 0;
				while(nrf24_isSending());
				debug = nrf24_lastMessageStatus();

				nrf24_powerUpRx();

				flagReceiver = 0;

				while(flagReceiver == 0){

					/* se nrf24 estiver se comunicando */
					if(nrf24_dataReady()){

						/* recebe dados enviados */
						nrf24_getData(data_receive);
						flagReceiver = 1;

					}
				}

				lcd_clear();
				sprintf(text, "Corrente: %d mA", data_receive[0]);
				lcd_put_cur(0, 0);
				lcd_send_string(text);
				lcd_put_cur(1, 0);
				lcd_send_string("Digite a senha!");
			}

		}
	}
