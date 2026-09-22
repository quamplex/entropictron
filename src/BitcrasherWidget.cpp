#include "BitcrasherWidget.h"
#include "BitcrasherModel.h"
#include "Knob.h"

#include "RkButton.h"
#include "RkContainer.h"
#include "RkLabel.h"
#include "ent_bitcrasher.h"

RK_DECLARE_IMAGE_RC(glitch_label);
RK_DECLARE_IMAGE_RC(glitch_repeats_knob_label);
RK_DECLARE_IMAGE_RC(glitch_probability_knob_label);
RK_DECLARE_IMAGE_RC(glitch_length_knob_label);
RK_DECLARE_IMAGE_RC(glitch_maxjump_knob_label);
RK_DECLARE_IMAGE_RC(glitch_minjump_knob_label);
RK_DECLARE_IMAGE_RC(switch_button_on);
RK_DECLARE_IMAGE_RC(switch_button_off);
RK_DECLARE_IMAGE_RC(knob_big_size_bk);
RK_DECLARE_IMAGE_RC(knob_big_size_marker);
RK_DECLARE_IMAGE_RC(knob_medium_size_bk);
RK_DECLARE_IMAGE_RC(knob_medium_size_marker);

BitcrasherWidget::BitcrasherWidget(EntWidget *parent, BitcrasherModel *model)
        : EntAbstractView(parent, model)
        , enableButton{nullptr}
        , bitsKnob{nullptr}
        , rateKnob{nullptr}
        , chaosKnob{nullptr}
        , foldKnob{nullptr}
        , mixKnob{nullptr}
{
        setFixedSize(350, 302);
        setBackgroundColor(37, 43, 53);
        createView();
        bindModel();
}

void BitcrasherWidget::createView()
{
        auto main = new RkContainer(this, Rk::Orientation::Vertical);
        main->addSpace(8);
        auto header = new RkContainer(this);
        header->setSize({width(), 18});
        header->addSpace(128);

        enableButton = new RkButton(this);
        enableButton->setCheckable();
        enableButton->setSize(36, 16);
        enableButton->setBackgroundColor(background());
        enableButton->setImage(RK_RC_IMAGE(switch_button_off), RkButton::State::Unpressed);
        enableButton->setImage(RK_RC_IMAGE(switch_button_on), RkButton::State::Pressed);
        enableButton->show();
        header->addWidget(enableButton);
        header->addSpace(10);

        auto label = new RkLabel(this);
        label->setBackgroundColor(background());
        label->setImage(RK_RC_IMAGE(glitch_label));
        label->show();
        header->addWidget(label);
        main->addContainer(header);
        main->addSpace(18);

        auto controls = new RkContainer(this);
        controls->setSize({width(), 103});
        controls->addSpace(30);
        bitsKnob = new Knob(this, RK_RC_IMAGE(glitch_repeats_knob_label));
        rateKnob = new Knob(this, RK_RC_IMAGE(glitch_probability_knob_label));
        chaosKnob = new Knob(this, RK_RC_IMAGE(glitch_length_knob_label));
        bitsKnob->setKnobImage(RK_RC_IMAGE(knob_medium_size_bk));
        bitsKnob->setMarkerImage(RK_RC_IMAGE(knob_medium_size_marker));
        rateKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        rateKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        chaosKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        chaosKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        controls->addWidget(bitsKnob);
        controls->addWidget(rateKnob);
        controls->addWidget(chaosKnob);
        main->addContainer(controls);
        main->addSpace(18);

        controls = new RkContainer(this);
        controls->setSize({width(), 103});
        controls->addSpace(82);
        foldKnob = new Knob(this, RK_RC_IMAGE(glitch_maxjump_knob_label));
        mixKnob = new Knob(this, RK_RC_IMAGE(glitch_minjump_knob_label));
        foldKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        foldKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        mixKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        mixKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        controls->addWidget(foldKnob);
        controls->addWidget(mixKnob);
        main->addContainer(controls);
        updateView();
}

