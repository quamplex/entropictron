/**
 * File name: ModuleWidgetTab.h
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

#ifndef ENT_MODULE_WIDGET_TAB_H
#define ENT_MODULE_WIDGET_TAB_H

#include "EntWidget.h"
#include "GuiTypes.h"

class EntropictronModel;
class RkButton;
class BitcrasherWidget;
class RkContainer;

class ModuleWidgetTab : public EntWidget
{
public:
        enum ModuleType {
                ModuleGenerator,
                ModuleEffect
        };

        explicit ModuleWidgetTab(EntWidget* parent,
                                 EntropictronModel *model,
                                 ModuleType type = {});

private:
        enum Module {
                ModuleNone,
                Noise1,
                Noise2,
                Crackle1,
                Crackle2,
                Glitch,
                Rgate,
                Bitcrasher
        };

        void createTabButtons();
        void createGeneratorsButtons(RkWidget* widget, RkContainer *container);
        void createEffectsButtons(RkWidget* widget, RkContainer *container);
        void showModuleControls(Module module);

        EntropictronModel *entModel;
        ModuleType moduleType {ModuleType::ModuleGenerator};
        Module currentModule {Module::ModuleNone};
        EntWidget *moduleWidget{nullptr};
        RkButton *noiseTabButton[2]{nullptr};
        RkButton *crackleTabButton[2]{nullptr};
        RkButton *glitchTabButton{nullptr};
        RkButton *rgateTabButton{nullptr};
        RkButton *bitcrasherTabButton{nullptr};
};

#endif // ENT_MODULES_WIDGET_TAB_H
