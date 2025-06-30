import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, sensor, remote_transmitter
from esphome import pins

AUTO_LOAD = ["climate", "remote_transmitter"]

electra_ns = cg.esphome_ns.namespace("electra")
ElectraClimate = electra_ns.class_("ElectraClimate", climate.Climate, cg.Component)

CONF_REMOTE_TRANSMITTER = "remote_transmitter"
CONF_SENSOR = "sensor"

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend({
    cv.Required(CONF_REMOTE_TRANSMITTER): cv.use_id(remote_transmitter.RemoteTransmitterComponent),
    cv.Optional(CONF_SENSOR): cv.use_id(sensor.Sensor),
})

async def to_code(config):
    cg.add_library("IRremoteESP8266", None)

    var = cg.new_Pvariable(config[cv.CONF_ID])

    remote = await cg.get_variable(config[CONF_REMOTE_TRANSMITTER])
    cg.add(var.set_remote_transmitter(remote))

    if CONF_SENSOR in config:
        sens = await cg.get_variable(config[CONF_SENSOR])
        cg.add(var.set_sensor(sens))
