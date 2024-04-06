//==============================================================================
/** A generic, low-level object that can exist within a GameMap.

    @see WorldEntity, Weapon, FightingMove, FightableEntity,
         Player, EngineTile, GameMap
*/
class EngineObject : public Identifiable
{
public:
    /** */
    EngineObject (const Identifier& id, UndoManager* undoManager = nullptr) :
        Identifiable (id),
        state (id)
    {
        setupPropAndCache (undoManager);
    }

    /** */
    EngineObject (const ValueTree& existingState, UndoManager* undoManager = nullptr) :
        EngineObject (existingState.getType(), undoManager)
    {
        state = existingState;
        setupPropAndCache (undoManager);
    }

    //==============================================================================
    /** Untranslated. */
    EngineObject& setName (const String& newName, UndoManager* undoManager = nullptr)
    {
        name.setValue (newName, undoManager);
        return *this;
    }

    /** @returns */
    [[nodiscard]] String getName() const noexcept { return name.get(); }

    /** Untranslated. */
    EngineObject& setDescription (const String& newDescription, UndoManager* undoManager = nullptr)
    {
        description.setValue (newDescription, undoManager);
        return *this;
    }

    /** @returns */
    [[nodiscard]] String getDescription() const noexcept { return description.get(); }

    //==============================================================================
    /** @returns the state of this EngineObject. */
    [[nodiscard]] ValueTree getState() const noexcept { return state; }

    /** @returns true if the EngineObject has the same type and state as this one.
        @see getType
    */
    bool operator== (const EngineObject& other) const noexcept { return other.state == state; }

    /** @returns true if the EngineObject has a different type and state as this one.
        @see getType
    */
    bool operator!= (const EngineObject& other) const noexcept { return ! operator== (other); }

    //==============================================================================
    /** @returns the value of a named property.
        If no such property has been set, this will return a void variant.
        You can also use operator[] to get a property.
        @see var, setProperty, getPropertyPointer, hasProperty
    */
    const var& getProperty (const Identifier& id) const noexcept { return state.getProperty (id); }

    /** @returns the value of a named property, or the value of defaultReturnValue
        if the property doesn't exist.
        You can also use operator[] and getProperty to get a property.
        @see var, getProperty, getPropertyPointer, setProperty, hasProperty
    */
    var getProperty (const Identifier& id, const var& defaultReturnValue) const { return state.getProperty (id, defaultReturnValue); }

    /** @returns a pointer to the value of a named property, or nullptr if the property
        doesn't exist.
        @see var, getProperty, setProperty, hasProperty
    */
    const var* getPropertyPointer (const Identifier& id) const noexcept { return state.getPropertyPointer (id); }

    /** @returns the value of a named property.
        If no such property has been set, this will return a void variant. This is the same as
        calling getProperty().
        @see getProperty
    */
    const var& operator[] (const Identifier& id) const noexcept { return state[id]; }

    /** Changes a named property of the tree.
        The name identifier must not be an empty string.
        If the undoManager parameter is not nullptr, its UndoManager::perform() method will be used,
        so that this change can be undone. Be very careful not to mix undoable and non-undoable changes!
        @see var, getProperty, removeProperty
        @returns a reference to the value tree, so that you can daisy-chain calls to this method.
    */
    ValueTree& setProperty (const Identifier& id, const var& newValue, UndoManager* undoManager) { return state.setProperty (id, newValue, undoManager); }

    /** @returns true if the tree contains a named property. */
    bool hasProperty (const Identifier& id) const noexcept { return state.hasProperty (id); }

    /** Removes a property from the tree.
        If the undoManager parameter is not nullptr, its UndoManager::perform() method will be used,
        so that this change can be undone. Be very careful not to mix undoable and non-undoable changes!
    */
    void removeProperty (const Identifier& id, UndoManager* undoManager) { state.removeProperty (id, undoManager); }

    /** Removes all properties from the tree.
        If the undoManager parameter is not nullptr, its UndoManager::perform() method will be used,
        so that this change can be undone. Be very careful not to mix undoable and non-undoable changes!
    */
    void removeAllProperties (UndoManager* undoManager) { state.removeAllProperties (undoManager); }

