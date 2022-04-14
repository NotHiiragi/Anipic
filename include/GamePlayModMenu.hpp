#pragma once

#include "main.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "HMUI/ImageView.hpp"
#include "UnityEngine/UI/Button.hpp"


DECLARE_CLASS_CODEGEN(AniPic, GamePlayModMenu, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, PIC);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, AniButton);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstactivation);
)