/**
 * File name: CrackleWidget.cpp
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

#include "CrackleWidget.h"
#include "Knob.h"
#include "CrackleModel.h"

#include "RkLabel.h"
#include "RkContainer.h"
#include "RkButton.h"

RK_DECLARE_IMAGE_RC(crackle_label);
RK_DECLARE_IMAGE_RC(switch_button_on);
RK_DECLARE_IMAGE_RC(switch_button_off);
RK_DECLARE_IMAGE_RC(knob_big_size_bk);
RK_DECLARE_IMAGE_RC(knob_big_size_marker);
RK_DECLARE_IMAGE_RC(knob_medium_size_bk);
RK_DECLARE_IMAGE_RC(knob_medium_size_marker);
RK_DECLARE_IMAGE_RC(crackle_rate_knob_label);
RK_DECLARE_IMAGE_RC(crackle_randomness_knob_label);
RK_DECLARE_IMAGE_RC(crackle_amplitude_knob_label);
RK_DECLARE_IMAGE_RC(crackle_brightness_knob_label);
RK_DECLARE_IMAGE_RC(crackle_duration_knob_label);
RK_DECLARE_IMAGE_RC(crackle_stereospread_knob_label);
RK_DECLARE_IMAGE_RC(crackle_exp_env_button);
RK_DECLARE_IMAGE_RC(crackle_exp_env_button_on);
RK_DECLARE_IMAGE_RC(crackle_exp_env_button_hover);
RK_DECLARE_IMAGE_RC(crackle_exp_env_button_hover_on);
RK_DECLARE_IMAGE_RC(crackle_linear_env_button);
RK_DECLARE_IMAGE_RC(crackle_linear_env_button_on);
RK_DECLARE_IMAGE_RC(crackle_linear_env_button_hover);
RK_DECLARE_IMAGE_RC(crackle_linear_env_button_hover_on);
RK_DECLARE_IMAGE_RC(crackle_triangle_env_button);
RK_DECLARE_IMAGE_RC(crackle_triangle_env_button_on);
RK_DECLARE_IMAGE_RC(crackle_triangle_env_button_hover);
RK_DECLARE_IMAGE_RC(crackle_triangle_env_button_hover_on);

CrackleWidget::CrackleWidget(EntWidget* parent, CrackleModel* model)
        : EntAbstractView(parent, model)
        , enableCrackleButton{nullptr}
        , rateKnob{nullptr}
        , randomnessKnob{nullptr}
        , amplitudeKnob{nullptr}
        , brightnessKnob{nullptr}
        , durationKnob{nullptr}
        , stereospreadKnob{nullptr}
        , exponentialEnvButton{nullptr}
        , linearEnvButton{nullptr}
        , tiangleEnvButton{nullptr}
{
        setFixedSize(350, 302);
        setBackgroundColor(37, 43, 53);
        createView();
        bindModel();
}

CrackleWidget::~CrackleWidget()
{
}

void CrackleWidget::createView()
{
        auto mainContainer = new RkContainer(this, Rk::Orientation::Vertical);
        mainContainer->addSpace(8);

        auto topContianer = new RkContainer(this, Rk::Orientation::Horizontal);
        topContianer->setSize({width(), 14});
        mainContainer->addContainer(topContianer);
        topContianer->addSpace(122);

        enableCrackleButton = new RkButton(this);
        enableCrackleButton->setCheckable();
        enableCrackleButton->setBackgroundColor(background());
        enableCrackleButton->setSize(36, 16);
        enableCrackleButton->setImage(RK_RC_IMAGE(switch_button_off),
                                    RkButton::State::Unpressed);
        enableCrackleButton->setImage(RK_RC_IMAGE(switch_button_on),
                                    RkButton::State::Pressed);
        enableCrackleButton->show();
        topContianer->addWidget(enableCrackleButton);

        topContianer->addSpace(10);
        auto crackleLabel = new RkLabel(this);
        crackleLabel->setBackgroundColor(background());
        crackleLabel->setImage(RK_RC_IMAGE(crackle_label));
        crackleLabel->show();
        topContianer->addWidget(crackleLabel);

        createCrackleControls(mainContainer);

        updateView();
}

void CrackleWidget::updateView()
{
        auto model = static_cast<CrackleModel*>(getModel());
        if (!model)
                return;

        enableCrackleButton->setPressed(model->isEnabled());
        setEnvelopeShape(model->envelopeShape());

        auto [rateFrom, rateTo] = model->getRateRange();
        rateKnob->setRange(rateFrom, rateTo);
        rateKnob->setRangeType(Knob::RangeType::Logarithmic);
        rateKnob->setDefaultValue(model->getRateDefaultValue());
        rateKnob->setValue(model->rate());

        auto [randomnessFrom, randomnessTo] = model->getRandomnessRange();
        randomnessKnob->setRange(randomnessFrom, randomnessTo);
        randomnessKnob->setRangeType(Knob::RangeType::Logarithmic);
        randomnessKnob->setDefaultValue(model->getRandomnessDefaultValue());
        randomnessKnob->setValue(model->randomness());

        auto [amplitudeFrom, amplitudeTo] = model->getAmplitudeRange();
        amplitudeKnob->setRange(amplitudeFrom, amplitudeTo);
        amplitudeKnob->setRangeType(Knob::RangeType::Logarithmic);
        amplitudeKnob->setDefaultValue(model->getAmplitudeDefaultValue());
        amplitudeKnob->setValue(model->amplitude());

        auto [brightnessFrom, brightnessTo] = model->getBrightnessRange();
        brightnessKnob->setRange(brightnessFrom, brightnessTo);
        brightnessKnob->setRangeType(Knob::RangeType::Logarithmic);
        brightnessKnob->setDefaultValue(model->getBrightnessDefaultValue());
        brightnessKnob->setValue(model->brightness());

        auto [durationFrom, durationTo] = model->getDurationRange();
        durationKnob->setRange(durationFrom, durationTo);
        durationKnob->setRangeType(Knob::RangeType::Logarithmic);
        durationKnob->setDefaultValue(model->getDurationDefaultValue());
        durationKnob->setValue(model->duration());

        auto [stereospreadFrom, stereospreadTo] = model->getStereospreadRange();
        stereospreadKnob->setRange(stereospreadFrom, stereospreadTo);
        stereospreadKnob->setDefaultValue(model->getStereospreadDefaultValue());
        stereospreadKnob->setValue(model->stereospread());
}

void CrackleWidget::bindModel()
{
        auto model = static_cast<CrackleModel*>(getModel());
        if (!model)
                return;

        RK_ACT_BIND(enableCrackleButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    model,
                    enable(b));
        RK_ACT_BIND(rateKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setRate(value));
        RK_ACT_BIND(randomnessKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setRandomness(value));
        RK_ACT_BIND(amplitudeKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setAmplitude(value));
        RK_ACT_BIND(exponentialEnvButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    model, setEnvelopeShape(CrackleEnvelopeShape::Exponential));
        RK_ACT_BIND(linearEnvButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    model, setEnvelopeShape(CrackleEnvelopeShape::Linear));
        RK_ACT_BIND(tiangleEnvButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    model, setEnvelopeShape(CrackleEnvelopeShape::Triangle));
        RK_ACT_BIND(brightnessKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setBrightness(value));
        RK_ACT_BIND(durationKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setDuration(value));
        RK_ACT_BIND(stereospreadKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setStereospread(value));

        RK_ACT_BIND(model,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    enableCrackleButton,
                    setPressed(b));
        RK_ACT_BIND(model,
                    modelUpdated,
                    RK_ACT_ARGS(),
                    this,
                    updateView());
        RK_ACT_BIND(model,
                    rateUpdated,
                    RK_ACT_ARGS(double value),
                    rateKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    randomnessUpdated,
                    RK_ACT_ARGS(double value),
                    randomnessKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    amplitudeUpdated,
                    RK_ACT_ARGS(double value),
                    amplitudeKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    envelopeShapeUpdated,
                    RK_ACT_ARGS(CrackleEnvelopeShape shape),
                    this,
                    setEnvelopeShape(shape));
        RK_ACT_BIND(model,
                    brightnessUpdated,
                    RK_ACT_ARGS(double value),
                    brightnessKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    durationUpdated,
                    RK_ACT_ARGS(double value),
                    durationKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    stereospreadUpdated,
                    RK_ACT_ARGS(double value),
                    stereospreadKnob,
                    setValue(value));
}

void CrackleWidget::unbindModel()
{
        auto model = getModel();
        unbindObject(model);
        enableCrackleButton->unbindObject(model);
        rateKnob->unbindObject(model);
        randomnessKnob->unbindObject(model);
        amplitudeKnob->unbindObject(model);
        brightnessKnob->unbindObject(model);
        durationKnob->unbindObject(model);
        stereospreadKnob->unbindObject(model);
}

void CrackleWidget::createCrackleControls(RkContainer *container)
{
        // Rate, Randomness, Amplitude
        auto horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(horizontalContainer);
        horizontalContainer->addSpace(46);

        rateKnob = new Knob(this, RK_RC_IMAGE(crackle_rate_knob_label));
        rateKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        rateKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(rateKnob);

        randomnessKnob = new Knob(this, RK_RC_IMAGE(crackle_randomness_knob_label));
        randomnessKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        randomnessKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        horizontalContainer->addWidget(randomnessKnob);

        amplitudeKnob = new Knob(this, RK_RC_IMAGE(crackle_amplitude_knob_label));
        amplitudeKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        amplitudeKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(amplitudeKnob);

        // Envelope types
        horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({width(), 20});
        container->addSpace(5);
        container->addContainer(horizontalContainer);
        horizontalContainer->addSpace(92);

        exponentialEnvButton = new RkButton(this);
        exponentialEnvButton->setImage(RK_RC_IMAGE(crackle_exp_env_button),
                                   RkButton::State::Unpressed);
        exponentialEnvButton->setImage(RK_RC_IMAGE(crackle_exp_env_button_on),
                                   RkButton::State::Pressed);
        exponentialEnvButton->setImage(RK_RC_IMAGE(crackle_exp_env_button_hover),
                                   RkButton::State::UnpressedHover);
        exponentialEnvButton->setImage(RK_RC_IMAGE(crackle_exp_env_button_hover_on),
                                   RkButton::State::PressedHover);
        exponentialEnvButton->setCheckable(true);
        exponentialEnvButton->show();
        horizontalContainer->addWidget(exponentialEnvButton);

        linearEnvButton = new RkButton(this);
        linearEnvButton->setImage(RK_RC_IMAGE(crackle_linear_env_button),
                                   RkButton::State::Unpressed);
        linearEnvButton->setImage(RK_RC_IMAGE(crackle_linear_env_button_on),
                                   RkButton::State::Pressed);
        linearEnvButton->setImage(RK_RC_IMAGE(crackle_linear_env_button_hover),
                                   RkButton::State::UnpressedHover);
        linearEnvButton->setImage(RK_RC_IMAGE(crackle_linear_env_button_hover_on),
                                   RkButton::State::PressedHover);
        linearEnvButton->setCheckable(true);
        linearEnvButton->show();
        horizontalContainer->addWidget(linearEnvButton);

        tiangleEnvButton = new RkButton(this);
        tiangleEnvButton->setImage(RK_RC_IMAGE(crackle_triangle_env_button),
                                   RkButton::State::Unpressed);
        tiangleEnvButton->setImage(RK_RC_IMAGE(crackle_triangle_env_button_on),
                                   RkButton::State::Pressed);
        tiangleEnvButton->setImage(RK_RC_IMAGE(crackle_triangle_env_button_hover),
                                   RkButton::State::UnpressedHover);
        tiangleEnvButton->setImage(RK_RC_IMAGE(crackle_triangle_env_button_hover_on),
                                   RkButton::State::PressedHover);
        tiangleEnvButton->setCheckable(true);
        tiangleEnvButton->show();
        horizontalContainer->addWidget(tiangleEnvButton);

        // Brightness, Duraiton, Stereo Spread
        horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(horizontalContainer);
        horizontalContainer->addSpace(57);

        brightnessKnob = new Knob(this, RK_RC_IMAGE(crackle_brightness_knob_label));
        brightnessKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        brightnessKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        horizontalContainer->addWidget(brightnessKnob);

        durationKnob = new Knob(this, RK_RC_IMAGE(crackle_duration_knob_label));
        durationKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        durationKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(durationKnob);

        stereospreadKnob = new Knob(this, RK_RC_IMAGE(crackle_stereospread_knob_label));
        stereospreadKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        stereospreadKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        horizontalContainer->addWidget(stereospreadKnob);
}


void CrackleWidget::setEnvelopeShape(CrackleEnvelopeShape shape)
{
        exponentialEnvButton->setPressed(shape == CrackleEnvelopeShape::Exponential);
        linearEnvButton->setPressed(shape == CrackleEnvelopeShape::Linear);
        tiangleEnvButton->setPressed(shape == CrackleEnvelopeShape::Triangle);
}
