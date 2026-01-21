#include "main.h"

#define MAX_STRUCT 12

uint32_t actuators_reg = 0;

// Structures
typedef struct {
    uint8_t sem_id;
    uint32_t sensor_id;
    uint8_t sensor_state;
} subscribe_message_t;

// Prototypes
static void SystemClock_Config(void);
void set_actuator_state(uint8_t actuator_id, uint8_t state);

// Kernel Objects
QueueHandle_t xWriteQueue;
QueueHandle_t xReadQueue;
SemaphoreHandle_t xSem1, xSem2, xSem3, xSem4;

// FreeRTOS tasks
void vTask_EntreeTapis(void *pvParameters);
void vTask_ChargementPalette(void *pvParameters);
void vTask_Ascenceur(void *pvParameters);
void vTask_Palette(void *pvParameters);
void vTask_Write(void *pvParameters);
void vTask_Pub(void *pvParameters);

// Task Handles
TaskHandle_t vTask_Write_handle;
TaskHandle_t vTask_ChargementPalette_handle;
TaskHandle_t vTask_Ascenceur_handle;
TaskHandle_t vTask_Palette_handle;


int main() {
    SystemClock_Config();
    BSP_PB_Init();
    BSP_Console_Init();

    vTraceEnable(TRC_START);

    // Queue pour les commandes
    xWriteQueue = xQueueCreate(10, sizeof(uint32_t));
    // Queue pour les abonnements
    xReadQueue = xQueueCreate(10, sizeof(subscribe_message_t));

    xSem1 = xSemaphoreCreateBinary();
    xSem2 = xSemaphoreCreateBinary();
    xSem3 = xSemaphoreCreateBinary();
    xSem4 = xSemaphoreCreateBinary();

    xTaskCreate(vTask_EntreeTapis, "EntreeTapis", 128, NULL, 1, NULL);
    xTaskCreate(vTask_ChargementPalette, "ChargementPalette", 128, NULL, 1, &vTask_ChargementPalette_handle);
    xTaskCreate(vTask_Ascenceur, "Ascenceur", 128, NULL, 1, &vTask_Ascenceur_handle);
    xTaskCreate(vTask_Palette, "Palette", 128, NULL, 1, &vTask_Palette_handle);
    xTaskCreate(vTask_Write, "Write", 128, NULL, 2, NULL);
    xTaskCreate(vTask_Pub, "Pub", 128, NULL, 2, NULL);

    vTaskStartScheduler();
    while (1);
}


void set_actuator_state(uint8_t actuator_id, uint8_t state) {

    uint8_t byte_index = actuator_id / 7; // Trouve l'octet (0, 1, 2 ou 3)
    uint8_t bit_index  = actuator_id % 7; // Trouve le bit dans cet octet (0 à 6)

    if (byte_index < 4) {
        uint8_t real_bit_pos = (byte_index * 8) + bit_index;

        if (state) {
            actuators_reg |= (1UL << real_bit_pos);  // Met le bit à 1
        } else {
            actuators_reg &= ~(1UL << real_bit_pos); // Met le bit à 0
        }
    }
}


void vTask_EntreeTapis(void *pvParameters) {
    uint32_t msg;
    subscribe_message_t sub = { .sem_id = 1 };

    while (1) {
    	set_actuator_state(0, 1);
    	set_actuator_state(1, 1);
    	set_actuator_state(11, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        sub.sensor_id = 0; sub.sensor_state = 1;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem1, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(4000));

        set_actuator_state(0, 0);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        sub.sensor_id = 2; sub.sensor_state = 0;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem1, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(1000));

        sub.sensor_id = 2; sub.sensor_state = 0;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem1, portMAX_DELAY);

        xTaskNotifyGive(vTask_ChargementPalette_handle);
        vTaskDelay(pdMS_TO_TICKS(400));
    }
}

void vTask_ChargementPalette(void *pvParameters) {
    uint32_t msg;
    subscribe_message_t sub = { .sem_id = 2 };
    uint8_t count = 0;

    set_actuator_state(2, 1);
    msg = actuators_reg;
    xQueueSendToBack(xWriteQueue, &msg, 0);

    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        set_actuator_state(2, 0);
        set_actuator_state(14, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        vTaskDelay(pdMS_TO_TICKS(3000));

        set_actuator_state(14, 0);
        set_actuator_state(2, 1);
        set_actuator_state(4, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        vTaskDelay(pdMS_TO_TICKS(400));

        sub.sensor_id = 4; sub.sensor_state = 1;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem2, portMAX_DELAY);

        set_actuator_state(4, 0);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        if (++count == 3) {
            count = 0;
            xTaskNotifyGive(vTask_Ascenceur_handle);
        }
    }
}

