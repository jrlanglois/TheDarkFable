/** */
class WorldEntity : public WorldObject
{
public:
    /** */
    WorldEntity (const Identifier& id,
                 bool shouldBeAnNPC,
                 double directionDegrees = 0.0,
                 UndoManager* undoManager = nullptr) :
        WorldObject (id),
        npc (state, isNPCId, undoManager, shouldBeAnNPC),
        direction (state, directionId, undoManager, snapAngleToWorld (directionDegrees))
    {
        state.appendChild (inventory, undoManager);
    }

    /** */
    WorldEntity (const Identifier& id,
                 bool shouldBeAnNPC,
                 CardinalDirection cd = CardinalDirection::north,
                 UndoManager* undoManager = nullptr) :
        WorldEntity (id, shouldBeAnNPC, toDegrees (cd), undoManager)
    {
    }

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
        if (approximatelyEqual (degs, toDegrees (CardinalDirection::notApplicable)))    return TRANS ("north");
        else if (approximatelyEqual (degs, toDegrees (CardinalDirection::north)))       return TRANS ("north");
        else if (approximatelyEqual (degs, toDegrees (CardinalDirection::east)))        return TRANS ("east");
        else if (approximatelyEqual (degs, toDegrees (CardinalDirection::south)))       return TRANS ("south");
        else if (approximatelyEqual (degs, toDegrees (CardinalDirection::west)))        return TRANS ("west");

        return String (degs);
    }

    /** @returns */
    [[nodiscard]] bool isNPC() const noexcept { return npc.get(); }
    /** */
    WorldEntity& setNPC (bool shouldBeAnNPC, UndoManager* undoManager = nullptr)
    {
        npc.setValue (shouldBeAnNPC, undoManager);
        return *this;
    }

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

    CREATE_INLINE_CLASS_IDENTIFIER (direction)
    CREATE_INLINE_CLASS_IDENTIFIER (isNPC)
    CREATE_INLINE_CLASS_IDENTIFIER (inventory)

private:
    CachedValue<bool> npc;
    CachedValue<double> direction;
    ValueTree inventory { inventoryId };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WorldEntity)
};

/** */
class Weapon : public WorldEntity
{
public:
    /** */
    Weapon (const Identifier& id,
            double directionDegs,
            UndoManager* undoManager = nullptr) :
        WorldEntity (id, true, directionDegs, undoManager)
    {
    }

    /** */
    Weapon (const Identifier& id,
            CardinalDirection cd = CardinalDirection::north,
            UndoManager* undoManager = nullptr) :
        WorldEntity (id, true, cd, undoManager)
    {
    }

    #undef SET_GET
    #define SET_GET(Name, varName, paramName) \
        private: \
            CachedValue<int> varName; \
            \
        public: \
            CREATE_INLINE_CLASS_IDENTIFIER (varName) \
            void set##Name (int paramName, UndoManager* undoManager = nullptr) \
            { \
                varName.setValue (paramName, undoManager); \
            } \
            \
            [[nodiscard]] int get##Name() const noexcept \
            { \
                return varName.get(); \
            }

    SET_GET (Priority, hitPoints, newHitPoints)
    SET_GET (NormalDamage, normalDamage, newNormalDamage)
    SET_GET (FireDamage, fireDamage, newFireDamage)
    SET_GET (IceDamage, iceDamage, newIceDamage)
    SET_GET (PoisonDamage, poisonDamage, newPoisonDamage)

    #undef SET_GET

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Weapon)
};

/** */
class FightingMove : public EngineObject
{
public:
    /** */
    FightingMove (const Identifier& id,
                  UndoManager* undoManager = nullptr) :
        EngineObject (id, undoManager)
    {
    }

    #undef SET_GET
    #define SET_GET(Type, Name, varName, paramName) \
        private: \
            CachedValue<Type> varName; \
            \
        public: \
            CREATE_INLINE_CLASS_IDENTIFIER (varName) \
            FightingMove& set##Name (Type paramName, UndoManager* undoManager = nullptr) \
            { \
                varName.setValue (paramName, undoManager); \
                return *this; \
            } \
            \
            [[nodiscard]] Type get##Name() const noexcept \
            { \
                return varName.get(); \
            }

