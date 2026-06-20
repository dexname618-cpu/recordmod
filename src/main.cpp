#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

static LevelEditorLayer* g_editor = nullptr;

class $modify(MyLevelEditorLayer, LevelEditorLayer) {
    bool init(GJGameLevel* level, bool unk) {
        if (!LevelEditorLayer::init(level, unk)) return false;
        g_editor = this;
        return true;
    }

    void onExit() {
        LevelEditorLayer::onExit();
        g_editor = nullptr;
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

    bool pushButton(PlayerButton button) {
        bool result = PlayerObject::pushButton(button);

        if (button != PlayerButton::Jump) return result;
        if (!Mod::get()->getSettingValue<bool>("enabled")) return result;
        if (!g_editor) return result;
        if (g_editor->m_player1 != this) return result;
        if (m_fields->isHolding) return result;
        m_fields->isHolding = true;

        float px   = this->getPositionX();
        int itemID = (int)Mod::get()->getSettingValue<int64_t>("pickup-item-id");

        // Press: Enable P1 Controls (-1), Count = 1
        g_editor->createObjectsFromString(buildOptionString(px, -1), true, true);
        g_editor->createObjectsFromString(buildPickupString(px, itemID, 1), true, true);

        return result;
    }

    bool releaseButton(PlayerButton button) {
        bool result = PlayerObject::releaseButton(button);

        if (button != PlayerButton::Jump) return result;
        if (!Mod::get()->getSettingValue<bool>("enabled")) return result;
        if (!g_editor) return result;
        if (g_editor->m_player1 != this) return result;
        if (!m_fields->isHolding) return result;
        m_fields->isHolding = false;

        float px   = this->getPositionX();
        int itemID = (int)Mod::get()->getSettingValue<int64_t>("pickup-item-id");

        // Release: Disable P1 Controls (1), Count = 0
        g_editor->createObjectsFromString(buildOptionString(px, 1), true, true);
        g_editor->createObjectsFromString(buildPickupString(px, itemID, 0), true, true);

        return result;
    }
};
