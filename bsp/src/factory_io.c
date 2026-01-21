

#include "stm32f0xx.h"
#include "factory_io.h"

// Déclaration des buffers au plus près du hardware
uint8_t tx_dma_buffer[7];
uint8_t rx_dma_buffer[14];


/*
 * FACTORY_IO_Actuators_Set
 */

void FACTORY_IO_Actuators_Set(uint32_t cmd)
{
	uint8_t	buffer[7];
	uint8_t	n;

	// Prepare frame buffer
	buffer[0] = TAG_ACTUATORS; 							// Actuators tag

	buffer[1] = (uint8_t) (cmd & 0x000000FF);			// data byte #1
	buffer[2] = (uint8_t)((cmd & 0x0000FF00) >>8U );	// data byte #2
	buffer[3] = (uint8_t)((cmd & 0x00FF0000) >>16U);	// data byte #2
	buffer[4] = (uint8_t)((cmd & 0xFF000000) >>24U);	// data byte #2

	buffer[5] = 0x00;									// CRC (not yet implemented)
	buffer[6] = '\n';									// End byte

	// Send buffer over UART
	for(n=0; n<7; n++)
	{
		while ( (USART2->ISR & USART_ISR_TC) != USART_ISR_TC);
		USART2->TDR = buffer[n];
	}
}


/*
 * FACTORY_IO_Sensors_Get
 */

uint8_t FACTORY_IO_Sensors_Get(uint32_t sensor_id)
{
    uint32_t sstates = 0;

    // 1. Reconstruction du mot 32 bits à partir du buffer DMA
    // On s'assure de caster en uint32_t avant le décalage pour éviter les erreurs
    sstates |= (uint32_t)rx_dma_buffer[1];
    sstates |= ((uint32_t)rx_dma_buffer[2] << 8U);
    sstates |= ((uint32_t)rx_dma_buffer[3] << 16U);
    sstates |= ((uint32_t)rx_dma_buffer[4] << 24U);

    // 2. Traduction de l'ID en position réelle (Logique 7-bits)
    // Si l'ID 11 est au bit 12 (octet 1, bit 4), on applique le même calcul
    uint8_t byte_index = sensor_id / 7;
    uint8_t bit_index  = sensor_id % 7;
    uint8_t real_bit_pos = (byte_index * 8) + bit_index;

    // 3. Création du masque dynamique
    uint32_t msk = (1UL << real_bit_pos);

    // 4. Comparaison logique
    if ((sstates & msk) == 0)
    {
        return 0; // Capteur inactif
    }
    else
    {
        return 1; // Capteur actif
    }
}


/*
 * Force Factory IO to send sensors and actuators states
 */

void FACTORY_IO_update(void)
{
	uint8_t	buffer[2];
	uint8_t	n;

	// Prepare frame buffer
	buffer[0] = TAG_UPDATE; 		// Update tag
	buffer[1] = '\n';

	// Send buffer over UART
	for(n=0; n<2; n++)
	{
		while ( (USART2->ISR & USART_ISR_TC) != USART_ISR_TC);
		USART2->TDR = buffer[n];
	}
}

