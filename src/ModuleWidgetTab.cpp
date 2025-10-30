/**
 * File name: ModuleWidgetTab.cpp
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

#include "ModuleWidgetTab.h"
#include "EntropictronModel.h"
#include "NoiseWidget.h"
#include "CrackleWidget.h"
#include "GlitchWidget.h"

#include "RkButton.h"
#include "RkContainer.h"

RK_DECLARE_IMAGE_RC(tab_noise_button);
RK_DECLARE_IMAGE_RC(tab_noise_button_hover);
RK_DECLARE_IMAGE_RC(tab_noise_button_on);
RK_DECLARE_IMAGE_RC(tab_noise_button_hover_on);
RK_DECLARE_IMAGE_RC(tab_crackle_button);
RK_DECLARE_IMAGE_RC(tab_crackle_button_hover);
RK_DECLARE_IMAGE_RC(tab_crackle_button_on);
RK_DECLARE_IMAGE_RC(tab_crackle_button_hover_on);
RK_DECLARE_IMAGE_RC(tab_glitch_button);
RK_DECLARE_IMAGE_RC(tab_glitch_button_hover);
RK_DECLARE_IMAGE_RC(tab_glitch_button_on);
RK_DECLARE_IMAGE_RC(tab_glitch_button_hover_on);

ModuleWidgetTab::ModuleWidgetTab(EntWidget* parent,
                                 EntropictronModel *model,
                                 size_t id)
        : EntWidget(parent)
        , tabId{id}
        , entropictronModel{model}
        , moduleWidget {nullptr}
{
        setFixedSize(350, 282);
        createTabButtons();
        showNoise();
}

void ModuleWidgetTab::createTabButtons()
{
        auto tabButtonContianer = new RkContainer(this);
        tabButtonContianer->setSize({width(), 20});

        // Noise
        auto tabButton = new RkButton(this);
        tabButton->setImage(RK_RC_IMAGE(tab_noise_button),
                            RkButton::State::Unpressed);
        tabButton->setImage(RK_RC_IMAGE(tab_noise_button_on),
                            RkButton::State::Pressed);
        tabButton->setImage(RK_RC_IMAGE(tab_noise_button_hover),
                            RkButton::State::UnpressedHover);
        tabButton->setImage(RK_RC_IMAGE(tab_noise_button_on),
                            RkButton::State::PressedHover);
        tabButton->setCheckable(true);
        tabButton->show();
        tabButtonContianer->addWidget(tabButton);
        RK_ACT_BIND(tabButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    this,
                    showNoise());

        // Crackle
        tabButton = new RkButton(this);
        tabButton->setImage(RK_RC_IMAGE(tab_crackle_button),
                            RkButton::State::Unpressed);
        tabButton->setImage(RK_RC_IMAGE(tab_crackle_button_on),
                            RkButton::State::Pressed);
        tabButton->setImage(RK_RC_IMAGE(tab_crackle_button_hover),
                            RkButton::State::UnpressedHover);
        tabButton->setImage(RK_RC_IMAGE(tab_crackle_button_on),
                            RkButton::State::PressedHover);
        tabButton->setCheckable(true);
        tabButton->show();
        tabButtonContianer->addWidget(tabButton);
        RK_ACT_BIND(tabButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    this,
                    showCrackle());

        // Glitch
        tabButton = new RkButton(this);
        tabButton->setImage(RK_RC_IMAGE(tab_glitch_button),
                            RkButton::State::Unpressed);
        tabButton->setImage(RK_RC_IMAGE(tab_glitch_button_on),
                            RkButton::State::Pressed);
        tabButton->setImage(RK_RC_IMAGE(tab_glitch_button_hover),
                            RkButton::State::UnpressedHover);
        tabButton->setImage(RK_RC_IMAGE(tab_glitch_button_on),
                            RkButton::State::PressedHover);
        tabButton->setCheckable(true);
        tabButton->show();
        tabButtonContianer->addWidget(tabButton);
        RK_ACT_BIND(tabButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    this,
                    showGlitch());
}

void ModuleWidgetTab::showNoise()
{
        delete moduleWidget;
        moduleWidget = new NoiseWidget(this,
                                       tabId ? entropictronModel->getNoise1()
                                       : entropictronModel->getNoise2());
        moduleWidget->setPosition(0, 25);
}

void ModuleWidgetTab::showCrackle()
{
        delete moduleWidget;
        moduleWidget = new CrackleWidget(this,
                                         tabId ? entropictronModel->getCrackle1()
                                         : entropictronModel->getCrackle2());
        moduleWidget->setPosition(0, 25);
}

void ModuleWidgetTab::showGlitch()
{
        delete moduleWidget;
        moduleWidget = new GlitchWidget(this,
                                        tabId ? entropictronModel->getGlitch1()
                                        : entropictronModel->getGlitch2());
        moduleWidget->setPosition(0, 25);
}
