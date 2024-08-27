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
    dependencies:       squarepine_graphics

    END_JUCE_MODULE_DECLARATION
*/

//==============================================================================
#include <squarepine_audio/squarepine_audio.h>
#include <squarepine_graphics/squarepine_graphics.h>

//==============================================================================
namespace darkEngine
{
    using namespace sp;  

    #include "model/dark_engine_IDs.h"
    #include "model/dark_engine_Core.h"
}

#include "model/dark_engine_VariantConverters.h"

namespace darkEngine
{
    using namespace sp;

    #include "model/dark_engine_Entities.h"
    #include "model/dark_engine_Screen.h"

    #include "mechanics/dark_engine_GameEngine.h"
    #include "mechanics/dark_engine_GameProcessor.h"

    #include "components/dark_engine_PropertyComponents.h"
}

#endif // JRLANGLOIS_DARK_ENGINE_H
