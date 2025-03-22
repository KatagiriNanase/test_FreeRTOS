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

_Bool led1_flag;
/**
 *@brief LED1ÿ500ms��˸һ��
 *
 * @param pvParameters
 */
void task1(void* pvParameters)
{
    while (1)
    {
        if (led1_flag == 0)
        {
            printf("task1����\n");
            LED_Toggle(LED1_Pin);
            vTaskDelay(500);
        }

    }

}

/**
 *@brief LED2ÿ500ms��˸һ��
 *
 * @param pvParameters
 */
void task2(void* pvParameters)
{
    while (1)
    {
        printf("task2����\n");
        LED_Toggle(LED2_Pin);
        vTaskDelay(500);
    }
}

char task_info[200];
/**
 *@brief KEY1���£�����task1������KEY2�������лָ�task1��KEY3���£������������KEY4���£��ָ�������������ӡ�����״̬��
 *
 * @param pvParameters
 */
void task3(void* pvParameters)
{
    uint8_t key_num;
    while (1)
    {
        printf("task3����\n");
        key_num = Key_Detect();
        if (key_num == KEY1_PRESS)
        {
            vTaskSuspend(task1_handle);
        }
        else if (key_num == KEY2_PRESS)
        {
            /* �ָ�����1 */
            vTaskResume(task1_handle);

        }
        else if (key_num == KEY3_PRESS)
        {
            /* ��������� */
            vTaskSuspendAll();
        }
        else if (key_num == KEY4_PRESS)
        {
            /* �ָ������� */
            xTaskResumeAll();
        }

        /* �鿴����״̬ */
        vTaskList(task_info);
        printf("%s\r\n", task_info);
        vTaskDelay(500);    
    }
}
