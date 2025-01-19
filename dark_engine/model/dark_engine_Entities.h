/** An entity in the game world.

    This is can be a player, an NPC, a tile, or a world object.

    It provides a direction, an inventory, a difficulty
    (which is intended to be used for difficulty scaling at the map's context).

    The subtype is intended to be used for special behaviour, specific to the subclasses.
    It should allow higher level APIs to reflect on the underlying ValueTree state
    to construct higher level behaviours, objects, etc, all without having to know
    the exact type of the entity.

    @see WorldObject, Difficulty, CardinalDirection
*/
class WorldEntity : public WorldObject
{
public:
    /** Constructs a basic world entity. */
    WorldEntity (const Identifier& id,
                 bool shouldBeAnNPC,
                 double directionDegrees,
                 UndoManager* undoManager = nullptr) :
        WorldObject (id)
    {
        setupPropAndCache (undoManager);

        setNPC (shouldBeAnNPC, undoManager);
        setDirection (snapAngleToWorld (directionDegrees), undoManager);
    }

    /** Constructs a north facing world entity. */
    WorldEntity (const Identifier& id,
                 bool shouldBeAnNPC,
                 CardinalDirection cd = CardinalDirection::north,
                 UndoManager* undoManager = nullptr) :
        WorldEntity (id, shouldBeAnNPC, toDegrees (cd), undoManager)
    {
    }

    //==============================================================================
    /** */
    WorldEntity& setSubtype (String subtypeIdToUse, UndoManager* undoManager = nullptr)
    {
        subtypeIdToUse = subtypeIdToUse.trim();

        jassert (subtypeIdToUse.isNotEmpty());

        if (subtypeIdToUse.isNotEmpty())
            subtype.setValue (subtypeIdToUse, undoManager);

        return *this;
    }

    /** @returns */
    [[nodiscard]] String getSubtype() const noexcept { return subtype.get(); }

    //==============================================================================
    /** @returns */
    [[nodiscard]] double getDirectionDegrees() const noexcept { return direction.get(); }

    /** */
    WorldEntity& setDirection (double newDirectionDeg, UndoManager* undoManager = nullptr)
    {
        direction.setValue (snapAngleToWorld (newDirectionDeg), undoManager);
        return *this;
    }

    /** */
    WorldEntity& setDirection (CardinalDirection cd, UndoManager* undoManager = nullptr)
    {
        setDirection (toDegrees (cd), undoManager);
        return *this;
    }

    /** */
    [[nodiscard]] String getDisplayDirection() const
    {
        const auto degs = getDirectionDegrees();
        if (approximatelyEqual (degs, toDegrees (CardinalDirection::notApplicable)))    return TRANS ("omnidirectional");
        else if (approximatelyEqual (degs, toDegrees (CardinalDirection::north)))       return TRANS ("north");
        else if (approximatelyEqual (degs, toDegrees (CardinalDirection::east)))        return TRANS ("east");
        else if (approximatelyEqual (degs, toDegrees (CardinalDirection::south)))       return TRANS ("south");
        else if (approximatelyEqual (degs, toDegrees (CardinalDirection::west)))        return TRANS ("west");

        return String (degs);
    }

    //==============================================================================
    /** @returns */
    [[nodiscard]] bool isNPC() const noexcept { return npc.get(); }
    /** */
    WorldEntity& setNPC (bool shouldBeAnNPC, UndoManager* undoManager = nullptr)
    {
        npc.setValue (shouldBeAnNPC, undoManager);
        return *this;
    }

    //==============================================================================
    /** */
    WorldEntity& addInventoryItem (const WorldObject& object, UndoManager* undoManager = nullptr)
    {
        inventory.appendChild (object.getState(), undoManager);
        return *this;
    }

    /** */
    WorldEntity& removeInventoryItem (const WorldObject& object, UndoManager* undoManager = nullptr)
    {
        inventory.removeChild (object.getState(), undoManager);
        return *this;
    }

    /** */
    WorldEntity& removeInventoryItem (int index, UndoManager* undoManager = nullptr)
    {
        inventory.removeChild (index, undoManager);
        return *this;
    }

