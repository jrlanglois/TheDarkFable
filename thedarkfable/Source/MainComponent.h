#pragma once

#include "DarkFableLookAndFeel.h"

using namespace darkEngine;

//==============================================================================
/** @todo
    - On right click, allow setting type
    - On hover, change property window

*/
class GameMapTile final : public TextButton
{
public:
    GameMapTile()
    {
        setSize (tileSizePx, tileSizePx);
        update();
    }

    //==============================================================================
    void setWorldObject (const WorldObject& wo, UndoManager* undoManager)
    {
        state = wo.getState();
        worldObject.reset (new WorldObject (state, undoManager));
        update();
    }

    //==============================================================================
    static inline constexpr auto tileSizePx = 32;

private:
    //==============================================================================
    std::unique_ptr<WorldObject> worldObject;
    ValueTree state;

    //==============================================================================
    void update()
    {
        const auto objName = [this]()
        {
            return worldObject != nullptr
                    ? worldObject->getName()
                    : TRANS ("(Empty)");
        }();

        setName (objName);
        setTitle (objName);

        if (worldObject != nullptr)
        {
            setDescription (worldObject->getDescription());
            setTopLeftPosition (worldObject->getPosition());
        }
        else
        {
            setDescription ({});
            setTopLeftPosition ({});
        }

        auto bt = objName;

        if (worldObject != nullptr)
        {
            if (const auto id = worldObject->getIdentifier(); id.isValid())
                bt << " { id: " << id << " }";

            if (const auto iid = worldObject->getInteractionId(); iid.isNotEmpty())
                bt << " { iid: " << iid << " }";
        }

        setButtonText (bt.trim());
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameMapTile)
};

//==============================================================================
class GameMapEditorComponent final : public Component
{
public:
    GameMapEditorComponent (GameMap& gm) :
        gameMap (gm)
    {
    }

    //==============================================================================
    void resized() override
    {
    }

private:
    //==============================================================================
    GameMap& gameMap;
    OwnedArray<GameMapTile> tiles;

    Grid grid;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameMapEditorComponent)
};

//==============================================================================
class ObjectEditor : public Component
{
public:
    ObjectEditor ()
    {
    }

    ~ObjectEditor() override
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObjectEditor)
};

//==============================================================================
class MainComponent final : public Component,
                            public ValueTree::Listener,
                            public AsyncUpdater
{
public:
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override;
    void handleAsyncUpdate() override;

private:
    //==============================================================================
    UndoManager undoManager;
    GameProcessor gameProcessor;
    GameMap& gameMap = gameProcessor.gameMap;
    ValueTree worldState = gameMap.getWorldState();
    GameMapEditorComponent editor { gameMap };
    Viewport viewport;

    ValueTreeEditor worldStateEditor { worldState };

    TabbedComponent tabbedComp { TabbedButtonBar::TabsAtTop };

    //==============================================================================
    Rectangle<int> calculateMapBounds() const;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
