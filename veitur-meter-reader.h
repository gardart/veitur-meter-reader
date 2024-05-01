#include "esphome.h"

#define BUF_SIZE 60 // Data buffer size

class MeterReadings {
public:
    double cumulativeActiveImport = 0.0;
    double momentaryActiveImport = 0.0;
    double momentaryActiveImportL1 = 0.0;
    double momentaryActiveImportL2 = 0.0;
    double momentaryActiveImportL3 = 0.0;
    double voltageL1 = 0.0;
    double voltageL2 = 0.0;
    double voltageL3 = 0.0;
    double currentL1 = 0.0;
    double currentL2 = 0.0;
    double currentL3 = 0.0;
    double hotWaterVolume = 0.0;
    double hotWaterFlowRate = 0.0;
    double inletWaterTemperature = 0.0;
    double outletWaterTemperature = 0.0;
    double energyContentVolume = 0.0;
    double energyFlowRate = 0.0;
    double meterId = 0.0;
};

class MeterReader : public Component, public UARTDevice {
    const char* DELIMITERS = "()*:";
    char buffer[BUF_SIZE];

public:
    Sensor *cumulativeActiveImport = new Sensor();
    Sensor *momentaryActiveImport = new Sensor();
    Sensor *momentaryActiveImportL1 = new Sensor();
    Sensor *momentaryActiveImportL2 = new Sensor();
    Sensor *momentaryActiveImportL3 = new Sensor();
    Sensor *voltageL1 = new Sensor();
    Sensor *voltageL2 = new Sensor();
    Sensor *voltageL3 = new Sensor();
    Sensor *currentL1 = new Sensor();
    Sensor *currentL2 = new Sensor();
    Sensor *currentL3 = new Sensor();
    Sensor *hotWaterVolume = new Sensor();
    Sensor *hotWaterFlowRate = new Sensor();
    Sensor *inletWaterTemperature = new Sensor();
    Sensor *outletWaterTemperature = new Sensor();
    Sensor *energyContentVolume = new Sensor();
    Sensor *energyFlowRate = new Sensor();
    Sensor *meterId = new Sensor();

    MeterReader(UARTComponent *parent) : UARTDevice(parent) {}

    void setup() override {
    }

    void loop() override {
        readMeterData();
    }

private:
    void decodeMeterLine(MeterReadings* meterReadings, char* dataId, char* obisCode, char* value) {
        char fullCode[20]; // large enough to hold both dataId, obisCode, and separators
        sprintf(fullCode, "%s:%s", dataId, obisCode); // Combine them into one string
        ESP_LOGD("MREADER", "fullcode: %s", fullCode);
        ESP_LOGD("MREADER", "value: %s", value);

        //
        // Parsing logic for OBIS codes
        //

        // Meter ID
        if (strcmp(fullCode, "0-0:96.1.0") == 0) {
        meterReadings->meterId = atof(value); // Store the meter ID
        }

        // Electricity delivered to client (cumulative/increasing) -  Total kWh
        if (strcmp(fullCode, "1-0:1.8.0") == 0) {
            meterReadings->cumulativeActiveImport = atof(value);
        }
        // Electricity delivered to client (instantaneous) kW
        else if (strcmp(fullCode, "1-0:1.7.0") == 0) {
            meterReadings->momentaryActiveImport = atof(value);
        }

        // Electricity delivered to client (instantaneous) kW
        else if (strcmp(fullCode, "1-0:21.7.0") == 0) {
            meterReadings->momentaryActiveImportL1 = atof(value);
        }

        // Electricity delivered to client (instantaneous) kW
        else if (strcmp(fullCode, "1-0:41.7.0") == 0) {
            meterReadings->momentaryActiveImportL2 = atof(value);
        }

        // Electricity delivered to client (instantaneous) kW
        else if (strcmp(fullCode, "1-0:61.7.0") == 0) {
            meterReadings->momentaryActiveImportL3 = atof(value);
        }

        // Current
        else if (strcmp(fullCode, "1-0:31.7.0") == 0) {
            meterReadings->currentL1 = atof(value);
        }
        else if (strcmp(fullCode, "1-0:51.7.0") == 0) {
            meterReadings->currentL2 = atof(value);
        }
        else if (strcmp(fullCode, "1-0:71.7.0") == 0) {
            meterReadings->currentL3 = atof(value);
        }

        // Voltage
        else if (strcmp(fullCode, "1-0:32.7.0") == 0) {
            meterReadings->voltageL1 = atof(value);
        }

        else if (strcmp(fullCode, "1-0:52.7.0") == 0) {
            meterReadings->voltageL2 = atof(value);
        }

        else if (strcmp(fullCode, "1-0:72.7.0") == 0) {
            meterReadings->voltageL3 = atof(value);
        }

        // Hot water related data (from hot water sub-meter)
        // Hot water volume - Total cubic meters
        else if (strcmp(fullCode, "0-1:24.2.1") == 0) {
            meterReadings->hotWaterVolume = atof(value);
        }
        // Current hot water flow rate in m3/h
        else if (strcmp(fullCode, "0-1:24.2.2") == 0) {
            meterReadings->hotWaterFlowRate = atof(value);
        }
        // Current inlet water temperature in degrees Celsius
        else if (strcmp(fullCode, "0-1:24.2.3") == 0) {
            meterReadings->inletWaterTemperature = atof(value);
        }
        // Current outlet water temperature in degrees Celsius
        else if (strcmp(fullCode, "0-1:24.2.4") == 0) {
            meterReadings->outletWaterTemperature = atof(value);
        }
        // Energy content in the volume of water in Wh
        else if (strcmp(fullCode, "0-1:24.2.5") == 0) {
            meterReadings->energyContentVolume = atof(value);
        }
        // Rate of energy transfer in Wh
        else if (strcmp(fullCode, "0-1:24.2.6") == 0) {
            meterReadings->energyFlowRate = atof(value);
        }

    }

