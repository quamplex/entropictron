/**
 * File name: NoiseWidget.cpp
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

#include "NoiseWidget.h"

#include <RkLabel.h>
#include <RkContainer.h>
#include <RkButton.h>

RK_DECLARE_IMAGE_RC(noise1_label);
RK_DECLARE_IMAGE_RC(noise2_label);
RK_DECLARE_IMAGE_RC(switch_button_on);
RK_DECLARE_IMAGE_RC(switch_button_off);
RK_DECLARE_IMAGE_RC(noise_density_knob_label);
RK_DECLARE_IMAGE_RC(noise_density_knob_bk);
RK_DECLARE_IMAGE_RC(noise_density_knob_marker);
RK_DECLARE_IMAGE_RC(noise_brightness_knob_label);
RK_DECLARE_IMAGE_RC(noise_brightness_knob_bk);
RK_DECLARE_IMAGE_RC(noise_brightness_knob_marker);
RK_DECLARE_IMAGE_RC(noise_gain_knob_label);
RK_DECLARE_IMAGE_RC(noise_gain_knob_bk);
RK_DECLARE_IMAGE_RC(noise_gain_knob_marker);

NoiseWidget::NoiseWidget(EntWidget* parent)
        : EntWidget(parent)
{
        setFixedSize(314, 282);
        setBackgroundColor(37, 43, 53);

        auto mainContianer = new RkContainer(this, Rk::Orientation::Vertical);
        mainContianer->addSpace(8);

        auto topContianer = new RkContainer(this, Rk::Orientation::Horizontal);
        topContianer->setSize({width(), 14});
        mainContianer->addContainer(topContianer);
        topContianer->addSpace(100);

        auto enableNoiseButton = new RkButton(this);
        enableNoiseButton->setCheckable();
        enableNoiseButton->setBackgroundColor(background());
        enableNoiseButton->setSize(36, 16);
        enableNoiseButton->setImage(RK_RC_IMAGE(switch_button_off),
                                    RkButton::State::Unpressed);
        enableNoiseButton->setImage(RK_RC_IMAGE(switch_button_on),
                                    RkButton::State::Pressed);
        enableNoiseButton->show();
        topContianer->addWidget(enableNoiseButton);

        // temp
        static int i = 0;
        auto label = new RkLabel(this, i++ ? RK_RC_IMAGE(noise2_label) : RK_RC_IMAGE(noise1_label));
        label->show();
        topContianer->addSpace(10);
        topContianer->addWidget(label);

        createNoiseControls(mainContianer);

}

NoiseWidget::~NoiseWidget()
{
}

void NoiseWidget::createNoiseControls()
{
        auto noiseControlsContianer = new RkContainer(this);
        noiseControlsContianer->setSize(width(), 103);
        mainLayout->addContianer(noiseControlsContianer);

        auto densityKnob = new Knob(this, RK_RC_IMAGE(noise_density_knob_label));
        densityKnob->setBackroundImage(RK_RC_IMAGE(noise_density_knob_bk));
        densityKnob->setMarkerImage(RK_RC_IMAGE(noise_density_knob_marker));
        noiseControlsContianer->addWidget(densityKnob);

        auto birghtnessKnob = new Knob(this, RK_RC_IMAGE(noise_brightness_knob_label));
        brightnessKnob->setBackroundImage(RK_RC_IMAGE(noise_brightness_knob_bk));
        brightnessKnob->setMarkerImage(RK_RC_IMAGE(noise_brightness_knob_marker));
        noiseControlsContianer->addWidget(brightnessKnob);

        auto birghtnessKnob = new Knob(this, RK_RC_IMAGE(noise_gian_knob_label));
        gianKnob->setBackroundImage(RK_RC_IMAGE(noise_gian_knob_bk));
        gianKnob->setMarkerImage(RK_RC_IMAGE(noise_gian_knob_marker));
        noiseControlsContianer->addWidget(gianKnob);
}