    /** @returns the total number of properties that the tree contains.
        @see getProperty.
    */
    int getNumProperties() const noexcept { return state.getNumProperties(); }

    /** @returns the identifier of the property with a given index.
        Note that properties are not guaranteed to be stored in any particular order, so don't
        expect that the index will correspond to the order in which the property was added, or
        that it will remain constant when other properties are added or removed.
        @see getNumProperties
    */
    Identifier getPropertyName (int index) const noexcept { return state.getPropertyName (index); }

    /** @returns a Value object that can be used to control and respond to one of the tree's properties.

        The Value object will maintain a reference to this tree, and will use the undo manager when
        it needs to change the value. Attaching a Value::Listener to the value object will provide
        callbacks whenever the property changes.
        If shouldUpdateSynchronously is true the Value::Listener will be updated synchronously.
        @see ValueSource::sendChangeMessage (bool)
    */
    Value getPropertyAsValue (const Identifier& propNameId, UndoManager* undoManager,
                              bool shouldUpdateSynchronously = false)
    {
        return state.getPropertyAsValue (propNameId, undoManager, shouldUpdateSynchronously);
    }

    //==============================================================================
    /** @returns a created XmlElement that holds a complete image of this tree and all its children.
        If this tree is invalid, this may return nullptr. Otherwise, the XML that is produced can
        be used to recreate a similar tree by calling ValueTree::fromXml().
        @see fromXml, toXmlString
    */
    [[nodiscard]] std::unique_ptr<XmlElement> createXml() const { return state.createXml(); }

    /** @returns a string containing an XML representation of the tree.
        This is quite handy for debugging purposes, as it provides a quick way to view a tree.
        @see toJSONString()
    */
    [[nodiscard]] String toXmlString() const
    {
        XmlElement::TextFormat format;
        format.lineWrapLength = 4096;
        return state.toXmlString (format);
    }

    /** @returns a string containing a JSON representation of the tree.
        This is quite handy for debugging purposes, as it provides a quick way to view a tree.
        @see toXmlString()
    */
    [[nodiscard]] String toJSONString() const
    {
        return sp::toJSONString (state);
    }

    /** @returns */
    [[nodiscard]] Result saveXML (const File& dest) const
    {
        if (dest.replaceWithText (toXmlString()))
            return Result::ok();

        jassertfalse;
        return Result::fail (TRANS ("Failed to save!"));
    }

    /** @returns */
    [[nodiscard]] Result loadXML (const File& source)
    {
        const auto vt = ValueTree::fromXml (source.loadFileAsString());

        if (vt.hasType (getIdentifier()))
        {
            state = vt;
            return Result::ok();
        }

        jassertfalse;
        return Result::fail (TRANS ("Failed to load!"));
    }

    /** @returns */
    [[nodiscard]] Result saveJSON (const File& dest) const
    {
        if (dest.replaceWithText (toJSONString()))
            return Result::ok();

        jassertfalse;
        return Result::fail (TRANS ("Failed to save!"));
    }

    /** @returns */
    [[nodiscard]] Result loadJSON (const File& source)
    {
        const auto vt = createValueTreeFromJSON (source.loadFileAsString(), getIdentifier());

        if (vt.hasType (getIdentifier()))
        {
            state = vt;
            return Result::ok();
        }

        jassertfalse;
        return Result::fail (TRANS ("Failed to load!"));
    }

protected:
    //==============================================================================
    ValueTree state;

    /** @returns */
    template<typename Type>
    ValueTree& setupPropAndCache (CachedValue<Type>& cv, const Identifier& id,
                                  const Type& value, UndoManager* undoManager)
    {
        setProperty (id, juce::VariantConverter<Type>::toVar (value), undoManager);
        cv.referTo (state, id, undoManager);
        return state;
    }

private:
    //==============================================================================
    CachedValue<String> name, description;

    //==============================================================================
    void setupPropAndCache (UndoManager* undoManager)
    {
        setupPropAndCache (name, nameId, {}, undoManager);
        setupPropAndCache (description, descriptionId, {}, undoManager);
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EngineObject)
};

