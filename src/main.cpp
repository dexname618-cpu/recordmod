#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

static bool g_recordMode = false;

void spawnToggleTrigger(bool p1On) {
    auto* editor = LevelEditorLayer::get();
    if (!editor) return;

    auto* player = editor->m_player1;
    if (!player) return;

    auto pos = player->getPosition();

    auto data = fmt::format(
        "1,2899,2,{},3,{},36,1,165,{};",
        (int)pos.x,
        (int)pos.y,
        p1On ? 1 : -1
    );

    auto* objs = editor->createObjectsFromString(
        gd::string(data), true, true
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

        auto* menu = CCMenu::create();

        auto* btn = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(MyEditorUI::onRecordToggle),
            0.7f
        );

        btn->m_onButton->setColor({0, 255, 0});
        btn->m_offButton->setColor({180, 180, 180});

        m_fields->m_recordBtn = btn;
        menu->addChild(btn);
        menu->setPosition(ccp(230, 570));
        this->addChild(menu);

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
}
