# 🌸 FreeRTOS学习日志 

✨ *"呐~今天也要元气满满地写代码哦！"* ✨

---

## 🚀 今日进展
成功在STM32上完成：
- 🧵 **FreeRTOS移植**（现在可以愉快地多线程啦~）
- 🎮 **任务创建**  
  - 动态创建 `xTaskCreate()` (*像泡泡一样随时出现*)  
  - 静态创建 `xTaskCreateStatic()` (*像乐高积木一样稳固*)
- ⏸️ **任务管理**  
  - 用`vTaskSuspend()`让任务乖乖睡觉  
  - 用`vTaskResume()`唤醒装睡的任务们~   

>"代码再不打完的话...就...就哭给你看哦！(๑•́ ₃ •̀๑)✧"
>—— 在背后催促的VS Code姬

2025年3月22日

---
## 🌸 FreeRTOS研修日记2

✨ *"今天的代码也要元气满满地运行哦~ (๑>ᴗ<๑)✧"* ✨

---

### 🚀 今日技能解锁
### ⏳ **时间片轮询调度**
- 学会了用`configUSE_TIME_SLICING = 1`开启甜蜜分时～  
- 给任务们平均分配🍰时间片（默认同优先级自动轮转）
```c
// 像分蛋糕一样配置任务优先级
xTaskCreate(task1, "T1", 128, NULL, 2, NULL);  // 🎂优先级2
xTaskCreate(task2, "T2", 128, NULL, 2, NULL);  // 🧁同优先级自动轮换
```
### 📊 任务状态侦查术
- 召唤uxTaskGetSystemState()获取全员状态快照
- 使用vTaskList()生成ASCII艺术报表   

### 🐾 踩到BUG小强
```c
// (╯‵□′)╯ 当忘记给vTaskList分配足够内存时...
char bugReport[10]; // 太小啦！
vTaskList(bugReport);  // 💥 内存溢出→系统扑街！
```   
### 解决之道：
- 1️⃣ 先用uxTaskGetNumberOfTasks()检测任务数量
- 2️⃣ 动态分配缓冲区：char *buf = pvPortMalloc(需要的长度);   
   

   
2025年3月23日   

---    

## 🌸 FreeRTOS时间管理特训

✨ *"任务执行时间就像少女的年龄～要好好记录下来哦！(๑>ᴗ<๑)♡"* ✨



### 🚀 今日技能点满
### ⏱️ **任务执行时间快照**

```c
// 在FreeRTOSConfig.h召唤时间精灵
#define configGENERATE_RUN_TIME_STATS  1  // 🧚♀️ 开启时间统计魔法
#define configUSE_STATS_FORMATTING_FUNCTIONS 1  // 📊 解锁报表格式转换

// 在stm32中配置时间统计专用定时器（如TIM2）
void ConfigureTimeStatsTimer() {
    __HAL_RCC_TIM2_CLK_ENABLE();
    TIM2->PSC = SystemCoreClock/1000000 -1;  // 1us精度✨
    TIM2->ARR = 0xFFFFFFFF;  // 32位自由奔跑模式
    HAL_TIM_Base_Start(&htim2);
}
```
### ⏰ 延时魔法对比表   
| 魔法类型 | 咒语  | 特性  | 使用场景 |
| :---    | :--: | :--: | ---:   |
| 相对延时 | vTaskDelay()  | 从当前开始计算  | 简单延时 |
| 绝对延时 | xTaskDelayUntil()  | 保持固定周期(防累积误差)  | 心跳任务/精确周期 |   
```c
// 绝对延时の正确姿势
TickType_t xLastWakeTime = xTaskGetTickCount();
while(1) {
    LED_Toggle();
    xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));  // 精确100ms周期✨
}

```   
### 🐾 掉进时间漩涡
```c
// (╯°□°）╯ 当忘记启动统计定时器时...
void vTaskGetRunTimeStats(char *pcBuffer);  
// 输出：全任务运行时间都是0%！💢

// 绝对延时的危险操作
xLastWakeTime += pdMS_TO_TICKS(100);  // 手动修改基准时间→时间线崩坏！

```   
**解决方案：**

- 用CubeMX配置专用定时器并自动启动

- 永远让xTaskDelayUntil()自己管理基准时间～   

### 📊 时间统计报表范例
```text
任务名  状态   优先级  剩余栈  运行时间(%)  
LED     R     1       88      12.3%  
UART    B     2       120     45.6%  
IDLE    R     0       64      42.1%   👻
```
>*IDLE君又在偷偷摸鱼啦～）*   

>"绝对延时就像约会时间～迟到的话会被其他任务NTR的哦！(๑•́ ₃ •̀๑)"
>—— 看着调度器的VS Code姬      


2025年3月24日   

--- 


## 🌸 FreeRTOS通信与同步修行 

✨ *"队列是任务间的小信箱，信号量是资源的小门卫～要好好相处哦！(๑>ᴗ<๑)♡"* ✨


