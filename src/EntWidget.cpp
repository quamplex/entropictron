/**
 * File name: EntWidget.cpp
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

#include "EntWidget.h"

#include <RkPainter.h>

EntWidget::EntWidget(RkMain& app,
                     const RkNativeWindowInfo &info,
                     Rk::WidgetFlags flags)
        : RkWidget(app, info, flags)
{
        setBackgroundColor(68, 68, 70);
        setTextColor({210, 226, 226, 140});
        auto f = font();
        f.setSize(12);
        setFont(f);
}

EntWidget::EntWidget(RkMain& app, Rk::WidgetFlags flags)
        : RkWidget(app, flags)
{
        setBackgroundColor(19, 19, 19);
        setTextColor({204, 204, 204});
        auto f = font();
        f.setSize(12);
        setFont(f);
}

EntWidget::EntWidget(EntWidget *parent, Rk::WidgetFlags flags)
        : RkWidget(parent, flags)
{
        setBackgroundColor(19, 19, 19);
        setTextColor({204, 204, 204});
        auto f = font();
        f.setSize(12);
        setFont(f);
}

void EntWidget::paintEvent(RkPaintEvent *event)
{
        if (!m_backgroundImage.isNull()) {
                RkPainter painter(this);
                painter.fillRect(rect(), background());
                painter.drawImage(m_backgroundImage, 0, 0);
        }
}

void EntWidget::setBackgroundImage(const RkImage &img)
{
        if (!img.isNull()) {
                m_backgroundImage = img;
                update();
        }
}

void EntWidget::mouseButtonPressEvent(RkMouseEvent *event)
{
        setFocus(true);
}
