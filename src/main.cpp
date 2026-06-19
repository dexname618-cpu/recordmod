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

    void handleAction(bool push, bool p1) {
        LevelEditorLayer::handleAction(push, p1);

        if (!Mod::get()->getSettingValue<bool>("enabled")) return;
        if (!p1) return;

        float px   = getPlayerX();
        int itemID = (int)Mod::get()->getSettingValue<int64_t>("pickup-item-id");

        if (push) {
            if (m_fields->isHolding) return;
            m_fields->isHolding = true;

            int count = (int)Mod::get()->getSettingValue<int64_t>("pickup-count-press");
            this->createObjectsFromString(buildOptionString(px, 1), true, true);
            this->createObjectsFromString(buildPickupString(px, itemID, count), true, true);
        } else {
            if (!m_fields->isHolding) return;
            m_fields->isHolding = false;

            int count = (int)Mod::get()->getSettingValue<int64_t>("pickup-count-release");
            this->createObjectsFromString(buildOptionString(px, -1), true, true);
            this->createObjectsFromString(buildPickupString(px, itemID, count), true, true);
        }
    }
};
