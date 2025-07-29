/**
 * File name: entropictron_lv2.cpp
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor 
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include <lv2/lv2plug.in/ns/ext/instance-access/instance-access.h>
#include <lv2/lv2plug.in/ns/ext/state/state.h>

#include "MainWindow.h"
#include "entropictron_api.h"
#include "kit_state.h"
#include "GeonkickConfig.h"

#include <RkMain.h>
#include <RkPlatform.h>

#include <vector>
#include <memory>
#include <atomic>

#ifdef ENTROPICTRON_SINGLE
#define ENTROPICTRON_URI "http://entropictron.org/entropictron/single"
#define ENTROPICTRON_URI_UI "http://entropictron.org/entropictron/single#ui"
#define ENTROPICTRON_URI_STATE "http://entropictron.org/entropictron/single#state"
#else
#define ENTROPICTRON_URI "http://entropictron.org/entropictron"
#define ENTROPICTRON_URI_UI "http://entropictron.org/entropictron#ui"
#define ENTROPICTRON_URI_STATE "http://entropictron.org/entropictron#state"
#endif // ENTROPICTRON_SINGLE
#define ENTROPICTRON_URI_STATE_CHANGED "http://lv2plug.in/ns/ext/state#StateChanged"

class GeonkickLv2Plugin : public RkObject
{
  public:
        GeonkickLv2Plugin(int sampleRate)
                : entropictronApi{std::make_unique<GeonkickApi>(sampleRate, GeonkickApi::InstanceType::Lv2)}
                , midiIn{nullptr}
                , notifyHostChannel{nullptr}
                , atomInfo{0}
                , kickIsUpdated{false}
        {
                RK_ACT_BIND(entropictronApi.get(), kickUpdated, RK_ACT_ARGS(), this, kickUpdated());
                RK_ACT_BIND(entropictronApi.get(), stateChanged, RK_ACT_ARGS(), this, kickUpdated());
        }

        bool init()
        {
                if (!entropictronApi->init()) {
                        ENTROPICTRON_LOG_ERROR("can't init DSP");
                        return false;
                }
                outputChannels = std::vector<float*>(2 * entropictronApi->numberOfChannels(), nullptr);
                return true;
        }

        void setAudioChannel(float *data, size_t channel)
        {
                if (channel < outputChannels.size())
                        outputChannels[channel] = data;
        }

        void setMidiIn(LV2_Atom_Sequence *data)
        {
                midiIn = data;
        }

        void setNotifyHostChannel(LV2_Atom_Sequence *data)
        {
                notifyHostChannel = data;
        }

        void setStateId(LV2_URID id)
        {
                atomInfo.stateId = id;
        }

        LV2_URID getStateId() const
        {
                return atomInfo.stateId;
        }

        void setAtomChunkId(LV2_URID id)
        {
                atomInfo.atomStringId = id;
        }

        LV2_URID getAtomChunkId() const
        {
                return atomInfo.atomStringId;
        }

        void setAtomSequence(LV2_URID sequence)
        {
                atomInfo.atomSequence = sequence;
        }

        LV2_URID getAtomSequence(void) const
        {
                return atomInfo.atomSequence;
        }

        void setAtomStateChanged(LV2_URID changed)
        {
                atomInfo.atomStateChanged = changed;
        }

        LV2_URID getAtomStateChanged(void) const
        {
                return atomInfo.atomStateChanged;
        }

        void setAtomObject(LV2_URID object)
        {
                atomInfo.atomObject = object;
        }

        LV2_URID getAtomObject(void) const
        {
                return atomInfo.atomObject;
        }

        void setStateData(const std::string &data, int flags = 0)
        {
                RK_UNUSED(flags);
                if (data.find("UiSettings") == std::string::npos) {
                        ENTROPICTRON_LOG_INFO("old plugin state version");
                        entropictronApi->setKitState(data);
                        entropictronApi->notifyUpdateGui();
                        entropictronApi->notifyKitUpdated();
                } else {
                        entropictronApi->setState(data);
                        entropictronApi->notifyUpdateGui();
                        entropictronApi->notifyKitUpdated();
                }
        }

        std::string getStateData()
        {
                return entropictronApi->getState();
        }

        GeonkickApi* getApi() const
        {
                return entropictronApi.get();
        }

        bool isNote(const uint8_t* buffer) const
        {
                return (((*(buffer) & 0xf0)) == 0x90) || (((*(buffer) & 0xf0)) == 0x80);
        }

        void processSamples(size_t nsamples)
        {
                if (!midiIn || nsamples < 1)
                        return;

                for (size_t i = 0; i < outputChannels.size(); i++)
                        memset(outputChannels[i], 0, nsamples * sizeof(float));

                size_t currentFrame = 0;
                size_t offset = 0;
                auto it = lv2_atom_sequence_begin(&midiIn->body);
                while (!lv2_atom_sequence_is_end(&midiIn->body, midiIn->atom.size, it)) {
                        size_t eventFrame = it->time.frames;
                        size_t size = eventFrame - currentFrame;

                        if (size > 0 && size <= nsamples) {
                                entropictronApi->process(outputChannels.data(), offset, size);
                                offset += size;
                        }

                        const uint8_t* const msg = (const uint8_t*)(it + 1);
                        switch (lv2_midi_message_type(msg))
                        {
                                case LV2_MIDI_MSG_NOTE_ON:
                                        entropictronApi->setKeyPressed(true, msg[1], msg[2]);
                                        break;
                                case LV2_MIDI_MSG_NOTE_OFF:
                                        entropictronApi->setKeyPressed(false, msg[1], msg[2]);
                                        break;
                                default:
                                        break;
                        }
                        currentFrame = eventFrame;
                        it = lv2_atom_sequence_next(it);
                }

                if (currentFrame < nsamples)
                        entropictronApi->process(outputChannels.data(), offset, nsamples - currentFrame);

                if (isKickUpdated()) {
                        notifyHost();
                        setKickUpdated(false);
                }
        }

        void notifyHost() const
        {
                if (!notifyHostChannel)
                        return;

                auto sequence = static_cast<LV2_Atom_Sequence*>(notifyHostChannel);
                size_t neededAtomSize = sizeof(LV2_Atom_Event) + sizeof(LV2_Atom_Object_Body);
                if (sequence) {
                        sequence->atom.type = getAtomSequence();
                        sequence->atom.size = sizeof(LV2_Atom_Sequence_Body);
                        sequence->body.unit = 0;
                        sequence->body.pad  = 0;
                        auto event = reinterpret_cast<LV2_Atom_Event*>(sequence + 1);
                        event->time.frames = 0;
                        auto atomObject = reinterpret_cast<LV2_Atom_Object*>(&event->body);
                        atomObject->atom.type = getAtomObject();
                        atomObject->atom.size = sizeof(LV2_Atom_Object_Body);
                        atomObject->body.id = 0;
                        atomObject->body.otype = getAtomStateChanged();
                        sequence->atom.size += neededAtomSize;
                }
        }

protected:
        void setKickUpdated(bool b)
        {
                kickIsUpdated = b;
        }

        void kickUpdated()
        {
                setKickUpdated(true);
        }

        bool isKickUpdated() const
        {
                return kickIsUpdated;
        }

private:
        std::unique_ptr<GeonkickApi> entropictronApi;
        LV2_Atom_Sequence *midiIn;
        LV2_Atom_Sequence *notifyHostChannel;
        std::vector<float*> outputChannels;

        struct AtomInfo {
                LV2_URID stateId;
                LV2_URID atomStringId;
                LV2_URID atomSequence;
                LV2_URID atomStateChanged;
                LV2_URID atomObject;
        };

        AtomInfo atomInfo;
        std::atomic<bool> kickIsUpdated;
};

/**
 * Creates and shows an instance of Geonkick GUI that takes
 * the entropictron API instance as a pointer.
 */
