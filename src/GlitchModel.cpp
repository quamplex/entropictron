/**
 * File name: GlitchModel.cpp
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

#include "GlitchModel.h"
#include "DspGlitchProxy.h"

GlitchModel::GlitchModel(RkObject *parent, DspGlitchProxy *dspGlitchProxy)
        : EntAbstractModel(parent)
        , dspGlitchProxy {dspGlitchProxy}
{
        RK_ACT_BIND(dspGlitchProxy,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    this, enabled(b));
}

GlitchId GlitchModel::getId() const
{
        return dspGlitchProxy->getGlitchId();
}

void GlitchModel::enable(bool b)
{
        if (dspGlitchProxy->enable(b))
                action enabled(b);
}

bool GlitchModel::isEnabled() const
{
        return dspGlitchProxy->isEnabled();
}

void GlitchModel::setRepeats(double value)
{
        if (dspGlitchProxy->setRepeats(value))
                action repeatsUpdated(value);
}

double GlitchModel::repeats() const
{
        return dspGlitchProxy->repeats();
}

void GlitchModel::setProbability(double value)
{
        if (dspGlitchProxy->setProbability(value))
                action probabilityUpdated(value);
}

double GlitchModel::probability() const
{
        return dspGlitchProxy->probability();
}

void GlitchModel::setLength(double value)
{
        if (dspGlitchProxy->setLength(value))
                action lengthUpdated(value);
}

double GlitchModel::length() const
{
        return dspGlitchProxy->length();
}

void GlitchModel::setMaxJump(double value)
{
        if (dspGlitchProxy->setMaxJump(value))
                action maxJumpUpdated(value);
}

double GlitchModel::maxJump() const
{
        return dspGlitchProxy->maxJump();
}

void GlitchModel::setMinJump(double value)
{
        if (dspGlitchProxy->setMinJump(value))
                action minJumpUpdated(value);
}

double GlitchModel::minJump() const
{
        return dspGlitchProxy->minJump();
}

