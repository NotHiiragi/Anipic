#define RAPIDJSON_HAS_STDSTRING 1
#include "GamePlayModMenu.hpp"
#include "WebUtils.hpp"
#include "ModConfig.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "System/IO/File.hpp"

using namespace UnityEngine;

DEFINE_TYPE(AniPic, GamePlayModMenu);

// Function gets url for the current selected animal
std::string get_api_path() {
    // Get all config parametes
    std::string defaulty = getModConfig().defaulty.GetValue();

    std::string url = "https://some-random-api.ml/img/";      

    url += defaulty;

    return url;
}

void AniPic::GamePlayModMenu::DidActivate(bool firstActivation)
{
    if(firstActivation)
    {
        std::vector<StringW>  animals = { "cat", "dog", "fox", "panda", "red_panda", "birb", "koala" };

        auto vert = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
        vert->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);

        PIC = QuestUI::BeatSaberUI::CreateImage(vert->get_transform(), nullptr, Vector2::get_zero(), Vector2(50, 50));
        PIC->set_preserveAspect(true);
        auto ele = PIC->get_gameObject()->AddComponent<UnityEngine::UI::LayoutElement*>();
        ele->set_preferredHeight(50);
        ele->set_preferredWidth(50);

        auto horz = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(vert->get_transform());
        horz->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
        horz->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
        horz->set_spacing(10);

        this->AniButton = QuestUI::BeatSaberUI::CreateUIButton(horz->get_transform(), "Randimal!", "PlayButton",
            [this]() {
                this->AniButton->set_interactable(false);

                WebUtils::GetAsync(get_api_path(), 10.0, [&](long code, std::string result){
                    switch (code)
                    {
                        case 200:
                            rapidjson::Document document;
                            document.Parse(result);
                            if(document.HasParseError() || !document.IsObject())
                                return;
                            std::string url = "";
                            if(document.HasMember("link"))
                            {
                                url = document.FindMember("link")->value.GetString();
                            }
                            getLogger().debug("%s", url.c_str());

                            WebUtils::GetAsync(url, 10.0, [this](long code, std::string result){
                                std::vector<uint8_t> bytes(result.begin(), result.end());

                                getLogger().debug("Downloaded Image!");
                                getLogger().debug("%lu", bytes.size());
                                switch (code)
                                {
                                    case 200:
                                        getLogger().debug("Les go!");
                                        QuestUI::MainThreadScheduler::Schedule([this, bytes]
                                        {
                                            Destroy(this->PIC->get_sprite()->get_texture());
                                            Destroy(this->PIC->get_sprite());
                                            this->PIC->set_sprite(QuestUI::BeatSaberUI::VectorToSprite(bytes));
                                            this->AniButton->set_interactable(true);
                                        });
                                        getLogger().debug("Les go! x2");

                                }
                            });
                            break;
                    }
                });
            });

        // Settings button
        UnityEngine::UI::Button* settingsButton = QuestUI::BeatSaberUI::CreateUIButton(horz->get_transform(), to_utf16("Settings"), "PracticeButton",
        [this, animals]() {
            getLogger().debug("Settings button clicked");
            // Run UI on the main thread
            QuestUI::MainThreadScheduler::Schedule([this, animals]
            {
                HMUI::ModalView* modal =  QuestUI::BeatSaberUI::CreateModal(get_transform(),  { 65, 65 }, nullptr);

                // Create a text that says "Hello World!" and set the parent to the container.
                UnityEngine::UI::VerticalLayoutGroup* vert = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(modal->get_transform());
                vert->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
                vert->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
                vert->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(60.0);


                TMPro::TextMeshProUGUI* title = QuestUI::BeatSaberUI::CreateText(vert->get_transform(), "Settings");
                title->GetComponent<TMPro::TMP_Text*>()->set_alignment(TMPro::TextAlignmentOptions::Center);
                title->GetComponent<TMPro::TMP_Text*>()->set_fontSize(7.0);

                // Choose your animal!
                std::string defaulty = getModConfig().defaulty.GetValue();
                QuestUI::BeatSaberUI::CreateDropdown(vert->get_transform(), to_utf16("Animals"), defaulty, animals, [](StringW value){
                    getModConfig().defaulty.SetValue(std::string(value));
                 
                });

                UnityEngine::UI::HorizontalLayoutGroup* horz = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(vert->get_transform());
                horz->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
                horz->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_horizontalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
                horz->set_spacing(10);


                UnityEngine::UI::Button* closeButton = QuestUI::BeatSaberUI::CreateUIButton(horz->get_transform(), to_utf16("Close"), "PlayButton",
                [this, modal]() {
                    modal->Hide(true, nullptr);
                });

                modal->Show(true, true, nullptr);
            });
        });
    }

    WebUtils::GetAsync(get_api_path(), 10.0, [&](long code, std::string result){
        switch (code)
        {
            case 200:
                rapidjson::Document document;
                document.Parse(result);
                if(document.HasParseError() || !document.IsObject())
                    return;
                std::string url = "";
                if(document.HasMember("link"))
                {
                    url = document.FindMember("link")->value.GetString();
                }
                getLogger().debug("%s", url.c_str());
                WebUtils::GetAsyncBytes(url, 10.0, [this](long code, std::shared_ptr<std::vector<uint8_t>> result){
                    getLogger().debug("Downloaded Image!");
                    getLogger().debug("%lu", result.get()->size());
                    switch (code)
                    {
                        case 200:
                            getLogger().debug("Les go!");
                            QuestUI::MainThreadScheduler::Schedule([this, result]
                            {
                                this->PIC->set_sprite(QuestUI::BeatSaberUI::VectorToSprite(*result.get()));
                                this->AniButton->set_interactable(true);
                            });
                            getLogger().debug("Les go! x2");
                    }
                });
                break;
        }
    });
}