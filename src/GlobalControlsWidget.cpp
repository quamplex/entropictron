/**
 * File name: GlobalControlsWidget.h
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

#include "GlobalControlsWidget.h"
#include "EntropictronModel.h"
#include "PresetWidget.h"

#include "RkContainer.h"
#include "RkButton.h"

RK_DECLARE_IMAGE_RC(play_mode_playback);
RK_DECLARE_IMAGE_RC(play_mode_playback_hover);
RK_DECLARE_IMAGE_RC(play_mode_playback_on);
RK_DECLARE_IMAGE_RC(play_mode_playback_hover_on);
RK_DECLARE_IMAGE_RC(play_mode_hold);
RK_DECLARE_IMAGE_RC(play_mode_hold_hover);
RK_DECLARE_IMAGE_RC(play_mode_hold_on);
RK_DECLARE_IMAGE_RC(play_mode_hold_hover_on);
RK_DECLARE_IMAGE_RC(play_mode_on);
RK_DECLARE_IMAGE_RC(play_mode_on_hover);
RK_DECLARE_IMAGE_RC(play_mode_on_on);
RK_DECLARE_IMAGE_RC(play_mode_on_hover_on);

GlobalControlsWidget::GlobalControlsWidget(EntWidget* parent, EntropictronModel* model)
        : EntWidget(parent)
        , entModel{model}
        , playbackModeButton{nullptr}
        , holdModeButton{nullptr}
        , onModeButton{nullptr}
{
        setBackgroundColor(37, 43, 53);
        setFixedSize(250, 331);

        auto mainContainer = new RkContainer(this, Rk::Orientation::Vertical);
        mainContainer->addSpace(5);
        createPlayModeControls(mainContainer);
        mainContainer->addSpace(5);
        mainContainer->addWidget(new PresetWidget(this, entModel));
}

void GlobalControlsWidget::createPlayModeControls(RkContainer *container)
{
        auto modeButtonsContainer = new RkContainer(this);
        modeButtonsContainer->setSize(width(), 28);
        container->addContainer(modeButtonsContainer);
        modeButtonsContainer->addSpace(20);

        // Playback mode
        playbackModeButton = new RkButton(this);
        playbackModeButton->setImage(RK_RC_IMAGE(play_mode_playback),
                            RkButton::State::Unpressed);
        playbackModeButton->setImage(RK_RC_IMAGE(play_mode_playback_on),
                            RkButton::State::Pressed);
        playbackModeButton->setImage(RK_RC_IMAGE(play_mode_playback_hover),
                            RkButton::State::UnpressedHover);
        playbackModeButton->setImage(RK_RC_IMAGE(play_mode_playback_hover_on),
                            RkButton::State::PressedHover);
        playbackModeButton->setCheckable(true);
        playbackModeButton->show();
        modeButtonsContainer->addWidget(playbackModeButton);
        RK_ACT_BIND(playbackModeButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    entModel,
                    setPlayMode(PlayMode::PlaybackMode));

        // Hold mode
        holdModeButton = new RkButton(this);
        holdModeButton->setImage(RK_RC_IMAGE(play_mode_hold),
                            RkButton::State::Unpressed);
        holdModeButton->setImage(RK_RC_IMAGE(play_mode_hold_on),
                            RkButton::State::Pressed);
        holdModeButton->setImage(RK_RC_IMAGE(play_mode_hold_hover),
                            RkButton::State::UnpressedHover);
        holdModeButton->setImage(RK_RC_IMAGE(play_mode_hold_hover_on),
                            RkButton::State::PressedHover);
        holdModeButton->setCheckable(true);
        holdModeButton->show();
        modeButtonsContainer->addWidget(holdModeButton);
        RK_ACT_BIND(holdModeButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    entModel,
                    setPlayMode(PlayMode::HoldMode));

        // On mode
        onModeButton = new RkButton(this);
        onModeButton->setImage(RK_RC_IMAGE(play_mode_on),
                            RkButton::State::Unpressed);
        onModeButton->setImage(RK_RC_IMAGE(play_mode_on_on),
                            RkButton::State::Pressed);
        onModeButton->setImage(RK_RC_IMAGE(play_mode_on_hover),
                            RkButton::State::UnpressedHover);
        onModeButton->setImage(RK_RC_IMAGE(play_mode_on_hover_on),
                            RkButton::State::PressedHover);
        onModeButton->setCheckable(true);
        onModeButton->show();
        modeButtonsContainer->addWidget(onModeButton);
        RK_ACT_BIND(onModeButton,
                    toggled,
                    RK_ACT_ARGS(bool b),
                    entModel,
                    setPlayMode(PlayMode::OnMode));

        RK_ACT_BIND(entModel,
                    playModeUpdated,
                    RK_ACT_ARGS(PlayMode mode),
                    this,
                    setPlayMode(mode));
        RK_ACT_BIND(entModel,
                    modelUpdated,
                    RK_ACT_ARGS(),
                    this,
                    setPlayMode(entModel->playMode()));

        setPlayMode(entModel->playMode());
}

void GlobalControlsWidget::setPlayMode(PlayMode mode)
{
        playbackModeButton->setPressed(mode == PlayMode::PlaybackMode);
        holdModeButton->setPressed(mode == PlayMode::HoldMode);
        onModeButton->setPressed(mode == PlayMode::OnMode);
}
