#pragma once

#include "olc/olcPGEX_Sound.h"
#include "event.hpp"

#include "utils/debugging.hpp"

class SoundEngine
{

  public:
    GET_BASE(SoundEngine)
    void init();

    // void on_notify(const entt::registry& entity, const Event& event) override;

  private:
    olc::SOUND::AudioSample sample;
};
