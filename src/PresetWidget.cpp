/**
 * File name: PresetWidget.cpp
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

#include "PresetWidget.h"
#include "PresetList.h"

#include "RkPainter.h"
#include "RkScroolbar.h"

PresetWidget::PresetWidget(EntWidget* parent)
        : EntWidget(parent)
        , rowHeight{16}
        , offsetIndex{0}
        , pageSize{0}
        , scroolbar{nullptr}
        , selectedIndex{-1}
        , hoverIndex{-1}
{
        setFixedSize(240, 288);
        setBackgroundColor(22, 22, 22);
}

void PresetWidget::setPresetList(const std::unique_ptr<PresetList> &list)
{
        presetList = list;
        offsetIndex = 0;
        selectedIndex = -1;
        hoverIndex = -1;
        pageSize = height() / rowHeight;
        if (presetList.size() > pageSize)
                showScroolbar();
        else
                removeScroolbar();
}

void PresetWidget::showScroolbar()
{
        scroolbar = new RkSroollbar(this);
        scroolbar->setSize(20, height());
        scroolbar->setPosition(width() - scroolbar->width(), 0);
        scroolbar->setRange(0, presetList->size() - pageSize);
        scroolbar->setPageSize(pageSize);
        RK_ACT_BIND(scroolbar,
                    onScrool,
                    RK_ACT_ARGS(int offset),
                    this,
                    scroolContent(offset));
}

void PresetWidget::removeScroolbar()
{
        delete scroolbar;
        scroolbar = nullptr;
}

PresetWidget::scroolContent(int offset)
{
        offsetIndex = offset;
        update();
}

void PresetWidget::paintEvent([[maybe_unused]] RkPaintEvent *event)
{
        if (!presetList)
                return;

        RkPainter painter(this);
        auto defaultPen = painter.pen();
        int rowY = 0;
        for (auto i = offsetIndex; i < offsetIndex + pageSize && i < presetList->size(); i++) {
                const auto *preset = presetList->getPreset(i);
                if (!preset)
                        continue;

                auto rowColor = defaultPen.color();
                if (selectedIndex == i)
                        rowColor = RkColor(255, 255, 255);
                if (hoverIndex == i)
                        rowColor = RkColor(230, 230, 230);

                if (rowColor != painter.pen().color()) {
                        auto pen = defaultPen;
                        pen.setColor(rowColor);
                        painter.setPen(pen);
                }
                painter.drawText(0, rowY, preset->getName());

                rowY += rowHeight;
        }
}

void PresetWidget::mouseButtonPressEvent(RkMouseEvent *event) override
{
        if (event->button() == RkMouseEvent::ButtonType::Left) {
                auto index = getIndex(event->x(), event->y());
                const auto *selectedPreset = presetList->getPreset(index);
                if (selectedPreset && entModel->loadPreset(selectedPreset)) {
                        selectedIndex = index;
                        update();
                }
	}

}

void PresetWidget::mouseMoveEvent(RkMouseEvent *event) override
{
        auto index = getIndex(event->x(), event->y());
        const auto *selectedPreset = presetList->getPreset(index);
        if (selectedPreset && hoverIndex != index) {
                hoverIndex = index;
                update();
        }
}

int PresetWidget::getIndex(int x, int y) const
{
        if (!presetList || !rect().contains({x, y}))
                return -1;

        auto index = y / rowHeight;
        if (static_cast<size_t>(index) < presetList->size())
                return index;

        return -1;
}