    SET_GET (bool, MakesPhysicalContact, makesContact, makesPhysicalContact)
    SET_GET (MoveType, MoveType, moveType, newMoveType)

    #undef SET_GET

    #undef SET_GET
    #define SET_GET(Name, varName, paramName) \
        private: \
            CachedValue<int> varName; \
            \
        public: \
            CREATE_INLINE_CLASS_IDENTIFIER (varName) \
            FightingMove& set##Name (int paramName, UndoManager* undoManager = nullptr) \
            { \
                varName.setValue (paramName, undoManager); \
                return *this; \
            } \
            \
            [[nodiscard]] int get##Name() const noexcept \
            { \
                return varName.get(); \
            }

    SET_GET (Priority, hitPoints, newHitPoints)
    SET_GET (NormalDamage, normalDamage, newNormalDamage)
    SET_GET (FireDamage, fireDamage, newFireDamage)
    SET_GET (IceDamage, iceDamage, newIceDamage)
    SET_GET (PoisonDamage, poisonDamage, newPoisonDamage)

    #undef SET_GET

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FightingMove)
};

/** */
class FightableEntity : public WorldEntity
{
public:
    /** */
    FightableEntity (const Identifier& id,
                     bool shouldBeAnNPC,
                     double directionDegrees = 0.0,
                     UndoManager* undoManager = nullptr) :
        WorldEntity (id, shouldBeAnNPC, directionDegrees, undoManager),
        statusCondition (state, statusConditionId, undoManager, 0)
    {
    }

    /** */
    FightableEntity (const Identifier& id,
                     bool shouldBeAnNPC,
                     CardinalDirection cd = CardinalDirection::north,
                     UndoManager* undoManager = nullptr) :
        FightableEntity (id, shouldBeAnNPC, toDegrees (cd), undoManager)
    {
    }

    #undef SET_GET
    #define SET_GET(Type, Name, varName, paramName) \
        private: \
            CachedValue<Type> varName; \
            \
        public: \
            CREATE_INLINE_CLASS_IDENTIFIER (varName) \
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

    #undef SET_GET

    #undef SET_GET
    #define SET_GET(ClassName, varName) \
        private: \
            CachedValue<ClassName> varName; \
            \
        public: \
            CREATE_INLINE_CLASS_IDENTIFIER (varName) \
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

    #undef SET_GET
    #define SET_GET(Name, varName, paramName) \
        private: \
            CachedValue<int> varName; \
            \
        public: \
            CREATE_INLINE_CLASS_IDENTIFIER (varName) \
            void set##Name (int paramName, UndoManager* undoManager = nullptr) \
            { \
                varName.setValue (jmax (0, paramName), undoManager); \
            } \
            \
            [[nodiscard]] int get##Name() const noexcept \
            { \
                return varName.get(); \
            }

    SET_GET (Level, level, newLevel)
    SET_GET (HitPoints, hitPoints, newHitPoints)
    SET_GET (MaxHitPoints, maxHitPoints, newMaxHitPoints)
    SET_GET (Attack, attack, newAttack)
    SET_GET (Defense, defense, newDefense)
    SET_GET (SpecialAttack, specialAttack, newSpecialAttack)
    SET_GET (SpecialDefense, specialDefense, newSpecialDefense)
    SET_GET (Speed, speed, newSpeed)

    #undef SET_GET

    /** */
    bool isAlive() const { return getHitPoints() > 0; }
    /** */
    bool isDead() const { return getHitPoints() <= 0; }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FightableEntity)
};

/** */
class Player final : public FightableEntity
{
public:
    /** */
    Player (int playerIndexValue = 0,
            double directionDegrees = 0.0,
            UndoManager* undoManager = nullptr) :
        FightableEntity (playerId, false, directionDegrees, undoManager),
        playerIndex (state, playerIndexId, undoManager, 0)
    {
        playerIndex.setValue (playerIndexValue, nullptr);
    }

    /** */
    Player (int playerIndexValue = 0,
            CardinalDirection cd = CardinalDirection::north,
            UndoManager* undoManager = nullptr) :
        Player (playerIndexValue, toDegrees (cd), undoManager)
    {
    }

