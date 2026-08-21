#include "PlayerClient.h"
#include "PlatformEnums.h"
#include "PlatformSetup.h"
#include "ServerApplication.h"
#include "enet/enet.h"
#include "gt/Protocol.h"
#include "player/command/PlayerCommandManager.h"
#include "util/MiscUtils.h"
#include "util/ResourceUtils.h"
#include "util/Variant.h"
#include "world/World.h"

void PlayerClient::Initialize(ENetPeer* peer)
{
    m_peer = peer;

    SendMessagePacket("", NetMessageType::ServerHello);
}

void PlayerClient::SendMessagePacket(const std::string& message, NetMessageType messageType, uint32 enetPacketFlags)
{
    if (m_peer == nullptr)
    {
        return;
    }

    ENetPacket* packet = enet_packet_create(nullptr, message.length() + sizeof(uint32) + 1, enetPacketFlags);
    
    *(uint32*)packet->data = (uint32)messageType;
    memcpy(packet->data + 4, message.c_str(), message.length());

    if (enet_peer_send(m_peer, 0, packet) != 0)
    {
        enet_packet_destroy(packet);
    }
}

void PlayerClient::HandleIncomingPacket(ENetPacket* packet)
{
    if (packet == nullptr)
    {
        LogError("Packet is nullptr, strange.");
        return;
    }
    
    NetMessageType messageType = Protocol::GetMessageType(packet);
    
    switch(messageType)
    {
    case NetMessageType::GameMessage:
    {
        HandleGameMessage(packet);
        break;
    }
    case NetMessageType::GenericMessage:
    {
        HandleGenericMessage(packet);
        break;
    }
    case NetMessageType::GamePacket:
    {
        HandleGamePacket(packet);
        break;
    }
    default:
    {
        break;
    }
    }
}

void PlayerClient::SendGamePacket(GameUpdatePacket* packet, uint8* extendedData, uint32 enetPacketFlags)
{
    if (packet == nullptr || m_peer == nullptr)
    {
        return;
    }

    constexpr NetMessageType netMessageType = NetMessageType::GamePacket;
    
    ENetPacket* enetPacket = enet_packet_create(NULL, sizeof(GameUpdatePacket) + 5 + packet->extendedDataSize, enetPacketFlags);

    if (enetPacket == nullptr)
    {
        LogError("Weird, enetPacket is nullptr");
        return;
    }

    memcpy(enetPacket->data, &netMessageType , sizeof(netMessageType));
    memcpy(enetPacket->data + 4, packet, sizeof(GameUpdatePacket));
    
    if (packet->HasFlag(GamePacketFlag::Extended))
    {
        memcpy(&enetPacket->data[sizeof(GameUpdatePacket) + 4], extendedData, packet->extendedDataSize);
    }
    
    if (enet_peer_send(m_peer, 0, enetPacket) != 0)
    {
        enet_packet_destroy(enetPacket);
    }
}

void PlayerClient::HandleGameMessage(ENetPacket* packet)
{
    if (packet == nullptr)
    {
        return;
    }

    char* message = Protocol::GetMessage(packet);
    
    LogMsg("game message: %s", message);

    std::string command = "";
    if (!Protocol::GetStringFromText(message, "action|", command, 1, false))
    {
        LogError("Bad game message");
        return;
    }

    if (command == "join_request")
    {
        std::string worldName = "";

        if (!Protocol::GetStringFromText(message, "name|", worldName, 1, true))
        {
            LogError("Bad join request");
            return;
        }

        m_worldInfo.world = GetApp()->GetWorldManager().GetOrRegisterWorld(worldName);

        SendMapPacket(m_worldInfo.world);
        SendConsoleMessage("`wEntered a world. Hopefully.");
        SendSpawn();
        SendClothes();
    }
    else if (command == "quit")
    {
        Logout();
    }
}

void PlayerClient::SendSpawn()
{
    VariantList v{};
    v.Get(0).Set("OnSpawn");
    v.Get(1).Set("spawn|avatar\nnetID|1\nuserID|1\ncolrect|0|0|20|30|\nposXY|500|928|\nname|blud\ncountry|us\ntype|local");
    SendFunctionCall(v, -1);
}

void PlayerClient::SendMapPacket(World* world)
{
    GameUpdatePacket packet{};
    packet.SetType(GamePacketType::SendMap);   
    packet.SetFlag(GamePacketFlag::Extended);
    
    int extendedSize = 0;
    uint8* extendedData = world->SaveToMem(extendedSize);
    
    packet.extendedDataSize = (uint32)extendedSize;

    SendGamePacket(&packet, extendedData);
    SAFE_DELETE_ARRAY(extendedData);
}

