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
#define TASK2_STACK 256
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

/**
 *@brief LED1ÿ500ms��˸һ��
 *
 * @param pvParameters
 */
void task1(void* pvParameters)
{
    while (1)
    {
        printf("task1����\n");
        LED_Toggle(LED1_Pin);
        vTaskDelay(500);
        //HAL_Delay(500);
    }

}

/**
 *@brief LED2ÿ500ms��˸һ��
 *
 * @param pvParameters
 */
void task2(void* pvParameters)
{
    UBaseType_t task_priority;
    UBaseType_t task_num;
    TaskStatus_t task_state[10];
    TaskHandle_t task_handle;
    UBaseType_t task_stack_remain_min;

    /* ��ȡtask1�������ȼ� */
    task_priority = uxTaskPriorityGet(task1_handle);
    printf("task1 priority:%d\r\n", task_priority);
    /* ����task1�������ȼ� */
    vTaskPrioritySet(task1_handle, 3);
    task_priority = uxTaskPriorityGet(task1_handle);
    printf("task1 priority:%d\r\n", task_priority);

    /* ��ȡ�������� */
    task_num = uxTaskGetNumberOfTasks();
    printf("Num of sysTask:%d\r\n", task_num);

    /* ��ȡ��������״̬��Ϣ */
    uxTaskGetSystemState(task_state, task_num, NULL);
    printf("������\t\t������\t\t��ǰ״̬\t\t��ǰ���ȼ�\t\tԭʼ���ȼ�\t\n");
    for (uint8_t i = 0;i < task_num;++i)
    {
        printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\r\n",
            task_state[i].pcTaskName,
            task_state[i].xTaskNumber,
            task_state[i].eCurrentState,
            task_state[i].uxCurrentPriority,
            task_state[i].uxBasePriority);
    }

    /* ��ȡ��ǰ������ */
    task_handle = xTaskGetCurrentTaskHandle();
    printf("current handle:%p\n", task_handle);

    /* ͨ����������ȡָ�������� */
    task_handle = xTaskGetHandle("task1");
    printf("task1 handle:%p\r\n", task_handle);

    /* ��ȡ����ջ��ʷ��Сֵ */
    task_stack_remain_min = uxTaskGetStackHighWaterMark(task2_handle);
    printf("����ջ��ʷ��Сֵ:%d\n", task_stack_remain_min);

    while (1)
    {
        vTaskDelay(500);
        //HAL_Delay(500);
    }
    

}
