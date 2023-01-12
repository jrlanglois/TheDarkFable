#pragma once

#include <JuceHeader.h>

using namespace darkEngine;

//==============================================================================
class GameMapTile final : public Component
{
public:
    GameMapTile()
    {
        setSize (tileSizePx, tileSizePx);
    }

    //==============================================================================
    static inline constexpr auto tileSizePx = 32;

    //==============================================================================
    void setWorldObject (const WorldObject& wo, UndoManager* undoManager)
    {
        state = wo.getState();
        worldObject.reset (new WorldObject (state, undoManager));

        setTopLeftPosition (worldObject->getPosition());
    }

    //==============================================================================
    void paint (Graphics&) override
    {
    }

    void resized() override
    {
    }

private:
    //==============================================================================
    std::unique_ptr<WorldObject> worldObject;
    ValueTree state;

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

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameMapEditorComponent)
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
    GameProcessor gameProcessor;
    UndoManager undoManager;
    GameMap gameMap { &undoManager };
    ValueTree gameMapState { gameMap.getState() };
    GameMapEditorComponent editor { gameMap };
    Viewport viewport;

    //==============================================================================
    Rectangle<int> calculateMapBounds() const;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
