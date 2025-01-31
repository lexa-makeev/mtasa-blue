/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        game_sa/CAnimBlockSA.cpp
 *  PURPOSE:     Animation block
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

#include <game/FileTypes.h>

int CAnimBlockSAInterface::GetIndex()
{
    return (((DWORD)this - ARRAY_CAnimManager_AnimBlocks) / 32);
}

void CAnimBlockSA::Request(EModelRequestType requestType, bool bAllowBlockingFail)
{
    if (IsLoaded())
        return;

    DWORD dwModelID = GetIndex() + GetBaseIDforIFP();

    if (requestType == BLOCKING)
    {
        CModelInfo* pModelInfo = pGame->GetModelInfo(dwModelID);
        pGame->GetStreaming()->RequestModel(dwModelID, 0x14);
        AddRef();
        pGame->GetStreaming()->LoadAllRequestedModels(true);

        // bAllowBlockingFail is for debugging only (Blocking load should never fail)
        if (!bAllowBlockingFail)
        {
            assert(IsLoaded());
            assert(pGame->GetStreaming()->HasModelLoaded(dwModelID));
        }
    }
    else
    {
        pGame->GetStreaming()->RequestModel(dwModelID, 0x04);
        AddRef();
    }
}

CAnimBlendHierarchySAInterface* CAnimBlockSA::GetAnimationHierarchyInterface(size_t iAnimation)
{
    if (!IsLoaded())
        return nullptr;

    iAnimation += m_pInterface->idOffset;
    BYTE* arrAnimations = reinterpret_cast<BYTE*>(ARRAY_CAnimManager_Animations);
    return reinterpret_cast<CAnimBlendHierarchySAInterface*>(arrAnimations + sizeof(CAnimBlendHierarchySAInterface) * iAnimation);
}
