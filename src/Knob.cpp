/**
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

#include "RkEvent.h"
#include "RkPainter.h"

Knob::Knob(EntWidget* parent, const RkImage &label)
        : EntWidget(parent)
        , labelImage{label}
        , rangeFrom{0}
        , rangeTo {0}
        , rangeType{RangeType::Linear}
        , minimumDegree {45}
        , maximumDegree {360 - 45}
        , rangeDegree{maximumDegree - minimumDegree}
        , knobValueDegree {minimumDegree}
        , isSelected {false}
        , defaultValue {0}
{
}

void Knob::setLabelImage(const RkImage &img)
{
        labelImage = img;
        setSize(knobImage.width(),  knobImage.height() + labelImage.height() + 5);
}

void Knob::setKnobImage(const RkImage &img)
{
        knobImage = img;
        setSize(knobImage.width(),  knobImage.height() + labelImage.height() + 5);
}

void Knob::setMarkerImage(const RkImage &img)
{
        markerImage = img;
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

void Knob::setValue(double val)
{
        knobValueDegree = valueToDegree(val);
        update();
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

void Knob::setDefaultValue(double val)
{
        defaultValue = val;
        setValue(defaultValue);
}

void Knob::paintEvent(RkPaintEvent *event)
{
        EntWidget::paintEvent(event);
        RkPainter painter(this);

        // Draw knob label
        int yOffset = 0;
        if (!labelImage.isNull()) {
                painter.drawImage(labelImage, {(width() - labelImage.width()) / 2, 0});
                yOffset += labelImage.height();
        }

        // Draw kbob background static image
        if (!knobImage.isNull()) {
                painter.drawImage(knobImage, {0, yOffset});
                yOffset += knobImage.height() / 2;
        }

        // Draw knob marker
        if (!markerImage.isNull()) {
                auto translateOffset = RkPoint(width() / 2, yOffset);
                painter.translate(translateOffset);
                auto degree = (2 * M_PI / 360) * knobValueDegree;
                painter.rotate(degree);
                painter.drawImage(markerImage,
                                  -markerImage.width() / 2,
                                  -markerImage.height() / 2);
                painter.rotate(-degree);
                painter.translate(RkPoint() - translateOffset);
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

        if (!knobImage.isNull()) {
                int xCenter = width() / 2;
                int yCenter = height() / 2;
                int r = knobImage.width() / 2;
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
        setValue(defaultValue);
        action valueUpdated(defaultValue);
}

void Knob::rotateKnob(double degree)
{
        knobValueDegree += degree;
        knobValueDegree = std::clamp(knobValueDegree, minimumDegree, maximumDegree);

        double k = (knobValueDegree - minimumDegree) / rangeDegree;
        if (getRangeType() == RangeType::Logarithmic) {
                if (rangeFrom <= 0.0) {
                        constexpr double curve = 4.0;
                        valueUpdated(std::pow(k, curve) * (rangeTo - rangeFrom) + rangeFrom);
                } else {
                        double logVal = std::log10(rangeFrom) + k * (std::log10(rangeTo) - std::log10(rangeFrom));
                        valueUpdated(std::pow(10.0, logVal));
                }
        } else {
                valueUpdated(rangeFrom + k * (rangeTo - rangeFrom));
        }

        update();
}

double Knob::valueToDegree(double value) const
{
        if (std::fabs(rangeTo - rangeFrom) < std::numeric_limits<double>::epsilon())
                return minimumDegree;

        value = std::clamp(value, rangeFrom, rangeTo);

        double k = 0.0;
        if (getRangeType() == RangeType::Logarithmic) {
                if (rangeFrom <= 0.0) {
                        // pseudo-log (power) mapping for [0, 1]
                        constexpr double curve = 4.0; // same curve as in rotateKnob()
                        double norm = (value - rangeFrom) / (rangeTo - rangeFrom);
                        k = std::pow(norm, 1.0 / curve);
                } else {
                        // true logarithmic mapping
                        double logFrom = std::log10(rangeFrom);
                        double logTo   = std::log10(rangeTo);
                        double logVal  = std::log10(value);
                        k = (logVal - logFrom) / (logTo - logFrom);
                }
        } else {
                // linear mapping
                k = (value - rangeFrom) / (rangeTo - rangeFrom);
        }

        return minimumDegree + k * rangeDegree;
}


