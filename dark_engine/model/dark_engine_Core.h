/** */
class EngineObject : public Identifiable
{
public:
    /** */
    EngineObject (const Identifier& id, UndoManager* undoManager = nullptr) :
        Identifiable (id),
        state (id),
        name (state, nameId, undoManager),
        description (state, descriptionId, undoManager)
    {
    }

    /** */
    EngineObject (const ValueTree& startState, UndoManager* undoManager = nullptr) :
        EngineObject (startState.getType(), undoManager)
    {
        state = startState;
    }

    //==============================================================================
    /** Untranslated. */
    EngineObject& setName (const String& newName, UndoManager* undoManager = nullptr)
    {
        name.setValue (newName, undoManager);
        return *this;
    }

    /** @returns */
    [[nodiscard]] String getName() const noexcept { return TRANS (name.get()); }

    /** Untranslated. */
    EngineObject& setDescription (const String& newDescription, UndoManager* undoManager = nullptr)
    {
        description.setValue (newDescription, undoManager);
        return *this;
    }

    /** @returns */
    [[nodiscard]] String getDescription() const noexcept { return TRANS (description.get()); }

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

    //==============================================================================
    CREATE_INLINE_CLASS_IDENTIFIER (name)
    CREATE_INLINE_CLASS_IDENTIFIER (description)

protected:
    //==============================================================================
    ValueTree state;

private:
    //==============================================================================
    CachedValue<String> name, description;

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
        EngineObject (id, undoManager),
        position (state, positionId, undoManager, {}),
        interactionId (state, interactionIdId, undoManager, interactionIdToUse)
    {
    }

    /** */
    WorldObject (const ValueTree& startState, UndoManager* undoManager = nullptr) :
        EngineObject (startState, undoManager),
        position (state, positionId, undoManager, {}),
        interactionId (state, interactionIdId, undoManager)
    {
    }

    //==============================================================================
    /** */
    WorldObject& setPosition (Point<int> newPosition, UndoManager* undoManager = nullptr)
    {
        position.setValue (newPosition, undoManager);
        return *this;
    }
    /** */
    WorldObject& setPosition (int x, int y, UndoManager* undoManager = nullptr)
    {
        setPosition ({ x, y }, undoManager);
        return *this;
    }
    /** @returns */
    [[nodiscard]] Point<int> getPosition() const noexcept   { return position.get(); }

    //==============================================================================
    /** @returns */
    [[nodiscard]] String getInteractionId() const noexcept  { return interactionId.get(); }
    /** @returns */
    [[nodiscard]] bool isInteractable() const noexcept      { return getInteractionId().isNotEmpty(); }
    /** */
    WorldObject& setInteractableId (StringRef newInteractionId, UndoManager* undoManager = nullptr)
    {
        interactionId.setValue (newInteractionId, undoManager);
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
    virtual String createMapIcon() const { return {}; }
    /** */
    virtual String createScreenIcon() const { return {}; }
    /** */
    virtual String createInventoryIcon() const { return {}; }

    //==============================================================================
    CREATE_INLINE_CLASS_IDENTIFIER (position)
    CREATE_INLINE_CLASS_IDENTIFIER (interactionId)

private:
    //==============================================================================
    CachedValue<Point<int>> position;
    CachedValue<String> interactionId;

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
    /** Returns the raw flags that are set for this Justification object. */
    [[nodiscard]] constexpr int getFlags() const noexcept                                   { return flags; }

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
        burned      = 1 << 1,
        /** */
        frozen      = 1 << 2,
        /** */
        paralysed   = 1 << 3,
        /** */
        poisoned    = 1 << 4,
        /** */
        asleep      = 1 << 5,
        /** */
        drowsy      = 1 << 6,
        /** */
        frostbitten = 1 << 7,
        /** */
        bound       = 1 << 8,
        /** */
        cursed      = 1 << 9
    };

private:
    //==============================================================================
    int flags = 0;
};

/** @returns */
inline [[nodiscard]] String toString (StatusCondition statusCondition)
{
    if (statusCondition.isNormal())
        return NEEDS_TRANS ("Normal");

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
    north,
    east,
    south,
    west
};

/** */
inline [[nodiscard]] double toDegrees (CardinalDirection cd) noexcept
{
    switch (cd)
    {
        case CardinalDirection::notApplicable:  return 0.0;
        case CardinalDirection::north:          return 0.0;
        case CardinalDirection::east:           return 90.0;
        case CardinalDirection::south:          return 180.0;
        case CardinalDirection::west:           return 270.0;

        default:
            jassertfalse;
        break;
    };

    return 0.0;
}

/** @returns */
inline [[nodiscard]] String toString (CardinalDirection cd)
{
    switch (cd)
    {
        case CardinalDirection::notApplicable:  return TRANS ("(N/A)");
        case CardinalDirection::north:          return TRANS ("north");
        case CardinalDirection::east:           return TRANS ("east");
        case CardinalDirection::south:          return TRANS ("south");
        case CardinalDirection::west:           return TRANS ("west");
        default: break;
    };

    jassertfalse;
    return {};
}

/** */
inline [[nodiscard]] double snapAngleToWorld (double angleDegrees) noexcept
{
    return std::round (angleDegrees / 90.0) * 90.0;
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
    plastic
};

/** @returns */
inline [[nodiscard]] String toString (Material materialType, bool asAdjective = false)
{
    switch (materialType)
    {
        case Material::tile:        { return asAdjective ? NEEDS_TRANS ("tiled")    : NEEDS_TRANS ("tile"); }
        case Material::brick:       { return asAdjective ? NEEDS_TRANS ("brick")    : NEEDS_TRANS ("brick"); }
        case Material::glass:       { return asAdjective ? NEEDS_TRANS ("glass")    : NEEDS_TRANS ("glass"); }
        case Material::wood:        { return asAdjective ? NEEDS_TRANS ("wooden")   : NEEDS_TRANS ("wood"); }
        case Material::metal:       { return asAdjective ? NEEDS_TRANS ("metallic") : NEEDS_TRANS ("metal"); }
        case Material::vinyl:       { return asAdjective ? NEEDS_TRANS ("vinyl")    : NEEDS_TRANS ("vinyl"); }
        case Material::stone:       { return asAdjective ? NEEDS_TRANS ("stone")    : NEEDS_TRANS ("stone"); }
        case Material::marble:      { return asAdjective ? NEEDS_TRANS ("marbled")  : NEEDS_TRANS ("marble"); }
        case Material::concrete:    { return asAdjective ? NEEDS_TRANS ("concrete") : NEEDS_TRANS ("concrete"); }
        case Material::plastic:     { return asAdjective ? NEEDS_TRANS ("plastic")  : NEEDS_TRANS ("plastic"); }

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
    fire
};

/** */
inline [[nodiscard]] String toString (MoveType moveType)
{
    switch (moveType)
    {
        case MoveType::normal:  return NEEDS_TRANS ("Normal");
        case MoveType::earth:   return NEEDS_TRANS ("Earth");
        case MoveType::wind:    return NEEDS_TRANS ("Wind");
        case MoveType::water:   return NEEDS_TRANS ("Water");
        case MoveType::fire:    return NEEDS_TRANS ("Fire");

        default: break;
    };

    jassertfalse;
    return {};
}
