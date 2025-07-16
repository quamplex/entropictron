/**
 * File name: EntWidget.h
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

#ifndef ENT_WIDGET_H
#define ENT_WIDGET_H

#include "globals.h"

#include <RkWidget.h>
#include <RkImage.h>

class RkPaintEvent;
class RkNativeWindowInfo;

class EntWidget: public RkWidget
{
public:
        explicit EntWidget(RkMain& app,
                           Rk::WidgetFlags flags = Rk::WidgetFlags::Widget);
        explicit EntWidget(RkMain& app,
                           const RkNativeWindowInfo &info,
                           Rk::WidgetFlags flags = Rk::WidgetFlags::Widget);
        explicit EntWidget(EntWidget *parent,
                           Rk::WidgetFlags flags = Rk::WidgetFlags::Widget);
        virtual ~EntWidget() = default;
        void setBackgroundImage(const RkImage &img);
        const RkImage& backgroundImage() const;

protected:
        void paintEvent(RkPaintEvent *event) override;
        void mouseButtonPressEvent(RkMouseEvent *event) override;

private:
        RkImage m_backgroundImage;
};

#endif // ENT_WIDGET_H
