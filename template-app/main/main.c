#include <stdio.h>
#include "driver/gpio.h"
#include "driver/can.h"

int mensagem[10] = {0,1,1,0,0,1,1,1,0,1};

void app_main()
{ 
  can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(
      GPIO_NUM_5, GPIO_NUM_4, CAN_MODE_NORMAL);
  can_timing_config_t t_config = CAN_TIMING_CONFIG_500KBITS();
  can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();

  //Install CAN driver
  if (can_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
      printf("Driver installed\n");
  } else {
      printf("Failed to install driver\n");
      return;
  }

  //Start CAN driver
  if (can_start() == ESP_OK) {
      printf("Driver started\n");
  } else {
      printf("Failed to start driver\n");
      return;
  }

  //Configure message to transmit
  can_message_t message;
  message.identifier = 0x01;
  message.flags = CAN_MSG_FLAG_EXTD;
  message.data_length_code = 4;
  for (int i = 0; i < 10; i++) {
      message.data[i] = mensagem[i];
      printf("%i\n", message.data[i]);
  }

  //Queue message for transmission
  if (can_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
      printf("Message queued for transmission\n");
  } else {
      printf("Failed to queue message for transmission\n");
  }
}