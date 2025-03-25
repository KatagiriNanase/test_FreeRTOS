#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"
#include "LED.h"
#include "queue.h"
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

QueueHandle_t queue1_handle;
QueueHandle_t bigQueue_handle;
const char* test_info = "hello world!\n";
/**
 *@brief 启动任务：用于创建其他Task
 *
 * @param pvParameters
 */
void start_task(void* pvParameters)
{
    /* 进入临界区代码 保护当前任务不会被打断 */
    taskENTER_CRITICAL();

    /* 创建队列1 存放key值 */
    queue1_handle = xQueueCreate(1, sizeof(uint8_t));
    /* 创建队列2 存放字符串指针 */
    bigQueue_handle = xQueueCreate(1, sizeof(char*));

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
        TASK3_STACK,
        (void*)NULL,
        TASK3_PRIORITY,
        (TaskHandle_t*)task3_handle);

    /* 启动任务只需要用一次，用完删除自身 */
    vTaskDelete(NULL);

    /* 退出临界区代码 */
    taskEXIT_CRITICAL();
}

_Bool led1_flag;
/**
 *@brief 当按键key1或key2按下，将键值拷贝到队列queue1（入队）；
 *当按键key3按下，将传输大数据，这里拷贝大数据的地址到队列big_queue中。
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
 *@brief 读取队列queue1中的消息（出队），打印出接收到的键值。
 *
 * @param pvParameters
 */
void task2(void* pvParameters)
{
    uint8_t rec;
    while (1)
    {
        if (xQueueReceive(queue1_handle, &rec, portMAX_DELAY))   printf("task2接收成功！\n");
        printf("rec:%d\r\n", rec);
    }
}

/**
 *@brief 从队列big_queue读取大数据地址，通过地址访问大数据
 *
 * @param pvParameters
 */
void task3(void* pvParameters)
{
    char* infoBuff;
    while (1)
    {
        if (xQueueReceive(bigQueue_handle, infoBuff, portMAX_DELAY))   printf("task3接收成功！\n");
        printf("rec:%s\r\n", infoBuff);
    }

}
