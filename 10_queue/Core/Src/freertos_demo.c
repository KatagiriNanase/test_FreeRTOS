#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"
#include "LED.h"
#include "queue.h"
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
#define TASK2_PRIORITY 3
TaskHandle_t task2_handle;
void task2(void* pvParameters);

/* Task3�������� */
#define TASK3_STACK 128
#define TASK3_PRIORITY 4
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

QueueHandle_t queue1_handle;
QueueHandle_t bigQueue_handle;
const char* test_info = "hello world!\n";
/**
 *@brief �����������ڴ�������Task
 *
 * @param pvParameters
 */
void start_task(void* pvParameters)
{
    /* �����ٽ������� ������ǰ���񲻻ᱻ��� */
    taskENTER_CRITICAL();

    /* ��������1 ���keyֵ */
    queue1_handle = xQueueCreate(1, sizeof(uint8_t));
    /* ��������2 ����ַ���ָ�� */
    bigQueue_handle = xQueueCreate(1, sizeof(char*));

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
        TASK3_STACK,
        (void*)NULL,
        TASK3_PRIORITY,
        (TaskHandle_t*)task3_handle);

    /* ��������ֻ��Ҫ��һ�Σ�����ɾ������ */
    vTaskDelete(NULL);

    /* �˳��ٽ������� */
    taskEXIT_CRITICAL();
}

_Bool led1_flag;
/**
 *@brief ������key1��key2���£�����ֵ����������queue1����ӣ���
 *������key3���£�����������ݣ������������ݵĵ�ַ������big_queue�С�
 *
 * @param pvParameters
 */
void task1(void* pvParameters)
{
    uint8_t key_num;
    while (1)
    {
        key_num = Key_Detect();
        if (key_num == KEY1_PRESS || key_num == KEY2_PRESS)
        {
            xQueueSend(queue1_handle, (void*)&key_num, portMAX_DELAY);
        }
        else if (key_num == KEY3_PRESS)
        {
            xQueueSend(bigQueue_handle, (void*)&test_info, portMAX_DELAY);
        }
        vTaskDelay(20);
    }

}

/**
 *@brief ��ȡ����queue1�е���Ϣ�����ӣ�����ӡ�����յ��ļ�ֵ��
 *
 * @param pvParameters
 */
void task2(void* pvParameters)
{
    uint8_t rec;
    while (1)
    {
        if (xQueueReceive(queue1_handle, &rec, portMAX_DELAY))   printf("task2���ճɹ���\n");
        printf("rec:%d\r\n", rec);
    }
}

/**
 *@brief �Ӷ���big_queue��ȡ�����ݵ�ַ��ͨ����ַ���ʴ�����
 *
 * @param pvParameters
 */
void task3(void* pvParameters)
{
    char* infoBuff;
    while (1)
    {
        if (xQueueReceive(bigQueue_handle, infoBuff, portMAX_DELAY))   printf("task3���ճɹ���\n");
        printf("rec:%s\r\n", infoBuff);
    }

}
