/**
 * File name: NoiseWidget.h
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

#ifndef ENT_NOISE_WIDGET_H
#define ENT_NOISE_WIDGET_H

#include "EntAbstractView.h"
#include "GuiTypes.h"

class NoiseModel;
class RkContainer;
class RkButton;
class RkLabel;
class Knob;

class NoiseWidget : public EntAbstractView
{
public:
        explicit NoiseWidget(EntWidget* parent, NoiseModel* model);
        ~NoiseWidget();
        void createView() override;
        void updateView() override;

protected:
        void bindModel() override;
        void unbindModel() override;
        void createNoiseControls(RkContainer *container);
        void setType(NoiseType type);

private:
        RkButton *enableNoiseButton;
        RkButton *whiteNoiseButton;
        RkButton *pinkNoiseButton;
        RkButton *brownNoiseButton;
        Knob *densityKnob;
        Knob *brightnessKnob;
        Knob *gainKnob;
};

#endif // ENT_NOISE_WIDGET_H
