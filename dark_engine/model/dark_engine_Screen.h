/** */
class TextScreen : public Identifiable
{
public:
    /** */
    TextScreen (const Identifier& id) :
        Identifiable (id)
    {
    }

    /** */
    virtual void goBack() { }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextScreen)
};

/** */
class TextScreenStack
{
public:
    /** */
    TextScreenStack()
    {
    }

    /** */
    void add (std::unique_ptr<TextScreen> newScreen)
    {
        jassert (newScreen != nullptr);
        if (newScreen == nullptr)
            return;

        for (auto* s : screens)
        {
            if (s->getUuid() == newScreen->getUuid())
            {
                jassertfalse;
                return;
            }
        }

        screens.add (newScreen.release());
    }

    /** */
    int getNumScreens() const           { return screens.size(); }
    /** */
    void remove (int index)             { screens.remove (index); }
    /** */
    void remove (TextScreen* screen)    { screens.removeObject (screen); }
    /** */
    void pop (int amount = -1)          { screens.removeLast (amount); }

private:
    OwnedArray<TextScreen> screens;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextScreenStack)
};
