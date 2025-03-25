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
#define TASK1_PRIORITY 3
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

QueueHandle_t semaphr_count_handle;
/**
 *@brief �����������ڴ�������Task
 *
 * @param pvParameters
 */
void start_task(void* pvParameters)
{
    /* �����ٽ������� ������ǰ���񲻻ᱻ��� */
    taskENTER_CRITICAL();

    /* �����������ź��� */
    semaphr_count_handle=xSemaphoreCreateCounting(100, 0);
    if (semaphr_count_handle != NULL) printf("�ɹ������ź���\r\n");
    else                              printf("�ź�������ʧ�ܣ�\r\n");

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


/**
 *@brief ���ڰ���ɨ�裬����⵽����KEY1������ʱ���ͷŶ�ֵ�ź������ź�������ֵ++����
 *
 * @param pvParameters
 */
void task1(void* pvParameters)
{
    uint8_t led_num;
    while (1)
    {
        led_num = Key_Detect();
        if (led_num == KEY1_PRESS)
        {
            if (xSemaphoreGive(semaphr_count_handle) == pdTRUE)  printf("�ͷųɹ�!\n");
            else                                           printf("�ͷ�ʧ�ܣ�\n");
        }
        vTaskDelay(50);
    }

}

/**
 *@brief ��ȡ��ֵ�ź��������ɹ���ȡ���ӡ��ʾ��Ϣ��
 *
 * @param pvParameters
 */
void task2(void* pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(semaphr_count_handle, portTICK_PERIOD_MS*1000) == pdTRUE)  printf("�ɹ���ȡ�ź�����\n");
        printf("�ź�ֵʣ��:[%d]\n", uxSemaphoreGetCount(semaphr_count_handle));
        vTaskDelay(1000);
        
    }
}
