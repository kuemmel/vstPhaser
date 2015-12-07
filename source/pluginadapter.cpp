#include "plugin.h"
#include "public.sdk/source/vst/vst2wrapper/vst2wrapper.h"
#include "version.h"	// for versioning

#include "public.sdk/source/main/pluginfactoryvst3.h"

#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "pluginterfaces/base/ustring.h"

#include <stdio.h>

namespace Steinberg {
namespace Vst {


//-----------------------------------------------------------------------------
PluginAdapter::PluginAdapter ()
{
}

//------------------------------------------------------------------------------
// drei Hilfsfunktion zum Lesen und Setzen von Parameteraenderungen
// ACHTUNG: eine performance-optimiertere Loesung findet sich in again.cpp!
//------------------------------------------------------------------------------

void PluginAdapter::processInputParameter (ProcessData& data)
{
    if (data.inputParameterChanges)	{
        int32 paramChangeCount = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < paramChangeCount; index++){
            IParamValueQueue* queue = data.inputParameterChanges->getParameterData (index);
            if (queue)	{
                int32 valueChangeCount = queue->getPointCount ();
                ParamValue value;
                int32 sampleOffset;
                if (queue->getPoint (valueChangeCount-1, sampleOffset, value) == kResultTrue){
	                ParamID paramId = queue->getParameterId();
                    Parameter* parameter = parameters.getParameter(paramId);
					if (parameter){
						parameter->setNormalized(value);
                    }
                }
            }
        }
    }
}

ParamValue PluginAdapter::getPlainValue(ParamID paramId){
	Parameter* parameter = parameters.getParameter(paramId);
	ParamValue normalizedValue = parameter->getNormalized();
	return parameter->toPlain(normalizedValue);
}

bool PluginAdapter::hasInputParameterChanged (ProcessData& data, ParamID paramId)
{
    if (data.inputParameterChanges)	{
        int32 paramChangeCount = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < paramChangeCount; index++){
            IParamValueQueue* queue = data.inputParameterChanges->getParameterData (index);
            if (queue)	{
                if (paramId == queue->getParameterId()){
                    return true;
                }
            }
        }
    }
    return false;
}
ParamValue PluginAdapter::getInputParameterChange (ProcessData& data, ParamID paramId)
{
    if (data.inputParameterChanges)	{
        int32 paramChangeCount = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < paramChangeCount; index++){
            IParamValueQueue* queue = data.inputParameterChanges->getParameterData (index);
            if (queue)	{
                if (paramId == queue->getParameterId()){
                    int32 valueChangeCount = queue->getPointCount ();
                    ParamValue value;
                    int32 sampleOffset;
                    if (queue->getPoint (valueChangeCount-1, sampleOffset, value) == kResultTrue){
                        return value;
                    }
                }
            }
        }
    }
    return 0;
}
void PluginAdapter::setOutputParameterChange(ProcessData &data, ParamID paramId, ParamValue value){
    IParameterChanges* paramChanges = data.outputParameterChanges;
    
    int32 index = 0;
    IParamValueQueue* paramQueue = paramChanges->addParameterData (paramId, index);
    if (paramQueue){
        int32 index2 = 0;
        paramQueue->addPoint (0, value, index2); 
    }
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PluginAdapter::setActive (TBool state)
{
	SpeakerArrangement arr;
	if (getBusArrangement (kOutput, 0, arr) != kResultTrue)
		return kResultFalse;

	int numChannels = SpeakerArr::getChannelCount (arr);
	if (numChannels == 0)
		return kResultFalse;
	SampleRate sampleRate = processSetup.sampleRate;
	startProcessing(numChannels, sampleRate);
	return kResultOk;
//	return AudioEffect::setActive (state);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PluginAdapter::initialize (FUnknown* context)
{
	tresult result = SingleComponentEffect::initialize (context);
	if (result != kResultOk)
		return result;

	addAudioInput (STR16 ("AudioInput"), SpeakerArr::kStereo);
	addAudioOutput (STR16 ("AudioOutput"), SpeakerArr::kStereo);

	return result;
}
//-----------------------------------------------------------------------------
tresult PLUGIN_API PluginAdapter::setBusArrangements (SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
{
	// see againsimple.cpp
	if (numIns == 1 && numOuts == 1)
	{
		if (inputs[0] == SpeakerArr::kMono && outputs[0] == SpeakerArr::kMono)
		{
			AudioBus* bus = FCast<AudioBus> (audioInputs.at (0));
			if (bus)
			{
				if (bus->getArrangement () != SpeakerArr::kMono)
				{
					removeAudioBusses ();
					addAudioInput  (USTRING ("Mono In"),  SpeakerArr::kMono);
					addAudioOutput (USTRING ("Mono Out"), SpeakerArr::kMono);
				}
				return kResultOk;
			}
		}
		else
		{
			AudioBus* bus = FCast<AudioBus> (audioInputs.at (0));
			if (bus)
			{
				if (bus->getArrangement () != SpeakerArr::kStereo)
				{
					removeAudioBusses ();
					addAudioInput  (USTRING ("Stereo In"),  SpeakerArr::kStereo);
					addAudioOutput (USTRING ("Stereo Out"), SpeakerArr::kStereo);
				}
				return kResultOk;
			}
		}
	}
	return kResultFalse;
}

//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//------------------------------------------------------------------------
IPlugView* PLUGIN_API PluginAdapter::createView (const char* name)
{
	// someone wants my editor

	if (name && strcmp (name, "editor") == 0)
	{
      return new VSTGUI::VST3Editor (this, "view", "plugin.uidesc");
	}
	return 0;
}
//------------------------------------------------------------------------
tresult PLUGIN_API PluginAdapter::terminate  ()
{
	viewsArray.removeAll ();
	
	return EditController::terminate ();
}



//------------------------------------------------------------------------
void PluginAdapter::addDependentView (IParamUpdater* view)
{
	viewsArray.add (view);
}

//------------------------------------------------------------------------
void PluginAdapter::removeDependentView (IParamUpdater* view)
{
	for (int32 i = 0; i < viewsArray.total (); i++)
	{
		if (viewsArray.at (i) == view)
		{
			viewsArray.removeAt (i);
			break;
		}
	}
}

//------------------------------------------------------------------------
void PluginAdapter::editorAttached (EditorView* editor)
{
	IParamUpdater* view = dynamic_cast<IParamUpdater*> (editor);
	if (view)
	{
		addDependentView (view);
	}
}

//------------------------------------------------------------------------
void PluginAdapter::editorRemoved (EditorView* editor)
{
	IParamUpdater* view = dynamic_cast<IParamUpdater*> (editor);
	if (view)
	{
		removeDependentView (view);
	}
}

//------------------------------------------------------------------------
tresult PLUGIN_API PluginAdapter::setParamNormalized (ParamID tag, ParamValue value)
{
	// called from host to update our parameters state
	tresult result = SingleComponentEffect::setParamNormalized (tag, value);
	
	for (int32 i = 0; i < viewsArray.total (); i++)
	{
		if (viewsArray.at (i))
		{
			viewsArray.at (i)->update (tag, value);
		}
	}

	return result;}


}} // namespaces



// VST2 entry
//------------------------------------------------------------------------
::AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return Steinberg::Vst::Vst2Wrapper::create (GetPluginFactory (), Steinberg::Vst::PluginUID, 'GnV2', audioMaster);
}