//==============================================================================
/** */
class WorldObject : public EngineObject
{
public:
    /** */
    WorldObject (const Identifier& id,
                 StringRef interactionIdToUse = {},
                 UndoManager* undoManager = nullptr) :
        EngineObject (id, undoManager)
    {
        setupPropAndCache (undoManager);
        setInteractionId (interactionIdToUse, undoManager);
    }

    /** */
    WorldObject (const ValueTree& existingState, UndoManager* undoManager = nullptr) :
        EngineObject (existingState, undoManager)
    {
        setupPropAndCache (undoManager);
    }

    //==============================================================================
    /** */
    WorldObject& setDimensions (Rectangle<int> newDimensions, UndoManager* undoManager = nullptr)
    {
        dimensions.setValue (newDimensions, undoManager);
        return *this;
    }

    /** */
    WorldObject& setPosition (Point<int> newPosition, UndoManager* undoManager = nullptr)
    {
        return setDimensions (getDimensions().withPosition (newPosition), undoManager);
    }

    /** */
    WorldObject& setPosition (int x, int y, UndoManager* undoManager = nullptr)
    {
        return setPosition ({ x, y }, undoManager);
    }

    /** */
    WorldObject& setSize (int width, int height, UndoManager* undoManager = nullptr)
    {
        return setDimensions (getDimensions().withSize (width, height), undoManager);
    }

    /** @returns */
    [[nodiscard]] Rectangle<int> getDimensions() const noexcept { return dimensions.get(); }
    /** @returns */
    [[nodiscard]] Point<int> getPosition() const noexcept       { return getDimensions().getPosition(); }
    /** @returns */
    [[nodiscard]] int getWidth() const noexcept                 { return getDimensions().getWidth(); }
    /** @returns */
    [[nodiscard]] int getHeight() const noexcept                { return getDimensions().getHeight(); }

    //==============================================================================
    /** @returns */
    [[nodiscard]] String getInteractionId() const noexcept  { return interactionId.get(); }
    /** @returns */
    [[nodiscard]] bool isInteractable() const noexcept      { return getInteractionId().isNotEmpty(); }
    /** */
    WorldObject& setInteractionId (StringRef newInteractionId, UndoManager* undoManager = nullptr)
    {
        interactionId.setValue (newInteractionId, undoManager);
        return *this;
    }

    //==============================================================================
    /** @returns */
    [[nodiscard]] Colour getLightColour() const noexcept    { return lightColour.get(); }
    /** @returns */
    [[nodiscard]] bool castsLight() const noexcept          { return ! getLightColour().isTransparent(); }
    /** @returns */
    [[nodiscard]] int getLightRadius() const noexcept       { return lightRadius.get(); }
    /** */
    WorldObject& setLightColour (Colour newLightColour, UndoManager* undoManager = nullptr)
    {
        lightColour.setValue (newLightColour, undoManager);
        return *this;
    }
    /** */
    WorldObject& setLightRadius (int newLightRadius, UndoManager* undoManager = nullptr)
    {
        lightRadius.setValue (newLightRadius, undoManager);
        return *this;
    }

    //==============================================================================
    /** Depending on your object's state, interactibility could be
        selectively enabled and disabled here, and any other state might want
        to be considered or changed.

        This will be called prior to recreating icons.
    */
    virtual void update() { }

    /** */
    String getMapIcon() const       { return mapIcon.get(); }
    /** */
    String getScreenIcon() const    { return screenIcon.get(); }
    /** */
    String getInventoryIcon() const { return inventoryIcon.get(); }

    /** */
    WorldObject& setMapIcon (StringRef newIcon, UndoManager* undoManager = nullptr)
    {
        mapIcon.setValue (newIcon, undoManager);
        return *this;
    }

    /** */
    WorldObject& setScreenIcon (StringRef newIcon, UndoManager* undoManager = nullptr)
    {
        screenIcon.setValue (newIcon, undoManager);
        return *this; 
    }

