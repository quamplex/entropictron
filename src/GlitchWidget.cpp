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

RK_DECLARE_IMAGE_RC(glitch1_label);
RK_DECLARE_IMAGE_RC(glitch2_label);
RK_DECLARE_IMAGE_RC(switch_button_on);
RK_DECLARE_IMAGE_RC(switch_button_off);
RK_DECLARE_IMAGE_RC(knob_big_size_bk);
RK_DECLARE_IMAGE_RC(knob_big_size_marker);
RK_DECLARE_IMAGE_RC(knob_medium_size_bk);
RK_DECLARE_IMAGE_RC(knob_medium_size_marker);
RK_DECLARE_IMAGE_RC(crackle_repeats_knob_label);
RK_DECLARE_IMAGE_RC(crackle_probability_knob_label);
RK_DECLARE_IMAGE_RC(crackle_lengh_knob_label);
RK_DECLARE_IMAGE_RC(crackle_maxjump_knob_label);
RK_DECLARE_IMAGE_RC(crackle_minjump_knob_label);

GlitchWidget::GlitchWidget(EntWidget* parent, GlitchModel* model)
        : EntAbstractView(parent, model)
        , enableGlitchButton {nullptr}
        , repeatsKnob{nullptr}
        , probabilityKnob{nullptr}
        , lengthKnob{nullptr}
        , maxJumpKnob{nullptr}
        , minJumpKnob{nullptr}
{
        setFixedSize(350, 282);
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
        topContianer->addSpace(100);

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

        auto glitchLabel = new RkLabel(this);
        glitchLabel->setImage(RK_RC_IMAGE(glitch_label));
        topContianer->addSpace(10);
        topContianer->addWidget(glitchLabel);

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
        repeatsKnob->setDefaultValue(model->getRepeatsDefaultValue());
        repeatsKnob->setValue(model->repeats());

        auto [probabilityFrom, probabilityTo] = model->getProbabilityRange();
        probabilityKnob->setRange(probabilityFrom, probabilityTo);
        probabilityKnob->setRangeType(Knob::RangeType::Logarithmic);
        probabilityKnob->setDefaultValue(model->getProbabilityDefaultValue());
        probabilityKnob->setValue(model->probability());

        auto [lengthFrom, lengthTo] = model->getLengthRange();
        lengthKnob->setRange(lengthFrom, lengthTo);
        lengthKnob->setDefaultValue(model->getLengthDefaultValue());
        lengthKnob->setValue(model->length());

        auto [maxJumpFrom, maxJumpTo] = model->getMaxJumpRange();
        maxJumpKnob->setRange(maxJumpFrom, maxJumpTo);
        maxJumpKnob->setRangeType(Knob::RangeType::Logarithmic);
        maxJumpKnob->setDefaultValue(model->getMaxJumpDefaultValue());
        maxJumpKnob->setValue(model->maxJump());

        auto [minJumpFrom, minJumpTo] = model->getMinJumpRange();
        minJumpKnob->setRange(minJumpFrom, minJumpTo);
        minJumpKnob->setRangeType(Knob::RangeType::Logarithmic);
        minJumpKnob->setDefaultValue(model->getMinJumpDefaultValue());
        minJumpKnob->setValue(model->minJump());
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
                    brightnessUpdated,
                    RK_ACT_ARGS(double value),
                    brightnessKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    durationUpdated,
                    RK_ACT_ARGS(double value),
                    durationKnob,
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

        repeatsKnob = new Knob(this, RK_RC_IMAGE(crackle_repeats_knob_label));
        repeatsKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_knob_bk));
        repeatsKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_knob_marker));
        horizontalContainer->addWidget(repeatsKnob);

        probabilityKnob = new Knob(this, RK_RC_IMAGE(crackle_probability_knob_label));
        probabilityKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        probabilityKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        horizontalContainer->addWidget(probabilityKnob);

        lengthKnob = new Knob(this, RK_RC_IMAGE(crackle_length_knob_label));
        lengthKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_knob_bk));
        lengthKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_knob_marker));
        horizontalContainer->addWidget(lengthKnob);

        // Max Jump, Min Jump
        horizontalContainer = new RkContainer(this);
        crackleControlsContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(horizontalContainer);

        maxJumpKnob = new Knob(this, RK_RC_IMAGE(cracke_maxJump_knob_label));
        maxJumpKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_knob_bk));
        maxJumpKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_knob_marker));
        horizontalContainer->addWidget(maxJumpKnob);

        minJumpKnob = new Knob(this, RK_RC_IMAGE(cracke_minJump_knob_label));
        minJumpKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_knob_bk));
        minJumpKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_knob_marker));
        horizontalContainer->addWidget(minJumpKnob);
}
