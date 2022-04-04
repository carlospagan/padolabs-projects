/*
 * radioPins.c
 *
 *  Created on: 17 de mar de 2022
 *      Author: nathan
 */

/* ------------------------------------------------------------------------- */
#include "main.h"

extern SPI_HandleTypeDef hspi1;

void nrf24_setupPins()
{

}
/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(uint8_t state)
{
    HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, state);
}
/* ------------------------------------------------------------------------- */
void nrf24_csn_digitalWrite(uint8_t state)
{
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, state);
}
/* ------------------------------------------------------------------------- */
void nrf24_sck_digitalWrite(uint8_t state)
{
}
/* ------------------------------------------------------------------------- */
void nrf24_mosi_digitalWrite(uint8_t state)
{}
/* ------------------------------------------------------------------------- */
uint8_t nrf24_miso_digitalRead()
{
	return 0;
}
/* ------------------------------------------------------------------------- */

uint8_t nrf24_spi_transfer(uint8_t *tx){
	uint8_t rx;

	HAL_SPI_TransmitReceive(&hspi1, tx, &rx, 1, 100);

	return rx;
}