    /** */
    WorldObject& setInventoryIcon (StringRef newIcon, UndoManager* undoManager = nullptr)
    {
        inventoryIcon.setValue (newIcon, undoManager);
        return *this;
    }

private:
    //==============================================================================
    CachedValue<Rectangle<int>> dimensions;
    CachedValue<String> interactionId, mapIcon, screenIcon, inventoryIcon;
    CachedValue<Colour> lightColour;
    CachedValue<int> lightRadius;

    //==============================================================================
    void setupPropAndCache (UndoManager* undoManager)
    {
        EngineObject::setupPropAndCache (dimensions, dimensionsId, {}, undoManager);
        EngineObject::setupPropAndCache (interactionId, interactionIdId, {}, undoManager);
        EngineObject::setupPropAndCache (mapIcon, mapIconId, {}, undoManager);
        EngineObject::setupPropAndCache (screenIcon, screenIconId, {}, undoManager);
        EngineObject::setupPropAndCache (inventoryIcon, inventoryIconId, {}, undoManager);
        EngineObject::setupPropAndCache (lightColour, lightColourId, {}, undoManager);
        EngineObject::setupPropAndCache (lightRadius, lightRadiusId, {}, undoManager);

        setSize (1, 1, undoManager);
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WorldObject)
};

//==============================================================================
/** Represents a type of StatusCondition to be used on Weapons and WorldEntities.

    @see Weapon, WeaponDefinitions, WorldEntities
*/
class StatusCondition final
{
public:
    //==============================================================================
    /** Creates a StatusCondition that's 'normal', having no status conditions. */
    constexpr StatusCondition() noexcept = default;

    /** Creates a StatusCondition object using a combination of flags from the Flags enum. */
    constexpr StatusCondition (int statusFlags) noexcept : flags (statusFlags) { }

    /** Creates a StatusCondition object using a bitset. */
    constexpr StatusCondition (const std::bitset<16>& bits) noexcept :
        StatusCondition ((int) bits.to_ulong())
    {
    }

    /** Creates a copy of another StatusCondition object. */
    StatusCondition (const StatusCondition&) noexcept = default;

    //==============================================================================
    /** Copies another StatusCondition object. */
    StatusCondition& operator= (const StatusCondition&) noexcept = default;

    /** @returns */
    [[nodiscard]] constexpr bool operator== (const StatusCondition& other) const noexcept   { return flags == other.flags; }
    /** @returns */
    [[nodiscard]] constexpr bool operator!= (const StatusCondition& other) const noexcept   { return ! operator== (other); }

    //==============================================================================
    /** Returns the raw flags that are set for this StatusCondition object. */
    [[nodiscard]] constexpr int getFlags() const noexcept                                   { return flags; }

    /** Returns the raw flags that are set for this StatusCondition object as a bitset. */
    [[nodiscard]] std::bitset<16> toBitset() const noexcept
    {
        return std::bitset<16> ((uint64_t) flags);
    }

    /** Tests a set of flags for this object.
        @returns true if any of the flags passed in are set on this object.
    */
    [[nodiscard]] constexpr bool testFlags (int flagsToTest) const noexcept                 { return (flags & flagsToTest) != 0; }

    /** @returns */
    [[nodiscard]] bool hasMultipleConditions() const noexcept
    {
        return std::popcount (static_cast<uint32> (flags)) > 0;
    }

    /** @returns */
    [[nodiscard]] constexpr bool isNormal() const noexcept                                  { return flags == 0; }
    /** @returns */
    [[nodiscard]] constexpr bool isBurned() const noexcept                                  { return testFlags (burned); }
    /** @returns */
    [[nodiscard]] constexpr bool isFrozen() const noexcept                                  { return testFlags (frozen); }
    /** @returns */
    [[nodiscard]] constexpr bool isParalysed() const noexcept                               { return testFlags (paralysed); }
    /** @returns */
    [[nodiscard]] constexpr bool isPoisoned() const noexcept                                { return testFlags (poisoned); }
    /** @returns */
    [[nodiscard]] constexpr bool isAsleep() const noexcept                                  { return testFlags (asleep); }
    /** @returns */
    [[nodiscard]] constexpr bool isDrowsy() const noexcept                                  { return testFlags (drowsy); }
    /** @returns */
    [[nodiscard]] constexpr bool isFrostbitten() const noexcept                             { return testFlags (frostbitten); }
    /** @returns */
    [[nodiscard]] constexpr bool isBound() const noexcept                                   { return testFlags (bound); }
    /** @returns */
    [[nodiscard]] constexpr bool isCursed() const noexcept                                  { return testFlags (cursed); }

