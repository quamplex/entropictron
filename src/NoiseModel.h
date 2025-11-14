/**
 * File name: NoiseModel.h
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

#ifndef ENT_NOISE_MODEL_H
#define ENT_NOISE_MODEL_H

#include "EntAbstractModel.h"
#include "GuiTypes.h"

class DspNoiseProxy;

class NoiseModel: public EntAbstractModel
{
 public:
        explicit NoiseModel(RkObject *parent, DspNoiseProxy *dspNoiseProxy);
        NoiseId getId() const;
        void enable(bool b = true);
        bool isEnabled() const;
        void setType(NoiseType type);
        NoiseType noiseType() const;
        void setDensity(double value);
        double density() const;
        void setDensityDefaultValue(double value);
        double getDensityDefaultValue() const;
        void setDensityRange(double from, double to);
        std::pair<double, double> getDensityRange() const;
        void setBrightness(double value);
        double brightness() const;
        void setBrightnessDefaultValue(double value);
        double getBrightnessDefaultValue() const;
        void setBrightnessRange(double from, double to);
        std::pair<double, double> getBrightnessRange() const;
        void setGain(double value);
        double gain() const;
        void setGainDefaultValue(double value);
        double getGainDefaultValue() const;
        void setGainRange(double from, double to);
        std::pair<double, double> getGainRange() const;
        void setStereo(double value);
        double stereo() const;
        void setStereoDefaultValue(double value);
        double getStereoDefaultValue() const;
        void setStereoRange(double from, double to);
        std::pair<double, double> getStereoRange() const;
        void setCutOff(double value);
        double cutOff() const;
        void setCutOffDefaultValue(double value);
        double getCutOffDefaultValue() const;
        void setCutOffRange(double from, double to);
        std::pair<double, double> getCutOffRange() const;
        void setResonance(double value);
        double resonance() const;
        void setResonanceDefaultValue(double value);
        double getResonanceDefaultValue() const;
        void setResonanceRange(double from, double to);
        std::pair<double, double> getResonanceRange() const;

        RK_DECL_ACT(enabled,
                    enabled(bool b),
                    RK_ARG_TYPE(bool b),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(typeUpdated,
                    typeUpdated(NoiseType type),
                    RK_ARG_TYPE(NoiseType),
                    RK_ARG_VAL(type));
        RK_DECL_ACT(densityUpdated,
                    densityUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(brightnessUpdated,
                    brightnessUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(gainUpdated,
                    gainUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(stereoUpdated,
                    stereoUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(filterTypeUpdated,
                    filterTypeUpdated(FilterType type),
                    RK_ARG_TYPE(FilterType),
                    RK_ARG_VAL(type));
        RK_DECL_ACT(cutOffUpdated,
                    cutOffUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(resonanceUpdated,
                    resonanceUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));

 private:
        DspNoiseProxy *dspNoiseProxy;
        double desnityDefaultvalue;
        double brightnessDefaultvalue;
        double gainDefaultvalue;
        double stereoDefaultvalue;
        double cutOffDefaultvalue;
        double resonanceDefaultvalue;
        std::pair<double, double> densityRange;
        std::pair<double, double> brightnessRange;
        std::pair<double, double> gainRange;
        std::pair<double, double> stereoRange;
        std::pair<double, double> cutOffRange;
        std::pair<double, double> resonanceRange;
};

#endif // ENT_NOISE_MODEL_H
