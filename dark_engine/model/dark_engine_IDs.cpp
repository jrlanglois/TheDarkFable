struct DarkEngineIdToNameMapper final
{
    DarkEngineIdToNameMapper()
    {
        ids.minimiseStorageOverheads();
        names.strings.minimiseStorageOverheads();

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
        NEEDS_TRANS ("Attack"),
        NEEDS_TRANS ("Colour"),
        NEEDS_TRANS ("Defense"),
        NEEDS_TRANS ("Definitions"),
        NEEDS_TRANS ("Description"),
        NEEDS_TRANS ("Difficulty"),
        NEEDS_TRANS ("Dimensions"),
        NEEDS_TRANS ("Direction"),
        NEEDS_TRANS ("Earth Damage"),
        NEEDS_TRANS ("Electric Damage"),
        NEEDS_TRANS ("Enemies"),
        NEEDS_TRANS ("Experience"),
        NEEDS_TRANS ("Eighting Moves"),
        NEEDS_TRANS ("Fire Damage"),
        NEEDS_TRANS ("Game Map"),
        NEEDS_TRANS ("Hit Points"),
        NEEDS_TRANS ("Ice Damage"),
        NEEDS_TRANS ("Inanimate Objects"),
        NEEDS_TRANS ("Interaction ID"),
        NEEDS_TRANS ("Inventory"),
        NEEDS_TRANS ("Inventory Icon"),
        NEEDS_TRANS ("IsNPC"),
        NEEDS_TRANS ("Level"),
        NEEDS_TRANS ("Light Colour"),
        NEEDS_TRANS ("Light Radius"),
        NEEDS_TRANS ("Lock State"),
        NEEDS_TRANS ("Makes Contact"),
        NEEDS_TRANS ("Map Icon"),
        NEEDS_TRANS ("Material"),
        NEEDS_TRANS ("Max Hit Points"),
        NEEDS_TRANS ("Moves"),
        NEEDS_TRANS ("Move Type"),
        NEEDS_TRANS ("Name"),
        NEEDS_TRANS ("Normal Damage"),
        NEEDS_TRANS ("NPCs"),
        NEEDS_TRANS ("Opened"),
        NEEDS_TRANS ("Plasma Damage"),
        NEEDS_TRANS ("Player"),
        NEEDS_TRANS ("Poison Damage"),
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
        NEEDS_TRANS ("Water Damage"),
        NEEDS_TRANS ("Weak-Against Type"),
        NEEDS_TRANS ("Weapons"),
        NEEDS_TRANS ("Wind Damage"),
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
