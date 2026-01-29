#pragma once

#include "olc/olcPGEX_Sound.h"
#include "event.hpp"

class SoundEngine : public Observer {

  public:
    void init();

    void on_notify(const entt::registry& entity, const Event& event) override;

  private:
    olc::SOUND::AudioSample sample;
};
