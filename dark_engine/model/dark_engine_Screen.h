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
    virtual bool goBack() { return false; }

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

        if (newScreen != nullptr)
        {
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
    }

    /** @returns */
    [[nodiscard]] int getNumScreens() const noexcept                { return screens.size(); }
    /** @returns */
    [[nodiscard]] TextScreen* getScreen (int index) const noexcept  { return screens[index]; }
    /** */
    void remove (int index)                                         { screens.remove (index); }
    /** */
    void remove (TextScreen* screen)                                { screens.removeObject (screen); }
    /** */
    void pop (int amount = 1)                                       { screens.removeLast (amount); }

private:
    OwnedArray<TextScreen> screens;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextScreenStack)
};
