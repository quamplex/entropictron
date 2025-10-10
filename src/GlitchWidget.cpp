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

GlitchWidget::GlitchWidget(EntWidget* parent, GlitchModel* model)
        : EntAbstractView(parent, model)
        , enableGlitchButton {nullptr}
        , glitchLabel {nullptr}
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

        glitchLabel = new RkLabel(this);
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

        if (model->getId() == GlitchId::Glitch1)
                glitchLabel->setImage(RK_RC_IMAGE(glitch1_label));
        else
                glitchLabel->setImage(RK_RC_IMAGE(glitch2_label));

        enableGlitchButton->setPressed(model->isEnabled());
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
}

void GlitchWidget::unbindModel()
{
        auto model = getModel();
        unbindObject(model);
        enableGlitchButton->unbindObject(model);
        whiteGlitchButton->unbindObject(model);
        pinkGlitchButton->unbindObject(model);
        brownGlitchButton->unbindObject(model);
        densityKnob->unbindObject(model);
        brightnessKnob->unbindObject(model);
        gainKnob->unbindObject(model);
}

void GlitchWidget::createGlitchControls(RkContainer *container)
{
        auto glitchControlsContainer = new RkContainer(this);
        glitchControlsContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(glitchControlsContainer);
}
