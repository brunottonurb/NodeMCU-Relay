# NodeMCU Relay Module REST API

REST API for a __ESP8266 MCU__ with a __8 Relay Module__. Only tested with Amica NodeMCU.

1. Add a `conf.h` file with your WiFi credentials:
```C++
#define SSID "YourWifiSsid"
#define PW "YourPassword"
```
2. Compile and flash.

The API documentation is available [on Postman](https://documenter.getpostman.com/view/3506315/S1TYTvTU?version=latest).

The relay module is active low, this explains why `ON` leads to a `LOW` GPIO signal.

Connect the relay module `IN1` to `IN8` to `GPIO 5`, `4`, `0`, `14`, `12`, `2`, `10`, and `13`.