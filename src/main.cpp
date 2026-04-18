#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <string>

using namespace geode::prelude;

static bool g_recordMode = false;

void spawnToggleTrigger(bool p1On) {
    auto* editor = LevelEditorLayer::get();
    if (!editor) return;

    auto* player = editor->m_player1;
    if (!player) return;

    auto pos = player->getPosition();

    auto data = gd::string(
        std::string("1,2899,2,") +
        std::to_string((int)pos.x) + ",3," +
        std::to_string((int)pos.y) + ",36,1,165," +
        std::to_string(p1On ? 1 : -1) + ";"
    );

    auto* objs = editor->createObjectsFromString(
        data, true, true
    );

    if (objs) {
        for (auto* obj : CCArrayExt<GameObject*>(objs)) {
            editor->addToSection(obj);
        }
    }

    log::info("[RecordMod] Spawned trigger at {},{} P1={}",
        (int)pos.x, (int)pos.y, p1On ? "ON" : "OFF"
    );
}

class $modify(MyEditorUI, EditorUI) {
    struct Fields {
        CCMenuItemToggler* m_recordBtn = nullptr;
    };

    bool init(LevelEditorLayer* lel) {
        if (!EditorUI::init(lel)) return false;

        auto* btn = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(MyEditorUI::onRecordToggle),
            0.7f
        );

        btn->m_onButton->setColor({0, 255, 0});
        btn->m_offButton->setColor({180, 180, 180});

        m_fields->m_recordBtn = btn;

        auto* musicBtn = this->getChildByID("music-button");
        CCPoint pos = ccp(160, 145);
        if (musicBtn) {
            pos = musicBtn->getPosition() + ccp(60, 0);
        }

        auto* menu = CCMenu::create();
        menu->setPosition(pos);
        menu->addChild(btn);
        this->addChild(menu, 10);

        return true;
    }

    void onRecordToggle(CCObject* sender) {
        g_recordMode = !g_recordMode;
        log::info("[RecordMod] Record mode: {}", g_recordMode);
    }
};

class $modify(PlayerObject) {
    void pushButton(PlayerButton btn) {
        PlayerObject::pushButton(btn);
        if (!g_recordMode) return;
        if (btn != PlayerButton::Jump) return;
        if (!LevelEditorLayer::get()) return;
        spawnToggleTrigger(false);
    }

    void releaseButton(PlayerButton btn) {
        PlayerObject::releaseButton(btn);
        if (!g_recordMode) return;
        if (btn != PlayerButton::Jump) return;
        if (!LevelEditorLayer::get()) return;
        spawnToggleTrigger(true);
    }
};
