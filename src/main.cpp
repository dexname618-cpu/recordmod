#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <string>

using namespace geode::prelude;

static bool g_recordMode = false;

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
        CCPoint pos = ccp(192, 301);
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
