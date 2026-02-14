/**
 * File name: RgateModel.h
 * Project: Entropictron (A texture synthesizer)
 *
 * Copyright (C) 2026 Iurie Nistor
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

#ifndef ENT_RGATE_MODEL_H
#define ENT_RGATE_MODEL_H

#include "EntAbstractModel.h"
#include "GuiTypes.h"

class DspRgateProxy;

class RgateModel: public EntAbstractModel
{
 public:
        explicit RgateModel(RkObject *parent, DspRgateProxy *dspRgateProxy);
        void enable(bool b = true);
        bool isEnabled() const;

        void setMinInterval(double value);
        double minInterval() const;
        void setMinIntervalDefaultValue(double value);
        double getMinIntervalDefaultValue() const;
        void setMinIntervalRange(double from, double to);
        std::pair<double, double> getMinIntervalRange() const;

        void setMaxInterval(double value);
        double maxInterval() const;
        void setMaxIntervalDefaultValue(double value);
        double getMaxIntervalDefaultValue() const;
        void setMaxIntervalRange(double from, double to);
        std::pair<double, double> getMaxIntervalRange() const;

        void setMinDuration(double value);
        double minDuration() const;
        void setMinDurationDefaultValue(double value);
        double getMinDurationDefaultValue() const;
        void setMinDurationRange(double from, double to);
        std::pair<double, double> getMinDurationRange() const;

        void setMaxDuration(double value);
        double maxDuration() const;
        void setMaxDurationDefaultValue(double value);
        double getMaxDurationDefaultValue() const;
        void setMaxDurationRange(double from, double to);
        std::pair<double, double> getMaxDurationRange() const;

        void setMinGain(double value);
        double minGain() const;
        void setMinGainDefaultValue(double value);
        double getMinGainDefaultValue() const;
        void setMinGainRange(double from, double to);
        std::pair<double, double> getMinGainRange() const;

        void setMaxGain(double value);
        double maxGain() const;
        void setMaxGainDefaultValue(double value);
        double getMaxGainDefaultValue() const;
        void setMaxGainRange(double from, double to);
        std::pair<double, double> getMaxGainRange() const;

        void setRandomness(double value);
        double randomness() const;
        void setRandomnessDefaultValue(double value);
        double getRandomnessDefaultValue() const;
        void setRandomnessRange(double from, double to);
        std::pair<double, double> getRandomnessRange() const;

        void setInverted(bool value);
        bool isInverted() const;

        RK_DECL_ACT(enabled,
                    enabled(bool b),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(minIntervalUpdated,
                    minIntervalUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(maxIntervalUpdated,
                    maxIntervalUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(minDurationUpdated,
                    minDurationUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(maxDurationUpdated,
                    maxDurationUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(minGainUpdated,
                    minGainUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(maxGainUpdated,
                    maxGainUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(randomnessUpdated,
                    randomnessUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(invertedUpdated,
                    invertedUpdated(bool value),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(value));

 private:
        DspRgateProxy *dspRgateProxy;
        double repeatsDefaultValue;
        double minIntervalDefaultValue;
        std::pair<double, double> minIntervalRange;
        double maxIntervalDefaultValue;
        std::pair<double, double> maxIntervalRange;
        double minDurationDefaultValue;
        std::pair<double, double> minDurationRange;
        double maxDurationDefaultValue;
        std::pair<double, double> maxDurationRange;
        double minGainDefaultValue;
        std::pair<double, double> minGainRange;
        double maxGainDefaultValue;
        std::pair<double, double> maxGainRange;
        double randomnessDefaultValue;
        std::pair<double, double> randomnessRange;
        bool invertedDefaultValue;
};

#endif // ENT_RGATE_MODEL_H