    /** @returns */
    [[nodiscard]] int getPlayerIndex() const noexcept { return playerIndex.get(); }

    CREATE_INLINE_CLASS_IDENTIFIER (player)
    CREATE_INLINE_CLASS_IDENTIFIER (playerIndex)

private:
    CachedValue<int> playerIndex;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Player)
};

/** */
class EngineTile : public WorldObject
{
public:
    /** */
    enum class Type
    {
        floor,
        wall,
        door,
        stairs,
        elevator,
        rope
    };

    /** */
    EngineTile (Type tileType, UndoManager* undoManager = nullptr) :
        WorldObject (tileId,
                     tileType != Type::floor && tileType != Type::wall,
                     undoManager),
        type (state, typeId, undoManager, static_cast<int> (tileType))
    {
        state.appendChild (inventory, undoManager);
    }

    /** @returns */
    [[nodiscard]] Type getType() const noexcept                             { return static_cast<Type> (type.get()); }
    /** */
    void setType (Type newType, UndoManager* undoManager = nullptr)         { type.setValue (static_cast<int> (newType), undoManager); }

    /** @returns */
    [[nodiscard]] int getNumInventoryItems() const noexcept                 { return inventory.getNumChildren(); }
    /** @returns */
    [[nodiscard]] WorldObject getInventoryItem (int index) const noexcept   { return inventory.getChild (index); }

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

    CREATE_INLINE_CLASS_IDENTIFIER (type)
    CREATE_INLINE_CLASS_IDENTIFIER (tile)
    CREATE_INLINE_CLASS_IDENTIFIER (inventory)

private:
    CachedValue<int> type;
    ValueTree inventory { inventoryId };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EngineTile)
};

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
    StairTile (Direction startDirection, UndoManager* undoManager = nullptr) :
        EngineTile (Type::stairs, undoManager),
        direction (state, directionId, undoManager, static_cast<int> (startDirection))
    {
    }

    /** @returns */
    [[nodiscard]] Direction getDirection() const noexcept                           { return static_cast<Direction> (direction.get()); }
    /** */
    void setDirection (Direction newDirection, UndoManager* undoManager = nullptr)  { direction.setValue (static_cast<int> (newDirection), undoManager); }

    CREATE_INLINE_CLASS_IDENTIFIER (direction)

private:
    CachedValue<int> direction;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StairTile)
};

/** */
class DoorTile final : public EngineTile
{
public:
    /** */
    enum class LockState
    {
        unlocked,
        needsKey,
        needsSpell,
        impassable
    };

    /** */
    DoorTile (LockState startLockState = LockState::unlocked, UndoManager* undoManager = nullptr) :
        EngineTile (Type::door, undoManager),
        lockState (state, lockStateId, undoManager, static_cast<int> (startLockState))
    {
        setUnlockableIDs (Array<int>(), undoManager);
    }

    /** */
    DoorTile (LockState startLockState, int unlockableID, UndoManager* undoManager = nullptr) :
        DoorTile (startLockState, undoManager)
    {
        jassert (startLockState != LockState::unlocked);
        setUnlockableID (unlockableID, undoManager);
    }

    /** @returns */
    [[nodiscard]] LockState getLockState() const noexcept                           { return static_cast<LockState> (lockState.get()); }
    /** */
    void setLockState (LockState newLockState, UndoManager* undoManager = nullptr)  { lockState.setValue (static_cast<int> (newLockState), undoManager); }

    /** @returns */
    [[nodiscard]] Array<int> getUnlockableItemIDs() const noexcept
    {
        Array<int> ids;

        if (auto* arr = state[unlockableIDsId].getArray())
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
        state.setProperty (unlockableIDsId, arr, undoManager);
    }

    /** */
    void setUnlockableID (int unlockableID, UndoManager* undoManager = nullptr)
    {
        Array<int> arr;
        arr.add (unlockableID);
        setUnlockableIDs (arr, undoManager);
    }

    CREATE_INLINE_CLASS_IDENTIFIER (lockState)
    CREATE_INLINE_CLASS_IDENTIFIER (unlockableIDs)

private:
    CachedValue<int> lockState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoorTile)
};
