/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== enemies.json ==================
static const unsigned char temp_binary_data_0[] =
"";

const char* enemies_json = (const char*) temp_binary_data_0;

//================== inanimateObjects.json ==================
static const unsigned char temp_binary_data_1[] =
"";

const char* inanimateObjects_json = (const char*) temp_binary_data_1;

//================== npcs.json ==================
static const unsigned char temp_binary_data_2[] =
"";

const char* npcs_json = (const char*) temp_binary_data_2;

//================== textTriggers.json ==================
static const unsigned char temp_binary_data_3[] =
"/*\r\n"
"  ==============================================================================\r\n"
"\r\n"
"    SourceCode.h\r\n"
"    Created: 22 Feb 2024 3:26:53pm\r\n"
"    Author:  joelr\r\n"
"\r\n"
"  ==============================================================================\r\n"
"*/\r\n"
"\r\n"
"#pragma once\r\n";

const char* textTriggers_json = (const char*) temp_binary_data_3;

//================== weapons.json ==================
static const unsigned char temp_binary_data_4[] =
"";

const char* weapons_json = (const char*) temp_binary_data_4;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0xc952af61:  numBytes = 0; return enemies_json;
        case 0xa878f96f:  numBytes = 0; return inanimateObjects_json;
        case 0x669e3355:  numBytes = 0; return npcs_json;
        case 0x09e19cbf:  numBytes = 266; return textTriggers_json;
        case 0x4eccb250:  numBytes = 0; return weapons_json;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "enemies_json",
    "inanimateObjects_json",
    "npcs_json",
    "textTriggers_json",
    "weapons_json"
};

const char* originalFilenames[] =
{
    "enemies.json",
    "inanimateObjects.json",
    "npcs.json",
    "textTriggers.json",
    "weapons.json"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
