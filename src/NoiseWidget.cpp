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
#include "Knob.h"

#include "RkLabel.h"
#include "RkContainer.h"
#include "RkButton.h"

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
RK_DECLARE_IMAGE_RC(noise_white_button);
RK_DECLARE_IMAGE_RC(noise_white_button_hover);
RK_DECLARE_IMAGE_RC(noise_white_button_on);
RK_DECLARE_IMAGE_RC(noise_pink_button);
RK_DECLARE_IMAGE_RC(noise_pink_button_hover);
RK_DECLARE_IMAGE_RC(noise_pink_button_on);
RK_DECLARE_IMAGE_RC(noise_brown_button);
RK_DECLARE_IMAGE_RC(noise_brown_button_hover);
RK_DECLARE_IMAGE_RC(noise_brown_button_on);

NoiseWidget::NoiseWidget(EntWidget* parent)
        : EntWidget(parent)
{
        setFixedSize(314, 282);
        setBackgroundColor(37, 43, 53);

        auto mainContainer = new RkContainer(this, Rk::Orientation::Vertical);
        mainContainer->addSpace(8);

        auto topContianer = new RkContainer(this, Rk::Orientation::Horizontal);
        topContianer->setSize({width(), 14});
        mainContainer->addContainer(topContianer);
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

        createNoiseControls(mainContainer);
}

NoiseWidget::~NoiseWidget()
{
}

void NoiseWidget::createNoiseControls(RkContainer *container)
{
        auto noiseControlsContainer = new RkContainer(this);
        noiseControlsContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(noiseControlsContainer);

        auto noiseTypeContianer = new RkContainer(this, Rk::Orientation::Vertical);
        noiseTypeContianer->setSize({40, 51});

        auto whiteNoiseBotton = new RkButton(this);
        whiteNoiseBotton->setImage(RK_RC_IMAGE(noise_white_button),
                                   RkButton::State::Unpressed);
        whiteNoiseBotton->setImage(RK_RC_IMAGE(noise_white_button_on),
                                   RkButton::State::Pressed);
        whiteNoiseBotton->setImage(RK_RC_IMAGE(noise_white_button_hover),
                                   RkButton::State::UnpressedHover);
        whiteNoiseBotton->setCheckable(true);
        whiteNoiseBotton->setPressed(true);
        whiteNoiseBotton->show();
        noiseTypeContianer->addWidget(whiteNoiseBotton);

        auto pinkNoiseBotton = new RkButton(this);
        pinkNoiseBotton->setImage(RK_RC_IMAGE(noise_pink_button),
                                   RkButton::State::Unpressed);
        pinkNoiseBotton->setImage(RK_RC_IMAGE(noise_pink_button_on),
                                   RkButton::State::Pressed);
        pinkNoiseBotton->setImage(RK_RC_IMAGE(noise_pink_button_hover),
                                   RkButton::State::UnpressedHover);
        pinkNoiseBotton->setCheckable(true);
        pinkNoiseBotton->setPressed(true);
        pinkNoiseBotton->show();
        noiseTypeContianer->addWidget(pinkNoiseBotton);

        auto brownNoiseBotton = new RkButton(this);
        brownNoiseBotton->setImage(RK_RC_IMAGE(noise_brown_button),
                                   RkButton::State::Unpressed);
        brownNoiseBotton->setImage(RK_RC_IMAGE(noise_brown_button_on),
                                   RkButton::State::Pressed);
        brownNoiseBotton->setImage(RK_RC_IMAGE(noise_brown_button_hover),
                                   RkButton::State::UnpressedHover);
        brownNoiseBotton->setCheckable(true);
        brownNoiseBotton->setPressed(true);
        brownNoiseBotton->show();
        noiseTypeContianer->addWidget(brownNoiseBotton);

        noiseControlsContainer->addContainer(noiseTypeContianer);

        auto densityKnob = new Knob(this, RK_RC_IMAGE(noise_density_knob_label));
        densityKnob->setKnobImage(RK_RC_IMAGE(noise_density_knob_bk));
        densityKnob->setMarkerImage(RK_RC_IMAGE(noise_density_knob_marker));
        noiseControlsContainer->addWidget(densityKnob);

        auto brightnessKnob = new Knob(this, RK_RC_IMAGE(noise_brightness_knob_label));
        brightnessKnob->setKnobImage(RK_RC_IMAGE(noise_brightness_knob_bk));
        brightnessKnob->setMarkerImage(RK_RC_IMAGE(noise_brightness_knob_marker));
        noiseControlsContainer->addWidget(brightnessKnob);

        auto gainKnob = new Knob(this, RK_RC_IMAGE(noise_gain_knob_label));
        gainKnob->setKnobImage(RK_RC_IMAGE(noise_gain_knob_bk));
        gainKnob->setMarkerImage(RK_RC_IMAGE(noise_gain_knob_marker));
        noiseControlsContainer->addWidget(gainKnob);
}
