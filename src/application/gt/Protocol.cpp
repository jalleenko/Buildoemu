#include "Protocol.h"

NetMessageType Protocol::GetMessageType(ENetPacket* packet)
{
    if (packet->dataLength < 4)
    {
        LogError("Got packet without a message header...");
        return NetMessageType::Unknown;
    }

    return *(NetMessageType*)packet->data;
}

char* Protocol::GetMessage(ENetPacket* packet)
{
    //egh, kinda ghetto way, but should do it.
    char* result = (char*)(packet->data + 4);
    result[packet->dataLength - 5] = 0;
    return result;
}

bool Protocol::GetStringFromText(const char* pText, const char* key, std::string& sOut, int count, bool bPicky)
{
    sOut.clear();

    if (!pText || !key)
    {
        return false;
    }
        
    int keyLen = strlen(key);
    if (keyLen <= 0)
    {
        return false;
    }

    int textLen = strlen(pText);
    if (textLen <= 0 || textLen < keyLen)
    {
        return false;
    }

    int matchCount = 0;
    int textPos = 0;
    int matchStart = 0;

    while (pText[textPos])
    {
        int prevMatchCount = matchCount;

        if (pText[textPos] == key[0] && strncmp(&pText[matchStart], key, keyLen) == 0)
        {
            if (++matchCount == count)
            {
                if (bPicky && matchStart > 0)
                {
                    char prev = pText[textPos - 1];
                    if (prev != '\n' && prev != '|' && prev != '\r')
                    {
                        matchCount = prevMatchCount;
                        textPos++;
                        matchStart++;
                        continue;
                    }
                }

                int valueStart = matchStart + keyLen;

                if (textLen <= valueStart)
                {
                    sOut.clear();
                    return false;
                }

                char firstChar = pText[valueStart];
                if (firstChar == '\r' || firstChar == '\n')
                {
                    sOut.clear();
                    return false;
                }

                int scanPos = valueStart + 1;

                while (scanPos < textLen)
                {
                    char c = pText[scanPos];
                    if (c <= '\r' && ((9217 >> c) & 1))
                    {
                        break;
                    }

                    scanPos++;
                }

                int valueLen = scanPos - matchStart - keyLen;
                sOut.resize(valueLen);
                for (int i = 0; i < valueLen; i++)
                {
                    sOut[i] = pText[matchStart + keyLen + i];
                }
                
                return true;
            }
        }

        textPos++;
        matchStart++;
    }

    return false;
}

uint8* Protocol::GetStructPointerFromTankPacket(ENetPacket* packet)
{
    if (packet->dataLength < 0x3C) // 60 bytes
        return NULL;

	uint32 dataLength = packet->dataLength;
    uint8* data = packet->data;

    if ((data[16] & uint32(GamePacketFlag::Extended)) != 0)
    {
        if (dataLength < *((uint32*)data + 14) + 60 )
        {
            LogMsg("Packet too small for extended packet to be valid");
            LogMsg("Sizeof float is %d.  TankUpdatePacket size: %d", 4, 56);
            return NULL;
        }
    }
    else
    {
        *((uint32*)data + 14) = 0; // dataLength
    }
	return (uint8*)(data + 4); // skip header (eNetMessageType)
}
