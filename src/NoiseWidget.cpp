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
#include "NoiseModel.h"

#include "RkLabel.h"
#include "RkContainer.h"
#include "RkButton.h"

RK_DECLARE_IMAGE_RC(noise_label);
RK_DECLARE_IMAGE_RC(switch_button_on);
RK_DECLARE_IMAGE_RC(switch_button_off);
RK_DECLARE_IMAGE_RC(knob_big_size_bk);
RK_DECLARE_IMAGE_RC(knob_big_size_marker);
RK_DECLARE_IMAGE_RC(knob_medium_size_bk);
RK_DECLARE_IMAGE_RC(knob_medium_size_marker);
RK_DECLARE_IMAGE_RC(noise_density_knob_label);
RK_DECLARE_IMAGE_RC(noise_brightness_knob_label);
RK_DECLARE_IMAGE_RC(noise_gain_knob_label);
RK_DECLARE_IMAGE_RC(noise_white_button);
RK_DECLARE_IMAGE_RC(noise_white_button_hover);
RK_DECLARE_IMAGE_RC(noise_white_button_on);
RK_DECLARE_IMAGE_RC(noise_white_button_hover_on);
RK_DECLARE_IMAGE_RC(noise_pink_button);
RK_DECLARE_IMAGE_RC(noise_pink_button_hover);
RK_DECLARE_IMAGE_RC(noise_pink_button_on);
RK_DECLARE_IMAGE_RC(noise_pink_button_hover_on);
RK_DECLARE_IMAGE_RC(noise_brown_button);
RK_DECLARE_IMAGE_RC(noise_brown_button_hover);
RK_DECLARE_IMAGE_RC(noise_brown_button_on);
RK_DECLARE_IMAGE_RC(noise_brown_button_hover_on);

NoiseWidget::NoiseWidget(EntWidget* parent, NoiseModel* model)
        : EntAbstractView(parent, model)
        , enableNoiseButton {nullptr}
        , whiteNoiseButton {nullptr}
        , pinkNoiseButton {nullptr}
        , brownNoiseButton {nullptr}
        , densityKnob {nullptr}
        , brightnessKnob {nullptr}
        , gainKnob {nullptr}
{
        setFixedSize(350, 302);
        setBackgroundColor(37, 43, 53);
        createView();
        bindModel();
}

NoiseWidget::~NoiseWidget()
{
}

void NoiseWidget::createView()
{
        auto mainContainer = new RkContainer(this, Rk::Orientation::Vertical);
        mainContainer->addSpace(8);

        auto topContianer = new RkContainer(this, Rk::Orientation::Horizontal);
        topContianer->setSize({width(), 14});
        mainContainer->addContainer(topContianer);
        topContianer->addSpace(100);

        enableNoiseButton = new RkButton(this);
        enableNoiseButton->setCheckable();
        enableNoiseButton->setBackgroundColor(background());
        enableNoiseButton->setSize(36, 16);
        enableNoiseButton->setImage(RK_RC_IMAGE(switch_button_off),
                                    RkButton::State::Unpressed);
        enableNoiseButton->setImage(RK_RC_IMAGE(switch_button_on),
                                    RkButton::State::Pressed);
        enableNoiseButton->show();
        topContianer->addWidget(enableNoiseButton);

        topContianer->addSpace(10);
        auto noiseLabel = new RkLabel(this);
        noiseLabel->setBackgroundColor(background());
        noiseLabel->setImage(RK_RC_IMAGE(noise_label));
        noiseLabel->show();
        topContianer->addWidget(noiseLabel);

        createNoiseControls(mainContainer);

        updateView();
}

void NoiseWidget::updateView()
{
        auto model = static_cast<NoiseModel*>(getModel());
        if (!model)
                return;

        enableNoiseButton->setPressed(model->isEnabled());
        setType(model->noiseType());

        auto [densityFrom, densityTo] = model->getDensityRange();
        densityKnob->setRange(densityFrom, densityTo);
        densityKnob->setRangeType(Knob::RangeType::Logarithmic);
        densityKnob->setDefaultValue(model->getDensityDefaultValue());
        densityKnob->setValue(model->density());

        auto [brightnessFrom, brightnessTo] = model->getBrightnessRange();
        brightnessKnob->setRange(brightnessFrom, brightnessTo);
        brightnessKnob->setDefaultValue(model->getBrightnessDefaultValue());
        brightnessKnob->setValue(model->brightness());

        auto [gainFrom, gainTo] = model->getGainRange();
        gainKnob->setRange(gainFrom, gainTo);
        gainKnob->setRangeType(Knob::RangeType::Logarithmic);
        gainKnob->setDefaultValue(model->getGainDefaultValue());
        gainKnob->setValue(model->gain());
}

