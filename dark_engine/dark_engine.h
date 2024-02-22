#ifndef JRLANGLOIS_DARK_ENGINE_H
#define JRLANGLOIS_DARK_ENGINE_H

/** BEGIN_JUCE_MODULE_DECLARATION

    ID:                 dark_engine
    vendor:             jrlanglois
    version:            0.0.1
    name:               Dark Engine
    description:        A text-based adventure game engine.
    website:            https://www.jrlanglois.io
    license:            Beerware
    minimumCppStandard: 20
    dependencies:       squarepine_audio squarepine_graphics

    END_JUCE_MODULE_DECLARATION
*/

//==============================================================================
#include <squarepine_audio/squarepine_audio.h>
#include <squarepine_graphics/squarepine_graphics.h>

//==============================================================================
namespace darkEngine
{
    using namespace juce;
    using namespace sp;

    //==============================================================================
    #include "model/dark_engine_Core.h"
}

namespace juce
{
    using namespace sp;
    using namespace darkEngine;

    //==============================================================================
    /** */
    template<>
    struct VariantConverter<StatusCondition> final
    {
        /** @returns */
        static StatusCondition fromVar (const var& v) { return static_cast<int> (v); }

        /** @returns */
        static var toVar (const StatusCondition& sc)  { return sc.getFlags(); }
    };

    //==============================================================================
    /** */
    template<>
    struct VariantConverter<MoveType> final
    {
        /** @returns */
        static MoveType fromVar (const var& v) { return static_cast<MoveType> (static_cast<int> (v)); }

        /** @returns */
        static var toVar (const MoveType& mt)  { return static_cast<int> (mt); }
    };
}

namespace darkEngine
{
    #include "model/dark_engine_Entities.h"
    #include "model/dark_engine_Screen.h"

    #include "world/dark_engine_Enemies.h"
    #include "world/dark_engine_Moves.h"
    #include "world/dark_engine_Weapons.h"
    #include "world/dark_engine_NPCs.h"
    #include "world/dark_engine_InanimateObjects.h"

    #include "mechanics/dark_engine_GameEngine.h"
    #include "mechanics/dark_engine_GameProcessor.h"
}

#endif // JRLANGLOIS_DARK_ENGINE_H
