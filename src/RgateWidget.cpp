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

RgateWidget::RgateWidget(EntWidget* parent, RgateModel* model)
        : EntAbstractView(parent, model)
        , enableButton {nullptr}
        , enableInvertButton {nullptr}
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

        auto [repeatsFrom, repeatsTo] = model->getRepeatsRange();
        repeatsKnob->setRange(repeatsFrom, repeatsTo);
        repeatsKnob->setSteps(ENT_RGATE_MAX_REPEATS);
        repeatsKnob->setDefaultValue(model->getRepeatsDefaultValue());
        repeatsKnob->setValue(model->repeats());

        auto [probabilityFrom, probabilityTo] = model->getProbabilityRange();
        probabilityKnob->setRange(probabilityFrom, probabilityTo);
        probabilityKnob->setRangeType(Knob::RangeType::Logarithmic);
        probabilityKnob->setDefaultValue(model->getProbabilityDefaultValue());
        probabilityKnob->setValue(model->probability());

        auto [lengthFrom, lengthTo] = model->getLengthRange();
        lengthKnob->setRange(lengthFrom, lengthTo);
        lengthKnob->setRangeType(Knob::RangeType::Logarithmic);
        lengthKnob->setDefaultValue(model->getLengthDefaultValue());
        lengthKnob->setValue(model->length());

        auto [minJumpFrom, minJumpTo] = model->getMinJumpRange();
        minJumpKnob->setRange(minJumpFrom, minJumpTo);
        minJumpKnob->setRangeType(Knob::RangeType::Logarithmic);
        minJumpKnob->setDefaultValue(model->getMinJumpDefaultValue());
        minJumpKnob->setValue(model->minJump());

        auto [maxJumpFrom, maxJumpTo] = model->getMaxJumpRange();
        maxJumpKnob->setRange(maxJumpFrom, maxJumpTo);
        maxJumpKnob->setRangeType(Knob::RangeType::Logarithmic);
        maxJumpKnob->setDefaultValue(model->getMaxJumpDefaultValue());
        maxJumpKnob->setValue(model->maxJump());
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
        // Repeats, Probability, Length
        auto horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(horizontalContainer);
        horizontalContainer->addSpace(57);

        /*repeatsKnob = new Knob(this, RK_RC_IMAGE(rgate_repeats_knob_label));
        repeatsKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        repeatsKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        horizontalContainer->addWidget(repeatsKnob);

        probabilityKnob = new Knob(this, RK_RC_IMAGE(rgate_probability_knob_label));
        probabilityKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        probabilityKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(probabilityKnob);

        lengthKnob = new Knob(this, RK_RC_IMAGE(rgate_length_knob_label));
        lengthKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        lengthKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        horizontalContainer->addWidget(lengthKnob);

        // Max Jump, Min Jump
        horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(horizontalContainer);
        horizontalContainer->addSpace(82);

        minJumpKnob = new Knob(this, RK_RC_IMAGE(rgate_minjump_knob_label));
        minJumpKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        minJumpKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(minJumpKnob);

        maxJumpKnob = new Knob(this, RK_RC_IMAGE(rgate_maxjump_knob_label));
        maxJumpKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        maxJumpKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(maxJumpKnob);*/
}
