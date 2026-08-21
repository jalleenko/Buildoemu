#pragma once
#include "PlatformSetup.h"
#include "enet/enet.h"
#include "gt/Protocol.h"
#include "player/PlayerInventory.h"
#include "util/Variant.h"
#include "world/World.h"

class PlayerClient
{
public:
    enum class State
    {
        INITIALIZING = 0,   /*< this is the state default*/
        AUTHENTICATED       /*< when the client sends us its logon packet, we set this to acknowledge that the next generic message is something else*/
    };

    struct LoginMetadata
    {
        std::string name = "";
        std::string password = "";
        std::string requestedName = "";
    };

    void Initialize(ENetPeer* peer);
    void SendMessagePacket(const std::string& message, NetMessageType messageType, uint32 enetPacketFlags = ENET_PACKET_FLAG_RELIABLE);
    void SendGamePacket(GameUpdatePacket* packet, uint8* extendedData = nullptr, uint32 enetPacketFlags = ENET_PACKET_FLAG_RELIABLE);
    void HandleIncomingPacket(ENetPacket* packet);
    void SendFunctionCall(VariantList& function, int delayMS = 0, int destinationNetID = -1);
    void SendConsoleMessage(const std::string& message, int delayMS = 0);
    void SendInventory();
    void SendNews();
    void SendClothes(bool bPlayClothesChangeSound = false);
    void OnDisconnect();
    void Logout();

    ENetPeer* GetPeer() { return m_peer; }
    PlayerInventory* GetInventory() { return &m_inventory; }

    int GetNetID() const { return m_worldInfo.netID; }
    World* GetWorld() { return m_worldInfo.world; }

    bool HasGrowID() const;
    std::string GetUsername();

private:
    struct WorldInfo
    {
        World*  world   = nullptr;
        int     netID   = 1; // TODO: make this actually be based on some world generated number
    };

    void HandleGameMessage(ENetPacket* packet);
    void HandleGenericMessage(ENetPacket* packet);
    void HandleGamePacket(ENetPacket* packet);
    void ParseLogonMetadata(const char* metadata);

    void SendLogonAccept();
    void SendWorldSelect();
    void SendMapPacket(World* world);
    void SendSpawn();

    void OnEnterGame();

    ENetPeer* m_peer = nullptr;
    
    WorldInfo m_worldInfo{};
    LoginMetadata m_loginMetadata{};
    PlayerInventory m_inventory{};
    State m_state = State::INITIALIZING;

    uint32 m_skinColor = 2022356223;
};