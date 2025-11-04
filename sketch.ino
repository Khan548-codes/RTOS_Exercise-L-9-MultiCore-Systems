/**
 * ESP32 Multicore Task Pinning Master Demo
 *
 * Combines four scenarios for pinning Task L (Priority 1) and Task H (Priority 2):
 * 1. Both on Core 0
 * 2. Task L on Core 0, Task H on Core 1
 * 3. Task L on Core 1, Task H on Core 0
 * 4. Both with No Affinity (Scheduler chooses)
 *
 * Author: Based on code by Shawn Hymel (2021)
 */

// -------------------------------------------------------------------
// 🔥 MASTER SETTING: CHANGE THIS VALUE (1-4)
// -------------------------------------------------------------------
const int core_pin_mode = 2; 
// 1: Both on Core 0
// 2: L on Core 0, H on Core 1 (Recommended for heavy loads)
// 3: L on Core 1, H on Core 0
// 4: Both No Affinity (Scheduler decides)
// -------------------------------------------------------------------

// Core definitions
static const BaseType_t pro_cpu = 0;
static const BaseType_t app_cpu = 1;

// Settings
static const TickType_t time_hog = 200;  // Time (ms) for the busy-wait loop

//*****************************************************************************
// Functions

// Hogs the processor (Simulates CPU-intensive work without yielding)
static void hog_delay(uint32_t ms) {
  for (uint32_t i = 0; i < ms; i++) {
    for (uint32_t j = 0; j < 40000; j++) {
      asm("nop");
    }
  }
}

//*****************************************************************************
// Tasks

// Task L (Low Priority: 1)
void doTaskL(void *parameters) {
  char str[30]; 
  while (1) {
    sprintf(str, "Task L (Pri 1), Core %i\r\n", xPortGetCoreID());
    Serial.print(str);
    // Note: A real task should use vTaskDelay() to yield the CPU
    hog_delay(time_hog);
  }
}

// Task H (High Priority: 2)
void doTaskH(void *parameters) {
  char str[30]; 
  while (1) {
    sprintf(str, "Task H (Pri 2), Core %i\r\n", xPortGetCoreID());
    Serial.print(str);
    // Note: A real task should use vTaskDelay() to yield the CPU
    hog_delay(time_hog);
  }
}

//*****************************************************************************
// Main (runs as its own task on Core 1 by default)

void setup() {
  
  // Configure Serial
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---ESP32 Dual-Core Pinning Demo---"); 

  BaseType_t coreL, coreH;
  
  // Logic to set core affinity based on the MASTER SETTING
  switch (core_pin_mode) {
    case 1:
      // Scenario 1: Both on Core 0
      coreL = pro_cpu;
      coreH = pro_cpu;
      Serial.println("Mode 1: Both Tasks Pinned to CORE 0.");
      break;
    case 2:
      // Scenario 2: L on Core 0, H on Core 1
      coreL = pro_cpu;
      coreH = app_cpu;
      Serial.println("Mode 2: Task L on CORE 0, Task H on CORE 1.");
      break;
    case 3:
      // Scenario 3: L on Core 1, H on Core 0
      coreL = app_cpu;
      coreH = pro_cpu;
      Serial.println("Mode 3: Task L on CORE 1, Task H on CORE 0.");
      break;
    case 4:
    default:
      // Scenario 4: Both No Affinity
      coreL = tskNO_AFFINITY;
      coreH = tskNO_AFFINITY;
      Serial.println("Mode 4: Both Tasks No Affinity (Scheduler Chooses).");
      break;
  }
  
  // Create Task L (Priority 1)
  xTaskCreatePinnedToCore(doTaskL,
                          "Task L",
                          2048,
                          NULL,
                          1,
                          NULL,
                          coreL);  // Core set by the switch statement
                        
  // Create Task H (Priority 2)
  xTaskCreatePinnedToCore(doTaskH,
                          "Task H",
                          2048,
                          NULL,
                          2,
                          NULL,
                          coreH);  // Core set by the switch statement
                          
  // Delete the Arduino "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}