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
#define TASK2_STACK 256
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

    /* 启动任务只需要用一次，用完删除自身 */
    vTaskDelete(NULL);

    /* 退出临界区代码 */
    taskEXIT_CRITICAL();
}

/**
 *@brief LED1每500ms闪烁一次
 *
 * @param pvParameters
 */
void task1(void* pvParameters)
{
    while (1)
    {
        printf("task1运行\n");
        LED_Toggle(LED1_Pin);
        vTaskDelay(500);
        //HAL_Delay(500);
    }

}

/**
 *@brief LED2每500ms闪烁一次
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

    /* 获取task1任务优先级 */
    task_priority = uxTaskPriorityGet(task1_handle);
    printf("task1 priority:%d\r\n", task_priority);
    /* 设置task1任务优先级 */
    vTaskPrioritySet(task1_handle, 3);
    task_priority = uxTaskPriorityGet(task1_handle);
    printf("task1 priority:%d\r\n", task_priority);

    /* 获取任务数量 */
    task_num = uxTaskGetNumberOfTasks();
    printf("Num of sysTask:%d\r\n", task_num);

    /* 获取所有任务状态信息 */
    uxTaskGetSystemState(task_state, task_num, NULL);
    printf("任务名\t\t任务编号\t\t当前状态\t\t当前优先级\t\t原始优先级\t\n");
    for (uint8_t i = 0;i < task_num;++i)
    {
        printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\r\n",
            task_state[i].pcTaskName,
            task_state[i].xTaskNumber,
            task_state[i].eCurrentState,
            task_state[i].uxCurrentPriority,
            task_state[i].uxBasePriority);
    }

    /* 获取当前任务句柄 */
    task_handle = xTaskGetCurrentTaskHandle();
    printf("current handle:%p\n", task_handle);

    /* 通过任务名获取指定任务句柄 */
    task_handle = xTaskGetHandle("task1");
    printf("task1 handle:%p\r\n", task_handle);

    /* 获取任务栈历史最小值 */
    task_stack_remain_min = uxTaskGetStackHighWaterMark(task2_handle);
    printf("任务栈历史最小值:%d\n", task_stack_remain_min);

    while (1)
    {
        vTaskDelay(500);
        //HAL_Delay(500);
    }
    

}