void PlayerClient::HandleGenericMessage(ENetPacket* packet)
{
    if (packet == nullptr)
    {
        return;
    }

    char* message = Protocol::GetMessage(packet);

    LogMsg("generic message: %s", message);

    if (m_state == State::INITIALIZING)
    {
        //LogMsg("Client sent a hello: %s", message);
        m_state = State::AUTHENTICATED;

        SendLogonAccept();
    }
    else
    {
        std::string command = "";
        if (!Protocol::GetStringFromText(message, "action|", command, 1, false))
        {
            LogError("Bad generic message");
            return;
        }
     
        // if-else vomit, but I don't have time to make anything better    
        if (command == "refresh_item_data")
        {
            SendConsoleMessage("One moment, updating item data...");

            GameUpdatePacket packet{};
            packet.SetType(GamePacketType::SendItemDatabaseData);
            packet.SetFlag(GamePacketFlag::Extended);
            packet.extendedDataSize = GetApp()->GetItemManager().GetPlayerItemData().size;
            packet.destinationNetID = -1;

            SendGamePacket(&packet, GetApp()->GetItemManager().GetPlayerItemData().data);
        }
        else if (command == "setSkin")
        {
            //could be used by a cheat to change to any skin color. I don't care, though.
            std::string colorStr = "";

            if (!Protocol::GetStringFromText(message, "color|", colorStr, 1, true))
            {
                return;
            }

            m_skinColor = std::stoull(colorStr.c_str());
            
            SendClothes();
        }
        else if (command == "enter_game")
        {
            SendConsoleMessage("`2Entered the game!");
            SendInventory();
            SendWorldSelect();
            SendNews();
        }
        else if (command == "input")
        {

            std::string inputMsg = "";
            if (!Protocol::GetStringFromText(message, "|text|", inputMsg, 1, false))
            {
                return;
            }
        
            if (inputMsg.empty())
            {
                return;
            }

            if (inputMsg[0] == '/')
            {
                SendConsoleMessage("`6" + inputMsg + "``");
                PlayerCommandManager::Get()->PerformCommand(this, inputMsg);
            }
        }
    }
}

void PlayerClient::SendWorldSelect()
{
    VariantList v{};
    v.Get(0).Set("OnRequestWorldSelectMenu");
    v.Get(1).Set("default|\nadd_floater|DUNGEON|0|0.6|3529161471|\nadd_floater|GIA|3|0.6|3529161471|\nadd_floater|FISTFULLOFBEES|0|0.6|3529161471|");
    SendFunctionCall(v);
}

void PlayerClient::SendInventory()
{
    uint8* buffer = (uint8*)malloc(1024); //yeah, it's way too much for what we write.
    
    int offset = 0;
    m_inventory.Serialize(buffer, offset, true);

    GameUpdatePacket packet{};
    packet.SetType(GamePacketType::SendInventoryState);
    packet.SetFlag(GamePacketFlag::Extended);
    packet.extendedDataSize = offset;

    SendGamePacket(&packet, buffer);

    SAFE_FREE(buffer);
}

void PlayerClient::SendNews()
{
    if (!GetApp()->GetConfig().enableNews)
    {
        return;
    }

    VariantList v{};
    v.Get(0).Set("OnDialogRequest");
    v.Get(1).Set(GetApp()->GetConfig().newsGazette);
    SendFunctionCall(v);
}

void PlayerClient::HandleGamePacket(ENetPacket* packet)
{
    if (packet == nullptr)
    {
        return;
    }
}

void PlayerClient::SendClothes(bool bPlayClothesChangeSound)
{
    VariantList v{};
    v.Get(0).Set("OnSetClothing");
    v.Get(1).Set(CL_Vec3f(0.0f, 0.0f, 0.0f));
    v.Get(2).Set(CL_Vec3f(0.0f, 0.0f, 0.0f));
    v.Get(3).Set(m_skinColor);
    v.Get(4).Set(uint32(bPlayClothesChangeSound));
    SendFunctionCall(v, 0, GetNetID());
}

void PlayerClient::SendConsoleMessage(const std::string& message, int delayMS)
{
    VariantList v{};
    v.Get(0).Set("OnConsoleMessage");
    v.Get(1).Set(message);
    SendFunctionCall(v, delayMS);
}

void PlayerClient::OnDisconnect()
{
    // well, we could put some disconnection logic here if we wanted to.
}

void PlayerClient::SendFunctionCall(VariantList& function, int delayMS, int destinationNetID)
{
    GameUpdatePacket packet{};
    packet.SetType(GamePacketType::FunctionCall);
    packet.SetFlag(GamePacketFlag::Extended);
    packet.delayMS = delayMS;
    packet.destinationNetID = destinationNetID;

    uint8* data = function.SerializeToMem(&packet.extendedDataSize, nullptr);
    SendGamePacket(&packet, data);
    SAFE_DELETE_ARRAY(data);
}

void PlayerClient::SendLogonAccept()
{
    VariantList v{};
    v.Get(0).Set("OnInitialLogonAccepted");
    v.Get(1).Set(GetApp()->GetItemManager().GetPlayerItemData().hash);
    v.Get(2).Set(GetApp()->GetConfig().cdnServerUrl);
    v.Get(3).Set(GetApp()->GetConfig().cdnBaseDirectory + "/cache/");
    
    // at this point growtopia didn't yet write anything else, I think.
    SendFunctionCall(v);
}

void PlayerClient::Logout()
{
    // We might want something more robust later
    enet_peer_disconnect_later(m_peer, 0);
}