    //==============================================================================
    /** Flag values that can be combined and used in the constructor. */
    enum Flags
    {
        /** */
        burned      = 1 << 0,
        /** */
        frozen      = 1 << 1,
        /** */
        paralysed   = 1 << 2,
        /** */
        poisoned    = 1 << 3,
        /** */
        asleep      = 1 << 4,
        /** */
        drowsy      = 1 << 5,
        /** */
        frostbitten = 1 << 6,
        /** */
        bound       = 1 << 7,
        /** */
        cursed      = 1 << 8,

        /** */
        numFlags    = 9
    };

private:
    //==============================================================================
    int flags = 0;
};

/** @returns */
inline String toString (StatusCondition statusCondition, bool asArray = false)
{
    if (statusCondition.isNormal())
        return TRANS ("Normal");

    if (! asArray)
    {
        if (statusCondition.isBurned())         return TRANS ("Burned");
        if (statusCondition.isFrozen())         return TRANS ("Frozen");
        if (statusCondition.isParalysed())      return TRANS ("Paralysed");
        if (statusCondition.isPoisoned())       return TRANS ("Poisoned");
        if (statusCondition.isAsleep())         return TRANS ("Asleep");
        if (statusCondition.isDrowsy())         return TRANS ("Drowsy");
        if (statusCondition.isFrostbitten())    return TRANS ("Frostbitten");
        if (statusCondition.isBound())          return TRANS ("Bound");
        if (statusCondition.isCursed())         return TRANS ("Cursed");

        jassertfalse; // ???
    }

    StringArray s;

    if (statusCondition.isBurned())         s.add (TRANS ("Burned"));
    if (statusCondition.isFrozen())         s.add (TRANS ("Frozen"));
    if (statusCondition.isParalysed())      s.add (TRANS ("Paralysed"));
    if (statusCondition.isPoisoned())       s.add (TRANS ("Poisoned"));
    if (statusCondition.isAsleep())         s.add (TRANS ("Asleep"));
    if (statusCondition.isDrowsy())         s.add (TRANS ("Drowsy"));
    if (statusCondition.isFrostbitten())    s.add (TRANS ("Frostbitten"));
    if (statusCondition.isBound())          s.add (TRANS ("Bound"));
    if (statusCondition.isCursed())         s.add (TRANS ("Cursed"));

    return s.joinIntoString (", ");
}

//==============================================================================
/** */
enum class CardinalDirection
{
    notApplicable,
    omnidirectional,
    north,
    east,
    south,
    west
};

/** */
inline double toDegrees (CardinalDirection cd) noexcept
{
    switch (cd)
    {
        case CardinalDirection::notApplicable:      return 0.0;
        case CardinalDirection::omnidirectional:    return 0.0;
        case CardinalDirection::north:              return 0.0;
        case CardinalDirection::east:               return 90.0;
        case CardinalDirection::south:              return 180.0;
        case CardinalDirection::west:               return 270.0;

        default:
            jassertfalse;
        break;
    };

    return 0.0;
}

/** @returns */
inline String toString (CardinalDirection cd)
{
    switch (cd)
    {
        case CardinalDirection::notApplicable:      return TRANS ("(N/A)");
        case CardinalDirection::omnidirectional:    return TRANS ("omni");
        case CardinalDirection::north:              return TRANS ("north");
        case CardinalDirection::east:               return TRANS ("east");
        case CardinalDirection::south:              return TRANS ("south");
        case CardinalDirection::west:               return TRANS ("west");
        default: break;
    };

    jassertfalse;
    return {};
}

/** @returns */
template<typename FloatType>
inline FloatType snapAngleToWorld (FloatType angleDegrees) noexcept
{
    constexpr auto ninetyDegs = static_cast<FloatType> (90);
    return std::round (angleDegrees / ninetyDegs) * ninetyDegs;
}

