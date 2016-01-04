# vstPhaser
Project for a lecture. VST plugin that implements a phaser effect.

### What is this for?
A phaser is an audio processor to be used with, for example, an instrument, or any other audio-stream.

### What you need
- A vst-host application (Wavosaur is a start)
- A working c++ environment (tried with Visual Studio 2013)
- The vst SDK with vstgui, read this to get started altering the gui otherwise go back a few commits:
  http://vstdev.richackard.com/vstgui.sf/vstgui/Documentation/html/tutorial_getting_started.htm

### how to start
- clone/download repository
- copy it into your sdk (VST3 SDK/public.sdk/)
- Open the project in <repository>/vcXX and hit build(<C-S-b>). 
- find the phaser.dll in vcXX/ and open it with your vsthost
  - with Wavosaur go to tools/VST/VST-Rack
- use program as described further down (or in doc).

### What the Phaser does, and how
The Phaser shifts the phase of the audiosignal and mixes it again into the input signal to create a effect known as phaser, it has the following controls:

- Mix: The amount of the processed part of the end result
- Depth: The strength of the effect
- Resonance: Further emphasise the effect by adding a feedback from a previous old (processed) signal
- Speed: The frequency of the effect (set by setSpeed), in simple terms: How fast it alternates
- stages: How often the signal is processed by the filter, making the system even more alien to the original
