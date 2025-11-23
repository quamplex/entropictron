/**
 * File name: RkScroolbar.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2025 Iurie Nistor
 *
 * This file is part of Redkite.
 *
 * Redkite is free software; you can redistribute it and/or modify
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

#include "RkScroolbar.h"

RkScroolbar::RkScroolbar(RkWidget *parent)
        : RkWidget(parent)
        , scroolRange{0, 0}
        , pageSize{0}
        , scroolStep{1}
        , pageScroolStep{1}
        , sliderButton{new RkButton(this)}
        , upButton{new RkButton(this)}
        , downButton{new RkButton(this)}
{
        upButton->setSize({24, 24});
        downButton->setSize({24, 24});
}

void RkScroolbar::setContentSize(int val)
{
        contentSize = val;
}

int RkScroolbar::getContentSize() const
{
        return contentSize;
}

void RkScroolbar::setPageSize(int val)
{
}

int RkScroolbar::getPageSize() const
{
}

void RkScroolbar::setScroolStep(int val)
{
}

int RkScroolbar::getScroolStep() const
{
}

void setPageScroolStep(int val)
{
}

int getPageScroolStep() const
{
}

void RkScroolbar::resizeEvent(RkResizeEvent *event) override
{
        upButton->width(evet->size().width());
        downButton->width(evet->size().width());
        sliderButton->width(evet->size().width());
        
}
