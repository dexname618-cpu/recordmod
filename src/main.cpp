#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

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

class $modify(MyGameLayer, GJBaseGameLayer) {

    struct Fields {
        bool isHolding = false;
    };

    void pushButton(int button, bool player1) {
        GJBaseGameLayer::pushButton(button, player1);

        if (!player1 || button != 1) return;
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;

        // Перевіряємо що ми в playtest редактора
        auto* editor = LevelEditorLayer::get();
        if (!editor) return;

        if (m_fields->isHolding) return;
        m_fields->isHolding = true;

        float px   = m_player1->getPositionX();
        int itemID = (int)Mod::get()->getSettingValue<int64_t>("pickup-item-id");
        int count  = (int)Mod::get()->getSettingValue<int64_t>("pickup-count-press");

        editor->createObjectsFromString(buildOptionString(px, 1), true, true);
        editor->createObjectsFromString(buildPickupString(px, itemID, count), true, true);
    }

    void releaseButton(int button, bool player1) {
        GJBaseGameLayer::releaseButton(button, player1);

        if (!player1 || button != 1) return;
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;

        auto* editor = LevelEditorLayer::get();
        if (!editor) return;

        if (!m_fields->isHolding) return;
        m_fields->isHolding = false;

        float px   = m_player1->getPositionX();
        int itemID = (int)Mod::get()->getSettingValue<int64_t>("pickup-item-id");
        int count  = (int)Mod::get()->getSettingValue<int64_t>("pickup-count-release");

        editor->createObjectsFromString(buildOptionString(px, -1), true, true);
        editor->createObjectsFromString(buildPickupString(px, itemID, count), true, true);
    }
};
