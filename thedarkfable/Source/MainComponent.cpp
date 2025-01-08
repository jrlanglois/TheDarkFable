#include "MainComponent.h"

MainComponent::MainComponent()
{
    worldState.addListener (this);
    editor.setSize (1024, 1024);
    viewport.setViewedComponent (&editor, false);

    worldStateEditor.addPropertyParser (std::make_unique<DifficultyPropertyParser>());
    worldStateEditor.addPropertyParser (std::make_unique<DoorLockStatePropertyParser>());
    worldStateEditor.addPropertyParser (std::make_unique<MaterialPropertyParser>());
    worldStateEditor.addPropertyParser (std::make_unique<MoveTypePropertyParser>());
    worldStateEditor.addPropertyParser (std::make_unique<StatusConditionPropertyParser>());

    worldStateEditor.translateIdToString = darkEngine::getEquivalentName;

    tabbedComp.addTab (TRANS ("Game Map"), Colours::black, &viewport, false);
    tabbedComp.addTab (TRANS ("World State"), Colours::black, &worldStateEditor, false);

    addAndMakeVisible (tabbedComp);
    setSize (800, 800);

    gameMap.getState().appendChild (FightingMove ("move").getState(), nullptr);
    gameMap.getState().appendChild (FightableEntity ("enemy", false).getState(), nullptr);

DBG (gameMap.toJSONString());

    triggerAsyncUpdate();
}

MainComponent::~MainComponent()
{
    worldState.removeListener (this);
    undoManager.clearUndoHistory(); // Do this explicitly because of the destruction order.
}

//==============================================================================
Rectangle<int> MainComponent::calculateMapBounds() const
{
    RectangleList<int> list;

    for (const auto& c : worldState)
    {
        const auto rect = WorldObject (c).getDimensions();
        if (! rect.isEmpty() || rect.isFinite())
            list.addWithoutMerging (rect);
    }

    return list.getBounds();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void MainComponent::resized()
{
    tabbedComp.setBounds (getLocalBounds().reduced (8));
}

void MainComponent::handleAsyncUpdate()
{
    editor.setBounds (calculateMapBounds());
}

void MainComponent::valueTreeChildAdded (ValueTree&, ValueTree&)
{
    triggerAsyncUpdate();
}

void MainComponent::valueTreeChildRemoved (ValueTree&, ValueTree&, int)
{
    triggerAsyncUpdate();
}
