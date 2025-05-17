/**
 * Copyright (c) 2023 HT Micron Semicondutores S.A.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "slpman_qcx212.h"
#include "HT_GPIO_Api.h"
#include "HT_adc_qcx212.h"
#include "htnb32lxxx_hal_usart.h"

// Definições do ADC
#define LDR_ADC_CHANNEL          2  // Canal ADC 2

static uint32_t uart_cntrl = (ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 | ARM_USART_PARITY_NONE | 
                                ARM_USART_STOP_BITS_1 | ARM_USART_FLOW_CONTROL_NONE);

extern USART_HandleTypeDef huart1;

// Definições dos LEDs (usando os definidos no HT_GPIO_Api.h)
#define LED1_PIN               BLUE_LED_PIN
#define LED1_PAD_ID            BLUE_LED_PAD_ID
#define LED2_PIN               WHITE_LED_PIN
#define LED2_PAD_ID            WHITE_LED_PAD_ID
#define LED3_PIN               GREEN_LED_PIN
#define LED3_PAD_ID            GREEN_LED_PAD_ID

// Definições do botão (usando o botão branco definido no HT_GPIO_Api.h)
#define BUTTON_PIN             WHITE_BUTTON_PIN
#define BUTTON_PAD_ID          WHITE_BUTTON_PAD_ID
#define BUTTON_INSTANCE        WHITE_BUTTON_INSTANCE

// Limite de luminosidade (ajustável conforme necessário)
#define LUMINOSITY_THRESHOLD   1500

// Fila para armazenar tempos de pressionamento do botão
QueueHandle_t buttonPressQueue;

// Variável para frequência do LED2
volatile uint32_t led2BlinkFrequency = 500; // ms (valor padrão)

// Estrutura para medição do botão
typedef struct {
    uint32_t pressTime;
} ButtonPressData;

// Protótipos das funções
static void HT_ADC_InitLDR(void);
void TaskLDR(void *pvParameters);
void TaskLED2(void *pvParameters);
void TaskButton(void *pvParameters);
void TaskLED3(void *pvParameters);
void UART_ReceiveHandler(uint8_t *data, uint32_t size);

// Inicialização do ADC para o LDR
static void HT_ADC_InitLDR(void) {
    HT_ADC_ChannelInit(LDR_ADC_CHANNEL);
}

// Tarefa para leitura do LDR e controle do LED1 (LED Azul)
void TaskLDR(void *pvParameters) {
    uint32_t ldrValue;
    
    while (1) {
        ldrValue = HT_ADC_Read(LDR_ADC_CHANNEL);
        
        // Se a luminosidade estiver abaixo do limite, acende o LED1
        if (ldrValue < LUMINOSITY_THRESHOLD) {
            HT_GPIO_WritePin(LED1_PIN, 0, LED_ON);
        } else {
            HT_GPIO_WritePin(LED1_PIN, 0, LED_OFF);
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Tarefa para piscar o LED2 (LED Branco) com frequência configurável
void TaskLED2(void *pvParameters) {
    while (1) {
        HT_GPIO_WritePin(LED2_PIN, 0, LED_ON);
        vTaskDelay(pdMS_TO_TICKS(led2BlinkFrequency / 2));
        HT_GPIO_WritePin(LED2_PIN, 0, LED_OFF);
        vTaskDelay(pdMS_TO_TICKS(led2BlinkFrequency / 2));
    }
}

// Tarefa para medir tempo de pressionamento do botão
void TaskButton(void *pvParameters) {
    TickType_t pressStartTime;
    ButtonPressData pressData;
    bool lastButtonState = false;
    bool currentButtonState;
    
    while (1) {
        currentButtonState = (bool) HT_GPIO_PinRead(BUTTON_INSTANCE, BUTTON_PIN);
        
        // Detecção de borda de descida (botão pressionado)
        if (currentButtonState == false && lastButtonState == true) {
            pressStartTime = xTaskGetTickCount();
        }
        
        // Detecção de borda de subida (botão liberado)
        if (currentButtonState == true && lastButtonState == false) {
            pressData.pressTime = (xTaskGetTickCount() - pressStartTime) * portTICK_PERIOD_MS;
            xQueueSend(buttonPressQueue, &pressData, portMAX_DELAY);
        }
        
        lastButtonState = currentButtonState;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Tarefa para controlar o LED3 (LED Verde) baseado no tempo de pressionamento
void TaskLED3(void *pvParameters) {
    ButtonPressData pressData;
    
    while (1) {
        if (xQueueReceive(buttonPressQueue, &pressData, portMAX_DELAY) == pdTRUE) {
            // Acende o LED3 pelo tempo que o botão foi pressionado
            HT_GPIO_WritePin(LED3_PIN, 0, LED_ON);
            vTaskDelay(pdMS_TO_TICKS(pressData.pressTime));
            
            // Apaga e espera pelo menos 500ms
            HT_GPIO_WritePin(LED3_PIN, 0, LED_OFF);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}

// Handler para recebimento UART (configura frequência do LED2)
void UART_ReceiveHandler(uint8_t *data, uint32_t size) {
    uint32_t receivedValue = atoi((char*)data);
    
    // Validação do valor recebido
    if (receivedValue >= 100 && receivedValue <= 5000) {
        led2BlinkFrequency = receivedValue;
        printf("Frequencia do LED2 atualizada para: %lu ms\n", led2BlinkFrequency);
    } else {
        printf("Valor invalido. Use entre 100 e 5000 ms\n");
    }
}

void main_entry(void) {
    // Inicializações de hardware
    HT_GPIO_ButtonInit();  // Inicializa o botão (usando a função da API)
    HT_GPIO_LedInit();     // Inicializa os LEDs (usando a função da API)
    HT_ADC_InitLDR();
    slpManNormalIOVoltSet(IOVOLT_3_30V);
    
    // Inicializa comunicação UART
    HAL_USART_InitPrint(&huart1, GPR_UART1ClkSel_26M, uart_cntrl, 115200);
    printf("Sistema Embarcado Iniciado\n");
    
    // Cria fila para tempos de pressionamento do botão
    buttonPressQueue = xQueueCreate(5, sizeof(ButtonPressData));
    
    // Configura callback de recebimento UART
    HAL_USART_RegisterRxCallback(&huart1, UART_ReceiveHandler);
    
    // Cria tarefas
    xTaskCreate(TaskLDR, "LDR", 128, NULL, 2, NULL);
    xTaskCreate(TaskLED2, "LED2", 128, NULL, 2, NULL);
    xTaskCreate(TaskButton, "Button", 128, NULL, 3, NULL);
    xTaskCreate(TaskLED3, "LED3", 128, NULL, 2, NULL);
    
    // Inicia scheduler
    vTaskStartScheduler();
    
    // Nunca deve chegar aqui
    while(1);
}