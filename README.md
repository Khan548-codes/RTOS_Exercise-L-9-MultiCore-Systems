# RTOS_Exercise-L-9-MultiCore-Systems


🧐 Observations from the Four Multicore Tasks
Mode 1: Both Pinned to Core 0
Observation: You'll see a lot more printouts from Task H (the high-priority one) than from Task L.

Why: Since both tasks are fighting for the same CPU (Core 0), the scheduler follows the rules: the high-priority task (H) always gets to run first. It only gives time to Task L when Task H is busy or finishes its job.

Mode 2: L on Core 0, H on Core 1 (Separated)
Observation: You'll see Task H and Task L print out at almost the same, regular rate.

Why: This is the ideal scenario for speed. They aren't competing at all! Each task has its own dedicated core, so they run simultaneously (in parallel). The high priority of Task H doesn't matter much here because Core 1 is all its own.

Mode 3: L on Core 1, H on Core 0 (Separated)
Observation: Just like Mode 2, you'll see Task H and Task L print out at almost the same, regular rate.

Why: It's the same result as Mode 2, just swapping the cores. As long as tasks run on different cores, they run in parallel and don't slow each other down based on priority.

Mode 4: Both with No Affinity
Observation: The tasks will likely run similarly to Mode 2 or 3 (mostly in parallel). You'll see roughly equal printouts, but they might randomly jump between Core 0 and Core 1 (look at the Core ID number in the output).

Why: When you use "No Affinity," the scheduler is smart. Since it sees two tasks ready to run and two cores available, it will almost always spread them out to run in parallel across the cores, behaving like Mode 2 or 3. However, it could move them around if other system processes need a core, which is why the Core ID might change.


![images](https://github.com/Khan548-codes/RTOS_Exercise-L-9-MultiCore-Systems/blob/main/images/Core_0_0.png)
![images](https://github.com/Khan548-codes/RTOS_Exercise-L-9-MultiCore-Systems/blob/main/images/Core_0_1.png)
![images](https://github.com/Khan548-codes/RTOS_Exercise-L-9-MultiCore-Systems/blob/main/images/Core_pro_cpu_app_cpu.png)
![images](https://github.com/Khan548-codes/RTOS_Exercise-L-9-MultiCore-Systems/blob/main/images/Core_taskNo_affinityt.png)
