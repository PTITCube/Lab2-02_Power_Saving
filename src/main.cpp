/**
 * VÍ DỤ 2-2: QUẢN LÝ TIẾT KIỆM NĂNG LƯỢNG - ĐIỀU KHIỂN DÒNG RA/VÀO MẠCH HẠ ÁP
 */
#include <Arduino.h>
#include <PTITCube.h>

#define PIN_BUCK_CTRL   4
#define BUCK_POWER_ON   LOW   // Mức 0: Bật nguồn tải
#define BUCK_POWER_OFF  HIGH  // Mức 1: Ngắt nguồn tải

PTIT_EPS eps;

const unsigned long LOAD_OFF_DURATION = 5000;
const unsigned long LOAD_ON_DURATION  = 1500;

void setBuckConverterState(bool enable) {
    digitalWrite(PIN_BUCK_CTRL, enable ? BUCK_POWER_ON : BUCK_POWER_OFF);
}

void setup() {
    Serial.begin(115200);
    pinMode(PIN_BUCK_CTRL, OUTPUT);
    setBuckConverterState(false);
    eps.init();
}

void loop() {
    // 1. Cấp nguồn cho mạch hạ áp & đợi ổn định
    setBuckConverterState(true);
    delay(100);

    // 2. Thu thập dữ liệu trạng thái pin
    eps.update();
    float vcell = eps.getBatteryVoltage();
    float soc   = eps.getBatteryPercent();

    // 3. Duy trì hoạt động trong thời gian định mức
    delay(LOAD_ON_DURATION);

    // 4. Ngắt dòng vào mạch hạ áp để tiết kiệm năng lượng
    setBuckConverterState(false);

    Serial.printf("[EPS] Active (V: %.2fV, SOC: %.1f%%) | Sleeping %lums\n", vcell, soc, LOAD_OFF_DURATION);
    delay(LOAD_OFF_DURATION);
}
