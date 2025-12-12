/**
 * File name: GlitchWidget.cpp
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

#include "GlitchWidget.h"
#include "Knob.h"
#include "GlitchModel.h"

#include "RkLabel.h"
#include "RkContainer.h"
#include "RkButton.h"

RK_DECLARE_IMAGE_RC(glitch_label);
RK_DECLARE_IMAGE_RC(switch_button_on);
RK_DECLARE_IMAGE_RC(switch_button_off);
RK_DECLARE_IMAGE_RC(knob_big_size_bk);
RK_DECLARE_IMAGE_RC(knob_big_size_marker);
RK_DECLARE_IMAGE_RC(knob_medium_size_bk);
RK_DECLARE_IMAGE_RC(knob_medium_size_marker);
RK_DECLARE_IMAGE_RC(glitch_repeats_knob_label);
RK_DECLARE_IMAGE_RC(glitch_probability_knob_label);
RK_DECLARE_IMAGE_RC(glitch_length_knob_label);
RK_DECLARE_IMAGE_RC(glitch_maxjump_knob_label);
RK_DECLARE_IMAGE_RC(glitch_minjump_knob_label);

GlitchWidget::GlitchWidget(EntWidget* parent, GlitchModel* model)
        : EntAbstractView(parent, model)
        , enableGlitchButton {nullptr}
        , repeatsKnob{nullptr}
        , probabilityKnob{nullptr}
        , lengthKnob{nullptr}
        , maxJumpKnob{nullptr}
        , minJumpKnob{nullptr}
{
        setFixedSize(350, 302);
        setBackgroundColor(37, 43, 53);
        createView();
        bindModel();
}

GlitchWidget::~GlitchWidget()
{
}

void GlitchWidget::createView()
{
        auto mainContainer = new RkContainer(this, Rk::Orientation::Vertical);
        mainContainer->addSpace(8);

        auto topContianer = new RkContainer(this, Rk::Orientation::Horizontal);
        topContianer->setSize({width(), 14});
        mainContainer->addContainer(topContianer);
        topContianer->addSpace(128);

        enableGlitchButton = new RkButton(this);
        enableGlitchButton->setCheckable();
        enableGlitchButton->setBackgroundColor(background());
        enableGlitchButton->setSize(36, 16);
        enableGlitchButton->setImage(RK_RC_IMAGE(switch_button_off),
                                    RkButton::State::Unpressed);
        enableGlitchButton->setImage(RK_RC_IMAGE(switch_button_on),
                                    RkButton::State::Pressed);
        enableGlitchButton->show();
        topContianer->addWidget(enableGlitchButton);

        topContianer->addSpace(10);
        auto glitchLabel = new RkLabel(this);
        glitchLabel->setBackgroundColor(background());
        glitchLabel->setImage(RK_RC_IMAGE(glitch_label));
        glitchLabel->show();
        topContianer->addWidget(glitchLabel);

        mainContainer->addSpace(5);
        createGlitchControls(mainContainer);

        updateView();
}

void GlitchWidget::updateView()
{
        auto model = static_cast<GlitchModel*>(getModel());
        if (!model)
                return;

        enableGlitchButton->setPressed(model->isEnabled());

        auto [repeatsFrom, repeatsTo] = model->getRepeatsRange();
        repeatsKnob->setRange(repeatsFrom, repeatsTo);
        repeatsKnob->setSteps(ENT_GLITCH_MAX_REPEATS);
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

void GlitchWidget::bindModel()
{
        auto model = static_cast<GlitchModel*>(getModel());
        if (!model)
                return;

        RK_ACT_BIND(enableGlitchButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    model,
                    enable(b));
        RK_ACT_BIND(repeatsKnob,
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
                    enableGlitchButton,
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
                    setValue(value));
}

void GlitchWidget::unbindModel()
{
        auto model = getModel();
        unbindObject(model);
        enableGlitchButton->unbindObject(model);
        repeatsKnob->unbindObject(model);
        probabilityKnob->unbindObject(model);
        lengthKnob->unbindObject(model);
        maxJumpKnob->unbindObject(model);
        minJumpKnob->unbindObject(model);
}

void GlitchWidget::createGlitchControls(RkContainer *container)
{
        // Repeats, Probability, Length
        auto horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(horizontalContainer);
        horizontalContainer->addSpace(57);

        repeatsKnob = new Knob(this, RK_RC_IMAGE(glitch_repeats_knob_label));
        repeatsKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        repeatsKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        horizontalContainer->addWidget(repeatsKnob);

        probabilityKnob = new Knob(this, RK_RC_IMAGE(glitch_probability_knob_label));
        probabilityKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        probabilityKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(probabilityKnob);

        lengthKnob = new Knob(this, RK_RC_IMAGE(glitch_length_knob_label));
        lengthKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        lengthKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        horizontalContainer->addWidget(lengthKnob);

        // Max Jump, Min Jump
        horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(horizontalContainer);
        horizontalContainer->addSpace(82);

        minJumpKnob = new Knob(this, RK_RC_IMAGE(glitch_minjump_knob_label));
        minJumpKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        minJumpKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(minJumpKnob);

        maxJumpKnob = new Knob(this, RK_RC_IMAGE(glitch_maxjump_knob_label));
        maxJumpKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        maxJumpKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(maxJumpKnob);
}
