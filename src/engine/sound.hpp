#pragma once

#include "olc/olcPGEX_Sound.h"
#include "observer.hpp"

class SoundEngine : public Observer {

  public:
    void init();

    void onNotify(const entt::registry& entity, const Event& event) override;

  private:
    olc::SOUND::AudioSample sample;
};
