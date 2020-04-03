/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Quant3noteOffAudioProcessor::Quant3noteOffAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	addParameter(noteC = new AudioParameterInt("C", "C", 0, 1, 0));
	addParameter(noteCs = new AudioParameterInt("Cs", "Cs", 0, 1, 0));
	addParameter(noteD = new AudioParameterInt("D", "D", 0, 1, 0));
	addParameter(noteDs = new AudioParameterInt("Ds", "Ds", 0, 1, 0));
	addParameter(noteE = new AudioParameterInt("E", "E", 0, 1, 0));
	addParameter(noteF = new AudioParameterInt("F", "F", 0, 1, 0));
	addParameter(noteFs = new AudioParameterInt("Fs", "Fs", 0, 1, 0));
	addParameter(noteG = new AudioParameterInt("G", "G", 0, 1, 0));
	addParameter(noteGs = new AudioParameterInt("Gs", "Gs", 0, 1, 0));
	addParameter(noteA = new AudioParameterInt("A", "A", 0, 1, 0));
	addParameter(noteAs = new AudioParameterInt("As", "As", 0, 1, 0));
	addParameter(noteB = new AudioParameterInt("B", "B", 0, 1, 0));
}

Quant3noteOffAudioProcessor::~Quant3noteOffAudioProcessor()
{
}

//==============================================================================
const String Quant3noteOffAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Quant3noteOffAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Quant3noteOffAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Quant3noteOffAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Quant3noteOffAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Quant3noteOffAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Quant3noteOffAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Quant3noteOffAudioProcessor::setCurrentProgram (int index)
{
}

const String Quant3noteOffAudioProcessor::getProgramName (int index)
{
    return {};
}

void Quant3noteOffAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Quant3noteOffAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Quant3noteOffAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Quant3noteOffAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Quant3noteOffAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midi)
{
	processedBuffer.clear();
	jassert(buffer.getNumChannels() == 0);
	MidiMessage msg;
	vector<int> userRemap = { *noteC, *noteCs, *noteD, *noteDs, *noteE, *noteF, *noteFs, *noteG, *noteGs, *noteA, *noteAs, *noteB };
	vector<int> remapped = remap(userRemap);
	int sample;

	for (MidiBuffer::Iterator it(midi); it.getNextEvent(msg, sample);) //building the noteDump                     
	{
		currentNote = msg.getNoteNumber();
		play = msg.isNoteOn();
		keycheck = adjust(remapped, currentNote);
		midiAssIt = midiAssociations.find(keycheck);
		velo = msg.getVelocity();
		if (play) //note on Condition checking, adding to associations list
		{
			if (midiAssIt == midiAssociations.end())
			{
				pair<int, vector<int>> midiInfo;
				midiInfo.first = keycheck;
				midiInfo.second.push_back(currentNote);
				midiAssociations.insert(midiInfo);
				processedBuffer.addEvent(MidiMessage::noteOn(1, keycheck, uint8(velo)), sample); // this is if the note is the first of it's kind, best case scenario
			}
			if (midiAssIt != midiAssociations.end())  //another note is pinged into the same keychecknote
			{
				midiAssociations[keycheck].push_back(currentNote);
				processedBuffer.addEvent(MidiMessage::noteOff(1, keycheck), sample);
				processedBuffer.addEvent(MidiMessage::noteOn(1, keycheck, uint8(velo)), sample);
			}
		}
		else if (!play) //note off condition checking, removing from associations checking
		{
			//need to frisk the whole map because there could be no associations a note quantization could be removed when all selections are disabled. Then what?
			for (midiAssIt = midiAssociations.begin(); midiAssIt != midiAssociations.end(); ++midiAssIt)
			{
				vecIt = find(midiAssIt->second.begin(), midiAssIt->second.end(), currentNote);
				if (vecIt != midiAssIt->second.end())
				{
					midiAssIt->second.erase(vecIt);
					if (size(midiAssIt->second) == 0)
					{
						processedBuffer.addEvent(MidiMessage::noteOff(1, keycheck), sample);
						midiAssociations.erase(keycheck);
					}
				}
			}


		}
	}

	midi.clear();
	midi.swapWith(processedBuffer);
}

//==============================================================================
bool Quant3noteOffAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Quant3noteOffAudioProcessor::createEditor()
{
    return new Quant3noteOffAudioProcessorEditor (*this);
}

//==============================================================================
void Quant3noteOffAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Quant3noteOffAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

vector<int> Quant3noteOffAudioProcessor::remap(vector<int> selected_notes)
{
	vector<int> output = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int start = 0;
	int last = 0;
	bool started = false;
	int adj = 0;
	int i = 0;
	while (i < 12)
	{
		if (selected_notes[i] == 1 && started == false)
		{
			started = true;
			start = i;
			output[i] = adj;
			adj -= 1;
			last = i;
			//cout << "first if statement " << " " << start << " " << started << " " << adj << " " << i << endl;
		}
		if (selected_notes[i] == 1 && started == true)
		{
			adj = 0;
			output[i] = adj;
			adj -= 1;
			last = i;
			//cout << "second if statement " << " " << start << " " << started << " " << adj << " " << i << endl;
		}
		if (selected_notes[i] == 0 && started == true)
		{
			output[i] = adj;
			adj -= 1;
			//cout << "third if statement " << " " << start << " " << started << " " << adj << " " << i << endl;
		}
		i += 1;
	}
	adj = last - 12;
	for (int g = 0; g < start; g++)
	{
		output[g] = adj;
		adj -= 1;
	}
	return output;
}

int Quant3noteOffAudioProcessor::adjust(vector<int> remapped, int note)
{
	return note + remapped[note % 12];
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Quant3noteOffAudioProcessor();
}
