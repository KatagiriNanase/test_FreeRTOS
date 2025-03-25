#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"
#include "LED.h"
#include <stdio.h>
#include "semphr.h"

/* 启动任务配置 */
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 1
TaskHandle_t stack_task_handle;
void start_task(void* pvParameters);

/* Task1任务配置 */
#define TASK1_STACK 128
#define TASK1_PRIORITY 3
TaskHandle_t task1_handle;
void task1(void* pvParameters);

/* Task2任务配置 */
#define TASK2_STACK 128
#define TASK2_PRIORITY 2
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

QueueHandle_t semaphr_handle;
/**
 *@brief 启动任务：用于创建其他Task
 *
 * @param pvParameters
 */
void start_task(void* pvParameters)
{
    /* 进入临界区代码 保护当前任务不会被打断 */
    taskENTER_CRITICAL();

    /* 创建二值信号量 */
    vSemaphoreCreateBinary(semaphr_handle);
    if (semaphr_handle != NULL) printf("成功创建信号量\r\n");
    else                        printf("信号量创建失败！\r\n");

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


/**
 *@brief 用于按键扫描，当检测到按键KEY1被按下时，释放二值信号量。
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
            if (xSemaphoreGive(semaphr_handle) == pdTRUE)  printf("释放成功!\n");
            else                                           printf("释放失败！\n");
        }
        vTaskDelay(50);
    }

}

/**
 *@brief 获取二值信号量，当成功获取后打印提示信息。
 *
 * @param pvParameters
 */
void task2(void* pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(semaphr_handle, portMAX_DELAY) == pdTRUE)  printf("成功获取信号量！\n");
        vTaskDelay(200);
    }
}
