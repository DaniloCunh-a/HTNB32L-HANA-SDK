/**
 *
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
 *
 */

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "HT_GPIO_Api.h"

static uint32_t uart_cntrl = (ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 | ARM_USART_PARITY_NONE | 
                              ARM_USART_STOP_BITS_1 | ARM_USART_FLOW_CONTROL_NONE);

extern USART_HandleTypeDef huart1;

/* Variáveis globais compartilhadas */
volatile uint8_t button_state = 0;  // Estado do botão (0 = solto, 1 = pressionado)

/* Protótipos de funções */
void Task_LeituraBotao(void *pvParameters);
void Task_ControleLED(void *pvParameters);

void Task_LeituraBotao(void *pvParameters) {
    // Inicialização do botão (executa apenas uma vez)
    HT_GPIO_ButtonInit();
    
    while (1) {
        // Lê o estado do botão (0 = pressionado, 1 = solto para botão pull-up)
        if (GPIO_PinRead(BLUE_BUTTON_INSTANCE, BLUE_BUTTON_PIN) == 0) {
            button_state = 1;  // Botão pressionado
            printf("Botao PRESSIONADO\n");
        } else {
            button_state = 0;  // Botão solto
        }
        
        // Delay para debounce e evitar leitura excessiva
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void Task_ControleLED(void *pvParameters) {
    // Inicialização do LED (executa apenas uma vez)
    HT_GPIO_LedInit();
    
    while (1) {
        // Controla o LED baseado no estado do botão
        if (button_state) {
            HT_GPIO_WritePin(BLUE_LED_PIN, BLUE_LED_INSTANCE, LED_ON);
            printf("LED LIGADO\n");
        } else {
            HT_GPIO_WritePin(BLUE_LED_PIN, BLUE_LED_INSTANCE, LED_OFF);
            printf("LED DESLIGADO\n");
        }
        
        // Delay para sincronização com a task de leitura
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void main_entry(void) {
    // Inicializa a comunicação serial para debug
    HAL_USART_InitPrint(&huart1, GPR_UART1ClkSel_26M, uart_cntrl, 115200);
    printf("Sistema de Controle LED-Botao com FreeRTOS\n");

    // Cria as tasks
    xTaskCreate(Task_LeituraBotao,   // Função da task
                "Leitura_Botao",     // Nome da task (para debug)
                128,                // Tamanho da stack
                NULL,               // Parâmetros
                2,                  // Prioridade (maior para leitura)
                NULL);              // Handle da task

    xTaskCreate(Task_ControleLED,
                "Controle_LED",
                128,
                NULL,
                1,                  // Prioridade menor para controle
                NULL);

    // Inicia o escalonador do FreeRTOS
    vTaskStartScheduler();
    
    // Nunca deverá chegar aqui
    printf("ERRO: Nao deveria chegar aqui!\n");
    while(1);
}

/******** HT Micron Semicondutores S.A **END OF FILE*/