    /** @returns */
    [[nodiscard]] int getNumInventoryItems() const noexcept                 { return inventory.getNumChildren(); }
    /** @returns */
    [[nodiscard]] WorldObject getInventoryItem (int index) const noexcept   { return inventory.getChild (index); }

    //==============================================================================
    /** @returns */
    [[nodiscard]] Difficulty getDifficulty() const noexcept  { return difficulty.get(); }
    /** */
    WorldObject& setDifficulty (Difficulty newDifficulty, UndoManager* undoManager = nullptr)
    {
        difficulty.setValue (newDifficulty, undoManager);
        return *this;
    }

    //==============================================================================
    /** @returns a weight in kilograms. */
    [[nodiscard]] double getWeight() const noexcept  { return weight.get(); }
    /** */
    WorldObject& setWeight (double newWeight, UndoManager* undoManager = nullptr)
    {
        weight.setValue (newWeight, undoManager);
        return *this;
    }

private:
    //==============================================================================
    CachedValue<String> subtype;
    CachedValue<bool> npc;
    CachedValue<double> direction, weight;
    ValueTree inventory { inventoryId };
    CachedValue<Difficulty> difficulty;

    //==============================================================================
    void setupPropAndCache (UndoManager* undoManager)
    {
        EngineObject::setupPropAndCache (npc, isNPCId, false, undoManager);
        EngineObject::setupPropAndCache (direction, directionId, 0.0, undoManager);
        EngineObject::setupPropAndCache (weight, weightId, 80.0, undoManager);
        EngineObject::setupPropAndCache (subtype, subtypeId, {}, undoManager);
        EngineObject::setupPropAndCache (difficulty, difficultyId, {}, undoManager);

        state.appendChild (inventory, undoManager);
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WorldEntity)
};

//==============================================================================
class TextTrigger
{
public:
    enum class Type
    {
        fullscreen,
        dialogPlain,
        dialogWithOptions,
    };

    TextTrigger()
    {
    }

private:
};

//==============================================================================
/** */
class FightingMove : public EngineObject
{
public:
    /** */
    FightingMove (const Identifier& id,
                  UndoManager* undoManager = nullptr) :
        EngineObject (id, undoManager)
    {
        setupPropAndCache (undoManager);
    }

    FightingMove (const FightingMove& other) :
        EngineObject (other.state.getType(), nullptr)
    {
        setupPropAndCache (nullptr);
    }

    FightingMove (const ValueTree& other) :
        EngineObject (other.getType(), nullptr)
    {
        setupPropAndCache (nullptr);
    }

    //==============================================================================
    #undef SET_GET
    #define SET_GET(Type, Name, varName, paramName) \
        private: \
            CachedValue<Type> varName; \
            \
        public: \
            FightingMove& set##Name (Type paramName, UndoManager* undoManager = nullptr) \
            { \
                varName.setValue (paramName, undoManager); \
                return *this; \
            } \
            \
            [[nodiscard]] Type get##Name() const noexcept { return varName.get(); }

    SET_GET (MoveType, MoveType, moveType, newMoveType)
    SET_GET (MoveCategory, MoveCategory, moveCategory, newMoveCategory)

    //==============================================================================
    #undef SET_GET
    #define SET_GET(Name, varName) \
        private: \
            CachedValue<int> varName; \
            \
        public: \
            FightingMove& set##Name (int new##Name, UndoManager* undoManager = nullptr) \
            { \
                varName.setValue (new##Name, undoManager); \
                return *this; \
            } \
            \
            [[nodiscard]] int get##Name() const noexcept { return varName.get(); }

    SET_GET (Priority, priority)
    SET_GET (Power, power)
    SET_GET (Accuracy, accuracy)
    SET_GET (PowerPoints, powerPoints)
    SET_GET (MaxPowerPoints, maxPowerPoints)

    #undef SET_GET

private:
    //==============================================================================
    void setupPropAndCache (UndoManager* undoManager)
    {
        EngineObject::setupPropAndCache (moveType, moveTypeId, MoveType::normal, undoManager);
        EngineObject::setupPropAndCache (moveCategory, moveCategoryId, MoveCategory::physical, undoManager);
        EngineObject::setupPropAndCache (priority, priorityId, 0, undoManager);
        EngineObject::setupPropAndCache (power, powerId, 40, undoManager);
        EngineObject::setupPropAndCache (accuracy, accuracyId, 100, undoManager);
        EngineObject::setupPropAndCache (powerPoints, powerPointsId, 5, undoManager);
        EngineObject::setupPropAndCache (maxPowerPoints, maxPowerPointsId, 10, undoManager);
    }
};

//==============================================================================
/** */
class FightableEntity : public WorldEntity
{
public:
    /** */
    FightableEntity (const Identifier& id,
                     bool shouldBeAnNPC,
                     double directionDegrees,
                     UndoManager* undoManager = nullptr) :
        WorldEntity (id, shouldBeAnNPC, directionDegrees, undoManager)
    {
        setupPropAndCache (undoManager);
    }

