#include "PlatformPrecomp.h"
#include "util/ResourceUtils.h"
#include "World.h"

uint8* World::SaveToMem(int& offsetInOut)
{
    uint8* buffer = (uint8*)new uint8[(4024 + GetTileMap()->GetMemEstimate() + GetObjectMap()->GetMemEstimate())];
	
    uint16 version = 1; //i don't know what the version is or if it even exists!
    MemorySerialize(version, buffer, offsetInOut, true);

    uint32 flags = 0; //this is an entirely server side number so i won't bother
    MemorySerialize(flags, buffer, offsetInOut, true);

    MemorySerialize(m_name, buffer, offsetInOut, true);

    GetTileMap()->Serialize(buffer, offsetInOut, true);
    GetObjectMap()->Serialize(buffer, offsetInOut, true, false);

    return buffer;
}