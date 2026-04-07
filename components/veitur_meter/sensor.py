import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID

from . import veitur_meter_ns

CONF_CUMULATIVE_ACTIVE_IMPORT = "cumulative_active_import"
CONF_CUMULATIVE_ACTIVE_EXPORT = "cumulative_active_export"
CONF_CUMULATIVE_REACTIVE_IMPORT = "cumulative_reactive_import"
CONF_CUMULATIVE_REACTIVE_EXPORT = "cumulative_reactive_export"
CONF_MOMENTARY_ACTIVE_IMPORT = "momentary_active_import"
CONF_MOMENTARY_ACTIVE_EXPORT = "momentary_active_export"
CONF_MOMENTARY_REACTIVE_IMPORT = "momentary_reactive_import"
CONF_MOMENTARY_REACTIVE_EXPORT = "momentary_reactive_export"
CONF_MOMENTARY_ACTIVE_IMPORT_L1 = "momentary_active_import_l1"
CONF_MOMENTARY_ACTIVE_EXPORT_L1 = "momentary_active_export_l1"
CONF_MOMENTARY_ACTIVE_IMPORT_L2 = "momentary_active_import_l2"
CONF_MOMENTARY_ACTIVE_EXPORT_L2 = "momentary_active_export_l2"
CONF_MOMENTARY_ACTIVE_IMPORT_L3 = "momentary_active_import_l3"
CONF_MOMENTARY_ACTIVE_EXPORT_L3 = "momentary_active_export_l3"
CONF_MOMENTARY_REACTIVE_IMPORT_L1 = "momentary_reactive_import_l1"
CONF_MOMENTARY_REACTIVE_EXPORT_L1 = "momentary_reactive_export_l1"
CONF_MOMENTARY_REACTIVE_IMPORT_L2 = "momentary_reactive_import_l2"
CONF_MOMENTARY_REACTIVE_EXPORT_L2 = "momentary_reactive_export_l2"
CONF_MOMENTARY_REACTIVE_IMPORT_L3 = "momentary_reactive_import_l3"
CONF_MOMENTARY_REACTIVE_EXPORT_L3 = "momentary_reactive_export_l3"
CONF_VOLTAGE_L1 = "voltage_l1"
CONF_VOLTAGE_L2 = "voltage_l2"
CONF_VOLTAGE_L3 = "voltage_l3"
CONF_CURRENT_L1 = "current_l1"
CONF_CURRENT_L2 = "current_l2"
CONF_CURRENT_L3 = "current_l3"
CONF_HOT_WATER_VOLUME = "hot_water_volume"
CONF_HOT_WATER_FLOW_RATE = "hot_water_flow_rate"
CONF_INLET_WATER_TEMPERATURE = "inlet_water_temperature"
CONF_OUTLET_WATER_TEMPERATURE = "outlet_water_temperature"
CONF_ENERGY_CONTENT_VOLUME = "energy_content_volume"
CONF_ENERGY_FLOW_RATE = "energy_flow_rate"
CONF_METER_ID = "meter_id"

VeiturMeter = veitur_meter_ns.class_("VeiturMeter", cg.Component, uart.UARTDevice)

