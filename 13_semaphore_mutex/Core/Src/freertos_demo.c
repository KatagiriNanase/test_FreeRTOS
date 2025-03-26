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
#define TASK1_PRIORITY 1
TaskHandle_t task1_handle;
void task1(void* pvParameters);

/* Task2任务配置 */
#define TASK2_STACK 128
#define TASK2_PRIORITY 2
TaskHandle_t task2_handle;
void task2(void* pvParameters);

/* Task3任务配置 */
#define TASK3_STACK 128
#define TASK3_PRIORITY 3
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
/* 互斥锁句柄 */
QueueHandle_t semaphr_mutex_handle;
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
    semaphr_mutex_handle = xSemaphoreCreateMutex();
    if (semaphr_mutex_handle != NULL) printf("成功创建互斥锁\r\n");
    else                        printf("互斥锁创建失败！\r\n");
    /* 释放互斥锁 */
    xSemaphoreGive(semaphr_mutex_handle);

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


/**
 *@brief 低优先级任务
 *
 * @param pvParameters
 */
void task1(void* pvParameters)
{
    
    while (1)
    {
        printf("低优先级Task1获取信号量\r\n");
        xQueueSemaphoreTake(semaphr_mutex_handle, portMAX_DELAY);
        printf("低优先级Task1正在运行\r\n");
        HAL_Delay(3000);
        printf("低优先级Task1释放信号量\r\n");
        xSemaphoreGive(semaphr_mutex_handle);
        vTaskDelay(1000);
    }

}

/**
 *@brief 中优先级任务
 *
 * @param pvParameters
 */
void task2(void* pvParameters)
{
    while (1)
    {
        printf("中优先级Task2正在执行\r\n");
        HAL_Delay(1500);
        printf("Task2 for 循环完毕\r\n");
        vTaskDelay(1000);
    }
}


/**
 * @brief 高优先级任务
 *
 * @param pvParameters 
 */
void task3(void* pvParameters)
{
    while (1)
    {
        printf("高优先级Task3获取信号量\r\n");
        xSemaphoreTake(semaphr_mutex_handle, portMAX_DELAY);
        printf("高优先Task3正在运行\r\n");
        HAL_Delay(1000);
        printf("高优先级Task3释放信号量\r\n");
        xSemaphoreGive(semaphr_mutex_handle);
        vTaskDelay(1000);
    }
    
}
