/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>
#include <vector>
using namespace std;

//==============================================================================
/**
*/
class Quant3noteOffAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Quant3noteOffAudioProcessor();
    ~Quant3noteOffAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	vector<int> remap(vector <int> selected_notes);
	int adjust(vector<int> remapped, int note);

private:

	map<int, vector<int>> midiAssociations;
	map<int, vector<int>>::iterator midiAssIt;

	vector<int>::iterator vecIt;
	int keycheck;
	int currentNote;
	bool play;
	int velo;
	AudioParameterInt * noteC;
	AudioParameterInt * noteCs;
	AudioParameterInt * noteD;
	AudioParameterInt * noteDs;
	AudioParameterInt * noteE;
	AudioParameterInt * noteF;
	AudioParameterInt * noteFs;
	AudioParameterInt * noteG;
	AudioParameterInt * noteGs;
	AudioParameterInt * noteA;
	AudioParameterInt * noteAs;
	AudioParameterInt * noteB;

	MidiBuffer processedBuffer;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Quant3noteOffAudioProcessor)
};
