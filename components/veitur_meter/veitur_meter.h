#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

namespace esphome {
namespace veitur_meter {

class VeiturMeter : public Component, public uart::UARTDevice {
 public:
  VeiturMeter() = default;

  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_cumulative_active_import_sensor(sensor::Sensor *sensor) { this->cumulative_active_import_sensor_ = sensor; }
  void set_cumulative_active_export_sensor(sensor::Sensor *sensor) { this->cumulative_active_export_sensor_ = sensor; }
  void set_cumulative_reactive_import_sensor(sensor::Sensor *sensor) { this->cumulative_reactive_import_sensor_ = sensor; }
  void set_cumulative_reactive_export_sensor(sensor::Sensor *sensor) { this->cumulative_reactive_export_sensor_ = sensor; }
  void set_momentary_active_import_sensor(sensor::Sensor *sensor) { this->momentary_active_import_sensor_ = sensor; }
  void set_momentary_active_export_sensor(sensor::Sensor *sensor) { this->momentary_active_export_sensor_ = sensor; }
  void set_momentary_reactive_import_sensor(sensor::Sensor *sensor) { this->momentary_reactive_import_sensor_ = sensor; }
  void set_momentary_reactive_export_sensor(sensor::Sensor *sensor) { this->momentary_reactive_export_sensor_ = sensor; }
  void set_momentary_active_import_l1_sensor(sensor::Sensor *sensor) { this->momentary_active_import_l1_sensor_ = sensor; }
  void set_momentary_active_export_l1_sensor(sensor::Sensor *sensor) { this->momentary_active_export_l1_sensor_ = sensor; }
  void set_momentary_active_import_l2_sensor(sensor::Sensor *sensor) { this->momentary_active_import_l2_sensor_ = sensor; }
  void set_momentary_active_export_l2_sensor(sensor::Sensor *sensor) { this->momentary_active_export_l2_sensor_ = sensor; }
  void set_momentary_active_import_l3_sensor(sensor::Sensor *sensor) { this->momentary_active_import_l3_sensor_ = sensor; }
  void set_momentary_active_export_l3_sensor(sensor::Sensor *sensor) { this->momentary_active_export_l3_sensor_ = sensor; }
  void set_momentary_reactive_import_l1_sensor(sensor::Sensor *sensor) { this->momentary_reactive_import_l1_sensor_ = sensor; }
  void set_momentary_reactive_export_l1_sensor(sensor::Sensor *sensor) { this->momentary_reactive_export_l1_sensor_ = sensor; }
  void set_momentary_reactive_import_l2_sensor(sensor::Sensor *sensor) { this->momentary_reactive_import_l2_sensor_ = sensor; }
  void set_momentary_reactive_export_l2_sensor(sensor::Sensor *sensor) { this->momentary_reactive_export_l2_sensor_ = sensor; }
  void set_momentary_reactive_import_l3_sensor(sensor::Sensor *sensor) { this->momentary_reactive_import_l3_sensor_ = sensor; }
  void set_momentary_reactive_export_l3_sensor(sensor::Sensor *sensor) { this->momentary_reactive_export_l3_sensor_ = sensor; }
  void set_voltage_l1_sensor(sensor::Sensor *sensor) { this->voltage_l1_sensor_ = sensor; }
  void set_voltage_l2_sensor(sensor::Sensor *sensor) { this->voltage_l2_sensor_ = sensor; }
  void set_voltage_l3_sensor(sensor::Sensor *sensor) { this->voltage_l3_sensor_ = sensor; }
  void set_current_l1_sensor(sensor::Sensor *sensor) { this->current_l1_sensor_ = sensor; }
  void set_current_l2_sensor(sensor::Sensor *sensor) { this->current_l2_sensor_ = sensor; }
  void set_current_l3_sensor(sensor::Sensor *sensor) { this->current_l3_sensor_ = sensor; }
  void set_hot_water_volume_sensor(sensor::Sensor *sensor) { this->hot_water_volume_sensor_ = sensor; }
  void set_hot_water_flow_rate_sensor(sensor::Sensor *sensor) { this->hot_water_flow_rate_sensor_ = sensor; }
  void set_inlet_water_temperature_sensor(sensor::Sensor *sensor) { this->inlet_water_temperature_sensor_ = sensor; }
  void set_outlet_water_temperature_sensor(sensor::Sensor *sensor) { this->outlet_water_temperature_sensor_ = sensor; }
  void set_energy_content_volume_sensor(sensor::Sensor *sensor) { this->energy_content_volume_sensor_ = sensor; }
  void set_energy_flow_rate_sensor(sensor::Sensor *sensor) { this->energy_flow_rate_sensor_ = sensor; }
  void set_meter_id_sensor(sensor::Sensor *sensor) { this->meter_id_sensor_ = sensor; }
  void set_meter_id_text_sensor(text_sensor::TextSensor *sensor) { this->meter_id_text_sensor_ = sensor; }

