#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"
#include "LED.h"
#include <stdio.h>

/* 启动任务配置 */
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 1
TaskHandle_t stack_task_handle;
void start_task(void* pvParameters);

/* Task1任务配置 */
#define TASK1_STACK 128
#define TASK1_PRIORITY 2
TaskHandle_t task1_handle;
void task1(void* pvParameters);

/* Task2任务配置 */
#define TASK2_STACK 128
#define TASK2_PRIORITY 3
TaskHandle_t task2_handle;
void task2(void* pvParameters);

/* Task3任务配置 */
#define TASK3_STACK 128
#define TASK3_PRIORITY 4
TaskHandle_t task3_handle;
void task3(void* pvParameters);

/**
 *@brief 启动FreeRTOS
 *
 */
void freertos_start(void)
{
    /* 创建一个启动任务 */
    xTaskCreate((TaskFunction_t)start_task,
        (char*)"start_task",
        (configSTACK_DEPTH_TYPE)START_TASK_STACK,
        (void*)NULL,
        (UBaseType_t)START_TASK_PRIORITY,
        (TaskHandle_t*)&stack_task_handle);

    /* 启动调度器 */
    vTaskStartScheduler();
}


/**
 *@brief 启动任务：用于创建其他Task
 *
 * @param pvParameters
 */
void start_task(void* pvParameters)
{
    /* 进入临界区代码 保护当前任务不会被打断 */
    taskENTER_CRITICAL();
    /* 创建Task1 */
    xTaskCreate((TaskFunction_t)task1,
        (char*)"task1",
        (configSTACK_DEPTH_TYPE)TASK1_STACK,
        (void*)NULL,
        (UBaseType_t)TASK1_PRIORITY,
        (TaskHandle_t*)&task1_handle);

    /* 创建Task2 */
    xTaskCreate((TaskFunction_t)task2,
        (char*)"task2",
        (configSTACK_DEPTH_TYPE)TASK2_STACK,
        (void*)NULL,
        (UBaseType_t)TASK2_PRIORITY,
        (TaskHandle_t*)&task2_handle);

    /* 创建Task3 */
    xTaskCreate((TaskFunction_t)task3,
        (char*)"task3",
        (configSTACK_DEPTH_TYPE)TASK3_STACK,
        (void*)NULL,
        (UBaseType_t)TASK3_PRIORITY,
        (TaskHandle_t*)&task3_handle);

    /* 启动任务只需要用一次，用完删除自身 */
    vTaskDelete(NULL);

    /* 退出临界区代码 */
    taskEXIT_CRITICAL();
}

_Bool led1_flag;
/**
 *@brief LED1每500ms闪烁一次
 *
 * @param pvParameters
 */
void task1(void* pvParameters)
{
    while (1)
    {
        if (led1_flag==0)
        {
            printf("task1运行\n");
            LED_Toggle(LED1_Pin);
            vTaskDelay(500);
        }
        
    }

}

/**
 *@brief LED2每500ms闪烁一次
 *
 * @param pvParameters
 */
void task2(void* pvParameters)
{
    while (1)
    {
        printf("task2运行\n");
        LED_Toggle(LED2_Pin);
        vTaskDelay(500);
    }
}

/**
 *@brief 按键KEY1是否按下，按下则删掉task1
 *
 * @param pvParameters
 */
void task3(void* pvParameters)
{
    uint8_t key_num;
    while (1)
    {
        printf("task3运行\n");
        key_num = Key_Detect();
        if (key_num == KEY1_PRESS)
        {
            /* 判断是否已经删过，避免重复执行删除 */
                /* key1按下，删除task1 */
                printf("删除Task1\n");
                led1_flag = !led1_flag;
        }
        vTaskDelay(20);
    }
}
