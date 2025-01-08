//==============================================================================
#undef DARK_ENGINE_CREATE_IDS
#define DARK_ENGINE_CREATE_IDS(X) \
    X (accuracy) \
    X (attack) \
    X (colour) \
    X (defense) \
    X (definitions) \
    X (description) \
    X (difficulty) \
    X (dimensions) \
    X (direction) \
    X (enemies) \
    X (experience) \
    X (fightingMoves) \
    X (gameMap) \
    X (hitPoints) \
    X (inanimateObjects) \
    X (interactionId) \
    X (inventory) \
    X (inventoryIcon) \
    X (isNPC) \
    X (level) \
    X (lightColour) \
    X (lightRadius) \
    X (lockState) \
    X (maxPowerPoints) \
    X (mapIcon) \
    X (material) \
    X (maxHitPoints) \
    X (moves) \
    X (moveCategory) \
    X (moveType) \
    X (name) \
    X (nature) \
    X (npcs) \
    X (opened) \
    X (player) \
    X (power) \
    X (powerPoints) \
    X (priority) \
    X (screenIcon) \
    X (secret) \
    X (specialAttack) \
    X (specialDefense) \
    X (speed) \
    X (statusCondition) \
    X (subtype) \
    X (tile) \
    X (type) \
    X (unlockableIDs) \
    X (weakAgainstType) \
    X (weapons) \
    X (weather) \
    X (weight) \
    X (windowTileSubtype) \
    X (world)

DARK_ENGINE_CREATE_IDS (CREATE_INLINE_IDENTIFIER)

//==============================================================================
/** @returns */
String getEquivalentName (const Identifier&);
