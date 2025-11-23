/**
 * File name: RkScroolbar.h
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

#ifndef RK_SCROOLBAR_H
#define RK_SCROOLBAR_H

#include "RkWidget.h"

class RkScroolbar : public RkWidget {
 public:
        explicit RkScroolbar(RkWidget *parent);
        ~RkFlowContainer() = default;
        void setContentSize(int val);
        int getContentSize() const;
        void setPageSize(int val);
        int getPageSize() const;
        void setScroolStep(int val);
        int getScroolStep() const;
        void setPageScroolStep(int val);
        int getPageScroolStep() const;

 protected:
        void resizeEvent(RkResizeEvent *event) override;

 private:
        int contentSize;
        int pageSize;
        int scroolStep;
        int pageScroolStep;
        RkButton *sliderButton;
        RkButton *upButton;
        RkButton *downButton;
};

