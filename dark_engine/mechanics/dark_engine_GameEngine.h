/** */
class Weather final : public EngineObject
{
public:
    /** */
    Weather (const Identifier& id, UndoManager* undoManager = nullptr) :
        EngineObject (id, undoManager)
    {
        setupPropAndCache (undoManager);
    }

    //==============================================================================
    /** */
    EngineObject& setWeatherType (WeatherType weatherTypeToUse, UndoManager* undoManager = nullptr)
    {
        weatherType.setValue (weatherTypeToUse, undoManager);
        return *this;
    }

    /** @returns */
    [[nodiscard]] WeatherType getWeatherType() const noexcept { return weatherType.get(); }

    //==============================================================================
    /** @returns */
    [[nodiscard]] double getWindDirectionDegrees() const noexcept { return windDirection.get(); }

    /** */
    EngineObject& setWindDirection (double newDirectionDeg, UndoManager* undoManager = nullptr)
    {
        windDirection.setValue (snapAngleToWorld (newDirectionDeg), undoManager);
        return *this;
    }

    /** */
    EngineObject& setWindDirection (CardinalDirection cd, UndoManager* undoManager = nullptr)
    {
        setWindDirection (toDegrees (cd), undoManager);
        return *this;
    }

    /** */
    [[nodiscard]] String getDisplayWindDirection() const
    {
        const auto degs = getWindDirectionDegrees();
        if (approximatelyEqual (degs, toDegrees (CardinalDirection::notApplicable)))    return TRANS ("omnidirectional");
        else if (approximatelyEqual (degs, toDegrees (CardinalDirection::north)))       return TRANS ("north");
        else if (approximatelyEqual (degs, toDegrees (CardinalDirection::east)))        return TRANS ("east");
        else if (approximatelyEqual (degs, toDegrees (CardinalDirection::south)))       return TRANS ("south");
        else if (approximatelyEqual (degs, toDegrees (CardinalDirection::west)))        return TRANS ("west");

        return String (degs);
    }

private:
    //==============================================================================
    CachedValue<double> windDirection;
    CachedValue<WeatherType> weatherType;

    void setupPropAndCache (UndoManager* undoManager)
    {
        EngineObject::setupPropAndCache (windDirection, moveTypeId, toDegrees (CardinalDirection::north), undoManager);
        EngineObject::setupPropAndCache (weatherType, moveCategoryId, WeatherType::clear, undoManager);
    }
};

/** An object that represents a state of an area, be it a room or a large environment,
    all the while containing NPCs, a reference to the player, objects of sorts, etc...

    @todo Move the Player to where a "starting point" should be upon load.

    @see GameProcessor, Player
*/
class GameMap final : public EngineObject
{
public:
    /** */
    GameMap (Player& player_, UndoManager* undoManager = nullptr) :
        EngineObject (gameMapId, undoManager),
        player (player_)
    {
        world.appendChild (player.getState(), undoManager);

        world.appendChild (testStairTileBlocked.getState(), undoManager);
        world.appendChild (testStairTileUp.getState(), undoManager);
        world.appendChild (testStairTileDown .getState(), undoManager);
        world.appendChild (testDoorTileUnlocked.getState(), undoManager);
        world.appendChild (testDoorTileNeedsKey.getState(), undoManager);
        world.appendChild (testDoorTileNeedsSpell.getState(), undoManager);
        world.appendChild (testDoorTileImpassable.getState(), undoManager);
        world.appendChild (testSecretDoorTileUnlocked.getState(), undoManager);
        world.appendChild (testSecretDoorTileNeedsKey.getState(), undoManager);
        world.appendChild (testSecretDoorTileNeedsSpell .getState(), undoManager);
        world.appendChild (testSecretDoorTileImpassable .getState(), undoManager);
        world.appendChild (testWallTile1.getState(), undoManager);
        world.appendChild (testWallTile2.getState(), undoManager);

        definitions.appendChild (enemies, undoManager);
        definitions.appendChild (weapons, undoManager);
        definitions.appendChild (npcs, undoManager);
        definitions.appendChild (moves, undoManager);
        definitions.appendChild (inanimateObjects, undoManager);

        state.appendChild (definitions, undoManager);
        state.appendChild (world, undoManager);
    }

    //==============================================================================
    /** @returns */
    [[nodiscard]] ValueTree getWorldState() const noexcept              { return world; }
    /** @returns */
    [[nodiscard]] ValueTree getDefinitionsState() const noexcept        { return definitions; }

    /** @returns */
    [[nodiscard]] ValueTree getEnemiesState() const noexcept            { return enemies; }
    /** @returns */
    [[nodiscard]] ValueTree getWeaponsState() const noexcept            { return weapons; }
    /** @returns */
    [[nodiscard]] ValueTree getNPCState() const noexcept                { return npcs; }
    /** @returns */
    [[nodiscard]] ValueTree getMovesState() const noexcept              { return moves; }
    /** @returns */
    [[nodiscard]] ValueTree getInanimateObjectsState() const noexcept   { return inanimateObjects; }

    /** @returns */
    [[nodiscard]] Player& getPlayer() noexcept                          { return player; }
    /** @returns */
    [[nodiscard]] const Player& getPlayer() const noexcept              { return player; }

    //==============================================================================
    /** */
    GameMap& setWorldObject (WorldObject& wo, Point<int> position, UndoManager* undoManager = nullptr)
    {
        wo.setPosition (position);
        world.appendChild (wo.getState(), undoManager);
        return *this;
    }

    /** */
    GameMap& setWorldObject (WorldObject& wo, int x, int y, UndoManager* undoManager = nullptr)
    {
        setWorldObject (wo, { x, y }, undoManager);
        return *this;
    }

private:
    //==============================================================================
    ValueTree world { worldId },
              definitions { definitionsId },
              enemies { enemiesId },
              weapons { weaponsId },
              npcs { npcsId },
              moves { movesId },
              inanimateObjects { inanimateObjectsId };

    Player& player;

    StairTile testStairTileBlocked { StairTile::Direction::blocked },
              testStairTileUp { StairTile::Direction::up },
              testStairTileDown { StairTile::Direction::down };

    DoorTile testDoorTileUnlocked { DoorLockState::unlocked },
             testDoorTileNeedsKey { DoorLockState::needsKey },
             testDoorTileNeedsSpell { DoorLockState::needsSpell },
             testDoorTileImpassable { DoorLockState::impassable };

    DoorTile testSecretDoorTileUnlocked { DoorLockState::unlocked, true },
             testSecretDoorTileNeedsKey { DoorLockState::needsKey, true },
             testSecretDoorTileNeedsSpell { DoorLockState::needsSpell, true },
             testSecretDoorTileImpassable { DoorLockState::impassable, true };

    WallTile testWallTile1 { Material::vinyl, Colours::white },
             testWallTile2 { Material::ooze, Colours::red };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameMap)
};

//==============================================================================
/** */
class WorldEntityDefinitionParser
{
public:
    /** */
    WorldEntityDefinitionParser() = default;
    /** */
    virtual ~WorldEntityDefinitionParser() = default;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WorldEntityDefinitionParser)
};
