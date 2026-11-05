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

void VeiturMeter::dump_config() { ESP_LOGCONFIG(TAG, "Veitur Meter"); }

void VeiturMeter::loop() {
  while (this->available()) {
    uint8_t byte;
    if (!this->read_byte(&byte))
      break;

    if (byte == '\r')
      continue;

    if (byte == '\n') {
      if (this->discarding_line_) {
        this->discarding_line_ = false;
        this->reset_buffer_();
      } else if (this->buffer_pos_ > 0) {
        this->process_buffer_();
      }
      continue;
    }

    if (this->discarding_line_)
      continue;

    if (this->buffer_pos_ >= BUFFER_SIZE - 1) {
      ESP_LOGW(TAG, "Discarding DSMR line longer than %d bytes", BUFFER_SIZE - 1);
      this->reset_buffer_();
      this->discarding_line_ = true;
      continue;
    }

    this->line_buffer_[this->buffer_pos_++] = static_cast<char>(byte);
  }
}

void VeiturMeter::reset_buffer_() {
  this->buffer_pos_ = 0;
  std::memset(this->line_buffer_, 0, sizeof(this->line_buffer_));
}

bool VeiturMeter::extract_code_and_value_(const char *line, char *full_code, size_t full_code_size, char *value,
                                          size_t value_size, bool *has_unit) {
  const char *first_open = std::strchr(line, '(');
  if (first_open == nullptr)
    return false;

  size_t code_length = static_cast<size_t>(first_open - line);
  while (code_length > 0 && line[code_length - 1] == ' ')
    code_length--;
  if (code_length == 0 || code_length >= full_code_size)
    return false;

  std::memcpy(full_code, line, code_length);
  full_code[code_length] = '\0';

  char fallback[64]{};
  char with_unit[64]{};
  const char *cursor = first_open;
  while (cursor != nullptr && *cursor != '\0') {
    const char *open = std::strchr(cursor, '(');
    if (open == nullptr)
      break;
    const char *close = std::strchr(open + 1, ')');
    if (close == nullptr)
      break;

    const size_t field_length = static_cast<size_t>(close - (open + 1));
    if (field_length > 0 && field_length < sizeof(fallback)) {
      std::memcpy(fallback, open + 1, field_length);
      fallback[field_length] = '\0';
      if (std::strchr(fallback, '*') != nullptr)
        std::snprintf(with_unit, sizeof(with_unit), "%s", fallback);
    }
    cursor = close + 1;
  }

  const char *selected = with_unit[0] != '\0' ? with_unit : fallback;
  if (selected[0] == '\0')
    return false;

  *has_unit = with_unit[0] != '\0';
  std::snprintf(value, value_size, "%s", selected);
  char *unit = std::strchr(value, '*');
  if (unit != nullptr)
    *unit = '\0';
  return value[0] != '\0';
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

  char full_code[32]{};
  char value[64]{};
  bool has_unit = false;
  if (this->extract_code_and_value_(this->line_buffer_, full_code, sizeof(full_code), value, sizeof(value),
                                    &has_unit)) {
    const char *separator = std::strchr(full_code, ':');
    const char *obis = separator == nullptr ? full_code : separator + 1;

    // Veitur DSMR can send an M-Bus timestamp and its value on separate rows
    // under the same 24.2.x code. Only the row with a unit is a measurement.
    if (std::strncmp(obis, "24.2.", 5) != 0 || has_unit)
      this->decode_meter_line_(full_code, value, this->line_buffer_);
  }

  this->reset_buffer_();
}

