#include "PlayerCommands.h"
#include "ServerApplication.h"
#include "util/MiscUtils.h"

void PlayerCommands::CheatCommand(PC_PARAMS)
{
    if (pClient == nullptr)
    {
        return;
    }
    
    PlayerInventory::Item item{};
    for (int i = 2; i < 111; i++)
    {
        if (i == 18 || i == 32)
        {
            item.m_count = 1;
        }
        else
        {
            item.m_count = RandomRange(33, 66);
        }

        item.m_itemID = i;
        pClient->GetInventory()->Add(item);
    }

    pClient->SendInventory();
    pClient->SendConsoleMessage(">> Cheat activated, gave you some items.");   
}

void PlayerCommands::NewsCommand(PC_PARAMS)
{
    pClient->SendConsoleMessage("`5Opening news gazette...");
    pClient->SendNews();
}

void PlayerCommands::ReloadNewsCommand(PC_PARAMS)
{
    GetApp()->LoadNews();
    pClient->SendConsoleMessage("`2Reloaded news.");
}