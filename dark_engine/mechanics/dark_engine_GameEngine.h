


/** */
class GameMap final : public EngineObject
{
public:
    /** */
    GameMap (UndoManager* undoManager = nullptr) :
        EngineObject (mapId, undoManager)
    {
    }

    /** */
    void setWorldObject (WorldObject& wo, Point<int> position, UndoManager* undoManager = nullptr)
    {
        wo.setPosition (position);
        state.appendChild (wo.getState(), undoManager);
    }

    /** */
    void setWorldObject (WorldObject& wo, int x, int y, UndoManager* undoManager = nullptr)
    {
        setWorldObject (wo, { x, y }, undoManager);
    }

    /** @returns */
    // [[nodiscard]] static std::unique_ptr<GameMap> parse (const var&);

    /** @returns */
    // [[nodiscard]] static std::unique_ptr<GameMap> parse (const File&);

    /** @returns */
    [[nodiscard]] Result save (const File& dest) const
    {
        if (dest.replaceWithText (toXmlString()))
            return Result::ok();

        return Result::fail (TRANS ("Failed to save!"));
    }

    CREATE_INLINE_CLASS_IDENTIFIER (map)

private:
    Player* player = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameMap)
};


