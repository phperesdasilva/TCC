#include <stdio.h>
#include "driver/gpio.h"
#include "driver/can.h"

//can_message_t message_config (int *mensagem);

can_message_t message_config (int *mensagem){
  //Configure message to transmit
  can_message_t message;
  message.identifier = 0x1;
  message.flags = CAN_MSG_FLAG_EXTD;
  message.data_length_code = 7;
  for (int i = 0; i < 7; i++) {
      message.data[i] = mensagem[i];  //DESCOBRIR QUAL A ESTRUTURA DO .data ---> BIT A BIT OU BYTE A BYTE | R: byte a byte
                                      //NOTAS:
                                      //1. .data recebe valor em binário, printa valor inteiro convertido
                                      //2. precisa fazer um protótipo de função por causa do ponteiro?
                                      //3. precisa usar ponteiro?
                                      //4. onde achar o header can.h? -> achar o tamanho do .data | R: data tem tamanho máximo 8
                                      //5. como colocar valor do sinal na mensagem?
                                      //6. valor de i no for funciona até 34, acima disso apresenta GURU MEDITATION ERROR | R: precisa apenas de 8
                                      //7. qual é o valor que deve ser colocado no .data?
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

int Pt_Edrv_Des1[7] = {{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1}};

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

  can_message_t mensagem_config = message_config(&Pt_Edrv_Des1);
  trans_queue(mensagem_config);

}