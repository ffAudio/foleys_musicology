/*
 ==============================================================================
    Copyright (c) 2019 Foleys Finest Audio Ltd - Daniel Walz
    All rights reserved.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
    OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
    OF THE POSSIBILITY OF SUCH DAMAGE.
 ==============================================================================

    BEGIN_JUCE_MODULE_DECLARATION

    ID:            foleys_musicology
    vendor:        Foleys Finest Audio Ltd.
    version:       1.0.0
    name:          Foleys Musicology
    description:   Contains scoring and harmony classes including musicxml
    dependencies:  juce_core, juce_graphics, juce_gui_basics
    website:       https://foleysfinest.com/
    license:       proprietary

    END_JUCE_MODULE_DECLARATION

 ==============================================================================
 */

#pragma once


#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "Fonts/include/smufl.h"
#include "Fonts/include/bravura.h"
#include "Fonts/include/petaluma.h"

#include "Model/foleys_Note.h"

#include "Rendering/foleys_ScoreRenderer.h"
