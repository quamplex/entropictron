/**
 * File name: ModuleWidgetTab.cpp
 * Project: Entropictron (A context generator and audio effect)
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
#include "RgateWidget.h"
#include "BitcrasherWidget.h"

#include "RkButton.h"
#include "RkContainer.h"

// Generators
RK_DECLARE_IMAGE_RC(tab_noise1_button);
RK_DECLARE_IMAGE_RC(tab_noise1_button_hover);
RK_DECLARE_IMAGE_RC(tab_noise1_button_on);
RK_DECLARE_IMAGE_RC(tab_noise1_button_hover_on);
RK_DECLARE_IMAGE_RC(tab_noise2_button);
RK_DECLARE_IMAGE_RC(tab_noise2_button_hover);
RK_DECLARE_IMAGE_RC(tab_noise2_button_on);
RK_DECLARE_IMAGE_RC(tab_noise2_button_hover_on);
RK_DECLARE_IMAGE_RC(tab_crackle1_button);
RK_DECLARE_IMAGE_RC(tab_crackle1_button_hover);
RK_DECLARE_IMAGE_RC(tab_crackle1_button_on);
RK_DECLARE_IMAGE_RC(tab_crackle1_button_hover_on);
RK_DECLARE_IMAGE_RC(tab_crackle2_button);
RK_DECLARE_IMAGE_RC(tab_crackle2_button_hover);
RK_DECLARE_IMAGE_RC(tab_crackle2_button_on);
RK_DECLARE_IMAGE_RC(tab_crackle2_button_hover_on);

// Effects
RK_DECLARE_IMAGE_RC(tab_glitch_button);
RK_DECLARE_IMAGE_RC(tab_glitch_button_hover);
RK_DECLARE_IMAGE_RC(tab_glitch_button_on);
RK_DECLARE_IMAGE_RC(tab_glitch_button_hover_on);
RK_DECLARE_IMAGE_RC(tab_rgate_button);
RK_DECLARE_IMAGE_RC(tab_rgate_button_hover);
RK_DECLARE_IMAGE_RC(tab_rgate_button_on);
RK_DECLARE_IMAGE_RC(tab_rgate_button_hover_on);
RK_DECLARE_IMAGE_RC(tab_bitcrasher_button);
RK_DECLARE_IMAGE_RC(tab_bitcrasher_button_hover);
RK_DECLARE_IMAGE_RC(tab_bitcrasher_button_on);
RK_DECLARE_IMAGE_RC(tab_bitcrasher_button_hover_on);

ModuleWidgetTab::ModuleWidgetTab(EntWidget* parent,
                                 EntropictronModel *model,
                                 ModuleType type)
        : EntWidget(parent)
        , entModel{model}
        , moduleType{type}
{
        setFixedSize(350, 331);
        createTabButtons();

        if (moduleType == ModuleType::ModuleGenerator)
                showModuleControls(Module::Noise1);
        else
                showModuleControls(Module::Glitch);
}

void ModuleWidgetTab::createTabButtons()
{
        auto tabButtonWidget = new EntWidget(this);
        tabButtonWidget->setBackgroundColor(37, 43, 53);
        tabButtonWidget->setSize({width(), 24});
        auto tabButtonContianer = new RkContainer(tabButtonWidget);

        if (moduleType == ModuleType::ModuleGenerator)
                createGeneratorsButtons(tabButtonWidget, tabButtonContianer);
        else
                createEffectsButtons(tabButtonWidget, tabButtonContianer);
}

void ModuleWidgetTab::showModuleControls(ModuleWidgetTab::Module module)
{
        if (moduleType == ModuleType::ModuleGenerator) {
                for (size_t i = 0; i < 2; i++) {
                        auto noise = i == 0 ? Module::Noise1 : Module::Noise2;
                        auto crackle = i == 0 ? Module::Crackle1 : Module::Crackle2;
                        noiseTabButton[i]->setPressed(module == noise);
                        crackleTabButton[i]->setPressed(module == crackle);
                }
        } else {
                glitchTabButton->setPressed(module == Module::Glitch);
                rgateTabButton->setPressed(module == Module::Rgate);
                bitcrasherTabButton->setPressed(module == Module::Bitcrasher);
        }

        if (currentModule == module)
                return;

        currentModule = module;
        delete moduleWidget;
        moduleWidget = nullptr;

        switch (module) {
        case Noise1:
                moduleWidget = new NoiseWidget(this, entModel->getNoise1());
                break;
        case Noise2:
                moduleWidget = new NoiseWidget(this, entModel->getNoise2());
                break;
        case Crackle1:
                moduleWidget = new CrackleWidget(this, entModel->getCrackle1());
                break;
        case Crackle2:
                moduleWidget = new CrackleWidget(this, entModel->getCrackle2());
                break;
        case Glitch:
                moduleWidget = new GlitchWidget(this, entModel->getGlitch());
                break;
        case Rgate:
                moduleWidget = new RgateWidget(this, entModel->getRgate());
                break;
        case Bitcrasher:
                moduleWidget = new BitcrasherWidget(this, entModel->getBitcrasher());
                break;
        default:
                break;
        }

        if (moduleWidget)
                moduleWidget->setPosition(0, 29);
}

void ModuleWidgetTab::createGeneratorsButtons(RkWidget* widget, RkContainer *container)
{
        for (size_t i = 0; i < 2; i++) {
                // Noise
                noiseTabButton[i] = new RkButton(widget);
                noiseTabButton[i]->setBackgroundColor(widget->background());
                noiseTabButton[i]->setImage( i == 0 ? RK_RC_IMAGE(tab_noise1_button)
                                             : RK_RC_IMAGE(tab_noise2_button),
                                             RkButton::State::Unpressed);
                noiseTabButton[i]->setImage( i == 0 ? RK_RC_IMAGE(tab_noise1_button_on)
                                             : RK_RC_IMAGE(tab_noise2_button_on),
                                             RkButton::State::Pressed);
                noiseTabButton[i]->setImage( i == 0 ? RK_RC_IMAGE(tab_noise1_button_hover)
                                             : RK_RC_IMAGE(tab_noise2_button_hover),
                                             RkButton::State::UnpressedHover);
                noiseTabButton[i]->setImage( i == 0 ? RK_RC_IMAGE(tab_noise1_button_hover_on)
                                             : RK_RC_IMAGE(tab_noise2_button_hover_on),
                                             RkButton::State::PressedHover);
                noiseTabButton[i]->setCheckable(true);
                noiseTabButton[i]->show();
                container->addWidget(noiseTabButton[i]);
                auto mod = i == 0 ? Module::Noise1 : Module::Noise2;
                RK_ACT_BIND(noiseTabButton[i],
                            toggled,
                            RK_ACT_ARGS(bool b),
                            this,
                            showModuleControls(mod));

                // Crackle
                crackleTabButton[i] = new RkButton(widget);
                crackleTabButton[i]->setBackgroundColor(widget->background());
                crackleTabButton[i]->setImage(i == 0 ? RK_RC_IMAGE(tab_crackle1_button)
                                              : RK_RC_IMAGE(tab_crackle2_button),
                                              RkButton::State::Unpressed);
                crackleTabButton[i]->setImage(i == 0 ? RK_RC_IMAGE(tab_crackle1_button_on)
                                              : RK_RC_IMAGE(tab_crackle2_button_on),
                                              RkButton::State::Pressed);
                crackleTabButton[i]->setImage(i == 0 ? RK_RC_IMAGE(tab_crackle1_button_hover)
                                              : RK_RC_IMAGE(tab_crackle2_button_hover),
                                              RkButton::State::UnpressedHover);
                crackleTabButton[i]->setImage(i == 0 ? RK_RC_IMAGE(tab_crackle1_button_hover_on)
                                              : RK_RC_IMAGE(tab_crackle2_button_hover_on),
                                              RkButton::State::PressedHover);
                crackleTabButton[i]->setCheckable(true);
                crackleTabButton[i]->show();
                container->addWidget(crackleTabButton[i]);
                mod = i == 0 ? Module::Crackle1 : Module::Crackle2;
                RK_ACT_BIND(crackleTabButton[i],
                            toggled,
                            RK_ACT_ARGS(bool b),
                            this,
                            showModuleControls(mod));
        }
}

void ModuleWidgetTab::createEffectsButtons(RkWidget *widget, RkContainer *container)
{
        // Glitch
        glitchTabButton = new RkButton(widget);
        glitchTabButton->setBackgroundColor(widget->background());
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
        container->addWidget(glitchTabButton);
        RK_ACT_BIND(glitchTabButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    this,
                    showModuleControls(Module::Glitch));

        // Rgate
        rgateTabButton = new RkButton(widget);
        rgateTabButton->setBackgroundColor(widget->background());
        rgateTabButton->setImage(RK_RC_IMAGE(tab_rgate_button),
                                 RkButton::State::Unpressed);
        rgateTabButton->setImage(RK_RC_IMAGE(tab_rgate_button_on),
                                 RkButton::State::Pressed);
        rgateTabButton->setImage(RK_RC_IMAGE(tab_rgate_button_hover),
                                 RkButton::State::UnpressedHover);
        rgateTabButton->setImage(RK_RC_IMAGE(tab_rgate_button_hover_on),
                                 RkButton::State::PressedHover);
        rgateTabButton->setCheckable(true);
        rgateTabButton->show();
        container->addWidget(rgateTabButton);
        RK_ACT_BIND(rgateTabButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    this,
                    showModuleControls(Module::Rgate));

        // Bitcrasher
        bitcrasherTabButton = new RkButton(widget);
        bitcrasherTabButton->setBackgroundColor(widget->background());
        bitcrasherTabButton->setImage(RK_RC_IMAGE(tab_bitcrasher_button),
                                      RkButton::State::Unpressed);
        bitcrasherTabButton->setImage(RK_RC_IMAGE(tab_bitcrasher_button_on),
                                      RkButton::State::Pressed);
        bitcrasherTabButton->setImage(RK_RC_IMAGE(tab_bitcrasher_button_hover),
                                      RkButton::State::UnpressedHover);
        bitcrasherTabButton->setImage(RK_RC_IMAGE(tab_bitcrasher_button_hover_on),
                                      RkButton::State::PressedHover);
        bitcrasherTabButton->setCheckable(true);
        bitcrasherTabButton->show();
        container->addWidget(bitcrasherTabButton);
        RK_ACT_BIND(bitcrasherTabButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    this,
                    showModuleControls(Module::Bitcrasher));
}
