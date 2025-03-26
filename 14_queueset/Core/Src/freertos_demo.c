#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"
#include "LED.h"
#include "queue.h"
#include "semphr.h"
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
#define TASK2_STACK 128
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

QueueHandle_t queue1_handle;
QueueHandle_t semaphr_handle;
QueueSetHandle_t queueset_handle;

/**
 *@brief �����������ڴ�������Task
 *
 * @param pvParameters
 */
void start_task(void* pvParameters)
{
    /* �����ٽ������� ������ǰ���񲻻ᱻ��� */
    taskENTER_CRITICAL();

    /* ��������1 */
    queue1_handle = xQueueCreate(1, sizeof(uint8_t));
    /* �����ź��� */
    semaphr_handle = xSemaphoreCreateBinary();      /*>>>---ע�⣺������ֵ�ź���Ҫ��֤����Ϊ��---<<<*/
    /* �������м� */
    queueset_handle = xQueueCreateSet(1 + 1);
    /* ��Ӷ��е����м��� */
    if (xQueueAddToSet(queue1_handle, queueset_handle) == pdPASS)   printf("queue1��ӵ����м�\r\n");
    if (xQueueAddToSet(semaphr_handle, queueset_handle) == pdPASS)   printf("��ֵ�ź�����ӵ����м�\r\n");

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

_Bool led1_flag;
/**
 *@brief ��KEY1���£�������д�����ݣ���KEY2���£��ͷŶ�ֵ�ź�����
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
 *@brief ��ȡ���м��е���Ϣ������ӡ
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
                printf("�ɹ���ȡ�ź���\r\n");
        }
        else if (queue1_handle == member_handle)
        {
            xQueueReceive(queue1_handle, &key, portMAX_DELAY);
            printf("�ɹ���ȡ��ֵ��%d\r\n", key);
        }
        //vTaskDelay(500);
    }
}
