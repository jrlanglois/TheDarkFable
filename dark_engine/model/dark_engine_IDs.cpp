struct DarkEngineIdToNameMapper final
{
    DarkEngineIdToNameMapper()
    {
        ids.minimiseStorageOverheads();
        names.strings.minimiseStorageOverheads();

        /** If you hit this, you probably added a new ID but didn't add a
            translatable equivalent name for it.

            This name will be used for display purposes, like when editing
            game properties and children in ValueTree format.
        */
        jassert (ids.size() == names.size());
    }

    #undef DARK_ENGINE_CREATE_ITEM
    #define DARK_ENGINE_CREATE_ITEM(name) name##Id,

    Array<Identifier> ids =
    {
        DARK_ENGINE_CREATE_IDS (DARK_ENGINE_CREATE_ITEM)
    };

    #undef DARK_ENGINE_CREATE_ITEM

    StringArray names =
    {
        NEEDS_TRANS ("Accuracy"),
        NEEDS_TRANS ("Attack"),
        NEEDS_TRANS ("Colour"),
        NEEDS_TRANS ("Defense"),
        NEEDS_TRANS ("Definitions"),
        NEEDS_TRANS ("Description"),
        NEEDS_TRANS ("Difficulty"),
        NEEDS_TRANS ("Dimensions"),
        NEEDS_TRANS ("Direction"),
        NEEDS_TRANS ("Enemies"),
        NEEDS_TRANS ("Experience"),
        NEEDS_TRANS ("Eighting Moves"),
        NEEDS_TRANS ("Game Map"),
        NEEDS_TRANS ("Hit Points"),
        NEEDS_TRANS ("Inanimate Objects"),
        NEEDS_TRANS ("Interaction ID"),
        NEEDS_TRANS ("Inventory"),
        NEEDS_TRANS ("Inventory Icon"),
        NEEDS_TRANS ("IsNPC"),
        NEEDS_TRANS ("Level"),
        NEEDS_TRANS ("Light Colour"),
        NEEDS_TRANS ("Light Radius"),
        NEEDS_TRANS ("Lock State"),
        NEEDS_TRANS ("Max Power Points"),
        NEEDS_TRANS ("Map Icon"),
        NEEDS_TRANS ("Material"),
        NEEDS_TRANS ("Max Hit Points"),
        NEEDS_TRANS ("Moves"),
        NEEDS_TRANS ("Moves Category"),
        NEEDS_TRANS ("Move Type"),
        NEEDS_TRANS ("Name"),
        NEEDS_TRANS ("Nature"),
        NEEDS_TRANS ("NPCs"),
        NEEDS_TRANS ("Opened"),
        NEEDS_TRANS ("Player"),
        NEEDS_TRANS ("Power"),
        NEEDS_TRANS ("Power Points"),
        NEEDS_TRANS ("Priority"),
        NEEDS_TRANS ("Screen Icon"),
        NEEDS_TRANS ("Secret"),
        NEEDS_TRANS ("Special Attack"),
        NEEDS_TRANS ("Special Defense"),
        NEEDS_TRANS ("Speed"),
        NEEDS_TRANS ("Status Condition"),
        NEEDS_TRANS ("Subtype"),
        NEEDS_TRANS ("Tile"),
        NEEDS_TRANS ("Type"),
        NEEDS_TRANS ("Unlockable IDs"),
        NEEDS_TRANS ("Weak-Against Type"),
        NEEDS_TRANS ("Weapons"),
        NEEDS_TRANS ("Weather"),
        NEEDS_TRANS ("Weight"),
        NEEDS_TRANS ("Window Tile Subtype"),
        NEEDS_TRANS ("World")
    };
};

String getEquivalentName (const Identifier& id)
{
    static const DarkEngineIdToNameMapper mapper;

    jassert (mapper.ids.contains (id));

    if (const int index = mapper.ids.indexOf (id); index >= 0)
        return TRANS (mapper.names.strings.getUnchecked (index));

    return {};
}
