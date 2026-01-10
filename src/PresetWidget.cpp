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
#include "EntState.h"
#include "EntropictronModel.h"

#include "RkPainter.h"
#include "RkScroolbar.h"
#include "RkEvent.h"

PresetWidget::PresetWidget(EntWidget* parent, EntropictronModel *model)
        : EntWidget(parent)
        , entModel{model}
        , presetList{std::make_unique<PresetList>()}
        , padding{5}
        , rowHeight{18}
        , offsetIndex{0}
        , pageSize{0}
        , scroolbar{nullptr}
        , selectedIndex{-1}
        , hoverIndex{-1}
{
        setFixedSize(240, (202 / rowHeight) * rowHeight);
        setBackgroundColor(22, 22, 22);
        setTextColor({180, 180, 180});
        updateListView();
}

void PresetWidget::updateListView()
{
        offsetIndex = 0;
        selectedIndex = -1;
        hoverIndex = -1;
        pageSize = height() / rowHeight;

        if (static_cast<int>(presetList->size()) > pageSize)
                showScroolbar();
        else
                removeScroolbar();

        update();
}

void PresetWidget::setPresetList(std::unique_ptr<PresetList> list)
{
        presetList = std::move(list);
        updateListView();
}

void PresetWidget::showScroolbar()
{
        scroolbar = new RkScroolbar(this);
        scroolbar->setSize(16, height());
        scroolbar->setPosition(width() - scroolbar->width(), 0);
        scroolbar->setContentSize(presetList->size());
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

void PresetWidget::scroolContent(int offset)
{
        offsetIndex = std::clamp(offset, 0, static_cast<int>(presetList->size()) - pageSize);
        if (scroolbar)
                scroolbar->setScrool(offsetIndex);
        hoverIndex = -1;
        update();
}

void PresetWidget::paintEvent([[maybe_unused]] RkPaintEvent *event)
{
        if (!presetList)
                return;

        RkPainter painter(this);
        int rowY = 0;
        int listSize = static_cast<int>(presetList->size());
        for (auto i = offsetIndex; i < offsetIndex + pageSize && i < listSize; i++) {
                const auto *preset = presetList->getPreset(i);
                if (!preset)
                        continue;

                auto rowColor = (i - offsetIndex) % 2 ? RkColor(22, 22, 22) : RkColor(35, 35, 35);
                auto txtColor = textColor();

                if (hoverIndex == i) {
                        txtColor = RkColor(255, 255, 255);
                        rowColor = rowColor + 5;
                }
                if (selectedIndex == i) {
                        rowColor = {100, 100, 255};
                        txtColor = RkColor(255, 255, 255);
                }

                auto pen = painter.pen();
                pen.setColor(txtColor);

                RkRect texRect(padding, rowY, width() - padding, rowHeight);
                RkRect rawRect(0, rowY, width(), rowHeight);
                painter.fillRect(rawRect, rowColor);
                painter.setPen(pen);
                painter.drawText(texRect, preset->getName(), Rk::Alignment::AlignLeft);

                rowY += rowHeight;
        }
}

void PresetWidget::mouseButtonPressEvent(RkMouseEvent *event)
{
        if (event->button() == RkMouseEvent::ButtonType::WheelUp
            || event->button() == RkMouseEvent::ButtonType::WheelDown) {
                auto delta = event->button() == RkMouseEvent::ButtonType::WheelUp ? -1 : 1;
                scroolContent(offsetIndex + delta);
                return;
        }

        if (event->button() == RkMouseEvent::ButtonType::Left) {
                auto index = getIndex(event->x(), event->y());
                const auto *selectedPreset = presetList->getPreset(index);
                if (selectedPreset && entModel->loadPreset(selectedPreset)) {
                        selectedIndex = index;
                        update();
                }
	}
}

void PresetWidget::mouseMoveEvent(RkMouseEvent *event)
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
                return index + offsetIndex;

        return -1;
}