static LV2UI_Handle ent_instantiate_ui(const LV2UI_Descriptor*   descriptor,
                                         const char*               plugin_uri,
                                         const char*               bundle_path,
                                         LV2UI_Write_Function      write_function,
                                         LV2UI_Controller          controller,
                                         LV2UI_Widget*             widget,
                                         const LV2_Feature* const* features)
{
        GeonkickLv2Plugin *entropictronLv2PLugin = nullptr;
        if (!features)
                return nullptr;
        void *parent = nullptr;
        LV2UI_Resize *resize = nullptr;
        const LV2_Feature *feature;
        while ((feature = *features)) {
                if (std::string(feature->URI) == std::string(LV2_UI__parent))
                        parent = static_cast<LV2UI_Resize*>(feature->data);

                if (std::string(feature->URI) == std::string(LV2_UI__resize))
                        resize = static_cast<LV2UI_Resize*>(feature->data);

                if (std::string(feature->URI) == std::string(LV2_INSTANCE_ACCESS_URI)) {
                        entropictronLv2PLugin = static_cast<GeonkickLv2Plugin*>(feature->data);
                        if (!entropictronLv2PLugin)
                                return nullptr;
                }
                features++;
        }
#ifdef ENTROPICTRON_OS_WINDOWS
	auto info = rk_from_native_win(reinterpret_cast<HWND>(parent),
                                       rk_win_api_instance(),
                                       rk_win_api_class_name());
#else // EONTRIPOCTRON_OS_GNU
        // Get the info about X Window parent window.
        const uintptr_t parentWinId = (uintptr_t)parent;
        Display* xDisplay = XOpenDisplay(nullptr);
        int screenNumber = DefaultScreen(xDisplay);
        auto info = rk_from_native_x11(xDisplay, screenNumber, parentWinId);
#endif // EONTRIPOCTRON_OS_GNU
        auto guiApp = new RkMain();
        entropictronLv2PLugin->getApi()->setEventQueue(guiApp->eventQueue());
        auto mainWidget = new MainWindow(*guiApp, entropictronLv2PLugin->getApi(), info);
        RK_ACT_BINDL(mainWidget,
                     onScaleFactor,
                     RK_ACT_ARGS(double factor),
                     [=](double factor) {
                             resize->ui_resize(resize->handle, mainWidget->width() * factor,
                                               mainWidget->height() * factor);
                     });
        if (!mainWidget->init()) {
                ENTROPICTRON_LOG_ERROR("can't init main window");
                delete guiApp;
                return nullptr;
        }

        auto winId = guiApp->nativeWindowInfo()->window;
#ifdef ENTROPICTRON_OS_WINDOWS
        *widget = reinterpret_cast<LV2UI_Widget>(winId);
#else // EONTRIPOCTRON_OS_GNU
	*widget = (LV2UI_Widget)static_cast<uintptr_t>(winId);
#endif // EONTRIPOCTRON_OS_GNU
        resize->ui_resize(resize->handle, mainWidget->width() * mainWidget->scaleFactor(),
                          mainWidget->height() * mainWidget->scaleFactor());
        return static_cast<LV2UI_Handle>(guiApp);
}