SENSOR_KEYS = [
    CONF_CUMULATIVE_ACTIVE_IMPORT,
    CONF_CUMULATIVE_ACTIVE_EXPORT,
    CONF_CUMULATIVE_REACTIVE_IMPORT,
    CONF_CUMULATIVE_REACTIVE_EXPORT,
    CONF_MOMENTARY_ACTIVE_IMPORT,
    CONF_MOMENTARY_ACTIVE_EXPORT,
    CONF_MOMENTARY_REACTIVE_IMPORT,
    CONF_MOMENTARY_REACTIVE_EXPORT,
    CONF_MOMENTARY_ACTIVE_IMPORT_L1,
    CONF_MOMENTARY_ACTIVE_EXPORT_L1,
    CONF_MOMENTARY_ACTIVE_IMPORT_L2,
    CONF_MOMENTARY_ACTIVE_EXPORT_L2,
    CONF_MOMENTARY_ACTIVE_IMPORT_L3,
    CONF_MOMENTARY_ACTIVE_EXPORT_L3,
    CONF_MOMENTARY_REACTIVE_IMPORT_L1,
    CONF_MOMENTARY_REACTIVE_EXPORT_L1,
    CONF_MOMENTARY_REACTIVE_IMPORT_L2,
    CONF_MOMENTARY_REACTIVE_EXPORT_L2,
    CONF_MOMENTARY_REACTIVE_IMPORT_L3,
    CONF_MOMENTARY_REACTIVE_EXPORT_L3,
    CONF_VOLTAGE_L1,
    CONF_VOLTAGE_L2,
    CONF_VOLTAGE_L3,
    CONF_CURRENT_L1,
    CONF_CURRENT_L2,
    CONF_CURRENT_L3,
    CONF_HOT_WATER_VOLUME,
    CONF_HOT_WATER_FLOW_RATE,
    CONF_INLET_WATER_TEMPERATURE,
    CONF_OUTLET_WATER_TEMPERATURE,
    CONF_ENERGY_CONTENT_VOLUME,
    CONF_ENERGY_FLOW_RATE,
    CONF_METER_ID,
]

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(VeiturMeter),
            cv.Optional(CONF_CUMULATIVE_ACTIVE_IMPORT): sensor.sensor_schema(),
            cv.Optional(CONF_CUMULATIVE_ACTIVE_EXPORT): sensor.sensor_schema(),
            cv.Optional(CONF_CUMULATIVE_REACTIVE_IMPORT): sensor.sensor_schema(),
            cv.Optional(CONF_CUMULATIVE_REACTIVE_EXPORT): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_ACTIVE_IMPORT): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_ACTIVE_EXPORT): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_REACTIVE_IMPORT): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_REACTIVE_EXPORT): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_ACTIVE_IMPORT_L1): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_ACTIVE_EXPORT_L1): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_ACTIVE_IMPORT_L2): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_ACTIVE_EXPORT_L2): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_ACTIVE_IMPORT_L3): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_ACTIVE_EXPORT_L3): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_REACTIVE_IMPORT_L1): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_REACTIVE_EXPORT_L1): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_REACTIVE_IMPORT_L2): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_REACTIVE_EXPORT_L2): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_REACTIVE_IMPORT_L3): sensor.sensor_schema(),
            cv.Optional(CONF_MOMENTARY_REACTIVE_EXPORT_L3): sensor.sensor_schema(),
            cv.Optional(CONF_VOLTAGE_L1): sensor.sensor_schema(),
            cv.Optional(CONF_VOLTAGE_L2): sensor.sensor_schema(),
            cv.Optional(CONF_VOLTAGE_L3): sensor.sensor_schema(),
            cv.Optional(CONF_CURRENT_L1): sensor.sensor_schema(),
            cv.Optional(CONF_CURRENT_L2): sensor.sensor_schema(),
            cv.Optional(CONF_CURRENT_L3): sensor.sensor_schema(),
            cv.Optional(CONF_HOT_WATER_VOLUME): sensor.sensor_schema(),
            cv.Optional(CONF_HOT_WATER_FLOW_RATE): sensor.sensor_schema(),
            cv.Optional(CONF_INLET_WATER_TEMPERATURE): sensor.sensor_schema(),
            cv.Optional(CONF_OUTLET_WATER_TEMPERATURE): sensor.sensor_schema(),
            cv.Optional(CONF_ENERGY_CONTENT_VOLUME): sensor.sensor_schema(),
            cv.Optional(CONF_ENERGY_FLOW_RATE): sensor.sensor_schema(),
            cv.Optional(CONF_METER_ID): sensor.sensor_schema(),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    for key in SENSOR_KEYS:
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, f"set_{key}_sensor")(sens))