// VST3 entry

//------------------------------------------------------------------------
//  Module init/exit
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// called after library was loaded
bool InitModule ()   
{
	return true; 
}

//------------------------------------------------------------------------
// called after library is unloaded
bool DeinitModule ()
{
	return true; 
}

using namespace Steinberg::Vst;

//------------------------------------------------------------------------
//  VST Plug-in Entry
//------------------------------------------------------------------------
// Windows: do not forget to include a .def file in your project to export
// GetPluginFactory function!
//------------------------------------------------------------------------
BEGIN_FACTORY_DEF ("Steinberg Media Technologies", 
				   "http://www.steinberg.net", 
				   "mailto:info@steinberg.de")

	//---First Plugin included in this factory-------
	// its kVstAudioEffectClass component
	DEF_CLASS2 (INLINE_UID_FROM_FUID(PluginUID),
				PClassInfo::kManyInstances,					// cardinality  
				kVstAudioEffectClass,						// the component category (dont changed this)
				stringPluginName,							// here the Plug-in name (to be changed)
				0,											// single component effects can not be destributed so this is zero
				"Fx",										// Subcategory for this Plug-in (to be changed)
				FULL_VERSION_STR,							// Plug-in version (to be changed)
				kVstVersionString,							// the VST 3 SDK version (dont changed this, use always this define)
				Steinberg::Vst::Phaser::createInstance)// function pointer called when this component should be instanciated

END_FACTORY

