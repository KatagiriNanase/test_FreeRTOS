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
StackType_t start_task_stack[START_TASK_STACK];     //静态任务栈，以数组形式存储
StaticTask_t stack_task_tcb;            //静态任务tcb结构体
void start_task(void* pvParameters);

/* Task1任务配置 */
#define TASK1_STACK 128
#define TASK1_PRIORITY 2
TaskHandle_t task1_handle;
StackType_t task1_stack[TASK1_STACK];     //静态任务栈，以数组形式存储
StaticTask_t task1_tcb;
void task1(void* pvParameters);

/* Task2任务配置 */
#define TASK2_STACK 128
#define TASK2_PRIORITY 3
TaskHandle_t task2_handle;
StackType_t task2_stack[TASK2_STACK];     //静态任务栈，以数组形式存储
StaticTask_t task2_tcb;
void task2(void* pvParameters);

/* Task3任务配置 */
#define TASK3_STACK 128
#define TASK3_PRIORITY 4
TaskHandle_t task3_handle;
StackType_t task3_stack[TASK3_STACK];     //静态任务栈，以数组形式存储
StaticTask_t task3_tcb;
void task3(void* pvParameters);

/* ==============静态创建任务，需要手动指定两个特殊任务的资源 ==============*/
/* 分配空闲任务的资源 */
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

/* 分配软件定时器任务的资源 */
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
 *@brief 启动FreeRTOS
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
    task1_handle=xTaskCreateStatic((TaskFunction_t)task1,
        (char*)"task1",
        (uint32_t)TASK1_STACK,
        (void*)NULL,
        (UBaseType_t)TASK1_PRIORITY,
        (StackType_t*)task1_stack,
        (StaticTask_t*)&task1_tcb);

    /* 创建Task2 */
    task2_handle = xTaskCreateStatic((TaskFunction_t)task2,
        (char*)"task2",
        (uint32_t)TASK2_STACK,
        (void*)NULL,
        (UBaseType_t)TASK2_PRIORITY,
        (StackType_t*)task2_stack,
        (StaticTask_t*)&task2_tcb);

    /* 创建Task3 */
    task3_handle = xTaskCreateStatic((TaskFunction_t)task3,
        (char*)"task3",
        (uint32_t)TASK3_STACK,
        (void*)NULL,
        (UBaseType_t)TASK3_PRIORITY,
        (StackType_t*)task3_stack,
        (StaticTask_t*)&task3_tcb);

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
        if (led1_flag == 0)
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
