/**
 * File name: CrackleModel.h
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

#ifndef ENT_CRACKLE_MODEL_H
#define ENT_CRACKLE_MODEL_H

#include "EntAbstractModel.h"
#include "GuiTypes.h"

class DspCrackleProxy;

class CrackleModel: public EntAbstractModel
{
 public:
        explicit CrackleModel(RkObject *parent, DspCrackleProxy *dspCrackleProxy);
        CrackleId getId() const;
        void enable(bool b = true);
        bool isEnabled() const;
        void setRate(double value);
        double rate() const;
        void setRateDefaultValue(double value);
        double getRateDefaultValue() const;
        void setRateRange(double from, double to);
        std::pair<double, double> getRateRange() const;
        void setRandomness(double value);
        double randomness() const;
        void setRandomnessDefaultValue(double value);
        double getRandomnessDefaultValue() const;
        void setRandomnessRange(double from, double to);
        std::pair<double, double> getRandomnessRange() const;
        void setAmplitude(double value);
        double amplitude() const;
        void setAmplitudeDefaultValue(double value);
        double getAmplitudeDefaultValue() const;
        void setAmplitudeRange(double from, double to);
        std::pair<double, double> getAmplitudeRange() const;
        void setEnvelopeShape(CrackleEnvelopeShape shape);
        CrackleEnvelopeShape envelopeShape() const;
        void setBrightness(double value);
        double brightness() const;
        void setBrightnessDefaultValue(double value);
        double getBrightnessDefaultValue() const;
        void setBrightnessRange(double from, double to);
        std::pair<double, double> getBrightnessRange() const;
        void setDuration(double value);
        double duration() const;
        void setDurationDefaultValue(double value);
        double getDurationDefaultValue() const;
        void setDurationRange(double from, double to);
        std::pair<double, double> getDurationRange() const;
        void setStereospread(double value);
        double stereospread() const;
        void setStereospreadDefaultValue(double value);
        double getStereospreadDefaultValue() const;
        void setStereospreadRange(double from, double to);
        std::pair<double, double> getStereospreadRange() const;

        RK_DECL_ACT(enabled,
                    enabled(bool b),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(rateUpdated,
                    rateUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(randomnessUpdated,
                    randomnessUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(amplitudeUpdated,
                    amplitudeUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(envelopeShapeUpdated,
                    envelopeShapeUpdated(CrackleEnvelopeShape shape),
                    RK_ARG_TYPE(CrackleEnvelopeShape),
                    RK_ARG_VAL(shape));
        RK_DECL_ACT(brightnessUpdated,
                    brightnessUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(durationUpdated,
                    durationUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(stereospreadUpdated,
                    stereospreadUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));

 private:
        DspCrackleProxy *dspCrackleProxy;
        double rateDefaultvalue;
        std::pair<double, double> rateRange;
        double randomnessDefaultvalue;
        std::pair<double, double> randomnessRange;
        double amplitudeDefaultvalue;
        std::pair<double, double> amplitudeRange;
        double brightnessDefaultvalue;
        std::pair<double, double> brightnessRange;
        double durationDefaultvalue;
        std::pair<double, double> durationRange;
        double stereospreadDefaultvalue;
        std::pair<double, double> stereospreadRange;
};

#endif // ENT_CRACKLE_MODEL_H