static void ent_cleanup_ui(LV2UI_Handle handle)
{
        delete static_cast<RkMain*>(handle);
}

static void ent_port_event_ui(LV2UI_Handle ui,
                                uint32_t port_index,
                                uint32_t buffer_size,
                                uint32_t format,
                                const void *buffer )
{
}

static int ent_idle(LV2UI_Handle ui)
{
        static_cast<RkMain*>(ui)->exec(false);
        return 0;
}

static const void* ent_extension_data(const char* uri)
{
    static const LV2UI_Idle_Interface idleInterface = {ent_idle};
    if (std::string(uri) == std::string(LV2_UI__idleInterface))
            return &idleInterface;
    return nullptr;
}

static const LV2UI_Descriptor ent_descriptor_ui = {
	ENTROPICTRON_URI_UI,
	ent_instantiate_ui,
	ent_cleanup_ui,
	ent_port_event_ui,
	ent_extension_data
};

const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
	switch (index)
        {
	case 0:	return &ent_descriptor_ui;
	default: return nullptr;
        }
}

static LV2_Handle ent_instantiate(const LV2_Descriptor*     descriptor,
                                    double                    rate,
                                    const char*               bundle_path,
                                    const LV2_Feature* const* features)
{
        auto entropictronLv2PLugin = new GeonkickLv2Plugin(rate);
        if (!entropictronLv2PLugin->init()) {
                ENTROPICTRON_LOG_ERROR("can't create DSP instance");
                delete entropictronLv2PLugin;
                return nullptr;
        }

        const LV2_Feature *feature;
        while ((feature = *features)) {
                if (std::string(feature->URI) == std::string(LV2_URID__map)) {
                        auto uridMap = static_cast<LV2_URID_Map*>(feature->data);
                        if (uridMap && uridMap->map && uridMap->handle) {
                                entropictronLv2PLugin->setStateId(uridMap->map(uridMap->handle, ENTROPICTRON_URI_STATE));
                                entropictronLv2PLugin->setAtomChunkId(uridMap->map(uridMap->handle, LV2_ATOM__Chunk));
                                entropictronLv2PLugin->setAtomSequence(uridMap->map(uridMap->handle, LV2_ATOM__Sequence));
                                entropictronLv2PLugin->setAtomStateChanged(uridMap->map(uridMap->handle, ENTROPICTRON_URI_STATE_CHANGED));
                                entropictronLv2PLugin->setAtomObject(uridMap->map(uridMap->handle, LV2_ATOM__Object));
                        }
                        break;
                }
                features++;
        }

        return static_cast<LV2_Handle>(entropictronLv2PLugin);
}