    /** */
    FightableEntity (const Identifier& id,
                     bool shouldBeAnNPC,
                     CardinalDirection cd = CardinalDirection::north,
                     UndoManager* undoManager = nullptr) :
        FightableEntity (id, shouldBeAnNPC, toDegrees (cd), undoManager)
    {
    }

    //==============================================================================
    #undef SET_GET
    #define SET_GET(Type, Name, varName, paramName) \
        private: \
            CachedValue<Type> varName; \
            \
        public: \
            FightableEntity& set##Name (Type paramName, UndoManager* undoManager = nullptr) \
            { \
                varName.setValue (paramName, undoManager); \
                return *this; \
            } \
            \
            [[nodiscard]] Type get##Name() const noexcept \
            { \
                return varName.get(); \
            }

    SET_GET (MoveType, WeakAgainstType, weakAgainstType, newWeakAgainstType)

    //==============================================================================
    #undef SET_GET
    #define SET_GET(ClassName, varName) \
        private: \
            CachedValue<ClassName> varName; \
            \
        public: \
            void set##ClassName (ClassName new##ClassName, UndoManager* undoManager = nullptr) \
            { \
                varName.setValue (new##ClassName, undoManager); \
            } \
            \
            [[nodiscard]] ClassName get##ClassName() const noexcept \
            { \
                return varName.get(); \
            }

    SET_GET (StatusCondition, statusCondition)
    SET_GET (Nature, nature)

    //==============================================================================
    #undef SET_GET
    #define SET_GET(ClassName, varName) \
        private: \
            CachedValue<int> varName; \
            \
        public: \
            void set##ClassName (int new##ClassName, UndoManager* undoManager = nullptr) \
            { \
                varName.setValue (jmax (0, new##ClassName), undoManager); \
            } \
            \
            [[nodiscard]] int get##ClassName() const noexcept \
            { \
                return varName.get(); \
            }

    SET_GET (Level, level)
    SET_GET (Experience, experience)
    SET_GET (HitPoints, hitPoints)
    SET_GET (MaxHitPoints, maxHitPoints)
    SET_GET (Attack, attack)
    SET_GET (Defense, defense)
    SET_GET (SpecialAttack, specialAttack)
    SET_GET (SpecialDefense, specialDefense)
    SET_GET (Speed, speed)

    #undef SET_GET

    //==============================================================================
    /** @returns */
    bool isAlive() const    { return getHitPoints() > 0; }
    /** @returns */
    bool isDead() const     { return getHitPoints() <= 0; }

    //==============================================================================
    /** @returns */
    [[nodiscard]] int getNumFightingMoves() const noexcept                  { return fightingMoves.getNumChildren(); }
    /** @returns */
    [[nodiscard]] FightingMove getFightingMove (int index) const noexcept   { return fightingMoves.getChild (index); }

    /** */
    FightableEntity& addFightingMove (const FightingMove& fightingMove, UndoManager* undoManager = nullptr)
    {
        fightingMoves.appendChild (fightingMove.getState(), undoManager);
        return *this;
    }

    /** */
    FightableEntity& removeFightingMove (const FightingMove& fightingMove, UndoManager* undoManager = nullptr)
    {
        fightingMoves.removeChild (fightingMove.getState(), undoManager);
        return *this;
    }

    /** */
    FightableEntity& removeFightingMove (int index, UndoManager* undoManager = nullptr)
    {
        fightingMoves.removeChild (index, undoManager);
        return *this;
    }

private:
    //==============================================================================
    ValueTree fightingMoves { fightingMovesId };

    //==============================================================================
    /** Loads up stats with reasonable low defaults. */
    void setupPropAndCache (UndoManager* undoManager)
    {
        EngineObject::setupPropAndCache (weakAgainstType, weakAgainstTypeId, MoveType::normal, undoManager);
        EngineObject::setupPropAndCache (statusCondition, statusConditionId, {}, undoManager);
        EngineObject::setupPropAndCache (nature, natureId, Nature::defaultNature, undoManager);
        EngineObject::setupPropAndCache (level, levelId, 1, undoManager);
        EngineObject::setupPropAndCache (experience, experienceId, 0, undoManager);
        EngineObject::setupPropAndCache (hitPoints, hitPointsId, 31, undoManager);
        EngineObject::setupPropAndCache (maxHitPoints, maxHitPointsId, 31, undoManager);
        EngineObject::setupPropAndCache (attack, attackId, 6, undoManager);
        EngineObject::setupPropAndCache (defense, defenseId, 4, undoManager);
        EngineObject::setupPropAndCache (specialAttack, specialAttackId, 6, undoManager);
        EngineObject::setupPropAndCache (specialDefense, specialDefenseId, 5, undoManager);
        EngineObject::setupPropAndCache (speed, speedId, 6, undoManager);

        state.appendChild (fightingMoves, undoManager);
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FightableEntity)
};

//==============================================================================
/** */
class Player final : public FightableEntity
{
public:
    /** */
    Player (double directionDegrees = 0.0,
            UndoManager* undoManager = nullptr) :
        FightableEntity (playerId, false, directionDegrees, undoManager)
    {
        // Remove inapplicable properties:
        state.removeProperty (interactionIdId, undoManager);
        state.removeProperty (isNPCId, undoManager);
        state.removeProperty (subtypeId, undoManager);
        state.removeProperty (difficultyId, undoManager);
    }

    /** */
    Player (CardinalDirection cd = CardinalDirection::north,
            UndoManager* undoManager = nullptr) :
        Player (toDegrees (cd), undoManager)
    {
    }

    Player (const Player& other) :
        FightableEntity (playerId, false, 0.0, nullptr)
    {
        state = other.state;
    }

private:
    //==============================================================================
    // Remove inapplicable methods:
    using WorldEntity::isNPC;
    using WorldEntity::setNPC;
    using WorldEntity::getSubtype;
    using WorldEntity::setSubtype;
    using WorldEntity::getDifficulty;
    using WorldEntity::setDifficulty;
    using FightableEntity::getInteractionId;
    using FightableEntity::isInteractable;
    using FightableEntity::setInteractionId;
};

//==============================================================================
/** 
*/
class EngineTile : public WorldObject
{
public:
    /** */
    enum class Type
    {
        floor,
        wall,
        window,
        door,
        stairs,
        elevator,
        rope
    };

    //==============================================================================
    /** */
    EngineTile (Type tileType = Type::floor,
                StringRef interactionId = {},
                UndoManager* undoManager = nullptr) :
        WorldObject (tileId,
                     interactionId,
                     undoManager),
        type (state, typeId, undoManager, static_cast<int> (Type::floor)),
        material (state, materialId, undoManager, static_cast<int> (Material::stone)),
        colour (state, colourId, undoManager, Colours::transparentBlack)
    {
        state.appendChild (inventory, undoManager);
        setType (tileType, undoManager);
    }

    //==============================================================================
    /** @returns */
    [[nodiscard]] Type getType() const noexcept                                 { return static_cast<Type> (type.get()); }
    /** */
    void setType (Type newType, UndoManager* undoManager = nullptr)             { type.setValue (static_cast<int> (newType), undoManager); }

    //==============================================================================
    /** @returns */
    [[nodiscard]] Material getMaterial() const noexcept                         { return static_cast<Material> (material.get()); }
    /** */
    void setMaterial (Material newMaterial, UndoManager* undoManager = nullptr) { material.setValue (static_cast<int> (newMaterial), undoManager); }

    //==============================================================================
    /** @returns */
    [[nodiscard]] Colour getColour() const noexcept                             { return colour.get(); }
    /** */
    void setColour (Colour newColour, UndoManager* undoManager = nullptr)       { colour.setValue (newColour, undoManager); }

    //==============================================================================
    /** @returns */
    [[nodiscard]] int getNumInventoryItems() const noexcept                     { return inventory.getNumChildren(); }
    /** @returns */
    [[nodiscard]] WorldObject getInventoryItem (int index) const noexcept       { return inventory.getChild (index); }

    /** */
    EngineTile& addInventoryItem (const WorldObject& object, UndoManager* undoManager = nullptr)
    {
        inventory.appendChild (object.getState(), undoManager);
        return *this;
    }

    /** */
    EngineTile& removeInventoryItem (const WorldObject& object, UndoManager* undoManager = nullptr)
    {
        inventory.removeChild (object.getState(), undoManager);
        return *this;
    }

    /** */
    EngineTile& removeInventoryItem (int index, UndoManager* undoManager = nullptr)
    {
        inventory.removeChild (index, undoManager);
        return *this;
    }

private:
    //==============================================================================
    CachedValue<int> type, material;
    CachedValue<Colour> colour;
    ValueTree inventory { inventoryId };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EngineTile)
};

//==============================================================================
/** */
class StairTile final : public EngineTile
{
public:
    /** */
    enum class Direction
    {
        blocked,
        down,
        up
    };

    /** */
    StairTile (Direction startDirection,
               StringRef interactionId = {},
               UndoManager* undoManager = nullptr) :
        EngineTile (Type::stairs, interactionId, undoManager),
        direction (state, directionId, undoManager, static_cast<int> (Direction::up))
    {
        setDirection (startDirection, undoManager);
    }

    //==============================================================================
    /** @returns */
    [[nodiscard]] Direction getDirection() const noexcept                           { return static_cast<Direction> (direction.get()); }
    /** */
    void setDirection (Direction newDirection, UndoManager* undoManager = nullptr)  { direction.setValue (static_cast<int> (newDirection), undoManager); }

private:
    //==============================================================================
    CachedValue<int> direction;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StairTile)
};

//==============================================================================
/** Eliminates boilerplate in defining a tile's set of unlockable logic where applicable.

    @see DoorTile, WindowTile
*/
class Unlockable
{
public:
    /** */
    Unlockable (ValueTree& sourceState) :
        unlockableState (sourceState)
    {
    }

