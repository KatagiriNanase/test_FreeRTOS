#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"
#include "LED.h"
#include <stdio.h>

/* ������������ */
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 1
TaskHandle_t stack_task_handle;
void start_task(void* pvParameters);

/* Task1�������� */
#define TASK1_STACK 128
#define TASK1_PRIORITY 2
TaskHandle_t task1_handle;
void task1(void* pvParameters);

/* Task2�������� */
#define TASK2_STACK 128
#define TASK2_PRIORITY 2
TaskHandle_t task2_handle;
void task2(void* pvParameters);

/**
 *@brief ����FreeRTOS
 *
 */
void freertos_start(void)
{
    /* ����һ���������� */
    xTaskCreate((TaskFunction_t)start_task,
        (char*)"start_task",
        (configSTACK_DEPTH_TYPE)START_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)START_TASK_PRIORITY,
        (TaskHandle_t*)&stack_task_handle);

    /* ���������� */
    vTaskStartScheduler();
}


/**
 *@brief �����������ڴ�������Task
 *
 * @param pvParameters
 */
void start_task(void* pvParameters)
{
    /* �����ٽ������� ������ǰ���񲻻ᱻ��� */
    taskENTER_CRITICAL();
    /* ����Task1 */
    xTaskCreate((TaskFunction_t)task1,
        (char*)"task1",
        (configSTACK_DEPTH_TYPE)TASK1_STACK,
        (void*)NULL,
        (UBaseType_t)TASK1_PRIORITY,
        (TaskHandle_t*)&task1_handle);

    /* ����Task2 */
    xTaskCreate((TaskFunction_t)task2,
        (char*)"task2",
        (configSTACK_DEPTH_TYPE)TASK2_STACK,
        (void*)NULL,
        (UBaseType_t)TASK2_PRIORITY,
        (TaskHandle_t*)&task2_handle);

    /* ��������ֻ��Ҫ��һ�Σ�����ɾ������ */
    vTaskDelete(NULL);

    /* �˳��ٽ������� */
    taskEXIT_CRITICAL();
}

_Bool led1_flag;
/**
 *@brief LED1ÿ500ms��˸һ��
 *
 * @param pvParameters
 */
void task1(void* pvParameters)
{
    uint16_t task1_count;
    while (1)
    {
        printf("task1����[%d]��\r\n", ++task1_count);
        // vTaskDelay(500);         ���ʹ����FreeRTOS����ʱ�������ͻ��л������������ˣ��۲첻�������������ִ���˶��
        HAL_Delay(10);
    }

}

/**
 *@brief LED2ÿ500ms��˸һ��
 *
 * @param pvParameters
 */
void task2(void* pvParameters)
{
    uint16_t task2_count;
    while (1)
    {
        printf("task2����[%d]��\r\n", ++task2_count);
        // vTaskDelay(500);         ���ʹ����FreeRTOS����ʱ�������ͻ��л������������ˣ��۲첻�������������ִ���˶��
        HAL_Delay(10);
    }
}
