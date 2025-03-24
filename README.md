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




