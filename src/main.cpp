#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

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

class $modify(MyPlayerObject, PlayerObject) {

    struct Fields {
        bool isHolding = false;
    };

    bool pushButton(PlayerButton button) {
        bool result = PlayerObject::pushButton(button);

        if (button != PlayerButton::Jump) return result;
        if (!Mod::get()->getSettingValue<bool>("enabled")) return result;

        // Перевіряємо що m_gameLayer це саме LevelEditorLayer
        auto* editor = typeinfo_cast<LevelEditorLayer*>(m_gameLayer);
        if (!editor) return result;
        if (editor->m_player1 != this) return result;

        if (m_fields->isHolding) return result;
        m_fields->isHolding = true;

        float px   = this->getPositionX();
        int itemID = (int)Mod::get()->getSettingValue<int64_t>("pickup-item-id");

        editor->createObjectsFromString(buildOptionString(px, -1), true, true);
        editor->createObjectsFromString(buildPickupString(px, itemID, 1), true, true);

        return result;
    }

    bool releaseButton(PlayerButton button) {
        bool result = PlayerObject::releaseButton(button);

        if (button != PlayerButton::Jump) return result;
        if (!Mod::get()->getSettingValue<bool>("enabled")) return result;

        auto* editor = typeinfo_cast<LevelEditorLayer*>(m_gameLayer);
        if (!editor) return result;
        if (editor->m_player1 != this) return result;

        if (!m_fields->isHolding) return result;
        m_fields->isHolding = false;

        float px   = this->getPositionX();
        int itemID = (int)Mod::get()->getSettingValue<int64_t>("pickup-item-id");

        editor->createObjectsFromString(buildOptionString(px, 1), true, true);
        editor->createObjectsFromString(buildPickupString(px, itemID, 0), true, true);

        return result;
    }
};
