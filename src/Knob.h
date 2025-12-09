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

#ifndef ENT_KNOB_WIDGET_H
#define ENT_KNOB_WIDGET_H

#include "EntWidget.h"

#include "RkImage.h"

class Knob : public EntWidget
{
public:
        enum RangeType: int {
                Linear,
                Logarithmic
        };

        explicit Knob(EntWidget* parent, const RkImage &label);
        virtual ~Knob() = default;
        void setLabelImage(const RkImage &img);
        void setKnobImage(const RkImage &img);
        void setMarkerImage(const RkImage &img);
        void setRange(double from, double to);
        void setRangeType(RangeType type);
        RangeType getRangeType() const;
        void setValue(double val);
        double getValue(void) const;
        void setDefaultValue(double val);
        void setSteps(int steps);
        int getSteps() const;

        RK_DECL_ACT(valueUpdated,
                    valueUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));

protected:
   void paintEvent(RkPaintEvent *event) override;
   void mouseButtonPressEvent(RkMouseEvent *event) override;
   void mouseButtonReleaseEvent(RkMouseEvent *event) override;
   void mouseMoveEvent(RkMouseEvent *event) override;
   void mouseDoubleClickEvent(RkMouseEvent *event) override;
   void rotateKnob(double degree);
   double valueToDegree(double val) const;
   double degreeToValue(double degree) const;
   double snapToNearestStep(double val) const;
   double stepValue() const;

private:
        RkImage labelImage;
        RkImage knobImage;
        RkImage markerImage;
        int maxSteps;
        double rangeFrom;
        double rangeTo;
        RangeType rangeType;
        double minimumDegree;
        double maximumDegree;
        double rangeDegree;
        double knobValueDegree;
        bool isSelected;
        double defaultValue;
        double knobValue;
        RkPoint lastPositionPoint;
};

#endif // ENT_KNOB_H

