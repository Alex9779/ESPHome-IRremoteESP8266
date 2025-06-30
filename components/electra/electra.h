#pragma once

#include "esphome/core/log.h"
#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "ir_Electra.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome
{
    namespace electra
    {

        class ElectraClimate : public climate_ir::ClimateIR
        {
        public:
            ElectraClimate(uint8_t pin)
                : ac_{pin} {}
            
            void setup() override;
            climate::ClimateTraits traits() override;

        protected:
            void transmit_state() override;

        private:
            void send();
            void apply_state();

            IRElectraAc ac_{255}; // default pin, will be set in constructor or setup
            sensor::Sensor *sensor_{nullptr};
        public:
            void set_sensor(sensor::Sensor *sensor) { sensor_ = sensor; }
        };

    } // namespace electra
} // namespace esphome