    /** */
    virtual ~Unlockable() = default;

    //==============================================================================
    /** @returns */
    [[nodiscard]] Array<int> getUnlockableItemIDs() const noexcept
    {
        Array<int> ids;

        if (auto* arr = unlockableState[unlockableIDsId].getArray())
        {
            for (const auto& v : *arr)
            {
                jassert (v.isInt());
                ids.addIfNotAlreadyThere (static_cast<int> (v));
            }
        }
        else
        {
            jassertfalse;
        }

        ids.sort();
        ids.minimiseStorageOverheads();
        return ids;
    }

    /** */
    void setUnlockableIDs (const Array<int>& newUnlockableItemIDs, UndoManager* undoManager = nullptr)
    {
        Array<var> arr;

        for (auto v : newUnlockableItemIDs)
            arr.add (v);

        arr.minimiseStorageOverheads();
        unlockableState.setProperty (unlockableIDsId, arr, undoManager);
    }

    /** */
    void setUnlockableID (int unlockableID, UndoManager* undoManager = nullptr)
    {
        Array<int> arr;
        arr.add (unlockableID);
        setUnlockableIDs (arr, undoManager);
    }


private:
    //==============================================================================
    ValueTree& unlockableState;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Unlockable)
};

//==============================================================================
/** */
class DoorTile final : public EngineTile,
                       public Unlockable
{
public:
    //==============================================================================
    /** */
    DoorTile (DoorLockState startLockState = DoorLockState::unlocked,
              bool shouldBeSecret = false,
              StringRef interactionId = {},
              UndoManager* undoManager = nullptr) :
        EngineTile (Type::door, interactionId, undoManager),
        Unlockable (state),
        lockState (state, lockStateId, undoManager, static_cast<DoorLockStateType> (DoorLockState::unlocked)),
        secret (state, secretId, undoManager, false)
    {
        setLockState (startLockState, undoManager);
        setUnlockableIDs ({}, undoManager);
        setAsSecret (shouldBeSecret, undoManager);
    }

    /** */
    DoorTile (DoorLockState startLockState,
              int unlockableID,
              bool shouldBeSecret = false,
              StringRef interactionId = {},
              UndoManager* undoManager = nullptr) :
        DoorTile (startLockState, shouldBeSecret, interactionId, undoManager)
    {
        jassert (startLockState != DoorLockState::unlocked);
        setUnlockableID (unlockableID, undoManager);
    }

    //==============================================================================
    /** @returns */
    [[nodiscard]] DoorLockState getLockState() const noexcept                           { return static_cast<DoorLockState> (lockState.get()); }
    /** */
    void setLockState (DoorLockState newLockState, UndoManager* undoManager = nullptr)  { lockState.setValue (static_cast<DoorLockStateType> (newLockState), undoManager); }

    //==============================================================================
    /** @returns */
    [[nodiscard]] bool isSecret() const noexcept                                    { return secret.get(); }
    /** */
    void setAsSecret (bool shouldBeSecret, UndoManager* undoManager = nullptr)      { secret.setValue (shouldBeSecret, undoManager); }

private:
    //==============================================================================
    using DoorLockStateType = VariantConverter<DoorLockState>::Type;
    CachedValue<DoorLockStateType> lockState;
    CachedValue<bool> secret;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoorTile)
};

