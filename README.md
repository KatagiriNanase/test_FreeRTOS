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