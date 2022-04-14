#include "main.hpp"
#include "GamePlayModMenu.hpp"
#include "ModConfig.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_CONFIG(ModConfig);

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = "AniPic";
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    getModConfig().Init(modInfo);

    QuestUI::Init();

    QuestUI::Register::RegisterGameplaySetupMenu<AniPic::GamePlayModMenu*>(modInfo, "Animals!");

}