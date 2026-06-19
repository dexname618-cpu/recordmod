#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

// Глобальне посилання на редактор
static LevelEditorLayer* g_editor = nullptr;

class $modify(MyLevelEditorLayer, LevelEditorLayer) {
    bool init(GJGameLevel* level, bool unk) {
        if (!LevelEditorLayer::init(level, unk)) return false;
        g_editor = this;
        return true;
    }

    void onStopPlaytest() {
        LevelEditorLayer::onStopPlaytest();
        g_editor = this;
    }
};

static std::string buildOptionString(float x, int mode) {
    return "1,2899,"
           "2," + std::to_string((int)x) + ","
           "3,15,"
           "36,1,"
           "165," + std::to_string(mode) + ";";
}

static std::string buildPickupString(float x, int itemID, int count) {
    return "1,1817,"
           "2," + std::to_string((int)x) + ","
           "3,45,"
           "36,1,"
           "80," + std::to_string(itemID) + ","
           "77," + std::to_string(count) + ","
           "139,1,"
           "449,1;";
}

class $modify(MyPlayerObject, PlayerObject) {

    struct Fields {
        bool isHolding = false;
    };

    void pushButton(PlayerButton button) {
        PlayerObject::pushButton(button);

        if (button != PlayerButton::Jump) return;
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;
        if (!g_editor) return;
        if (m_fields->isHolding) return;
        m_fields->isHolding = true;

        float px   = this->getPositionX();
        int itemID = (int)Mod::get()->getSettingValue<int64_t>("pickup-item-id");
        int count  = (int)Mod::get()->getSettingValue<int64_t>("pickup-count-press");

        g_editor->createObjectsFromString(buildOptionString(px, 1), true, true);
        g_editor->createObjectsFromString(buildPickupString(px, itemID, count), true, true);
    }

    void releaseButton(PlayerButton button) {
        PlayerObject::releaseButton(button);

        if (button != PlayerButton::Jump) return;
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;
        if (!g_editor) return;
        if (!m_fields->isHolding) return;
        m_fields->isHolding = false;

        float px   = this->getPositionX();
        int itemID = (int)Mod::get()->getSettingValue<int64_t>("pickup-item-id");
        int count  = (int)Mod::get()->getSettingValue<int64_t>("pickup-count-release");

        g_editor->createObjectsFromString(buildOptionString(px, -1), true, true);
        g_editor->createObjectsFromString(buildPickupString(px, itemID, count), true, true);
    }
};
