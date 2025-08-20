#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <geode.custom-keybinds/include/Keybinds.hpp> // I love this mod
#include <geode/modify/EditorUI.hpp>
#include <Geode/utils/general.hpp>

$execute {
    keybinds::BindManager::get()->registerBindable(
        keybinds::BindableAction(
            "decrease_trigger_duration_large"_spr, //ID
            "Decrease Duration - Large", // Name
            "", // Description, leave empty for none
            {}, // Default binds
            "Trigger Duration" // Category
        )
    );
    keybinds::BindManager::get()->registerBindable(
        keybinds::BindableAction(
            "decrease_trigger_duration_medium"_spr, //ID
            "Decrease Duration - Medium", // Name
            "", // Description, leave empty for none
            {keybinds::Keybind::create(KEY_Five, keybinds::Modifier::None)}, // Default binds
            "Trigger Duration" // Category
        )
    );
    keybinds::BindManager::get()->registerBindable(
        keybinds::BindableAction(
            "decrease_trigger_duration_small"_spr, //ID
            "Decrease Duration - Small", // Name
            "", // Description, leave empty for none
            {keybinds::Keybind::create(KEY_Five, keybinds::Modifier::Shift)}, // Default binds
            "Trigger Duration" // Category
        )
    );
    keybinds::BindManager::get()->registerBindable(
        keybinds::BindableAction(
            "increase_trigger_duration_large"_spr, //ID
            "Increase Duration - Large", // Name
            "", // Description, leave empty for none
            {}, // Default binds
            "Trigger Duration" // Category
        )
    );
    keybinds::BindManager::get()->registerBindable(
        keybinds::BindableAction(
            "increase_trigger_duration_medium"_spr, //ID
            "Increase Duration - Medium", // Name
            "", // Description, leave empty for none
            {keybinds::Keybind::create(KEY_Six, keybinds::Modifier::None)}, // Default binds
            "Trigger Duration" // Category
        )
    );
    keybinds::BindManager::get()->registerBindable(
        keybinds::BindableAction(
            "increase_trigger_duration_small"_spr, //ID
            "Increase Duration - Small", // Name
            "", // Description, leave empty for none
            {keybinds::Keybind::create(KEY_Six, keybinds::Modifier::Shift)}, // Default binds
            "Trigger Duration" // Category
        )
    );
    keybinds::BindManager::get()->registerBindable(
        keybinds::BindableAction(
            "copy_trigger_duration"_spr, //ID
            "Copy Duration", // Name
            "", // Description, leave empty for none
            {keybinds::Keybind::create(KEY_Eight, keybinds::Modifier::None)}, // Default binds
            "Trigger Duration" // Category
        )
    );
    keybinds::BindManager::get()->registerBindable(
        keybinds::BindableAction(
            "paste_trigger_duration"_spr, //ID
            "Paste Duration", // Name
            "", // Description, leave empty for none
            {keybinds::Keybind::create(KEY_Seven, keybinds::Modifier::None)}, // Default binds
            "Trigger Duration" // Category
        )
    );
}

float copied_trigger_duration = 0.0f; // this is global for a reason and i refuse to make a namespace for a single variable

class $modify(TDKEditorUI, EditorUI) {

    // "getting mod -> setting values in fields is thread unsafe" - preveter
    // "preveter can eat my fields" - TheFriendlyFoe
    
    struct Fields { 
        float small_duration = Mod::get()->getSettingValue<double>("small_duration");
        float medium_duration = Mod::get()->getSettingValue<double>("medium_duration");
        float large_duration = Mod::get()->getSettingValue<double>("large_duration");
    };

    void change_trigger_durations(float duration) {
        for (auto obj : CCArrayExt<GameObject*>(getSelectedObjects())) {
            auto trigger = typeinfo_cast<EffectGameObject*>(obj);
            if (!trigger) continue;
            trigger->m_duration = std::max(0.0f, trigger->m_duration + duration);
        }
        m_editorLayer->m_drawGridLayer->m_updateTimeMarkers = true; // this is how one updates the drawing of duration lines
    }

    bool init(LevelEditorLayer* p0) {
        if (!EditorUI::init(p0)) return false;
        
        // duration increases / decreases

        this->template addEventListener<keybinds::InvokeBindFilter>([=](keybinds::InvokeBindEvent* event) {
            if (event->isDown()) {
                log::info("decrease large");
                change_trigger_durations(-m_fields->large_duration);
            }
            return ListenerResult::Propagate;
        }, "decrease_trigger_duration_large"_spr);

        this->template addEventListener<keybinds::InvokeBindFilter>([=](keybinds::InvokeBindEvent* event) {
            if (event->isDown()) {
                log::info("decrease medium");
                change_trigger_durations(-m_fields->medium_duration);
            }
            return ListenerResult::Propagate;
        }, "decrease_trigger_duration_medium"_spr);

        this->template addEventListener<keybinds::InvokeBindFilter>([=](keybinds::InvokeBindEvent* event) {
            if (event->isDown()) {
                log::info("decrease small");
                change_trigger_durations(-m_fields->small_duration);
            }
            return ListenerResult::Propagate;
        }, "decrease_trigger_duration_small"_spr);

        this->template addEventListener<keybinds::InvokeBindFilter>([=](keybinds::InvokeBindEvent* event) {
            if (event->isDown()) {
                log::info("increase large");
                change_trigger_durations(m_fields->large_duration);
            }
            return ListenerResult::Propagate;
        }, "increase_trigger_duration_large"_spr);

        this->template addEventListener<keybinds::InvokeBindFilter>([=](keybinds::InvokeBindEvent* event) {
            if (event->isDown()) {
                log::info("increase medium");
                change_trigger_durations(m_fields->medium_duration);
            }
            return ListenerResult::Propagate;
        }, "increase_trigger_duration_medium"_spr);

        this->template addEventListener<keybinds::InvokeBindFilter>([=](keybinds::InvokeBindEvent* event) {
            if (event->isDown()) {
                log::info("increase small");
                change_trigger_durations(m_fields->small_duration);
            }
            return ListenerResult::Propagate;
        }, "increase_trigger_duration_small"_spr);

        // duration copy / paste
        
        this->template addEventListener<keybinds::InvokeBindFilter>([=](keybinds::InvokeBindEvent* event) {
            if (event->isDown()) {
                log::info("copy");
                for (auto obj : CCArrayExt<GameObject*>(getSelectedObjects())) {
                    auto trigger = typeinfo_cast<EffectGameObject*>(obj);
                    if (!trigger) continue;
                    copied_trigger_duration = trigger->m_duration;
                    break;
                }
            }
            return ListenerResult::Propagate;
        }, "copy_trigger_duration"_spr);

        this->template addEventListener<keybinds::InvokeBindFilter>([=](keybinds::InvokeBindEvent* event) {
            if (event->isDown()) {
                log::info("paste");
                for (auto obj : CCArrayExt<GameObject*>(getSelectedObjects())) {
                    auto trigger = typeinfo_cast<EffectGameObject*>(obj);
                    if (!trigger) continue;
                    trigger->m_duration = copied_trigger_duration;
                }
                m_editorLayer->m_drawGridLayer->m_updateTimeMarkers = true;
            }
            return ListenerResult::Propagate;
        }, "paste_trigger_duration"_spr);

        return true;
    }
};