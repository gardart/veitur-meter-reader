#pragma once

#include <cstdint>

#include "esphome/components/sensor/sensor.h"
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

 protected:
  struct MeterReadings {
    float cumulative_active_import = 0.0f;
    float cumulative_active_export = 0.0f;
    float cumulative_reactive_import = 0.0f;
    float cumulative_reactive_export = 0.0f;
    float momentary_active_import = 0.0f;
    float momentary_active_export = 0.0f;
    float momentary_reactive_import = 0.0f;
    float momentary_reactive_export = 0.0f;
    float momentary_active_import_l1 = 0.0f;
    float momentary_active_export_l1 = 0.0f;
    float momentary_active_import_l2 = 0.0f;
    float momentary_active_export_l2 = 0.0f;
    float momentary_active_import_l3 = 0.0f;
    float momentary_active_export_l3 = 0.0f;
    float momentary_reactive_import_l1 = 0.0f;
    float momentary_reactive_export_l1 = 0.0f;
    float momentary_reactive_import_l2 = 0.0f;
    float momentary_reactive_export_l2 = 0.0f;
    float momentary_reactive_import_l3 = 0.0f;
    float momentary_reactive_export_l3 = 0.0f;
    float voltage_l1 = 0.0f;
    float voltage_l2 = 0.0f;
    float voltage_l3 = 0.0f;
    float current_l1 = 0.0f;
    float current_l2 = 0.0f;
    float current_l3 = 0.0f;
    float hot_water_volume = 0.0f;
    float hot_water_flow_rate = 0.0f;
    float inlet_water_temperature = 0.0f;
    float outlet_water_temperature = 0.0f;
    float energy_content_volume = 0.0f;
    float energy_flow_rate = 0.0f;
    float meter_id = 0.0f;
  };

  void reset_buffer_();
  void process_buffer_();
  void decode_meter_line_(char *data_id, char *obis_code, char *value);
  void publish_readings_();
  void publish_sensor_(sensor::Sensor *target, float value);

  static const int BUFFER_SIZE = 64;
  const char *delimiters_ = "()*:";
  char line_buffer_[BUFFER_SIZE];
  int buffer_pos_{0};
  MeterReadings readings_;

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
};

}  // namespace veitur_meter
}  // namespace esphome