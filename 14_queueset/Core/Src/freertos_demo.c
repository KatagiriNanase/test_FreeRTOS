#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"
#include "LED.h"
#include "queue.h"
#include "semphr.h"
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
QueueHandle_t semaphr_handle;
QueueSetHandle_t queueset_handle;

/**
 *@brief 启动任务：用于创建其他Task
 *
 * @param pvParameters
 */
void start_task(void* pvParameters)
{
    /* 进入临界区代码 保护当前任务不会被打断 */
    taskENTER_CRITICAL();

    /* 创建队列1 */
    queue1_handle = xQueueCreate(1, sizeof(uint8_t));
    /* 创建信号量 */
    semaphr_handle = xSemaphoreCreateBinary();      /*>>>---注意：创建二值信号量要保证队列为空---<<<*/
    /* 创建队列集 */
    queueset_handle = xQueueCreateSet(1 + 1);
    /* 添加队列到队列集中 */
    if (xQueueAddToSet(queue1_handle, queueset_handle) == pdPASS)   printf("queue1添加到队列集\r\n");
    if (xQueueAddToSet(semaphr_handle, queueset_handle) == pdPASS)   printf("二值信号量添加到队列集\r\n");

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

_Bool led1_flag;
/**
 *@brief 当KEY1按下，往队列写入数据，当KEY2按下，释放二值信号量。
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
            xQueueSend(queue1_handle, &key_num, portMAX_DELAY);
        }
        else if (key_num == KEY2_PRESS)
        {
            xSemaphoreGive(semaphr_handle);
        }

        vTaskDelay(20);
    }

}

/**
 *@brief 读取队列集中的消息，并打印
 *
 * @param pvParameters
 */
void task2(void* pvParameters)
{
    uint8_t key;
		QueueSetMemberHandle_t member_handle;
    while (1)
    {
				 member_handle= xQueueSelectFromSet(queueset_handle, portMAX_DELAY);
        if (semaphr_handle == member_handle)
        {
            if (xSemaphoreTake(semaphr_handle, portMAX_DELAY) == pdTRUE)
                printf("成功获取信号量\r\n");
        }
        else if (queue1_handle == member_handle)
        {
            xQueueReceive(queue1_handle, &key, portMAX_DELAY);
            printf("成功获取键值：%d\r\n", key);
        }
        //vTaskDelay(500);
    }
}
