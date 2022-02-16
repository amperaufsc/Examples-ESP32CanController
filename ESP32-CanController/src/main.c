#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/can.h"
#define CAN_RX_PIN GPIO_NUM_17
#define CAN_TX_PIN GPIO_NUM_16
void setupCAN();

void app_main() {
    setupCAN(); // setup CAN bus
    while (1){
        // SEND CAN MESSAGE
        can_message_t message;
        message.identifier = 0x626;         // CAN message identifier
        message.flags = CAN_MSG_FLAG_EXTD;  // CAN message flags
        message.data_length_code = 8;       // CAN message data length
        // CAN DATA
        message.data[0] = 0x22;
        message.data[1] = 0x04;
        message.data[2] = 0x31;
        message.data[3] = 0x00;
        message.data[4] = 0x00;
        message.data[5] = 0x00;
        message.data[6] = 0x00;
        message.data[7] = 0x00;
        // SEND CAN MESSAGE
        can_transmit(&message, pdMS_TO_TICKS(1000));

        // RECIVE CAN MESSAGE
        //Wait for message to be received
        can_message_t message2;
        if (can_receive(&message2, pdMS_TO_TICKS(100)) == ESP_OK)
        {
            printf("message2 received! ");
        } 
        else
        {
            printf("Failed to receive message2\n");
        }

        //Process received message2
        printf("ID is %x\n", message2.identifier);
        for (int i = 0; i < message2.data_length_code; i++)
        {
        printf("0x%02X ", message2.data[i]);
        }
        printf("\n");
    }
    
}


void setupCAN(){
  can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(CAN_TX_PIN, CAN_RX_PIN, CAN_MODE_NORMAL);
  can_timing_config_t t_config = CAN_TIMING_CONFIG_125KBITS();
  can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();

  if(can_driver_install(&g_config, &t_config, &f_config) == ESP_OK){
    // This installs the CAN driver
    printf("CAN driver installed\n");
  }
  if (can_start() == ESP_OK) {
    // This starts the CAN driver
    printf("CAN driver started\n");
  }
  
}