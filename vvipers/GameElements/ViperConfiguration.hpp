#pragma once

#include "vvipers/Engine/Providers.hpp"
#include "vvipers/Utilities/Time.hpp"

namespace VVipers{

/** Nested helper class. Viper class holds one static instance. **/
class ViperConfiguration {
  public:
    ViperConfiguration(const OptionsProvider& options,
                    const TextureProvider& textures) {
        nominal_speed = options.option_double("Viper/nominalSpeed");
        nominal_segment_width =
            options.option_double("Viper/nominalSegmentWidth");  // px
        boost_max_charge = time_from_seconds(
            options.option_double("Viper/boostMaxCharge"));  // s
        boost_recharge_rate =
            options.option_double("Viper/boostRechargeRate");  // s per s
        boost_recharge_cooldown = time_from_seconds(options.option_double(
            "Viper/boostRechargeCooldown"));  // Countdown start

        head_nominal_length =
            options.option_double("ViperModel/ViperHead/nominalLength");  // px
        head_duration =
            time_from_seconds(head_nominal_length / nominal_speed);  // s
        head_nodes =
            options.option_2d_vector_array("ViperModel/ViperHead/nodes");

        body_nominal_length =
            options.option_double("ViperModel/ViperBody/nominalLength");  // px
        body_duration =
            time_from_seconds(body_nominal_length / nominal_speed);  // s
        body_nodes =
            options.option_2d_vector_array("ViperModel/ViperBody/nodes");

        tail_nominal_length =
            options.option_double("ViperModel/ViperTail/nominalLength");  // px
        tail_duration =
            time_from_seconds(tail_nominal_length / nominal_speed);  // s
        tail_nodes =
            options.option_2d_vector_array("ViperModel/ViperTail/nodes");

        head_texture = textures.texture("ViperHead");
        body_texture = textures.texture("ViperBody");
        tail_texture = textures.texture("ViperTail");
    }

    double nominal_speed;          // px/s
    double nominal_segment_width;  // px
    Time boost_max_charge;         // s
    double boost_recharge_rate;    // s per s
    Time boost_recharge_cooldown;  // Countdown start

    double head_nominal_length;  // px
    Time head_duration;          // s
    std::vector<Vec2> head_nodes;
    const sf::Texture* head_texture;

    double body_nominal_length;  // px
    Time body_duration;          // s
    std::vector<Vec2> body_nodes;
    const sf::Texture* body_texture;

    double tail_nominal_length;  // px
    Time tail_duration;          // s
    std::vector<Vec2> tail_nodes;
    const sf::Texture* tail_texture;
};

}