static void ent_connect_port(LV2_Handle instance,
                               uint32_t   port,
                               void*      data)
{
        auto entropictronLv2PLugin = static_cast<GeonkickLv2Plugin*>(instance);
	auto portNumber = static_cast<size_t>(port);
        if (portNumber == 0)
                entropictronLv2PLugin->setMidiIn(static_cast<LV2_Atom_Sequence*>(data));
        else if (portNumber == 1)
                entropictronLv2PLugin->setNotifyHostChannel(static_cast<LV2_Atom_Sequence*>(data));
        else if (portNumber > 1)
                entropictronLv2PLugin->setAudioChannel(static_cast<float*>(data), portNumber - 2);
}

static void ent_activate(LV2_Handle instance)
{
}

static void ent_run(LV2_Handle instance, uint32_t n_samples)
{
       auto entropictronLv2PLugin = static_cast<GeonkickLv2Plugin*>(instance);
       entropictronLv2PLugin->processSamples(n_samples);
}

static void ent_deactivate(LV2_Handle instance)
{
}

static void ent_cleaup(LV2_Handle instance)
{
        auto entropictronLv2PLugin = static_cast<GeonkickLv2Plugin*>(instance);
        delete entropictronLv2PLugin;
}

static LV2_State_Status
ent_state_save(LV2_Handle                instance,
                 LV2_State_Store_Function  store,
                 LV2_State_Handle          handle,
                 uint32_t                  flags,
                 const LV2_Feature* const* features)
{
        auto entropictronLv2PLugin = static_cast<GeonkickLv2Plugin*>(instance);
        if (entropictronLv2PLugin){
                std::string stateData = entropictronLv2PLugin->getStateData();
                store(handle, entropictronLv2PLugin->getStateId(), stateData.data(),
                      stateData.size(), entropictronLv2PLugin->getAtomChunkId(),
                      LV2_STATE_IS_POD | LV2_STATE_IS_PORTABLE);
        }

        return LV2_STATE_SUCCESS;
}

static LV2_State_Status
ent_state_restore(LV2_Handle                  instance,
                    LV2_State_Retrieve_Function retrieve,
                    LV2_State_Handle            handle,
                    uint32_t                    flags,
                    const LV2_Feature* const*   features)
{
        auto entropictronLv2PLugin = static_cast<GeonkickLv2Plugin*>(instance);
        if (entropictronLv2PLugin) {
                size_t size   = 0;
                LV2_URID type = 0;
                const char *data = (const char*)retrieve(handle, entropictronLv2PLugin->getStateId(),
                                                         &size, &type, &flags);
                if (data && size > 0)
                        entropictronLv2PLugin->setStateData(std::string(data, size), flags);
        }
        return LV2_STATE_SUCCESS;
}

static const void* ent_extention_data(const char* uri)
{
        static const LV2_State_Interface state = {ent_state_save, ent_state_restore};
        if (std::string(uri) == std::string(LV2_STATE__interface))
                return &state;
        return nullptr;
}

static const LV2_Descriptor ent_descriptor = {
	ENTROPICTRON_URI,
	ent_instantiate,
	ent_connect_port,
	ent_activate,
	ent_run,
	ent_deactivate,
	ent_cleaup,
	ent_extention_data,
};

const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
	switch (index)
        {
	case 0:  return &ent_descriptor;
	default: return nullptr;
	}
}
