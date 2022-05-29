#include <stdio.h>
#include "driver/gpio.h"
#include "driver/can.h"

//can_message_t message_config (int *mensagem);

can_message_t message_config (int index, int *mensagem){
  //Configure message to transmit
  can_message_t message;
  message.identifier = 0x1;
  message.flags = CAN_MSG_FLAG_EXTD;
  message.data_length_code = 7;
  for (int i = 0; i < index; i++) {
      message.data[i] = mensagem[i];  //DESCOBRIR QUAL A ESTRUTURA DO .data ---> BIT A BIT OU BYTE A BYTE
                                      //NOTAS:
                                      //1. .data recebe valor em binário, printa valor inteiro convertido
                                      //2. precisa fazer um protótipo de função por causa do ponteiro?
                                      //3. precisa usar ponteiro?
                                      //4. onde achar o header can.h? -> achar o tamanho do .data
                                      //5. como colocar valor do sinal na mensagem?
                                      //6. valor de i no for funciona até 34, acima disso apresenta GURU MEDITATION ERROR
      printf("mensagem: %i\t", mensagem[i]);
      printf("message.data: %i\n", message.data[i]);
  }

  return message;
}

void trans_queue (can_message_t message){
  //Queue message for transmission
  if (can_transmit(&message, pdMS_TO_TICKS(10)) == ESP_OK) {
      printf("Message queued for transmission\n");
  } else {
      printf("Failed to queue message for transmission\n");
  }
}

int Pt_Edrv_Des1_byte[7] = {11111111,00000000,11111111,00000000,11111111,00000000,11110111};
int Pt_Edrv_Des1_bit[56] = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,01,1,1,1,1,1,1,1};

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

  can_message_t mensagem_config = message_config(34, &Pt_Edrv_Des1_bit);
  trans_queue(mensagem_config);

}