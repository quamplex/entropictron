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
        , noiseTabButton {nullptr}
        , crackleTabButton {nullptr}
        , glitchTabButton {nullptr}
{
        setFixedSize(350, 331);
        createTabButtons();
        showNoise();
}

void ModuleWidgetTab::createTabButtons()
{
        auto tabButtonWidget = new EntWidget(this);
        tabButtonWidget->setBackgroundColor(37, 43, 53);
        tabButtonWidget->setSize({width(), 24});
        auto tabButtonContianer = new RkContainer(tabButtonWidget);

        // Noise
        noiseTabButton = new RkButton(tabButtonWidget);
        noiseTabButton->setBackgroundColor(tabButtonWidget->background());
        noiseTabButton->setImage(RK_RC_IMAGE(tab_noise_button),
                            RkButton::State::Unpressed);
        noiseTabButton->setImage(RK_RC_IMAGE(tab_noise_button_on),
                            RkButton::State::Pressed);
        noiseTabButton->setImage(RK_RC_IMAGE(tab_noise_button_hover),
                            RkButton::State::UnpressedHover);
        noiseTabButton->setImage(RK_RC_IMAGE(tab_noise_button_hover_on),
                            RkButton::State::PressedHover);
        noiseTabButton->setCheckable(true);
        noiseTabButton->show();
        tabButtonContianer->addWidget(noiseTabButton);
        RK_ACT_BIND(noiseTabButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    this,
                    showNoise());

        // Crackle
        crackleTabButton = new RkButton(tabButtonWidget);
        crackleTabButton->setBackgroundColor(tabButtonWidget->background());
        crackleTabButton->setImage(RK_RC_IMAGE(tab_crackle_button),
                            RkButton::State::Unpressed);
        crackleTabButton->setImage(RK_RC_IMAGE(tab_crackle_button_on),
                            RkButton::State::Pressed);
        crackleTabButton->setImage(RK_RC_IMAGE(tab_crackle_button_hover),
                            RkButton::State::UnpressedHover);
        crackleTabButton->setImage(RK_RC_IMAGE(tab_crackle_button_hover_on),
                            RkButton::State::PressedHover);
        crackleTabButton->setCheckable(true);
        crackleTabButton->show();
        tabButtonContianer->addWidget(crackleTabButton);
        RK_ACT_BIND(crackleTabButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    this,
                    showCrackle());

        // Glitch
        glitchTabButton = new RkButton(tabButtonWidget);
        glitchTabButton->setBackgroundColor(tabButtonWidget->background());
        glitchTabButton->setImage(RK_RC_IMAGE(tab_glitch_button),
                            RkButton::State::Unpressed);
        glitchTabButton->setImage(RK_RC_IMAGE(tab_glitch_button_on),
                            RkButton::State::Pressed);
        glitchTabButton->setImage(RK_RC_IMAGE(tab_glitch_button_hover),
                            RkButton::State::UnpressedHover);
        glitchTabButton->setImage(RK_RC_IMAGE(tab_glitch_button_hover_on),
                            RkButton::State::PressedHover);
        glitchTabButton->setCheckable(true);
        glitchTabButton->show();
        tabButtonContianer->addWidget(glitchTabButton);
        RK_ACT_BIND(glitchTabButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    this,
                    showGlitch());
}

void ModuleWidgetTab::showNoise()
{
        noiseTabButton->setPressed(true);
        crackleTabButton->setPressed(false);
        glitchTabButton->setPressed(false);

        delete moduleWidget;
        moduleWidget = new NoiseWidget(this,
                                       tabId == 0 ? entropictronModel->getNoise1()
                                       : entropictronModel->getNoise2());
        moduleWidget->setPosition(0, 29);
}

void ModuleWidgetTab::showCrackle()
{
        noiseTabButton->setPressed(false);
        crackleTabButton->setPressed(true);
        glitchTabButton->setPressed(false);

        delete moduleWidget;
        moduleWidget = new CrackleWidget(this,
                                         tabId == 0 ? entropictronModel->getCrackle1()
                                         : entropictronModel->getCrackle2());
        moduleWidget->setPosition(0, 29);
}

void ModuleWidgetTab::showGlitch()
{
        noiseTabButton->setPressed(false);
        crackleTabButton->setPressed(false);
        glitchTabButton->setPressed(true);

        delete moduleWidget;
        moduleWidget = new GlitchWidget(this,
                                        tabId == 0 ? entropictronModel->getGlitch1()
                                        : entropictronModel->getGlitch2());
        moduleWidget->setPosition(0, 29);
}
