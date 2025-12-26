# Entropictron

**Entropictron** is a [free software](https://www.gnu.org/philosophy/free-sw.en.html)
context synthesizer and audio effect.

**Website:** [https://quamplex.com/entropictron](https://quamplex.com/entropictron)

Entropictron can produce a variety of sound textures that can
serve as background or contextual elements in music tracks.
Useful for ambient music, sound effects, film and
video production, game audio, and other.

**License:** GNU General Public License, Version 3

**Version:** 1.0.0 (not released yet)

![Screenshot](artwork/screenshot.png)

## Features

Entropictron combines and mixes several sound generators and effects,
allowing to layer Noise, Crackle, and Glitch in a single patch.

### Noise (×2)
A noise generator producing various noise types with an integrated
filter to shape the frequency spectrum.

**Parameters:**
- Type (white, pink, brownian)
- Density
- Brightness
- Gain
- Stereo
- Filter type (low-pass, band-pass, high-pass)
- Filter cutoff
- Filter resonance

### Crackle (×2)
Generates short, irregular bursts with different shapes.

**Parameters:**
- Rate
- Randomness
- Amplitude
- Burst shape (exponential, linear, triangle)
- Brightness
- Duration
- Stereo spread

### Glitch (×2)
Simulates memory-based glitches with adjustable length, randomness,
and jump parameters, creating stutters and digital artifacts.

**Parameters:**
- Repeats
- Probability
- Length
- Minimum jump
- Maximum jump

## Requirements

In order Entropictron to run and operate correctly there is a need

 * GNU/Linux or Windows operating system.
 * A host that supports VST3 plugin format.

## Build & Install

##### Install dependencies

In order to build Entropictron there is a need to install
the following development packages:

* RapidJSON
* Cairo

On Debian, Ubuntu, Ubuntu Studio install:

    apt-get install build-essential cmake rapidjson-dev libcairo2-dev

##### Build & install Entropictron

Clone the Entropictron code repository, compile and install.

        copy the source code of the latest release
        mkdir entropictron/build
        cd entropictron/build
        cmake -DVST3_SDK_PATH=<full system path to VST3 SDK folder> ../
        make
        make install

##### Building on Windows

To build on Windows, there is a need to install MSYS2/UCRT64 and follow
the same steps as mentioned above.