void VeiturMeter::decode_meter_line_(const char *full_code, const char *value_text, const char *line) {
  char *end = nullptr;
  float value = static_cast<float>(std::strtod(value_text, &end));
  if (end == value_text) {
    ESP_LOGD(TAG, "Ignoring non-numeric value for %s", full_code);
    return;
  }

  ESP_LOGD(TAG, "OBIS %s = %.6f", full_code, value);

  if (std::strcmp(full_code, "0-0:96.1.0") == 0 || std::strcmp(full_code, "0-0:96.1.1") == 0) {
    this->readings_.meter_id = value;
  } else if (std::strcmp(full_code, "1-0:1.8.0") == 0) {
    this->readings_.cumulative_active_import = value;
  } else if (std::strcmp(full_code, "1-0:2.8.0") == 0) {
    this->readings_.cumulative_active_export = value;
  } else if (std::strcmp(full_code, "1-0:3.8.0") == 0) {
    this->readings_.cumulative_reactive_import = value;
  } else if (std::strcmp(full_code, "1-0:4.8.0") == 0) {
    this->readings_.cumulative_reactive_export = value;
  } else if (std::strcmp(full_code, "1-0:1.7.0") == 0) {
    this->readings_.momentary_active_import = value;
  } else if (std::strcmp(full_code, "1-0:2.7.0") == 0) {
    this->readings_.momentary_active_export = value;
  } else if (std::strcmp(full_code, "1-0:3.7.0") == 0) {
    this->readings_.momentary_reactive_import = value;
  } else if (std::strcmp(full_code, "1-0:4.7.0") == 0) {
    this->readings_.momentary_reactive_export = value;
  } else if (std::strcmp(full_code, "1-0:21.7.0") == 0) {
    this->readings_.momentary_active_import_l1 = value;
  } else if (std::strcmp(full_code, "1-0:22.7.0") == 0) {
    this->readings_.momentary_active_export_l1 = value;
  } else if (std::strcmp(full_code, "1-0:23.7.0") == 0) {
    this->readings_.momentary_reactive_import_l1 = value;
  } else if (std::strcmp(full_code, "1-0:24.7.0") == 0) {
    this->readings_.momentary_reactive_export_l1 = value;
  } else if (std::strcmp(full_code, "1-0:41.7.0") == 0) {
    this->readings_.momentary_active_import_l2 = value;
  } else if (std::strcmp(full_code, "1-0:42.7.0") == 0) {
    this->readings_.momentary_active_export_l2 = value;
  } else if (std::strcmp(full_code, "1-0:43.7.0") == 0) {
    this->readings_.momentary_reactive_import_l2 = value;
  } else if (std::strcmp(full_code, "1-0:44.7.0") == 0) {
    this->readings_.momentary_reactive_export_l2 = value;
  } else if (std::strcmp(full_code, "1-0:61.7.0") == 0) {
    this->readings_.momentary_active_import_l3 = value;
  } else if (std::strcmp(full_code, "1-0:62.7.0") == 0) {
    this->readings_.momentary_active_export_l3 = value;
  } else if (std::strcmp(full_code, "1-0:63.7.0") == 0) {
    this->readings_.momentary_reactive_import_l3 = value;
  } else if (std::strcmp(full_code, "1-0:64.7.0") == 0) {
    this->readings_.momentary_reactive_export_l3 = value;
  } else if (std::strcmp(full_code, "1-0:31.7.0") == 0) {
    this->readings_.current_l1 = value;
  } else if (std::strcmp(full_code, "1-0:51.7.0") == 0) {
    this->readings_.current_l2 = value;
  } else if (std::strcmp(full_code, "1-0:71.7.0") == 0) {
    this->readings_.current_l3 = value;
  } else if (std::strcmp(full_code, "1-0:32.7.0") == 0) {
    this->readings_.voltage_l1 = value;
  } else if (std::strcmp(full_code, "1-0:52.7.0") == 0) {
    this->readings_.voltage_l2 = value;
  } else if (std::strcmp(full_code, "1-0:72.7.0") == 0) {
    this->readings_.voltage_l3 = value;
  } else if (std::strcmp(full_code, "0-1:24.2.1") == 0) {
    this->readings_.hot_water_volume = value;
  } else if (std::strcmp(full_code, "0-1:24.2.2") == 0) {
    this->readings_.hot_water_flow_rate = value;
  } else if (std::strcmp(full_code, "0-1:24.2.3") == 0) {
    this->readings_.inlet_water_temperature = value;
  } else if (std::strcmp(full_code, "0-1:24.2.4") == 0) {
    this->readings_.outlet_water_temperature = value;
  } else if (std::strcmp(full_code, "0-1:24.2.5") == 0) {
    if (std::strstr(line, "*Wh)") != nullptr)
      value /= 1000.0f;
    this->readings_.energy_content_volume = value;
  } else if (std::strcmp(full_code, "0-1:24.2.6") == 0) {
    if (std::strstr(line, "*W)") != nullptr)
      value /= 1000.0f;
    this->readings_.energy_flow_rate = value;
  }
}

void VeiturMeter::publish_sensor_(sensor::Sensor *target, float value) {
  if (target != nullptr && !std::isnan(value))
    target->publish_state(value);
}

void VeiturMeter::publish_readings_() {
  ESP_LOGD(TAG, "Publishing complete DSMR telegram");
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
