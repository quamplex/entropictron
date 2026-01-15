/**
 * File name: DspFrameTimer.h
 * Project: Entropictron (A texture synthesizer)
 *
 * Copyright (C) 2025 Iurie Nistor
 *
 * This file is part of Entropictron.
 *
 * Entropictron is free software; you can redistribute it and/or modify
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

#include "DspFrameTimer.h"

DspFrameTimer::DspFrameTimer(int timeout)
        : timerTimeoutFrames{timeout}
        , timerFramesLeft{timerTimeoutFrames}
{
}

void DspFrameTimer::setTimeout(int frames)
{
        timerTimeoutFrames = frames;
}

int DspFrameTimer::getTimeout() const
{
        return timerTimeoutFrames;
}

void DspFrameTimer::setOnTimeoutCallback(std::function<void(size_t)> callback)
{
        timeoutCallback = std::move(callback);
}

void DspFrameTimer::reset()
{
        timerFramesLeft = timerTimeoutFrames;
}

void DspFrameTimer::process(size_t size)
{
    int samplesLeft = static_cast<int>(size);

    size_t timeoutFrame = 0;
    while (samplesLeft > 0) {
            if (timerFramesLeft - samplesLeft < 0) {
                    timeoutFrame += timerFramesLeft;
                    samplesLeft -= timerFramesLeft;
                    reset();
                    if (timeoutCallback)
                            timeoutCallback(timeoutFrame);
            } else {
                    timerFramesLeft -= samplesLeft;
                    samplesLeft = 0;
            }
    }
}

