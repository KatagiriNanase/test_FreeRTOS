#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"
#include "LED.h"
#include <stdio.h>
#include "semphr.h"

/* ������������ */
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 1
TaskHandle_t stack_task_handle;
void start_task(void* pvParameters);

/* Task1�������� */
#define TASK1_STACK 128
#define TASK1_PRIORITY 1
TaskHandle_t task1_handle;
void task1(void* pvParameters);

/* Task2�������� */
#define TASK2_STACK 128
#define TASK2_PRIORITY 2
TaskHandle_t task2_handle;
void task2(void* pvParameters);

/* Task3�������� */
#define TASK3_STACK 128
#define TASK3_PRIORITY 3
TaskHandle_t task3_handle;
void task3(void* pvParameters);

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
/* ��������� */
QueueHandle_t semaphr_mutex_handle;
/**
 *@brief �����������ڴ�������Task
 *
 * @param pvParameters
 */
void start_task(void* pvParameters)
{
    /* �����ٽ������� ������ǰ���񲻻ᱻ��� */
    taskENTER_CRITICAL();

    /* ������ֵ�ź��� */
    semaphr_mutex_handle = xSemaphoreCreateMutex();
    if (semaphr_mutex_handle != NULL) printf("�ɹ�����������\r\n");
    else                        printf("����������ʧ�ܣ�\r\n");
    /* �ͷŻ����� */
    xSemaphoreGive(semaphr_mutex_handle);

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

    /* ����Task3 */
    xTaskCreate((TaskFunction_t)task3,
        (char*)"task3",
        (configSTACK_DEPTH_TYPE)TASK3_STACK,
        (void*)NULL,
        (UBaseType_t)TASK3_PRIORITY,
        (TaskHandle_t*)&task3_handle);

    /* ��������ֻ��Ҫ��һ�Σ�����ɾ������ */
    vTaskDelete(NULL);

    /* �˳��ٽ������� */
    taskEXIT_CRITICAL();
}


/**
 *@brief �����ȼ�����
 *
 * @param pvParameters
 */
void task1(void* pvParameters)
{
    
    while (1)
    {
        printf("�����ȼ�Task1��ȡ�ź���\r\n");
        xQueueSemaphoreTake(semaphr_mutex_handle, portMAX_DELAY);
        printf("�����ȼ�Task1��������\r\n");
        HAL_Delay(3000);
        printf("�����ȼ�Task1�ͷ��ź���\r\n");
        xSemaphoreGive(semaphr_mutex_handle);
        vTaskDelay(1000);
    }

}

/**
 *@brief �����ȼ�����
 *
 * @param pvParameters
 */
void task2(void* pvParameters)
{
    while (1)
    {
        printf("�����ȼ�Task2����ִ��\r\n");
        HAL_Delay(1500);
        printf("Task2 for ѭ�����\r\n");
        vTaskDelay(1000);
    }
}


/**
 * @brief �����ȼ�����
 *
 * @param pvParameters 
 */
void task3(void* pvParameters)
{
    while (1)
    {
        printf("�����ȼ�Task3��ȡ�ź���\r\n");
        xSemaphoreTake(semaphr_mutex_handle, portMAX_DELAY);
        printf("������Task3��������\r\n");
        HAL_Delay(1000);
        printf("�����ȼ�Task3�ͷ��ź���\r\n");
        xSemaphoreGive(semaphr_mutex_handle);
        vTaskDelay(1000);
    }
    
}
