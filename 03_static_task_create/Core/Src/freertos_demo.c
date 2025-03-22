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
StackType_t start_task_stack[START_TASK_STACK];     //��̬����ջ����������ʽ�洢
StaticTask_t stack_task_tcb;            //��̬����tcb�ṹ��
void start_task(void* pvParameters);

/* Task1�������� */
#define TASK1_STACK 128
#define TASK1_PRIORITY 2
TaskHandle_t task1_handle;
StackType_t task1_stack[TASK1_STACK];     //��̬����ջ����������ʽ�洢
StaticTask_t task1_tcb;
void task1(void* pvParameters);

/* Task2�������� */
#define TASK2_STACK 128
#define TASK2_PRIORITY 3
TaskHandle_t task2_handle;
StackType_t task2_stack[TASK2_STACK];     //��̬����ջ����������ʽ�洢
StaticTask_t task2_tcb;
void task2(void* pvParameters);

/* Task3�������� */
#define TASK3_STACK 128
#define TASK3_PRIORITY 4
TaskHandle_t task3_handle;
StackType_t task3_stack[TASK3_STACK];     //��̬����ջ����������ʽ�洢
StaticTask_t task3_tcb;
void task3(void* pvParameters);

/* ==============��̬����������Ҫ�ֶ�ָ�����������������Դ ==============*/
/* ��������������Դ */
StackType_t idle_task_stack[configMINIMAL_STACK_SIZE];
StaticTask_t idle_task_tcb;

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
    StackType_t** ppxIdleTaskStackBuffer,
    uint32_t* pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &idle_task_tcb;
    *ppxIdleTaskStackBuffer = idle_task_stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* ���������ʱ���������Դ */
StackType_t timer_task_stack[configMINIMAL_STACK_SIZE];
StaticTask_t timer_task_tcb;

void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer,
    StackType_t** ppxTimerTaskStackBuffer,
    uint32_t* pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &timer_task_tcb;
    *ppxTimerTaskStackBuffer = timer_task_stack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}



/**
 *@brief ����FreeRTOS
 *
 */
void freertos_start(void)
{
    stack_task_handle=xTaskCreateStatic((TaskFunction_t)start_task,
        (char*)"start_task",
        (uint32_t)START_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)START_TASK_PRIORITY,
        (StackType_t*)start_task_stack,
        (StaticTask_t*)&stack_task_tcb);
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
    task1_handle=xTaskCreateStatic((TaskFunction_t)task1,
        (char*)"task1",
        (uint32_t)TASK1_STACK,
        (void*)NULL,
        (UBaseType_t)TASK1_PRIORITY,
        (StackType_t*)task1_stack,
        (StaticTask_t*)&task1_tcb);

    /* ����Task2 */
    task2_handle = xTaskCreateStatic((TaskFunction_t)task2,
        (char*)"task2",
        (uint32_t)TASK2_STACK,
        (void*)NULL,
        (UBaseType_t)TASK2_PRIORITY,
        (StackType_t*)task2_stack,
        (StaticTask_t*)&task2_tcb);

    /* ����Task3 */
    task3_handle = xTaskCreateStatic((TaskFunction_t)task3,
        (char*)"task3",
        (uint32_t)TASK3_STACK,
        (void*)NULL,
        (UBaseType_t)TASK3_PRIORITY,
        (StackType_t*)task3_stack,
        (StaticTask_t*)&task3_tcb);

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

/**
 *@brief ����KEY1�Ƿ��£�������ɾ��task1
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
            /* �ж��Ƿ��Ѿ�ɾ���������ظ�ִ��ɾ�� */
                /* key1���£�ɾ��task1 */
            printf("ɾ��Task1\n");
            led1_flag = !led1_flag;
        }
        vTaskDelay(20);
    }
}
