import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import cover
from esphome.const import CONF_ID

chicken_door_ns = cg.esphome_ns.namespace('chicken_door')
ChickenDoor = chicken_door_ns.class_('ChickenDoor', cover.Cover, cg.Component)

CONF_THRESHOLD = "threshold"  # Define the configuration parameter

CONFIG_SCHEMA = cover.COVER_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ChickenDoor),
    cv.Optional(CONF_THRESHOLD, default=250): cv.int_,  # Add threshold parameter with default value
}).extend(cv.COMPONENT_SCHEMA)
AUTO_LOAD = ['binary_sensor']


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    if CONF_THRESHOLD in config:
        cg.add(var.set_threshold(config[CONF_THRESHOLD]))

    yield cg.register_component(var, config)
    yield cover.register_cover(var, config)