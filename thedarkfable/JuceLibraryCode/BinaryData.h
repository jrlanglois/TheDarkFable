/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   enemies_json;
    const int            enemies_jsonSize = 0;

    extern const char*   inanimateObjects_json;
    const int            inanimateObjects_jsonSize = 0;

    extern const char*   npcs_json;
    const int            npcs_jsonSize = 0;

    extern const char*   textTriggers_json;
    const int            textTriggers_jsonSize = 266;

    extern const char*   weapons_json;
    const int            weapons_jsonSize = 0;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 5;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
