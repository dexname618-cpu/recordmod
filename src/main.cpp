#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

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

class $modify(MyEditorLayer, LevelEditorLayer) {

    struct Fields {
        bool isHolding = false;
    };

    float getPlayerX() {
        if (m_player1) return m_player1->getPositionX();
        return 0.f;
    }

    void onPlaytestKeyDown(int key) {
        LevelEditorLayer::onPlaytestKeyDown(key);
        if (key != 1) return;
        if (m_fields->isHolding) return;
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;
        m_fields->isHolding = true;

        float px   = getPlayerX();
        int itemID = (int)Mod::get()->getSettingValue<int64_t>("pickup-item-id");
        int count1 = (int)Mod::get()->getSettingValue<int64_t>("pickup-count-press");

        this->createObjectsFromString(buildOptionString(px, 1), true, true);
        this->createObjectsFromString(buildPickupString(px, itemID, count1), true, true);
    }

    void onPlaytestKeyUp(int key) {
        LevelEditorLayer::onPlaytestKeyUp(key);
        if (key != 1) return;
        if (!Mod::get()->getSettingValue<bool>("enabled")) {
            m_fields->isHolding = false;
            return;
        }
        m_fields->isHolding = false;

        float px   = getPlayerX();
        int itemID = (int)Mod::get()->getSettingValue<int64_t>("pickup-item-id");
        int count2 = (int)Mod::get()->getSettingValue<int64_t>("pickup-count-release");

        this->createObjectsFromString(buildOptionString(px, -1), true, true);
        this->createObjectsFromString(buildPickupString(px, itemID, count2), true, true);
    }
}
