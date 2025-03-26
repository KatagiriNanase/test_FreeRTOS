#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"
#include "LED.h"
#include <stdio.h>
#include "event_groups.h"

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

EventGroupHandle_t event_group_handle;
/**
 *@brief �����������ڴ�������Task
 *
 * @param pvParameters
 */
void start_task(void* pvParameters)
{
    /* �����ٽ������� ������ǰ���񲻻ᱻ��� */
    taskENTER_CRITICAL();

    event_group_handle = xEventGroupCreate();

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

#define  EVENT_BIT_0   (0x01<<0)
#define  EVENT_BIT_1   (0x00<<1)
/**
 *@brief ��ȡ�������¼�ֵ�����ݲ�ͬ��ֵ���¼���־����Ӧ�¼�λ��һ��ģ���¼�������
 *
 * @param pvParameters
 */
void task1(void* pvParameters)
{
    uint8_t key_num;
    while (1)
    {
        key_num = Key_Detect();
        if (key_num == KEY1_PRESS)
        {
            /* ���¼���Bit0��λ */
            xEventGroupSetBits(event_group_handle, EVENT_BIT_0);
            printf("Bit0 ��λ\r\n");
        }
        else if (key_num == KEY2_PRESS)
        {
            /* ���¼���Bit1��λ */
            xEventGroupSetBits(event_group_handle, EVENT_BIT_1);
            printf("Bit1 ��λ\r\n");
        }
    }

}


/**
 *@brief ͬʱ�ȴ��¼���־���еĶ���¼�λ������Щ�¼�λ���� 1 �Ļ���ִ����Ӧ�Ĵ���
 *
 * @param pvParameters
 */
void task2(void* pvParameters)
{
    EventBits_t res;
    while (1)
    {
        res = xEventGroupWaitBits(event_group_handle,
            EVENT_BIT_0 | EVENT_BIT_1,
            pdTRUE,
            pdTRUE,
            portMAX_DELAY);
        printf("�ȴ�����ʱ���־λ��%#x\r\n", res);
    }
}
