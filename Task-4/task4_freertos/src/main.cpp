#include <Arduino.h>
#include <Wire.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>

QueueHandle_t xQueue;

void TaskConsumer(void *pvParameters)
{
  char received[32];
  for (;;)
  {
    if (uxQueueMessagesWaiting(xQueue) > 0)
    {
      if (xQueueReceive(xQueue, &received, (TickType_t)10) == pdPASS)
      {
        Serial.println(received);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void TaskProducerA(void *pvParameters)
{
  char msg[] = "Task one is working";
  for (;;)
  {
    xQueueSend(xQueue, msg, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void TaskProducerB(void *pvParameters)
{
  char msg[] = "Task two is working";
  for (;;)
  {
    xQueueSend(xQueue, msg, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
void setup()
{

  Serial.begin(9600);
  xQueue = xQueueCreate(5, 32);

  xTaskCreate(TaskConsumer, "Consumer", 128, NULL, 2, NULL);
  xTaskCreate(TaskProducerA, "Producer", 128, NULL, 1, NULL);
  xTaskCreate(TaskProducerB, "Producer", 128, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop()
{
  // Empty. Things are done in Tasks.
}