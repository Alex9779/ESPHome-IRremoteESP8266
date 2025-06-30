#define _IR_ENABLE_DEFAULT_ false
#define SEND_ELECTRA_AC true
#define DECODE_ELECTRA_AC false

#include "esphome.h"
#include "ir_Electra.h"
#include "electra.h"

namespace esphome
{
    namespace electra
    {
        static const char *const TAG = "electra.climate";

        void ElectraClimate::setup()
        {
            ac_.begin();
            this->apply_state();

            // Optionally, set initial temperature
            if (sensor_ != nullptr && sensor_->has_state()) {
                this->current_temperature = sensor_->state;
            }
        }

        // Add this method if you want to update the temperature regularly
        void ElectraClimate::update()
        {
            if (sensor_ != nullptr && sensor_->has_state()) {
                this->current_temperature = sensor_->state;
                this->publish_state();
            }
        }

        climate::ClimateTraits ElectraClimate::traits()
        {
            // The capabilities of the climate device
            auto traits = climate::ClimateTraits();

            traits.set_supports_current_temperature(true);
            
            traits.set_supported_modes({
                climate::CLIMATE_MODE_OFF,
                climate::CLIMATE_MODE_AUTO,
                climate::CLIMATE_MODE_COOL,
                climate::CLIMATE_MODE_DRY,
                climate::CLIMATE_MODE_FAN_ONLY,
            });
            traits.set_supported_fan_modes({
                climate::CLIMATE_FAN_AUTO,
                climate::CLIMATE_FAN_HIGH,
                climate::CLIMATE_FAN_MEDIUM,
                climate::CLIMATE_FAN_LOW,
            });
            traits.set_supported_swing_modes({
                climate::CLIMATE_SWING_OFF,
                climate::CLIMATE_SWING_VERTICAL,
            });

            return traits;
        }

        void ElectraClimate::transmit_state()
        {
            this->apply_state();
            this->ac_.send();
        }

        void ElectraClimate::apply_state()
        {
            if (this->mode == climate::CLIMATE_MODE_OFF)
            {
                this->ac_.off();
            }
            else
            {
                this->ac_.setTemp(this->target_temperature);

                switch (this->mode)
                {
                case climate::CLIMATE_MODE_OFF:
                    this->ac_.off();
                    break;
                case climate::CLIMATE_MODE_AUTO:
                    this->ac_.on();
                    this->ac_.setMode(kElectraAcAuto);
                    break;
                case climate::CLIMATE_MODE_COOL:
                    this->ac_.on();
                    this->ac_.setMode(kElectraAcCool);
                    break;
                case climate::CLIMATE_MODE_DRY:
                    this->ac_.on();
                    this->ac_.setMode(kElectraAcDry);
                    break;
                case climate::CLIMATE_MODE_FAN_ONLY:
                    this->ac_.on();
                    this->ac_.setMode(kElectraAcFan);
                    break;
                case climate::CLIMATE_MODE_HEAT:
                case climate::CLIMATE_MODE_HEAT_COOL:
                    break;
                }

                if (this->fan_mode.has_value())
                {
                    switch (this->fan_mode.value())
                    {
                    case climate::CLIMATE_FAN_ON:
                    case climate::CLIMATE_FAN_AUTO:
                        this->ac_.setFan(kElectraAcFanAuto);
                        break;
                    case climate::CLIMATE_FAN_LOW:
                        this->ac_.setFan(kElectraAcFanLow);
                        break;
                    case climate::CLIMATE_FAN_MEDIUM:
                        this->ac_.setFan(kElectraAcFanMed);
                        break;
                    case climate::CLIMATE_FAN_HIGH:
                        this->ac_.setFan(kElectraAcFanHigh);
                        break;
                    case climate::CLIMATE_FAN_OFF:
                    case climate::CLIMATE_FAN_MIDDLE:
                    case climate::CLIMATE_FAN_FOCUS:
                    case climate::CLIMATE_FAN_DIFFUSE:
                    case climate::CLIMATE_FAN_QUIET:
                    break;
                    }
                }

                switch (this->swing_mode)
                {
                case climate::CLIMATE_SWING_OFF:
                    this->ac_.setSwingH(false);
                    this->ac_.setSwingV(false);
                    break;
                case climate::CLIMATE_SWING_VERTICAL:
                    this->ac_.setSwingH(true);
                    this->ac_.setSwingV(true);
                    break;
                case climate::CLIMATE_SWING_BOTH:
                case climate::CLIMATE_SWING_HORIZONTAL:
                    break;
                }
            }
        }

    } // namespace electra
} // namespace esphome
