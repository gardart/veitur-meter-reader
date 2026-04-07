#include "veitur_meter.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "esphome/core/log.h"

namespace esphome {
namespace veitur_meter {

static const char *const TAG = "veitur_meter";

void VeiturMeter::setup() {
  this->reset_buffer_();
  ESP_LOGD(TAG, "Veitur meter setup complete");
}

void VeiturMeter::dump_config() {
  ESP_LOGCONFIG(TAG, "Veitur Meter");
}

void VeiturMeter::loop() {
  while (this->available()) {
    uint8_t byte;
    if (!this->read_byte(&byte)) {
      break;
    }

    if (byte == '\r') {
      continue;
    }

    if (byte == '\n') {
      if (this->buffer_pos_ > 0) {
        this->process_buffer_();
      }
      continue;
    }

    if (this->buffer_pos_ >= BUFFER_SIZE - 1) {
      ESP_LOGW(TAG, "Discarding oversized line");
      this->reset_buffer_();
      continue;
    }

    this->line_buffer_[this->buffer_pos_++] = static_cast<char>(byte);
  }
}

void VeiturMeter::reset_buffer_() {
  this->buffer_pos_ = 0;
  std::memset(this->line_buffer_, 0, sizeof(this->line_buffer_));
}

void VeiturMeter::process_buffer_() {
  this->line_buffer_[this->buffer_pos_] = '\0';
  ESP_LOGD(TAG, "Received line: %s", this->line_buffer_);

  if (this->line_buffer_[0] == '/') {
    this->readings_ = MeterReadings();
    this->reset_buffer_();
    return;
  }

  if (this->line_buffer_[0] == '!') {
    this->publish_readings_();
    this->reset_buffer_();
    return;
  }

  char *data_id = std::strtok(this->line_buffer_, this->delimiters_);
  char *obis_code = std::strtok(nullptr, this->delimiters_);
  char *value = std::strtok(nullptr, this->delimiters_);
  if (data_id != nullptr && obis_code != nullptr && value != nullptr) {
    this->decode_meter_line_(data_id, obis_code, value);
  }

  this->reset_buffer_();
}

void VeiturMeter::decode_meter_line_(char *data_id, char *obis_code, char *value) {
  char full_code[20];
  std::snprintf(full_code, sizeof(full_code), "%s:%s", data_id, obis_code);
  ESP_LOGD(TAG, "fullcode: %s", full_code);
  ESP_LOGD(TAG, "value: %s", value);

  if (std::strcmp(full_code, "0-0:96.1.0") == 0) {
    this->readings_.meter_id = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:1.8.0") == 0) {
    this->readings_.cumulative_active_import = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:2.8.0") == 0) {
    this->readings_.cumulative_active_export = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:3.8.0") == 0) {
    this->readings_.cumulative_reactive_import = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:4.8.0") == 0) {
    this->readings_.cumulative_reactive_export = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:1.7.0") == 0) {
    this->readings_.momentary_active_import = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:2.7.0") == 0) {
    this->readings_.momentary_active_export = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:3.7.0") == 0) {
    this->readings_.momentary_reactive_import = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:4.7.0") == 0) {
    this->readings_.momentary_reactive_export = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:21.7.0") == 0) {
    this->readings_.momentary_active_import_l1 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:22.7.0") == 0) {
    this->readings_.momentary_active_export_l1 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:23.7.0") == 0) {
    this->readings_.momentary_reactive_import_l1 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:24.7.0") == 0) {
    this->readings_.momentary_reactive_export_l1 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:41.7.0") == 0) {
    this->readings_.momentary_active_import_l2 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:42.7.0") == 0) {
    this->readings_.momentary_active_export_l2 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:43.7.0") == 0) {
    this->readings_.momentary_reactive_import_l2 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:44.7.0") == 0) {
    this->readings_.momentary_reactive_export_l2 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:61.7.0") == 0) {
    this->readings_.momentary_active_import_l3 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:62.7.0") == 0) {
    this->readings_.momentary_active_export_l3 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:63.7.0") == 0) {
    this->readings_.momentary_reactive_import_l3 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:64.7.0") == 0) {
    this->readings_.momentary_reactive_export_l3 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:31.7.0") == 0) {
    this->readings_.current_l1 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:51.7.0") == 0) {
    this->readings_.current_l2 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:71.7.0") == 0) {
    this->readings_.current_l3 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:32.7.0") == 0) {
    this->readings_.voltage_l1 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:52.7.0") == 0) {
    this->readings_.voltage_l2 = std::atof(value);
  } else if (std::strcmp(full_code, "1-0:72.7.0") == 0) {
    this->readings_.voltage_l3 = std::atof(value);
  } else if (std::strcmp(full_code, "0-1:24.2.1") == 0) {
    this->readings_.hot_water_volume = std::atof(value);
  } else if (std::strcmp(full_code, "0-1:24.2.2") == 0) {
    this->readings_.hot_water_flow_rate = std::atof(value);
  } else if (std::strcmp(full_code, "0-1:24.2.3") == 0) {
    this->readings_.inlet_water_temperature = std::atof(value);
  } else if (std::strcmp(full_code, "0-1:24.2.4") == 0) {
    this->readings_.outlet_water_temperature = std::atof(value);
  } else if (std::strcmp(full_code, "0-1:24.2.5") == 0) {
    this->readings_.energy_content_volume = std::atof(value);
  } else if (std::strcmp(full_code, "0-1:24.2.6") == 0) {
    this->readings_.energy_flow_rate = std::atof(value);
  }
}

void VeiturMeter::publish_sensor_(sensor::Sensor *target, float value) {
  if (target != nullptr) {
    target->publish_state(value);
  }
}

void VeiturMeter::publish_readings_() {
  ESP_LOGD(TAG, "Publishing telegram: import=%.3f kWh power=%.3f kW meter=%.0f", this->readings_.cumulative_active_import,
           this->readings_.momentary_active_import, this->readings_.meter_id);
  this->publish_sensor_(this->cumulative_active_import_sensor_, this->readings_.cumulative_active_import);
  this->publish_sensor_(this->cumulative_active_export_sensor_, this->readings_.cumulative_active_export);
  this->publish_sensor_(this->cumulative_reactive_import_sensor_, this->readings_.cumulative_reactive_import);
  this->publish_sensor_(this->cumulative_reactive_export_sensor_, this->readings_.cumulative_reactive_export);
  this->publish_sensor_(this->momentary_active_import_sensor_, this->readings_.momentary_active_import);
  this->publish_sensor_(this->momentary_active_export_sensor_, this->readings_.momentary_active_export);
  this->publish_sensor_(this->momentary_reactive_import_sensor_, this->readings_.momentary_reactive_import);
  this->publish_sensor_(this->momentary_reactive_export_sensor_, this->readings_.momentary_reactive_export);
  this->publish_sensor_(this->momentary_active_import_l1_sensor_, this->readings_.momentary_active_import_l1);
  this->publish_sensor_(this->momentary_active_export_l1_sensor_, this->readings_.momentary_active_export_l1);
  this->publish_sensor_(this->momentary_active_import_l2_sensor_, this->readings_.momentary_active_import_l2);
  this->publish_sensor_(this->momentary_active_export_l2_sensor_, this->readings_.momentary_active_export_l2);
  this->publish_sensor_(this->momentary_active_import_l3_sensor_, this->readings_.momentary_active_import_l3);
  this->publish_sensor_(this->momentary_active_export_l3_sensor_, this->readings_.momentary_active_export_l3);
  this->publish_sensor_(this->momentary_reactive_import_l1_sensor_, this->readings_.momentary_reactive_import_l1);
  this->publish_sensor_(this->momentary_reactive_export_l1_sensor_, this->readings_.momentary_reactive_export_l1);
  this->publish_sensor_(this->momentary_reactive_import_l2_sensor_, this->readings_.momentary_reactive_import_l2);
  this->publish_sensor_(this->momentary_reactive_export_l2_sensor_, this->readings_.momentary_reactive_export_l2);
  this->publish_sensor_(this->momentary_reactive_import_l3_sensor_, this->readings_.momentary_reactive_import_l3);
  this->publish_sensor_(this->momentary_reactive_export_l3_sensor_, this->readings_.momentary_reactive_export_l3);
  this->publish_sensor_(this->voltage_l1_sensor_, this->readings_.voltage_l1);
  this->publish_sensor_(this->voltage_l2_sensor_, this->readings_.voltage_l2);
  this->publish_sensor_(this->voltage_l3_sensor_, this->readings_.voltage_l3);
  this->publish_sensor_(this->current_l1_sensor_, this->readings_.current_l1);
  this->publish_sensor_(this->current_l2_sensor_, this->readings_.current_l2);
  this->publish_sensor_(this->current_l3_sensor_, this->readings_.current_l3);
  this->publish_sensor_(this->hot_water_volume_sensor_, this->readings_.hot_water_volume);
  this->publish_sensor_(this->hot_water_flow_rate_sensor_, this->readings_.hot_water_flow_rate);
  this->publish_sensor_(this->inlet_water_temperature_sensor_, this->readings_.inlet_water_temperature);
  this->publish_sensor_(this->outlet_water_temperature_sensor_, this->readings_.outlet_water_temperature);
  this->publish_sensor_(this->energy_content_volume_sensor_, this->readings_.energy_content_volume);
  this->publish_sensor_(this->energy_flow_rate_sensor_, this->readings_.energy_flow_rate);
  this->publish_sensor_(this->meter_id_sensor_, this->readings_.meter_id);
}

}  // namespace veitur_meter
}  // namespace esphome