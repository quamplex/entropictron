/**
 * File name: GlobalControlsWidget.h
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

#ifndef ENT_GLOBAL_CONTROLS_WIDGET_H
#define ENT_GLOBAL_CONTROLS_WIDGET_H

#include "EntWidget.h"

class EntropictronModel;
class RkContainer;

class GlobalControlsWidget : public EntWidget
{
public:
        explicit GlobalControlsWidget(EntWidget* parent, EntropictronModel* model);
        ~GlobalControlsWidget() = default;

protected:
        void createPlayModeControls(RkContainer *container);
        void setPlayMode(PlayMode mode);

private:
        RkButton *playbackButton;
        RkButton *holdButton;
        RkButton *onButton;
};

#endif // ENT_GLOBAL_CONTROLS_WIDGET_H