### 🚀 今日技能觉醒
### 📮 **队列（Queue）的奇妙邮局**
```c
// 创建能存放10个整数的邮箱
QueueHandle_t xMailbox = xQueueCreate(10, sizeof(int32_t));

// 任务A寄信（非阻塞发送）
int32_t loveLetter = 520;
xQueueSend(xMailbox, &loveLetter, 0);  // ✉️ 立刻投递！

// 任务B收信（最多等100ms）
int32_t receivedData;
if(xQueueReceive(xMailbox, &receivedData, pdMS_TO_TICKS(100)) {
    printf("收到表白：%d\n", receivedData);  // 💌
}
```

### 🔐 **信号量家族对比表**
| 类型                | 魔法咒语                  | 应用场景                      | 特点                     |
|---------------------|--------------------------|-----------------------------|--------------------------|
| **二进制信号量**     | xSemaphoreCreateBinary() | 任务同步/互斥访问            | 只有0/1两种状态          |
| **计数型信号量**     | xSemaphoreCreateCounting(5,0)| 资源池管理（如内存块）       | 可记录可用资源数量       |

```c
// 二进制信号量（像厕所钥匙🔑）
SemaphoreHandle_t xBathroomKey = xSemaphoreCreateBinary();
xSemaphoreGive(xBathroomKey);  // 初始状态设为可用

// 任务使用厕所（获取信号量）
if(xSemaphoreTake(xBathroomKey, portMAX_DELAY)) {
    printf("🚽 正在使用厕所...\n");
    xSemaphoreGive(xBathroomKey);  // 记得还钥匙！
}

// 计数型信号量（如停车场🅿️）
SemaphoreHandle_t xParkingLot = xSemaphoreCreateCounting(5,5);  // 5个车位
xSemaphoreTake(xParkingLot, 0);  // 占用1个车位
```

---

### 🐾 掉进同步陷阱
```c
// (╯°□°）╯ 忘记释放信号量导致死锁！
xSemaphoreTake(xBathroomKey, portMAX_DELAY);  
printf("用完厕所但忘记还钥匙！\n");  
// 其他任务永远等不到厕所啦！💥

// 队列溢出惨案
int32_t bigData[100];
xQueueSend(xMailbox, bigData, 0);  // 💣 发送的数据超过队列单元大小！
```
**避坑指南**：  
1. 用`uxQueueSpacesAvailable()`检查队列剩余空间  
2. 将信号量操作放入`taskENTER_CRITICAL()`保护区域  
3. 使用Static版本API防止内存泄漏  



2025年3月25日   

---

## 🌸 FreeRTOS高级同步术 

✨ *"互斥锁是资源的VIP通道，事件组是任务间的摩斯密码～要优雅地使用哦！(๑>ᴗ<๑)♡"* ✨

---

### 🚀 今日技能觉醒
### 🔒 **互斥锁（Mutex）的贵族礼仪**
```c
// 创建高贵的互斥锁（带优先级继承！）
SemaphoreHandle_t xVIP通道 = xSemaphoreCreateMutex();

// 任务访问共享资源
if(xSemaphoreTake(xVIP通道, pdMS_TO_TICKS(100))) {
    printf("🎩 进入VIP区操作SD卡...\n");
    xSemaphoreGive(xVIP通道);  // 优雅归还
} else {
    printf("⏳ 等待超时，礼貌离开～\n");
}
```

### 📭 **队列集（QueueSet）的多路监听**
```c
// 创建能监控3个队列的监听站
QueueSetHandle_t x监视站 = xQueueCreateSet(3);

// 将队列和信号量加入监听
xQueueAddToSet(xUART队列, x监视站);
xQueueAddToSet(x传感器信号量, x监视站);

// 等待任意事件触发
QueueSetMemberHandle_t x激活成员 = xQueueSelectFromSet(x监视站, portMAX_DELAY);
if(x激活成员 == xUART队列) {
    // 处理UART数据
}
```

### 🚩 **事件标志组的位操作魔法**
```c
// 创建包含8个标志位的事件组
EventGroupHandle_t x摩斯密码本 = xEventGroupCreate();

// 任务A发送事件（置位BIT0和BIT1）
xEventGroupSetBits(x摩斯密码本, (1<<0)|(1<<1));  // ✨ 发送信号

// 任务B等待特定组合
EventBits_t ux收到信号 = xEventGroupWaitBits(
    x摩斯密码本, 
    (1<<0)|(1<<1),  // 要等待的位
    pdTRUE,         // 等待后清除这些位
    pdTRUE,         // 要求所有位同时满足
    portMAX_DELAY
);
```

---

### 🐾 同步灾难现场
```c
// (╯°□°）╯ 互斥锁嵌套导致死锁！
xSemaphoreTake(xMutex1, portMAX_DELAY);
xSemaphoreTake(xMutex2, portMAX_DELAY);  // 💀 其他任务可能以相反顺序获取

// 事件标志位溢出悲剧
xEventGroupSetBits(x组, 0xFF<<24);  // 超过24位！某些平台会截断💥
```
**避坑指南**：  
1. 使用`xSemaphoreGetMutexHolder()`检测死锁  
2. 事件标志组用`configUSE_16_BIT_TICKS`控制位数  
3. 队列集监听成员不要超过`configQUEUE_SET_LENGTH`  
    

    
2025年3月26日

---