void vTask_Ascenceur(void *pvParameters) {
    uint32_t msg;
    subscribe_message_t sub = { .sem_id = 3 };

    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        set_actuator_state(5, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        sub.sensor_id = 5; sub.sensor_state = 1;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem3, portMAX_DELAY);

        set_actuator_state(3, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        sub.sensor_id = 3; sub.sensor_state = 1;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem3, portMAX_DELAY);

        set_actuator_state(5, 0);
        set_actuator_state(7, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        sub.sensor_id = 7; sub.sensor_state = 0;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem3, portMAX_DELAY);

        set_actuator_state(7, 0);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        vTaskDelay(pdMS_TO_TICKS(500));

        set_actuator_state(3, 0);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        set_actuator_state(5, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        sub.sensor_id = 5; sub.sensor_state = 1;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem3, portMAX_DELAY);

        set_actuator_state(3, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        sub.sensor_id = 3; sub.sensor_state = 1;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem3, portMAX_DELAY);

        set_actuator_state(5, 0);
        set_actuator_state(7, 1);
        set_actuator_state(8, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        sub.sensor_id = 6; sub.sensor_state = 1;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem3, portMAX_DELAY);

        set_actuator_state(8, 0);
        set_actuator_state(7, 0);
        set_actuator_state(3, 0);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        xTaskNotifyGive(vTask_Palette_handle);
    }
}

void vTask_Palette(void *pvParameters) {
    uint32_t msg;
    subscribe_message_t sub = { .sem_id = 4 };

    while (1) {
        set_actuator_state(9, 1);
        set_actuator_state(17, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        vTaskDelay(pdMS_TO_TICKS(500));

        set_actuator_state(9, 0);
        set_actuator_state(16, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        sub.sensor_id = 12; sub.sensor_state = 1;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem4, portMAX_DELAY);

        set_actuator_state(10, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        sub.sensor_id = 9; sub.sensor_state = 1;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem4, portMAX_DELAY);

        set_actuator_state(10, 0);
        set_actuator_state(18, 0);
        set_actuator_state(19, 0);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        set_actuator_state(6, 1);
        set_actuator_state(8, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        sub.sensor_id = 7; sub.sensor_state = 1;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem4, portMAX_DELAY);

        set_actuator_state(8, 0);
        set_actuator_state(6, 0);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        set_actuator_state(10, 1);
        set_actuator_state(18, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);

        sub.sensor_id = 9; sub.sensor_state = 0;
        xQueueSendToBack(xReadQueue, &sub, 0);
        xSemaphoreTake(xSem4, portMAX_DELAY);

        set_actuator_state(10, 0);
        set_actuator_state(19, 1);
        msg = actuators_reg;
        xQueueSendToBack(xWriteQueue, &msg, 0);
    }
}


void vTask_Pub(void *pvParameters) {
    subscribe_message_t msg;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    subscribe_message_t subs[MAX_STRUCT] = {0};

    while (1) {
        if (xQueueReceive(xReadQueue, &msg, 0) == pdTRUE) {
            for (int i = 0; i < MAX_STRUCT; i++) {
                if (subs[i].sem_id == 0) {
                    subs[i] = msg;
                    break;
                }
            }
        }
        for (int i = 0; i < MAX_STRUCT; i++) {
            if (subs[i].sem_id != 0) {
                if (FACTORY_IO_Sensors_Get(subs[i].sensor_id) == subs[i].sensor_state) {
                    if (subs[i].sem_id == 1) xSemaphoreGive(xSem1);
                    else if (subs[i].sem_id == 2) xSemaphoreGive(xSem2);
                    else if (subs[i].sem_id == 3) xSemaphoreGive(xSem3);
                    else if (subs[i].sem_id == 4) xSemaphoreGive(xSem4);
                    subs[i].sem_id = 0;
                }
            }
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));
    }
}

void vTask_Write(void *pvParameters) {
    uint32_t command;
    while (1) {
        if (xQueueReceive(xWriteQueue, &command, portMAX_DELAY) == pdTRUE) {
            FACTORY_IO_Actuators_Set(command);
            vTaskDelay(pdMS_TO_TICKS(150));
        }
    }
}

// Clock Configuration
static void SystemClock_Config(void) {
    uint32_t HSE_Status;
    uint32_t SW_Status;
    uint32_t timeout = 1000000;

    // 1. Démarrer le HSE (8MHz sur les cartes Nucleo via ST-Link)
    RCC->CR |= RCC_CR_HSEBYP;
    RCC->CR |= RCC_CR_HSEON;

    // Attendre que le HSE soit prêt
    do {
        HSE_Status = RCC->CR & RCC_CR_HSERDY_Msk;
        timeout--;
    } while ((HSE_Status == 0) && (timeout > 0));

    // 2. IMPORTANT : Désactiver le PLL pour économiser l'énergie
    RCC->CR &= ~RCC_CR_PLLON;

    // 3. Configurer les diviseurs AHB et APB à /1 (HCLK = PCLK = 8MHz)
    RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR &= ~RCC_CFGR_PPRE_Msk;
    RCC->CFGR |= RCC_CFGR_PPRE_DIV1;

    // 4. Sélectionner HSE comme horloge système (SW = 01)
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSE;

    // Attendre que le switch soit effectif
    do {
        SW_Status = (RCC->CFGR & RCC_CFGR_SWS_Msk);
        timeout--;
    } while ((SW_Status != RCC_CFGR_SWS_HSE) && (timeout > 0));

    // 5. Mettre à jour la variable globale pour FreeRTOS et HAL
    SystemCoreClockUpdate();
}
