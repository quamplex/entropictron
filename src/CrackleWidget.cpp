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

RK_DECLARE_IMAGE_RC(crackle1_label);
RK_DECLARE_IMAGE_RC(crackle2_label);
RK_DECLARE_IMAGE_RC(switch_button_on);
RK_DECLARE_IMAGE_RC(switch_button_off);

CrackleWidget::CrackleWidget(EntWidget* parent, CrackleModel* model)
        : EntAbstractView(parent, model)
        , enableCrackleButton {nullptr}
        , crackleLabel {nullptr}
{
        setFixedSize(350, 282);
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
        topContianer->addSpace(100);

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

        crackleLabel = new RkLabel(this);
        topContianer->addSpace(10);
        topContianer->addWidget(crackleLabel);

        createCrackleControls(mainContainer);

        updateView();
}

void CrackleWidget::updateView()
{
        auto model = static_cast<CrackleModel*>(getModel());
        if (!model)
                return;

        if (model->getId() == CrackleId::Crackle1)
                crackleLabel->setImage(RK_RC_IMAGE(crackle1_label));
        else
                crackleLabel->setImage(RK_RC_IMAGE(crackle2_label));

        enableCrackleButton->setPressed(model->isEnabled());
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
        RK_ACT_BIND(model,
                    modelUpdated,
                    RK_ACT_ARGS(),
                    this,
                    updateView());
        RK_ACT_BIND(model,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    enableCrackleButton,
                    setPressed(b));
}

void CrackleWidget::unbindModel()
{
        auto model = getModel();
        unbindObject(model);
        enableCrackleButton->unbindObject(model);
        whiteCrackleButton->unbindObject(model);
        pinkCrackleButton->unbindObject(model);
        brownCrackleButton->unbindObject(model);
        densityKnob->unbindObject(model);
        brightnessKnob->unbindObject(model);
        gainKnob->unbindObject(model);
}

void CrackleWidget::createCrackleControls(RkContainer *container)
{
        auto crackleControlsContainer = new RkContainer(this);
        crackleControlsContainer->setSize({width(), 103});
        container->addSpace(20);
        container->addContainer(crackleControlsContainer);
}