void BitcrasherWidget::updateView()
{
        auto model = static_cast<BitcrasherModel *>(getModel());
        if (!model)
                return;
        enableButton->setPressed(model->isEnabled());
        bitsKnob->setRange(ENT_BITCRASHER_MIN_BITS, ENT_BITCRASHER_MAX_BITS);
        bitsKnob->setSteps(ENT_BITCRASHER_MAX_BITS - ENT_BITCRASHER_MIN_BITS);
        bitsKnob->setDefaultValue(ENT_BITCRASHER_DEFAULT_BITS);
        bitsKnob->setValue(model->bits());
        rateKnob->setRange(ENT_BITCRASHER_MIN_RATE, ENT_BITCRASHER_MAX_RATE);
        rateKnob->setDefaultValue(ENT_BITCRASHER_DEFAULT_RATE);
        rateKnob->setValue(model->rate());
        chaosKnob->setRange(ENT_BITCRASHER_MIN_CHAOS, ENT_BITCRASHER_MAX_CHAOS);
        chaosKnob->setDefaultValue(ENT_BITCRASHER_DEFAULT_CHAOS);
        chaosKnob->setValue(model->chaos());
        foldKnob->setRange(ENT_BITCRASHER_MIN_FOLD, ENT_BITCRASHER_MAX_FOLD);
        foldKnob->setDefaultValue(ENT_BITCRASHER_DEFAULT_FOLD);
        foldKnob->setValue(model->fold());
        mixKnob->setRange(ENT_BITCRASHER_MIN_MIX, ENT_BITCRASHER_MAX_MIX);
        mixKnob->setDefaultValue(ENT_BITCRASHER_DEFAULT_MIX);
        mixKnob->setValue(model->mix());
}

void BitcrasherWidget::bindModel()
{
        auto model = static_cast<BitcrasherModel *>(getModel());
        if (!model)
                return;
        RK_ACT_BIND(enableButton, toggled, RK_ACT_ARGS(bool value), model, enable(value));
        RK_ACT_BIND(bitsKnob, valueUpdated, RK_ACT_ARGS(double value), model, setBits(value));
        RK_ACT_BIND(rateKnob, valueUpdated, RK_ACT_ARGS(double value), model, setRate(value));
        RK_ACT_BIND(chaosKnob, valueUpdated, RK_ACT_ARGS(double value), model, setChaos(value));
        RK_ACT_BIND(foldKnob, valueUpdated, RK_ACT_ARGS(double value), model, setFold(value));
        RK_ACT_BIND(mixKnob, valueUpdated, RK_ACT_ARGS(double value), model, setMix(value));
        RK_ACT_BIND(model, modelUpdated, RK_ACT_ARGS(), this, updateView());
        RK_ACT_BIND(model, enabled, RK_ACT_ARGS(bool value), enableButton, setPressed(value));
        RK_ACT_BIND(model, bitsUpdated, RK_ACT_ARGS(int value), bitsKnob, setValue(value));
        RK_ACT_BIND(model, rateUpdated, RK_ACT_ARGS(double value), rateKnob, setValue(value));
        RK_ACT_BIND(model, chaosUpdated, RK_ACT_ARGS(double value), chaosKnob, setValue(value));
        RK_ACT_BIND(model, foldUpdated, RK_ACT_ARGS(double value), foldKnob, setValue(value));
        RK_ACT_BIND(model, mixUpdated, RK_ACT_ARGS(double value), mixKnob, setValue(value));
}

void BitcrasherWidget::unbindModel()
{
        auto model = getModel();
        unbindObject(model);
        enableButton->unbindObject(model);
        bitsKnob->unbindObject(model);
        rateKnob->unbindObject(model);
        chaosKnob->unbindObject(model);
        foldKnob->unbindObject(model);
        mixKnob->unbindObject(model);
}
