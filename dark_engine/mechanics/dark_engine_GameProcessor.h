/** */
class GameProcessor final
{
public:
    /** */
    GameProcessor()
    {
    }

    void draw();

private:
    OwnedArray<GameMap> gameMaps;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameProcessor)
};
