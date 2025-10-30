/**
 * File name: EntAbstractView.h
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

#ifndef ENT_ABSTRACT_VIEW_H
#define ENT_ABSTRACT_VIEW_H

#include "EntWidget.h"

class EntAbstractModel;

class EntAbstractView: public EntWidget {
 public:
        explicit EntAbstractView(EntWidget* parent, EntAbstractModel *model);
        void setModel(EntAbstractModel *model);
        EntAbstractModel* getModel() const;
        virtual void createView() = 0;
        virtual void updateView() = 0;

 protected:
        virtual void bindModel() = 0;
        virtual void unbindModel() = 0;

 private:
        EntAbstractModel* viewModel;
};

#endif // ENT_ABSTRACT_VIEW_H
