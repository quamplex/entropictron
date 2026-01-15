/**
 * File name: EntropyMeter.cpp
 * Project: Entropictron (A texture synthesizer)
 *
 * Copyright (C) 2026 Iurie Nistor
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

#include "EntropyMeter.h"
#include "EntropictronModel.h"

#include "RkTimer.h"
#include "RkPainter.h"

EntropyMeter::EntropyMeter(EntWidget* parent, EntropictronModel* model)
        : EntWidget(parent)
        , entModel{model}
        , entropyMeterWidth{90}
        , padding{2}
        , queueMaxSize{static_cast<size_t>(entropyMeterWidth - 2 * padding)}
        , entropyDataQueue(queueMaxSize, 0.0)
{
        setSize(entropyMeterWidth, 60);
        setBackgroundColor({0, 0, 0});

        auto timer = new RkTimer(this, 30);
        RK_ACT_BINDL(timer,
                     timeout,
                     RK_ACT_ARGS(),
                     [=, this]() {
                             if (entropyDataQueue.size() > queueMaxSize)
                                     entropyDataQueue.pop_front();
                             auto val = entModel->entropy() * entModel->entropyDepth();
                             entropyDataQueue.push_back(val);
                             update();
                     });
        timer->start();
}

void EntropyMeter::paintEvent(RkPaintEvent *event)
{
        RkPainter painter(this);

        std::vector<RkRealPoint> waveformPoints;
        int x = 0;
        int h = height() - 4 * padding;
        for (const auto &val: entropyDataQueue) {
                int y = h / 2 - (h / 2) * val;
                waveformPoints.emplace_back(padding + x, 2 * padding + y);
                x++;
        }

        auto pen = painter.pen();
        pen.setColor({0, 200, 0});
        painter.setPen(pen);
        painter.drawPolyline(waveformPoints);
}