//==============================================================================
/** */
enum class Material
{
    tile,
    dirt,
    grass,
    brick,
    glass,
    wood,
    metal,
    vinyl,
    stone,
    marble,
    concrete,
    cement,
    plastic,
    ice,
    ooze,

    numMaterials = ooze + 1
};

/** @returns */
inline String toString (Material materialType, bool asAdjective = false)
{
    switch (materialType)
    {
        case Material::tile:        { return asAdjective ? TRANS ("tiled")      : TRANS ("tile"); }
        case Material::dirt:        { return asAdjective ? TRANS ("dirty")      : TRANS ("dirt"); }
        case Material::grass:       { return asAdjective ? TRANS ("grassy")      : TRANS ("grass"); }
        case Material::brick:       { return asAdjective ? TRANS ("brick")      : TRANS ("brick"); }
        case Material::glass:       { return asAdjective ? TRANS ("glass")      : TRANS ("glass"); }
        case Material::wood:        { return asAdjective ? TRANS ("wooden")     : TRANS ("wood"); }
        case Material::metal:       { return asAdjective ? TRANS ("metallic")   : TRANS ("metal"); }
        case Material::vinyl:       { return asAdjective ? TRANS ("vinyl")      : TRANS ("vinyl"); }
        case Material::stone:       { return asAdjective ? TRANS ("stone")      : TRANS ("stone"); }
        case Material::marble:      { return asAdjective ? TRANS ("marbled")    : TRANS ("marble"); }
        case Material::concrete:    { return asAdjective ? TRANS ("concrete")   : TRANS ("concrete"); }
        case Material::cement:      { return asAdjective ? TRANS ("cemented")   : TRANS ("cement"); }
        case Material::plastic:     { return asAdjective ? TRANS ("plastic")    : TRANS ("plastic"); }
        case Material::ice:         { return asAdjective ? TRANS ("iced")       : TRANS ("ice"); }
        case Material::ooze:        { return asAdjective ? TRANS ("ooze")       : TRANS ("ooze"); }

        default: break;
    };

    jassertfalse;
    return {};
}

//==============================================================================
/** */
enum class MoveType
{
    normal,
    earth,
    wind,
    water,
    ice,
    fire,
    electric,
    plasma,
    poison,

    numMoveTypes = poison + 1
};

/** */
inline String toString (MoveType moveType)
{
    switch (moveType)
    {
        case MoveType::normal:      return TRANS ("Normal");
        case MoveType::earth:       return TRANS ("Earth");
        case MoveType::wind:        return TRANS ("Wind");
        case MoveType::water:       return TRANS ("Water");
        case MoveType::ice:         return TRANS ("Ice");
        case MoveType::fire:        return TRANS ("Fire");
        case MoveType::electric:    return TRANS ("Electric");
        case MoveType::plasma:      return TRANS ("Plasma");
        case MoveType::poison:      return TRANS ("Poison");

        default: break;
    };

    jassertfalse;
    return {};
}

//==============================================================================
/** Represents a type of difficulty to be used on WorldEntity derivatives.

    @see WorldEntity
*/
class Difficulty final
{
public:
    //==============================================================================
    /** Creates a Difficulty that's 'any', applying to all difficulties the same. */
    constexpr Difficulty() noexcept = default;

    /** Creates a Difficulty object using a combination of flags from the Flags enum. */
    constexpr Difficulty (int statusFlags) noexcept :
        flags (statusFlags)
    {
        jassert (flags >= 0);
        jassert (appliesToAll() || appliesToEasy() || appliesToMedium() || appliesToHard());
    }

    /** Creates a Difficulty object using a bitset. */
    constexpr Difficulty (const std::bitset<16>& bits) noexcept :
        Difficulty ((int) bits.to_ulong())
    {
    }

    /** Creates a copy of another Difficulty object. */
    Difficulty (const Difficulty&) noexcept = default;

    //==============================================================================
    /** Copies another Difficulty object. */
    Difficulty& operator= (const Difficulty&) noexcept = default;