void NoiseWidget::bindModel()
{
        auto model = static_cast<NoiseModel*>(getModel());
        if (!model)
                return;

        RK_ACT_BIND(enableNoiseButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    model,
                    enable(b));
        RK_ACT_BIND(whiteNoiseButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    model, setType(NoiseType::WhiteNoise));
        RK_ACT_BIND(pinkNoiseButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    model, setType(NoiseType::PinkNoise));
        RK_ACT_BIND(brownNoiseButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    model, setType(NoiseType::BrownNoise));
        RK_ACT_BIND(densityKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setDensity(value));
        RK_ACT_BIND(brightnessKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setBrightness(value));
        RK_ACT_BIND(gainKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setGain(value));

        RK_ACT_BIND(model,
                    modelUpdated,
                    RK_ACT_ARGS(),
                    this,
                    updateView());
        RK_ACT_BIND(model,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    enableNoiseButton,
                    setPressed(b));
        RK_ACT_BIND(model,
                    typeUpdated,
                    RK_ACT_ARGS(NoiseType type),
                    this,
                    setType(type));
        RK_ACT_BIND(model,
                    densityUpdated,
                    RK_ACT_ARGS(double value),
                    densityKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    brightnessUpdated,
                    RK_ACT_ARGS(double value),
                    brightnessKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    gainUpdated,
                    RK_ACT_ARGS(double value),
                    gainKnob,
                    setValue(value));
}

void NoiseWidget::unbindModel()
{
        auto model = getModel();
        unbindObject(model);
        enableNoiseButton->unbindObject(model);
        whiteNoiseButton->unbindObject(model);
        pinkNoiseButton->unbindObject(model);
        brownNoiseButton->unbindObject(model);
        densityKnob->unbindObject(model);
        brightnessKnob->unbindObject(model);
        gainKnob->unbindObject(model);
}

void NoiseWidget::createNoiseControls(RkContainer *container)
{
        auto noiseControlsContainer = new RkContainer(this);
        noiseControlsContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(noiseControlsContainer);

        auto noiseTypeContianer = new RkContainer(this, Rk::Orientation::Vertical);
        noiseTypeContianer->setSize({75, 96});

        whiteNoiseButton = new RkButton(this);
        whiteNoiseButton->setImage(RK_RC_IMAGE(noise_white_button),
                                   RkButton::State::Unpressed);
        whiteNoiseButton->setImage(RK_RC_IMAGE(noise_white_button_on),
                                   RkButton::State::Pressed);
        whiteNoiseButton->setImage(RK_RC_IMAGE(noise_white_button_hover),
                                   RkButton::State::UnpressedHover);
        whiteNoiseButton->setImage(RK_RC_IMAGE(noise_white_button_hover_on),
                                   RkButton::State::PressedHover);
        whiteNoiseButton->setCheckable(true);
        whiteNoiseButton->show();
        noiseTypeContianer->addWidget(whiteNoiseButton);

        pinkNoiseButton = new RkButton(this);
        pinkNoiseButton->setImage(RK_RC_IMAGE(noise_pink_button),
                                   RkButton::State::Unpressed);
        pinkNoiseButton->setImage(RK_RC_IMAGE(noise_pink_button_on),
                                   RkButton::State::Pressed);
        pinkNoiseButton->setImage(RK_RC_IMAGE(noise_pink_button_hover),
                                  RkButton::State::UnpressedHover);
        pinkNoiseButton->setImage(RK_RC_IMAGE(noise_pink_button_hover_on),
                                  RkButton::State::PressedHover);
        pinkNoiseButton->setCheckable(true);
        pinkNoiseButton->show();
        noiseTypeContianer->addWidget(pinkNoiseButton);

        brownNoiseButton = new RkButton(this);
        brownNoiseButton->setImage(RK_RC_IMAGE(noise_brown_button),
                                   RkButton::State::Unpressed);
        brownNoiseButton->setImage(RK_RC_IMAGE(noise_brown_button_on),
                                   RkButton::State::Pressed);
        brownNoiseButton->setImage(RK_RC_IMAGE(noise_brown_button_hover),
                                   RkButton::State::UnpressedHover);
        brownNoiseButton->setImage(RK_RC_IMAGE(noise_brown_button_hover_on),
                                   RkButton::State::PressedHover);
        brownNoiseButton->setCheckable(true);
        brownNoiseButton->show();
        noiseTypeContianer->addWidget(brownNoiseButton);

        noiseControlsContainer->addContainer(noiseTypeContianer);

        densityKnob = new Knob(this, RK_RC_IMAGE(noise_density_knob_label));
        densityKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        densityKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        noiseControlsContainer->addWidget(densityKnob);

        brightnessKnob = new Knob(this, RK_RC_IMAGE(noise_brightness_knob_label));
        brightnessKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        brightnessKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        noiseControlsContainer->addWidget(brightnessKnob);

        gainKnob = new Knob(this, RK_RC_IMAGE(noise_gain_knob_label));
        gainKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        gainKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        noiseControlsContainer->addWidget(gainKnob);
}

void NoiseWidget::setType(NoiseType type)
{
        whiteNoiseButton->setPressed(type == NoiseType::WhiteNoise);
        pinkNoiseButton->setPressed(type == NoiseType::PinkNoise);
        brownNoiseButton->setPressed(type == NoiseType::BrownNoise);
}