 protected:
  struct MeterReadings {
    float cumulative_active_import = NAN;
    float cumulative_active_export = NAN;
    float cumulative_reactive_import = NAN;
    float cumulative_reactive_export = NAN;
    float momentary_active_import = NAN;
    float momentary_active_export = NAN;
    float momentary_reactive_import = NAN;
    float momentary_reactive_export = NAN;
    float momentary_active_import_l1 = NAN;
    float momentary_active_export_l1 = NAN;
    float momentary_active_import_l2 = NAN;
    float momentary_active_export_l2 = NAN;
    float momentary_active_import_l3 = NAN;
    float momentary_active_export_l3 = NAN;
    float momentary_reactive_import_l1 = NAN;
    float momentary_reactive_export_l1 = NAN;
    float momentary_reactive_import_l2 = NAN;
    float momentary_reactive_export_l2 = NAN;
    float momentary_reactive_import_l3 = NAN;
    float momentary_reactive_export_l3 = NAN;
    float voltage_l1 = NAN;
    float voltage_l2 = NAN;
    float voltage_l3 = NAN;
    float current_l1 = NAN;
    float current_l2 = NAN;
    float current_l3 = NAN;
    float hot_water_volume = NAN;
    float hot_water_flow_rate = NAN;
    float inlet_water_temperature = NAN;
    float outlet_water_temperature = NAN;
    float energy_content_volume = NAN;
    float energy_flow_rate = NAN;
    float meter_id = NAN;
    char meter_id_text[64]{};
  };

  void reset_buffer_();
  void process_buffer_();
  bool extract_code_and_value_(const char *line, char *full_code, size_t full_code_size, char *value,
                               size_t value_size, bool *has_unit);
  void decode_meter_line_(const char *full_code, const char *value, const char *line);
  void publish_readings_();
  void publish_sensor_(sensor::Sensor *target, float value);
  static uint16_t crc16_update_(uint16_t crc, uint8_t data);
  void start_telegram_crc_();
  void track_crc_byte_(uint8_t byte);
  bool validate_telegram_crc_();

  static const int BUFFER_SIZE = 256;
  char line_buffer_[BUFFER_SIZE];
  int buffer_pos_{0};
  bool discarding_line_{false};
  MeterReadings readings_;
  uint16_t telegram_crc_{0};
  bool crc_active_{false};
  bool checksum_active_{false};
  char checksum_[5]{};
  uint8_t checksum_pos_{0};

  sensor::Sensor *cumulative_active_import_sensor_{nullptr};
  sensor::Sensor *cumulative_active_export_sensor_{nullptr};
  sensor::Sensor *cumulative_reactive_import_sensor_{nullptr};
  sensor::Sensor *cumulative_reactive_export_sensor_{nullptr};
  sensor::Sensor *momentary_active_import_sensor_{nullptr};
  sensor::Sensor *momentary_active_export_sensor_{nullptr};
  sensor::Sensor *momentary_reactive_import_sensor_{nullptr};
  sensor::Sensor *momentary_reactive_export_sensor_{nullptr};
  sensor::Sensor *momentary_active_import_l1_sensor_{nullptr};
  sensor::Sensor *momentary_active_export_l1_sensor_{nullptr};
  sensor::Sensor *momentary_active_import_l2_sensor_{nullptr};
  sensor::Sensor *momentary_active_export_l2_sensor_{nullptr};
  sensor::Sensor *momentary_active_import_l3_sensor_{nullptr};
  sensor::Sensor *momentary_active_export_l3_sensor_{nullptr};
  sensor::Sensor *momentary_reactive_import_l1_sensor_{nullptr};
  sensor::Sensor *momentary_reactive_export_l1_sensor_{nullptr};
  sensor::Sensor *momentary_reactive_import_l2_sensor_{nullptr};
  sensor::Sensor *momentary_reactive_export_l2_sensor_{nullptr};
  sensor::Sensor *momentary_reactive_import_l3_sensor_{nullptr};
  sensor::Sensor *momentary_reactive_export_l3_sensor_{nullptr};
  sensor::Sensor *voltage_l1_sensor_{nullptr};
  sensor::Sensor *voltage_l2_sensor_{nullptr};
  sensor::Sensor *voltage_l3_sensor_{nullptr};
  sensor::Sensor *current_l1_sensor_{nullptr};
  sensor::Sensor *current_l2_sensor_{nullptr};
  sensor::Sensor *current_l3_sensor_{nullptr};
  sensor::Sensor *hot_water_volume_sensor_{nullptr};
  sensor::Sensor *hot_water_flow_rate_sensor_{nullptr};
  sensor::Sensor *inlet_water_temperature_sensor_{nullptr};
  sensor::Sensor *outlet_water_temperature_sensor_{nullptr};
  sensor::Sensor *energy_content_volume_sensor_{nullptr};
  sensor::Sensor *energy_flow_rate_sensor_{nullptr};
  sensor::Sensor *meter_id_sensor_{nullptr};
  text_sensor::TextSensor *meter_id_text_sensor_{nullptr};
};

}  // namespace veitur_meter
}  // namespace esphome