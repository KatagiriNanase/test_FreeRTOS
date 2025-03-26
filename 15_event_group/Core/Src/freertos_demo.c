#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"
#include "LED.h"
#include <stdio.h>
#include "event_groups.h"

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

EventGroupHandle_t event_group_handle;
/**
 *@brief 启动任务：用于创建其他Task
 *
 * @param pvParameters
 */
void start_task(void* pvParameters)
{
    /* 进入临界区代码 保护当前任务不会被打断 */
    taskENTER_CRITICAL();

    event_group_handle = xEventGroupCreate();

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

    /* 启动任务只需要用一次，用完删除自身 */
    vTaskDelete(NULL);

    /* 退出临界区代码 */
    taskEXIT_CRITICAL();
}

#define  EVENT_BIT_0   (0x01<<0)
#define  EVENT_BIT_1   (0x00<<1)
/**
 *@brief 读取按键按下键值，根据不同键值将事件标志组相应事件位置一，模拟事件发生。
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
            /* 将事件组Bit0置位 */
            xEventGroupSetBits(event_group_handle, EVENT_BIT_0);
            printf("Bit0 置位\r\n");
        }
        else if (key_num == KEY2_PRESS)
        {
            /* 将事件组Bit1置位 */
            xEventGroupSetBits(event_group_handle, EVENT_BIT_1);
            printf("Bit1 置位\r\n");
        }
    }

}


/**
 *@brief 同时等待事件标志组中的多个事件位，当这些事件位都置 1 的话就执行相应的处理。
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
        printf("等待到的时间标志位：%#x\r\n", res);
    }
}
