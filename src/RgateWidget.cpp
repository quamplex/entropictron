/**
 * File name: RgateWidget.cpp
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

#include "RgateWidget.h"
#include "Knob.h"
#include "RgateModel.h"

#include "RkLabel.h"
#include "RkContainer.h"
#include "RkButton.h"

RK_DECLARE_IMAGE_RC(rgate_label);
RK_DECLARE_IMAGE_RC(switch_button_on);
RK_DECLARE_IMAGE_RC(switch_button_off);
RK_DECLARE_IMAGE_RC(knob_big_size_bk);
RK_DECLARE_IMAGE_RC(knob_big_size_marker);
RK_DECLARE_IMAGE_RC(knob_medium_size_bk);
RK_DECLARE_IMAGE_RC(knob_medium_size_marker);
RK_DECLARE_IMAGE_RC(rgate_min_interval_label);
RK_DECLARE_IMAGE_RC(rgate_max_interval_label);
RK_DECLARE_IMAGE_RC(rgate_min_duration_label);
RK_DECLARE_IMAGE_RC(rgate_max_duration_label);
RK_DECLARE_IMAGE_RC(rgate_min_gain_label);
RK_DECLARE_IMAGE_RC(rgate_max_gain_label);
RK_DECLARE_IMAGE_RC(rgate_randomness_label);
RK_DECLARE_IMAGE_RC(rgate_invert_button);
RK_DECLARE_IMAGE_RC(rgate_invert_button_hover);
RK_DECLARE_IMAGE_RC(rgate_invert_button_hover_on);
RK_DECLARE_IMAGE_RC(rgate_invert_button_on);
RK_DECLARE_IMAGE_RC(rgate_invert_label);

RgateWidget::RgateWidget(EntWidget* parent, RgateModel* model)
        : EntAbstractView(parent, model)
        , enableButton{nullptr}
        , minIntervalKnob{nullptr}
        , maxIntervalKnob{nullptr}
        , minDurationKnob{nullptr}
        , maxDurationKnob{nullptr}
        , minGainKnob{nullptr}
        , maxGainKnob{nullptr}
        , randomnessKnob{nullptr}
        , invertButton{nullptr}
{
        setFixedSize(350, 302);
        setBackgroundColor(37, 43, 53);
        createView();
        bindModel();
}

RgateWidget::~RgateWidget()
{
}

void RgateWidget::createView()
{
        auto mainContainer = new RkContainer(this, Rk::Orientation::Vertical);
        mainContainer->addSpace(8);

        auto topContianer = new RkContainer(this, Rk::Orientation::Horizontal);
        topContianer->setSize({width(), 14});
        mainContainer->addContainer(topContianer);
        topContianer->addSpace(128);

        enableButton = new RkButton(this);
        enableButton->setCheckable();
        enableButton->setBackgroundColor(background());
        enableButton->setSize(36, 16);
        enableButton->setImage(RK_RC_IMAGE(switch_button_off),
                                    RkButton::State::Unpressed);
        enableButton->setImage(RK_RC_IMAGE(switch_button_on),
                                    RkButton::State::Pressed);
        enableButton->show();
        topContianer->addWidget(enableButton);

        topContianer->addSpace(10);
        auto rgateLabel = new RkLabel(this);
        rgateLabel->setBackgroundColor(background());
        rgateLabel->setImage(RK_RC_IMAGE(rgate_label));
        rgateLabel->show();
        topContianer->addWidget(rgateLabel);

        mainContainer->addSpace(5);
        createRgateControls(mainContainer);

        updateView();
}

void RgateWidget::updateView()
{
        auto model = static_cast<RgateModel*>(getModel());
        if (!model)
                return;

        enableButton->setPressed(model->isEnabled());

        minIntervalKnob->setRange(model->getMinIntervalRange());
        minIntervalKnob->setDefaultValue(model->getMinIntervalDefaultValue());
        minIntervalKnob->setValue(model->minInterval());

        maxIntervalKnob->setRange(model->getMaxIntervalRange());
        maxIntervalKnob->setDefaultValue(model->getMaxIntervalDefaultValue());
        maxIntervalKnob->setValue(model->maxInterval());

        minDurationKnob->setRange(model->getMinDurationRange());
        minDurationKnob->setDefaultValue(model->getMinDurationDefaultValue());
        minDurationKnob->setValue(model->minDuration());

        maxDurationKnob->setRange(model->getMaxDurationRange());
        maxDurationKnob->setDefaultValue(model->getMaxDurationDefaultValue());
        maxDurationKnob->setValue(model->maxDuration());

        minGainKnob->setRange(model->getMinGainRange());
        minGainKnob->setDefaultValue(model->getMinGainDefaultValue());
        minGainKnob->setValue(model->minGain());

        maxGainKnob->setRange(model->getMaxGainRange());
        maxGainKnob->setDefaultValue(model->getMaxGainDefaultValue());
        maxGainKnob->setValue(model->maxGain());

        randomnessKnob->setRange(model->getRandomnessRange());
        randomnessKnob->setDefaultValue(model->getRandomnessDefaultValue());
        randomnessKnob->setValue(model->randomness());

        invertButton->setPressed(model->isInverted());
}

void RgateWidget::bindModel()
{
        auto model = static_cast<RgateModel*>(getModel());
        if (!model)
                return;

        RK_ACT_BIND(enableButton, toggled, RK_ACT_ARGS(bool b), model, enable(b));
        RK_ACT_BIND(minIntervalKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setMinInterval(value));
        RK_ACT_BIND(maxIntervalKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setMaxInterval(value));
        RK_ACT_BIND(minDurationKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setMinDuration(value));
        RK_ACT_BIND(maxDurationKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setMaxDuration(value));
        RK_ACT_BIND(minGainKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setMinGain(value));
        RK_ACT_BIND(maxGainKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setMaxGain(value));
        RK_ACT_BIND(randomnessKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setRandomness(value));
        RK_ACT_BIND(invertButton, toggled, RK_ACT_ARGS(bool b), model, setInverted(b));

        RK_ACT_BIND(model,
                    modelUpdated,
                    RK_ACT_ARGS(),
                    this,
                    updateView());
        RK_ACT_BIND(model,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    enableButton,
                    setPressed(b));
        RK_ACT_BIND(model,
                    minIntervalUpdated,
                    RK_ACT_ARGS(double value),
                    minIntervalKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    maxIntervalUpdated,
                    RK_ACT_ARGS(double value),
                    maxIntervalKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    minDurationUpdated,
                    RK_ACT_ARGS(double value),
                    minDurationKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    maxDurationUpdated,
                    RK_ACT_ARGS(double value),
                    maxDurationKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    minGainUpdated,
                    RK_ACT_ARGS(double value),
                    minGainKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    maxGainUpdated,
                    RK_ACT_ARGS(double value),
                    maxGainKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    randomnessUpdated,
                    RK_ACT_ARGS(double value),
                    randomnessKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    invertedUpdated,
                    RK_ACT_ARGS(bool b),
                    invertButton,
                    setPressed(b));
}

void RgateWidget::unbindModel()
{
        auto model = getModel();
        unbindObject(model);
        enableButton->unbindObject(model);
        minIntervalKnob->unbindObject(model);
        maxIntervalKnob->unbindObject(model);
        minDurationKnob->unbindObject(model);
        maxDurationKnob->unbindObject(model);
        minGainKnob->unbindObject(model);
        maxGainKnob->unbindObject(model);
        randomnessKnob->unbindObject(model);
        invertButton->unbindObject(model);
}

void RgateWidget::createRgateControls(RkContainer *container)
{
        auto horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(horizontalContainer);

        horizontalContainer->addSpace(41);
        minIntervalKnob = new Knob(this, RK_RC_IMAGE(rgate_min_interval_label));
        minIntervalKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        minIntervalKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(minIntervalKnob);

        maxIntervalKnob = new Knob(this, RK_RC_IMAGE(rgate_max_interval_label));
        maxIntervalKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        maxIntervalKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(maxIntervalKnob);

        horizontalContainer->addSpace(5);
        randomnessKnob = new Knob(this, RK_RC_IMAGE(rgate_randomness_label));
        randomnessKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        randomnessKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        horizontalContainer->addWidget(randomnessKnob);

        horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({width(), 103});
        container->addSpace(10);
        container->addContainer(horizontalContainer);

        horizontalContainer->addSpace(28);

        minDurationKnob = new Knob(this, RK_RC_IMAGE(rgate_min_duration_label));
        minDurationKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        minDurationKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        horizontalContainer->addWidget(minDurationKnob);

        horizontalContainer->addSpace(5);
        maxDurationKnob = new Knob(this, RK_RC_IMAGE(rgate_max_duration_label));
        maxDurationKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        maxDurationKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        horizontalContainer->addWidget(maxDurationKnob);

        horizontalContainer->addSpace(5);

        minGainKnob = new Knob(this, RK_RC_IMAGE(rgate_min_gain_label));
        minGainKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        minGainKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        horizontalContainer->addWidget(minGainKnob);

        maxGainKnob = new Knob(this, RK_RC_IMAGE(rgate_max_gain_label));
        maxGainKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        maxGainKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        horizontalContainer->addWidget(maxGainKnob);

        horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({width(), 26});
        container->addContainer(horizontalContainer);

        invertButton = new RkButton(this);
        invertButton->setImage(RK_RC_IMAGE(rgate_invert_button),
                               RkButton::State::Unpressed);
        invertButton->setImage(RK_RC_IMAGE(rgate_invert_button_on),
                               RkButton::State::Pressed);
        invertButton->setImage(RK_RC_IMAGE(rgate_invert_button_hover),
                               RkButton::State::UnpressedHover);
        invertButton->setImage(RK_RC_IMAGE(rgate_invert_button_hover_on),
                               RkButton::State::PressedHover);
        invertButton->setCheckable(true);
        invertButton->show();
        horizontalContainer->addSpace(31);
        horizontalContainer->addWidget(invertButton);

        auto invertLabel = new RkLabel(this, RK_RC_IMAGE(rgate_invert_label));
        invertLabel->setBackgroundColor(background());
        invertLabel->show();
        horizontalContainer->addWidget(invertLabel);
}
