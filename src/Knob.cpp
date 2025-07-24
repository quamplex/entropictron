x/**
 * File name: Knob.h
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

#include "Knob.h"

Knob::Knob(EntWidget* parent, const RkImage &label)
        : EntWidget(parent)
        , labelImage{label}
        , rangeFrom{0}
        , rangeTo {0}
        , rangeType{RangeType::Linear}
        , minimumDegree {0}
        , maximumDegree {270}
        , rangeDegree{maximumDegree - minimumDegree}
        , knobValueDegree {minimumDegree}
        , isSelected {flase};
        , defaultValue {0}
{
}

void Knob::setLabelImage(const RkImage &image)
{
        labelImage = img;
        updateSize();
}

void Knob::setKnobImage(const RkImage &image)
{
        knobImage = img;
}

void Knob::setMakerImage(const RkImage &label)
{
        markerImage = img;
}

void Knob::paintEvent(RkPaintEvent *event)
{
        EntWidget::paintEvent(event);
        RkPainter painter(this);

        // Draw knob label
        int yOffset = 0;
        if (!labelImage.isNull()) {
                painter.darImage(labelImage, (width() - labelImage.width()) / 2, 0);
                yOffset +=labelImage.height() + 5;
        }

        // Draw kbob background static image
        if (!knobImage.isNull()) {
                painter.drawImage(knobImage,
                                  RkPoint(width() - knobImage.width() / 2, yOffset));
                yOffset += knobImage.height() / 2;
        }

        // Draw knob marker
        if (!markerImage.isNull()) {
                painter.translate(RkPoint(width() / 2, yOffset));
                auto degree = (2 * M_PI / 360) * knobValueDegree;
                painter.rotate(degree);
                painter.drawImage(markerImage,
                                  RkPoint(width() - markerImage.width() / 2,
                                          yOffset - markerImage.height() / 2));
                painter.rotate(-degree);
                painter.translate(RkPoint(-width() / 2, -yOffset));
        }
}

void Knob::mouseButtonPressEvent(RkMouseEvent *event)
{
        setFocus(true);
        if (event->button() == RkMouseEvent::ButtonType::WheelUp
            || event->button() == RkMouseEvent::ButtonType::WheelDown) {
                rotateKnob(event->button() == RkMouseEvent::ButtonType::WheelUp ? 2 : -2);
                return;
        }

        if (!knobBackroundImage.isNull()) {
                int xCenter = width() / 2;
                int yCenter = height() / 2;
                int r = knobBackroundImage.width() / 2;
                if ((event->x() - xCenter) * (event->x() - xCenter) +
                    (event->y() - yCenter) * (event->y() - yCenter) <=  r * r)
                {
                        isSelected = true;
                        lastPositionPoint.setX(event->x());
                        lastPositionPoint.setY(event->y());
                }
        }
}

void Knob::mouseButtonReleaseEvent([[maybe_unused]] RkMouseEvent *event)
{
        isSelected = false;
}

void Knob::mouseMoveEvent(RkMouseEvent *event)
{
        if (isSelected) {
                int dy = event->y() - lastPositionPoint.y();
                rotateKnob(-dy);
                lastPositionPoint.setX(event->x());
                lastPositionPoint.setY(event->y());
                update();
        }
}

void Knob::mouseDoubleClickEvent(RkMouseEvent *event)
{
        setCurrentValue(defaultValue);
        action valueUpdated(defaultValue);
}

void Knob::rotateKnob(double degree)
{
        knobValueDegree += degree;
        if (knobValueDegree < minimumDegree)
                knobValueDegree = minimumDegree;
        else if (knobValueDegree > maximumDegree)
                knobValueDegree = maximumDegree;

        double k = (knobValueDegree - minimumDegree) / rangeDegree;
        if (getRangeType() == RangeType::Logarithmic) {
                double logVal = log10(rangeFrom) + k * (log10(rangeTo) - log10(rangeFrom));
                valueUpdated(pow(10, logVal));
        } else {
                valueUpdated(rangeFrom + k * (rangeTo - rangeFrom));
        }
        update();
}

double Knob::valueToDegree(double value) const
{
        if (std::fabs(rangeTo - rangeFrom) < std::numeric_limits<double>::epsilon())
                return minimumDegree;

        val = std::clamp(value, rangeFrom, rangeTo);
        auto offset = rangeTo - rangeFrom + 1.0;
        auto from = rangeFrom + offset;
        auto to = rangeTo + offset;
        auto val = value + offset;

        double k = 0.0;
        if (getRangeType() == RangeType::Logarithmic)
                k = (log10(val) - log10(calibRangeFrom)) / (log10(calibRangeTo) - log10(calibRangeFrom));
        else
                k = (val - calibRangeFrom) / (calibRangeTo - calibRangeFrom);

        return minimumDegree + k * rangeDegree;
}

double Knob::getValue(void) const
{
        double k = (knobValueDegree - minimumDegree) / rangeDegree;
        double val;
        if (getRangeType() == RangeType::Logarithmic) {
                double logVal = log10(rangeFrom) + k * (log10(rangeTo) - log10(rangeFrom));
                val = pow(10, logVal);
        } else {
                val = rangeFrom + k * (rangeTo - rangeFrom);
        }

	return val;
}

void Knob::setRange(double from, double to)
{
        rangeFrom = std::min(from, to);
        rangeTo = std::max(from, to);
}

void Knob::setRangeType(RangeType type)
{
        rangeType = type;
}

Knob::RangeType Knob::getRangeType() const
{
        return rangeType;
}

void Knob::setDefaultValue(double val)
{
        defaultValue = val;
        setCurrentValue(defaultValue);
}

void Knob::setCurrentValue(double val)
{
        knobValueDegree = valueToDegree(val);
        update();
}
