/*
 * Copyright (C) 2012 Igalia S.L.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301 USA
 */

#ifndef GLContext_h
#define GLContext_h

#include "GraphicsContext3D.h"
#include "PlatformDisplay.h"
#include <memory>
#include <wtf/Noncopyable.h>

#if USE(EGL)
#if !PLATFORM(GTK) && !PLATFORM(WPE)
#include "eglplatform.h"
#else
#if PLATFORM(WPE)
// FIXME: Find some way to deduce this.
#define __GBM__ 1
#endif
#include <EGL/eglplatform.h>
#endif
typedef EGLNativeWindowType GLNativeWindowType;
#else
typedef uint64_t GLNativeWindowType;
#endif

#if USE(CAIRO)
typedef struct _cairo_device cairo_device_t;
#endif

namespace WebCore {

class GLContext {
    WTF_MAKE_NONCOPYABLE(GLContext); WTF_MAKE_FAST_ALLOCATED;
public:
    static std::unique_ptr<GLContext> createContextForWindow(GLNativeWindowType windowHandle, PlatformDisplay* = nullptr);
    static std::unique_ptr<GLContext> createOffscreenContext(PlatformDisplay* = nullptr);
    static std::unique_ptr<GLContext> createSharingContext(PlatformDisplay&);
    static GLContext* current();
    static bool isExtensionSupported(const char* extensionList, const char* extension);

    PlatformDisplay& display() const { return m_display; }

    virtual ~GLContext();
    virtual bool makeContextCurrent();
    virtual void swapBuffers() = 0;
    virtual void waitNative() = 0;
    virtual bool canRenderToDefaultFramebuffer() = 0;
    virtual IntSize defaultFrameBufferSize() = 0;
    virtual void swapInterval(int) = 0;

    virtual bool isEGLContext() const = 0;

#if USE(CAIRO)
    virtual cairo_device_t* cairoDevice() = 0;
#endif

#if ENABLE(GRAPHICS_CONTEXT_3D)
    virtual PlatformGraphicsContext3D platformContext() = 0;
#endif

    class Data {
    public:
        virtual ~Data() = default;
    };

#if PLATFORM(X11)
private:
    static void addActiveContext(GLContext*);
    static void removeActiveContext(GLContext*);
    static void cleanupActiveContextsAtExit();
#endif

protected:
    GLContext(PlatformDisplay&);

    PlatformDisplay& m_display;
    std::unique_ptr<Data> m_contextData;
};

} // namespace WebCore

#endif // GLContext_h
