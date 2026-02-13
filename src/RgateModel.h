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
        void setMinInterval(int value);
        int minInterval() const;
        void setMinIntervalDefaultValue(int value);
        int getMinIntervalDefaultValue() const;
        void setMinIntervalRange(int from, int to);
        std::pair<int, int> getMinIntervalRange() const;
        void setMaxInterval(int value);
        int maxInterval() const;
        void setMaxIntervalDefaultValue(int value);
        int getMaxIntervalDefaultValue() const;
        void setMaxIntervalRange(int from, int to);
        std::pair<int, int> getMaxIntervalRange() const;

        void setMinDuration(int value);
        int minDuration() const;
        void setMinDurationDefaultValue(int value);
        int getMinDurationDefaultValue() const;
        void setMinDurationRange(int from, int to);
        std::pair<int, int> getMinDurationRange() const;
        void setMaxDuration(int value);
        int maxDuration() const;
        void setMaxDurationDefaultValue(int value);
        int getMaxDurationDefaultValue() const;
        void setMaxDurationRange(int from, int to);
        std::pair<int, int> getMaxDurationRange() const;

        void setMinGain(int value);
        int minGain() const;
        void setMinGainDefaultValue(int value);
        int getMinGainDefaultValue() const;
        void setMinGainRange(int from, int to);
        std::pair<int, int> getMinGainRange() const;
        void setMaxGain(int value);
        int maxGain() const;
        void setMaxGainDefaultValue(int value);
        int getMaxGainDefaultValue() const;
        void setMaxGainRange(int from, int to);
        std::pair<int, int> getMaxGainRange() const;

        void setRandomness(int value);
        int randomness() const;
        void setRandomnessDefaultValue(int value);
        int getRandomnessDefaultValue() const;
        void setRandomnessRange(int from, int to);
        std::pair<int, int> getRandomnessRange() const;

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
        RK_DECL_ACT(maxRandomnessUpdated,
                    maxRandomnessUpdated(double value),
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
        double maxRandomnessDefaultValue;
        std::pair<double, double> maxRandomnessRange;
        double maxInvertedDefaultValue;
        std::pair<double, double> maxInvertedRange;
};

#endif // ENT_RGATE_MODEL_H
