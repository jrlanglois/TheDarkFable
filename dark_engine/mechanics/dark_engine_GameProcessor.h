namespace
{
    enum class GameCommandIDs
    {
        pickUp,
        talk,
        push,
        pull,
        
    };
}

/** */
class GameProcessor final
{
public:
    /** */
    GameProcessor (UndoManager* undoManager = nullptr) :
        player (CardinalDirection::north, undoManager),
        gameMap (player, undoManager)
    {
    }

    std::optional<Result> processCheatMessage (const StringArray& parts)
    {
        const auto& command = parts.getReference (0);
        auto matches = [&] (StringRef c)
        {
            return command.equalsIgnoreCase (c);
        };

        if (matches ("kill"))
        {
            jassertfalse;
        }
        else if (matches ("add"))
        {
            jassertfalse; // add {item id} {x, y}
        }
        else if (matches ("remove"))
        {
            jassertfalse; // remove {item id} {x, y}
        }
        else if (matches ("set"))
        {
            /**
                set {id} {v}
            */
            jassertfalse;
        }
        else if (matches ("player"))
        {
            if (matches ("tgm"))
            {
                jassertfalse;
            }
            else if (matches ("add"))
            {
                jassertfalse;
            }
            else if (matches ("remove"))
            {
                jassertfalse;
            }
            else if (matches ("set"))
            {
                /**
                    player set {id} {v}
                    player set level {v}
                */
                jassertfalse;
            }
        }

        return Result::ok();
    }

    Result processMessage (String message)
    {
        const auto parts = [&]()
        {
            auto toks = StringArray::fromTokens (toLowerCase (message), " ", "\"'");
            toks.trim();
            toks.removeEmptyStrings (true);
            toks.minimiseStorageOverheads();
            return toks;
        }();

        if (parts.isEmpty())
        {
            jassertfalse; // fail + return commands
        }

        const auto& command = parts.getReference (0);
        auto matches = [&] (StringRef c)
        {
            return command.equalsIgnoreCase (c);
        };

        if (matches ("help"))
        {
            jassertfalse;
        }
        else if (matches ("quit")
                 || matches ("exit"))
        {
            jassertfalse;
        }
        else if (matches ("move"))
        {
            jassertfalse;
        }
        else
        {
            if (allowCheats)
                if (const auto r = processCheatMessage (parts); r.has_value())
                    return *r;

            return Result::fail (TRANS ("You can't do that here..."));
        }

        return Result::ok();
    }

    bool allowCheats = true;
    Player player;
    GameMap gameMap;

private:
    /** TODO:
        Create a giant list of GameMap objects, 1:1 with each room/environment.
        They should be called up via a static UUID or some other identifier.
    */
    // OwnedArray<GameMap> gameMaps;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameProcessor)
};