    /** @returns */
    [[nodiscard]] constexpr bool operator== (const Difficulty& other) const noexcept    { return flags == other.flags; }
    /** @returns */
    [[nodiscard]] constexpr bool operator!= (const Difficulty& other) const noexcept    { return ! operator== (other); }

    //==============================================================================
    /** Returns the raw flags that are set for this Difficulty object. */
    [[nodiscard]] constexpr int getFlags() const noexcept                               { return flags; }

    /** Returns the raw flags that are set for this Difficulty object as a bitset. */
    [[nodiscard]] std::bitset<16> toBitset() const noexcept
    {
        return std::bitset<16> ((uint64_t) flags);
    }

    /** Tests a set of flags for this object.
        @returns true if any of the flags passed in are set on this object.
    */
    [[nodiscard]] constexpr bool testFlags (int flagsToTest) const noexcept             { return (flags & flagsToTest) != 0; }

    /** @returns */
    [[nodiscard]] constexpr bool appliesToAll() const noexcept                          { return flags == 0; }
    /** @returns */
    [[nodiscard]] constexpr bool appliesToEasy() const noexcept                         { return testFlags (easy); }
    /** @returns */
    [[nodiscard]] constexpr bool appliesToMedium() const noexcept                       { return testFlags (medium); }
    /** @returns */
    [[nodiscard]] constexpr bool appliesToHard() const noexcept                         { return testFlags (hard); }

    /** @returns */
    [[nodiscard]] constexpr bool appliesToMultipleDifficulties() const noexcept
    {
        return appliesToAll() || std::popcount (static_cast<uint32> (flags)) > 1;
    }

    //==============================================================================
    /** Flag values that can be combined and used in the constructor. */
    enum Flags
    {
        easy        = 1 << 0,   //
        medium      = 1 << 1,   //
        hard        = 1 << 2,   //
        numFlags    = 3         //
    };

private:
    //==============================================================================
    int flags = 0;
};

/** @returns */
inline String toString (Difficulty difficulty, bool asArray = false)
{
    if (difficulty.appliesToAll())
        return TRANS ("Any");

    if (! asArray)
    {
        if (difficulty.appliesToEasy())     return TRANS ("Easy");
        if (difficulty.appliesToMedium())   return TRANS ("Medium");
        if (difficulty.appliesToHard())     return TRANS ("Hard");

        jassertfalse; // ???
    }

    StringArray s;

    if (difficulty.appliesToEasy())     s.add (TRANS ("Easy"));
    if (difficulty.appliesToMedium())   s.add (TRANS ("Medium"));
    if (difficulty.appliesToHard())     s.add (TRANS ("Hard"));

    jassert (! s.isEmpty());

    return s.joinIntoString (", ");
}

//==============================================================================
/** */
enum class DoorLockState
{
    unlocked,
    needsKey,
    needsSpell,
    impassable,

    numDoorLockStates = impassable + 1
};

/** @returns */
inline String toString (DoorLockState doorLockState)
{
    switch (doorLockState)
    {
        case DoorLockState::unlocked:   return TRANS ("Unlocked");
        case DoorLockState::needsKey:   return TRANS ("Needs Key");
        case DoorLockState::needsSpell: return TRANS ("Needs Spell");
        case DoorLockState::impassable: return TRANS ("Impassable");

        default: break;
    };

    jassertfalse;
    return {};
}

//==============================================================================
/** */
enum class WindowTileType
{
    permanentlyOpen,
    permanentlyClosed,
    openable,
    openableWithUnlockableId,

    numWindowTileTypes = openableWithUnlockableId + 1
};

/** @returns */
inline String toString (WindowTileType windowTileType)
{
    switch (windowTileType)
    {
        case WindowTileType::permanentlyOpen:           return TRANS ("Stuck Open");
        case WindowTileType::permanentlyClosed:         return TRANS ("Stuck Closed");
        case WindowTileType::openable:                  return TRANS ("Openable/Closeable");
        case WindowTileType::openableWithUnlockableId:  return TRANS ("Openable (with ID)");

        default: break;
    };

    jassertfalse;
    return {};
}
