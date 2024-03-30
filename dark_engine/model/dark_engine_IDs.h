//==============================================================================
#undef DARK_ENGINE_CREATE_IDS
#define DARK_ENGINE_CREATE_IDS(X) \
    X (attack) \
    X (colour) \
    X (defense) \
    X (definitions) \
    X (description) \
    X (difficulty) \
    X (dimensions) \
    X (direction) \
    X (earthDamage) \
    X (electricDamage) \
    X (enemies) \
    X (experience) \
    X (fightingMoves) \
    X (fireDamage) \
    X (gameMap) \
    X (hitPoints) \
    X (iceDamage) \
    X (inanimateObjects) \
    X (interactionId) \
    X (inventory) \
    X (inventoryIcon) \
    X (isNPC) \
    X (level) \
    X (lightColour) \
    X (lightRadius) \
    X (lockState) \
    X (makesContact) \
    X (mapIcon) \
    X (material) \
    X (maxHitPoints) \
    X (moves) \
    X (moveType) \
    X (name) \
    X (normalDamage) \
    X (npcs) \
    X (opened) \
    X (plasmaDamage) \
    X (player) \
    X (poisonDamage) \
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
    X (waterDamage) \
    X (weakAgainstType) \
    X (weapons) \
    X (windDamage) \
    X (windowTileSubtype) \
    X (world)

DARK_ENGINE_CREATE_IDS (CREATE_INLINE_IDENTIFIER)

//==============================================================================
/** @returns */
String getEquivalentName (const Identifier&);
