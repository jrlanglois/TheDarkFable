#include "MainComponent.h"

MainComponent::MainComponent()
{
    gameMapState.addListener (this);
    editor.setSize (1024, 1024);

    viewport.setViewedComponent (&editor, false);
    addAndMakeVisible (viewport);

    setSize (800, 800);
}

MainComponent::~MainComponent()
{
    gameMapState.removeListener (this);
    undoManager.clearUndoHistory(); // Do this explicitly because of destruction...
}

//==============================================================================
Rectangle<int> MainComponent::calculateMapBounds() const
{
    int top = 0, bottom = 0,
        left = 0, right = 0;

    for (const auto& c : gameMapState)
    {
        if (! c.hasProperty (WorldObject::positionId))
            continue;

        const auto pos = WorldObject (c).getPosition();
        top     = jmin (pos.y, top);
        bottom  = jmax (pos.y, top);
        left    = jmin (pos.x, top);
        right   = jmax (pos.x, top);
    }

    auto b = Rectangle<int>::leftTopRightBottom (left, top, right, bottom);

    const auto pos = b.getPosition();
    if (pos.y < 0 || pos.x < 0)
        b.translate (std::abs (pos.x), std::abs (pos.y));

    return b;
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void MainComponent::resized()
{
    viewport.setBounds (getLocalBounds().reduced (8));
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
