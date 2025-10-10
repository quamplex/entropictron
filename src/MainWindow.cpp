/**
 * File name: MainWindow.cpp
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

#include "MainWindow.h"
#include "NoiseWidget.h"
#include "DspProxy.h"
#include "EntropictronModel.h"

#include "RkContainer.h"
#include "RkLabel.h"
#include "RkTabWidget.h"

constexpr int MAIN_WINDOW_WIDTH  = 900;
constexpr int MAIN_WINDOW_HEIGHT = 352;

MainWindow::MainWindow(RkMain& app,
                       const RkNativeWindowInfo &info,
                       DspProxy *dspProxy)
        : EntWidget(app, info)
        , entropictronModel{new EntropictronModel(this, dspProxy)}
{
        setTitle(Entropictron::applicationName);
        setName("MainWindow");
        setFixedSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
        setBackgroundColor({20, 20, 20});
        createUi();
}

MainWindow::~MainWindow()
{
}

bool MainWindow::createUi(void)
{
        // Main container
        auto mainContainer = new RkContainer(this, Rk::Orientation::Vertical);
        mainContainer->setSize(size() - RkSize(10, 10));
        mainContainer->setPosition({5, 5});

        // Main horizontal container
        auto horizontalContainer = new RkContainer(this);
        horizontalContainer->setSize({mainContainer->width(), 282});
        mainContainer->addContainer(horizontalContainer);

        auto moduleTabs = new RkTabWidget(this);
        horizontalContainer->addWidget(moduleTabs);

        // Noise 1 controls widget
        auto noiseWidget = new NoiseWidget(moduleTabs, entropictronModel->getNoise1());
        moduleTabs->addTab(noiseWidget, RK_RC_IMAGE(noise1_tab));

        // Crackle 1
        auto crackleWidget = new CrackleWidget(moduleTabs, entropictronModel->getCrackle1());
        moduleTabs->addTab(crackleWidget, RK_RC_IMAGE(crackle1_tab));

        // Glitch 1
        auto glitchWidget = new GlitchWidget(moduleTabs, entropictronModel->getGlitch1());
        moduleTabs->addTab(glitchWidget, RK_RC_IMAGE(glitch1_tab));

        moduleTabs = new RkTabWidget(this);
        horizontalContainer->addWidget(moduleTabs);

        // Noise 2 controls widget
        noiseWidget = new NoiseWidget(moduleTabs, entropictronModel->getNoise2());
        moduleTabs->addTab(noiseWidget, RK_RC_IMAGE(noise2_tab));

        // Crackle 2
        crackleWidget = new CrackleWidget(moduleTabs, entropictronModel->getCrackle2());
        moduleTabs->addTab(crackleWidget, RK_RC_IMAGE(crackle2_tab));

        // Glitch 2
        glitchWidget = new GlitchWidget(moduleTabs, entropictronModel->getGlitch2());
        moduleTabs->addTab(glitchWidget, RK_RC_IMAGE(glitch2_tab));

        // Global controls widget
        horizontalContainer->addSpace(5);
        auto globalControlsWidget = new EntWidget(this);
        globalControlsWidget->setBackgroundColor(37, 43, 53);
        globalControlsWidget->setSize({252, horizontalContainer->height()});
        horizontalContainer->addWidget(globalControlsWidget);

        return true;
}

RkSize MainWindow::getWindowSize()
{
        return {MAIN_WINDOW_WIDTH,
                MAIN_WINDOW_HEIGHT};
}
