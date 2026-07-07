#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/*
 * ============================================================
 *                      Project
 * ============================================================
 */

#define PROJECT_NAME        "ESP32 OBD Dashboard"
#define PROJECT_VERSION     "0.1.0"

/*
 * ============================================================
 *                      WiFi
 * ============================================================
 */

constexpr char AP_SSID[] = "ESP32-OBD";
constexpr char AP_PASS[] = "12345678";

constexpr IPAddress AP_IP(192, 168, 4, 1);
constexpr IPAddress AP_GATEWAY(192, 168, 4, 1);
constexpr IPAddress AP_SUBNET(255, 255, 255, 0);

/*
 * ============================================================
 *                  HTTP / WebSocket
 * ============================================================
 */

constexpr uint16_t HTTP_PORT = 80;
constexpr uint16_t WS_PORT   = 81;

/*
 * ============================================================
 *                  CAN (TWAI)
 * ============================================================
 */

constexpr gpio_num_t CAN_TX_PIN = GPIO_NUM_5;
constexpr gpio_num_t CAN_RX_PIN = GPIO_NUM_4;

constexpr uint32_t CAN_SPEED = 500000;

/*
 * ============================================================
 *                  OBD Polling
 * ============================================================
 */

constexpr uint16_t RPM_INTERVAL_MS      = 40;   // 25 Гц
constexpr uint16_t THROTTLE_INTERVAL_MS = 40;   // 25 Гц
constexpr uint16_t IAT_INTERVAL_MS      = 100;  // 10 Гц

/*
 * ============================================================
 *                  OBD PIDs
 * ============================================================
 */

constexpr uint8_t PID_RPM       = 0x0C;
constexpr uint8_t PID_IAT       = 0x0F;
constexpr uint8_t PID_THROTTLE  = 0x11;
constexpr uint8_t PID_RELATIVE  = 0x45;
constexpr uint8_t PID_PEDAL_D   = 0x49;
constexpr uint8_t PID_PEDAL_E   = 0x4A;

/*
 * ============================================================
 *                  Debug
 * ============================================================
 */

#define DEBUG_SERIAL 1

#if DEBUG_SERIAL
    #define DEBUG_PRINT(x)    Serial.print(x)
    #define DEBUG_PRINTLN(x)  Serial.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif

#endif // CONFIG_H