//==============================================================================
/** */
class WallTile final : public EngineTile
{
public:
    /** */
    WallTile (Material material,
              Colour colour = Colours::transparentBlack,
              StringRef interactionId = {},
              UndoManager* undoManager = nullptr) :
        EngineTile (Type::wall, interactionId, undoManager)
    {
        setMaterial (material, undoManager);
        setColour (colour, undoManager);
    }

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WallTile)
};

//==============================================================================
/** */
class WindowTile final : public EngineTile,
                         public Unlockable
{
public:
    /** */
    WindowTile (bool shouldBeOpen = false,
                WindowTileType subtypeToUse = WindowTileType::permanentlyClosed,
                Material material = Material::glass,
                Colour colour = Colours::transparentBlack,
                StringRef interactionId = {},
                UndoManager* undoManager = nullptr) :
        EngineTile (Type::window, interactionId, undoManager),
        Unlockable (state),
        subtype (state, windowTileSubtypeId, undoManager, static_cast<WindowTileTypeType> (WindowTileType::permanentlyClosed)),
        opened (state, openedId, undoManager, false)
    {
        setMaterial (material, undoManager);
        setColour (colour, undoManager);
        setSubtype (subtypeToUse, undoManager);
        setOpen (shouldBeOpen, undoManager);
        setUnlockableIDs ({}, undoManager);
    }

    //==============================================================================
    /** @returns */
    [[nodiscard]] WindowTileType getSubtype() const noexcept                           { return static_cast<WindowTileType> (subtype.get()); }
    /** */
    void setSubtype (WindowTileType newSubtype, UndoManager* undoManager = nullptr)    { subtype.setValue (static_cast<WindowTileTypeType> (newSubtype), undoManager);
}
    /** @returns */
    [[nodiscard]] bool isOpen() const noexcept                                  { return opened.get(); }
    /** */
    void setOpen (bool shouldBeOpen, UndoManager* undoManager = nullptr)        { opened.setValue (shouldBeOpen, undoManager); }

private:
    //==============================================================================
    using WindowTileTypeType = VariantConverter<WindowTileType>::Type;
    CachedValue<WindowTileTypeType> subtype;
    CachedValue<bool> opened;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WindowTile)
};
