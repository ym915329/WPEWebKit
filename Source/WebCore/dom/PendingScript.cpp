/*
 * Copyright (C) 2010 Google, Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "PendingScript.h"

#include "Element.h"
#include "PendingScriptClient.h"

namespace WebCore {

Ref<PendingScript> PendingScript::create(Element& element, LoadableScript& loadableScript)
{
    Ref<PendingScript> pendingScript = adoptRef(*new PendingScript(element, loadableScript));
    loadableScript.addClient(pendingScript.get());
    return pendingScript;
}

Ref<PendingScript> PendingScript::create(Element& element, TextPosition scriptStartPosition)
{
    return adoptRef(*new PendingScript(element, scriptStartPosition));
}

PendingScript::PendingScript(Element& element, TextPosition startingPosition)
    : m_element(element)
    , m_startingPosition(startingPosition)
{
}

PendingScript::PendingScript(Element& element, LoadableScript& loadableScript)
    : m_element(element)
    , m_loadableScript(&loadableScript)
{
}

PendingScript::~PendingScript()
{
    if (m_loadableScript)
        m_loadableScript->removeClient(*this);
}

LoadableScript* PendingScript::loadableScript() const
{
    return m_loadableScript.get();
}

void PendingScript::notifyClientFinished()
{
    Ref<PendingScript> protectedThis(*this);
    if (m_client)
        m_client->notifyFinished(*this);
}

void PendingScript::notifyFinished(LoadableScript&)
{
    notifyClientFinished();
}

bool PendingScript::isLoaded() const
{
    return m_loadableScript && m_loadableScript->isLoaded();
}

bool PendingScript::wasErrored() const
{
    return m_loadableScript && m_loadableScript->wasErrored();
}

void PendingScript::setClient(PendingScriptClient* client)
{
    ASSERT(!m_client);
    m_client = client;
    if (isLoaded())
        notifyClientFinished();
}

void PendingScript::clearClient()
{
    ASSERT(m_client);
    m_client = nullptr;
}

}