    void updateSensors(MeterReadings* meterReadings) {
        cumulativeActiveImport->publish_state(meterReadings->cumulativeActiveImport);
        momentaryActiveImport->publish_state(meterReadings->momentaryActiveImport);
        momentaryActiveImportL1->publish_state(meterReadings->momentaryActiveImportL1);
        momentaryActiveImportL2->publish_state(meterReadings->momentaryActiveImportL2);
        momentaryActiveImportL3->publish_state(meterReadings->momentaryActiveImportL3);
        voltageL1->publish_state(meterReadings->voltageL1);
        voltageL2->publish_state(meterReadings->voltageL2);
        voltageL3->publish_state(meterReadings->voltageL3);
        currentL1->publish_state(meterReadings->currentL1);
        currentL2->publish_state(meterReadings->currentL2);
        currentL3->publish_state(meterReadings->currentL3);
        hotWaterVolume->publish_state(meterReadings->hotWaterVolume);
        hotWaterFlowRate->publish_state(meterReadings->hotWaterFlowRate);
        inletWaterTemperature->publish_state(meterReadings->inletWaterTemperature);
        outletWaterTemperature->publish_state(meterReadings->outletWaterTemperature);
        energyContentVolume->publish_state(meterReadings->energyContentVolume);
        energyFlowRate->publish_state(meterReadings->energyFlowRate);
        meterId->publish_state(meterReadings->meterId);
    }

void readMeterData() {
    if (available()) {
        MeterReadings meterReadings;
        bool telegramEnded = false;
        String line = "";

        while (available()) {
            int len = Serial.readBytesUntil('\n', buffer, BUF_SIZE);

            if (len > 0) {
                buffer[len] = '\0';
                ESP_LOGD("MREADER", "Received line: %s", buffer);

                if (buffer[0] == '!') {
                    telegramEnded = true;
                    break; // End of telegram
                } else {
                    char* dataId = strtok(buffer, DELIMITERS);
                    char* obisCode = strtok(NULL, DELIMITERS);
                    char* value = strtok(NULL, DELIMITERS);
                    if (dataId && obisCode && value) {
                        decodeMeterLine(&meterReadings, dataId, obisCode, value);
                    }
                }
            }

            // Clean buffer
            memset(buffer, 0, BUF_SIZE);

            if (!telegramEnded && !available()) {
            // Wait for more data
                delay(10);
            }
        }

        if (telegramEnded) {
            //ESP_LOGD("MREADER", "End of telegram. Updating sensors.");
            updateSensors(&meterReadings);
        }
    }
}

};
