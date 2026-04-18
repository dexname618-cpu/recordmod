#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <string>

using namespace geode::prelude;

static bool g_recordMode = false;
static bool g_freezeCount = false;

void spawnToggleTrigger(bool p1On) {
    auto* editor = LevelEditorLayer::get();
    if (!editor) return;

    auto* player = editor->m_player1;
    if (!player) return;

    auto* objLayer = editor->m_objectLayer;
    if (!objLayer) return;

    auto worldPos = objLayer->convertToNodeSpace(
        player->convertToWorldSpace(ccp(0, 0))
    );

    auto data = gd::string(
        std::string("1,2899,2,") +
        std::to_string((int)worldPos.x) + ",3," +
        std::to_string((int)worldPos.y) + ",36,1,165," +
        std::to_string(p1On ? 1 : -1) + ";"
    );

    auto* objs = editor->createObjectsFromString(
        data, true, true
    );

    if (objs) {
        for (auto* obj : CCArrayExt<GameObject*>(objs)) {
            obj->setPosition(worldPos);
        }
    }
}

class $modify(MyLevelEditorLayer, LevelEditorLayer) {
    void updateEditorLabel() {
        if (g_freezeCount) return;
        LevelEditorLayer::updateEditorLabel();
    }
};

class $modify(MyEditorUI, EditorUI) {
    struct Fields {
        CCMenuItemToggler* m_recordBtn = nullptr;
        CCMenuItemToggler* m_freezeBtn = nullptr;
    };

    bool init(LevelEditorLayer* lel) {
        if (!EditorUI::init(lel)) return false;

        // Кнопка запису (зелена)
        auto* recordBtn = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(MyEditorUI::onRecordToggle),
            0.7f
        );
        recordBtn->m_onButton->setColor({0, 255, 0});
        recordBtn->m_offButton->setColor({180, 180, 180});
        m_fields->m_recordBtn = recordBtn;

        auto* menu1 = CCMenu::create();
        menu1->setPosition(ccp(192, 301));
        menu1->addChild(recordBtn);
        this->addChild(menu1, 10);

        // Кнопка заморозки (помаранчева) — правіше
        auto* freezeBtn = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(MyEditorUI::onFreezeToggle),
            0.7f
        );
        freezeBtn->m_onButton->setColor({255, 165, 0});
        freezeBtn->m_offButton->setColor({180, 180, 180});
        m_fields->m_freezeBtn = freezeBtn;

        auto* menu2 = CCMenu::create();
        menu2->setPosition(ccp(222, 301));
        menu2->addChild(freezeBtn);
        this->addChild(menu2, 10);

        return true;
    }

    void onRecordToggle(CCObject* sender) {
        g_recordMode = !g_recordMode;
        log::info("[RecordMod] Record mode: {}", g_recordMode);
    }

    void onFreezeToggle(CCObject* sender) {
        g_freezeCount = !g_freezeCount;
        log::info("[RecordMod] Freeze count: {}", g_freezeCount);
    }
};

class $modify(GJBaseGameLayer) {
    void handleButton(bool push, int button, bool isPlayer1) {
        GJBaseGameLayer::handleButton(push, button, isPlayer1);

        if (!g_recordMode) return;
        if (!LevelEditorLayer::get()) return;
        if (button != 1) return;
        if (!isPlayer1) return;

        spawnToggleTrigger(!push);
    }
};
