### ESP8266 Module with STM32 F4 Discovery

This is a sample application using STM32F4 Discovery board connect with ESP-12-Q module
using IAR workbench

#### Pin Connection

| Pin  | State |
| -----| ------------- |
| VCC  | 3.3 V  |
| GND  | 0 V  |
| EN  | High  |
| GPIO 0  | High  |
| GPIO 15  | Gnd  |

### STM32F4 Pin Connection

The board is connected using UART 2

| Pin  | State |
| ------- | ------------- |
| GPIOA_2 | ESP8266_RX |
| GPIOA_3 | ESP8266_TX  |

#### Reference

 - [ESP8266 Wiki](http://www.esp8266.com/wiki/doku.php)
