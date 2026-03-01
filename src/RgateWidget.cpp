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
RK_DECLARE_IMAGE_RC(rgate_min_interval_label);
RK_DECLARE_IMAGE_RC(rgate_max_interval_label);

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

        /*minDurationKnob->setRange(model->getMinDurationRange());
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
        randomnessKnob->setValue(model->randomness());*/
}

void RgateWidget::bindModel()
{
        auto model = static_cast<RgateModel*>(getModel());
        if (!model)
                return;

        RK_ACT_BIND(enableButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    model,
                    enable(b));
        /*RK_ACT_BIND(repeatsKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setRepeats(value));
        RK_ACT_BIND(probabilityKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setProbability(value));
        RK_ACT_BIND(lengthKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setLength(value));
        RK_ACT_BIND(maxJumpKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setMaxJump(value));
        RK_ACT_BIND(minJumpKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setMinJump(value));

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
                    repeatsUpdated,
                    RK_ACT_ARGS(double value),
                    repeatsKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    probabilityUpdated,
                    RK_ACT_ARGS(double value),
                    probabilityKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    lengthUpdated,
                    RK_ACT_ARGS(double value),
                    lengthKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    maxJumpUpdated,
                    RK_ACT_ARGS(double value),
                    maxJumpKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    minJumpUpdated,
                    RK_ACT_ARGS(double value),
                    minJumpKnob,
                    setValue(value));*/
}

void RgateWidget::unbindModel()
{
        auto model = getModel();
        unbindObject(model);
        enableButton->unbindObject(model);
}

void RgateWidget::createRgateControls(RkContainer *container)
{
        auto vContainer = new RkContainer(this);
        vContainer->setSize({width(), container->height()});
        container->addSpace(20);
        container->addContainer(horizontalContainer);
        horizontalContainer->addSpace(57);

        auto horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(horizontalContainer);
        horizontalContainer->addSpace(57);

        minIntervalKnob = new Knob(this, RK_RC_IMAGE(rgate_min_interval_label));
        minIntervalKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        minIntervalKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(minIntervalKnob);

        maxIntervalKnob = new Knob(this, RK_RC_IMAGE(rgate_max_interval_label));
        maxIntervalKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        maxIntervalKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(maxIntervalKnob);

        /*minDurationKnob = new Knob(this, RK_RC_IMAGE(rgate_min_duration_label));
        minDurationKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        minDurationKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(minDurationKnob);

        maxDurationKnob = new Knob(this, RK_RC_IMAGE(rgate_max_duration_label));
        maxDurationKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        maxDurationKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(maxDurationKnob);

        minGainKnob = new Knob(this, RK_RC_IMAGE(rgate_min_gain_label));
        minGainKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        minGainKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(minGainKnob);

        maxGainKnob = new Knob(this, RK_RC_IMAGE(rgate_max_gain_label));
        maxGainKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        maxGainKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(maxGainKnob);

        randomnessKnob = new Knob(this, RK_RC_IMAGE(rgate_randomness_label));
        randomnessKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        randomnessKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(randomnessKnob);*/
}
