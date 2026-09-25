#include "BitcrasherWidget.h"
#include "BitcrasherModel.h"
#include "Knob.h"

#include "RkButton.h"
#include "RkContainer.h"
#include "RkLabel.h"

RK_DECLARE_IMAGE_RC(bitcrasher_label);
RK_DECLARE_IMAGE_RC(bitcrasher_bits_knob_label);
RK_DECLARE_IMAGE_RC(bitcrasher_rate_knob_label);
RK_DECLARE_IMAGE_RC(bitcrasher_chaos_knob_label);
RK_DECLARE_IMAGE_RC(bitcrasher_gain_knob_label);
RK_DECLARE_IMAGE_RC(bitcrasher_mix_knob_label);
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
        , gainKnob{nullptr}
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
        header->addSpace(115);

        enableButton = new RkButton(this);
        enableButton->setCheckable();
        enableButton->setSize(36, 16);
        enableButton->setBackgroundColor(background());
        enableButton->setImage(RK_RC_IMAGE(switch_button_off),
                               RkButton::State::Unpressed);
        enableButton->setImage(RK_RC_IMAGE(switch_button_on),
                               RkButton::State::Pressed);
        enableButton->show();
        header->addWidget(enableButton);
        header->addSpace(10);

        auto label = new RkLabel(this);
        label->setBackgroundColor(background());
        label->setImage(RK_RC_IMAGE(bitcrasher_label));
        label->show();
        header->addWidget(label);
        main->addContainer(header);
        main->addSpace(18);

        auto controls = new RkContainer(this);
        controls->setSize({width(), 103});
        controls->addSpace(30);
        bitsKnob = new Knob(this, RK_RC_IMAGE(bitcrasher_bits_knob_label));
        rateKnob = new Knob(this, RK_RC_IMAGE(bitcrasher_rate_knob_label));
        chaosKnob = new Knob(this, RK_RC_IMAGE(bitcrasher_chaos_knob_label));
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
        controls->addSpace(30);
        gainKnob = new Knob(this, RK_RC_IMAGE(bitcrasher_gain_knob_label));
        mixKnob = new Knob(this, RK_RC_IMAGE(bitcrasher_mix_knob_label));
        gainKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        gainKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        mixKnob->setKnobImage(RK_RC_IMAGE(knob_big_size_bk));
        mixKnob->setMarkerImage(RK_RC_IMAGE(knob_big_size_marker));
        controls->addWidget(gainKnob);
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
        auto [bitsFrom, bitsTo] = model->getBitsRange();
        bitsKnob->setRange(bitsFrom, bitsTo);
        bitsKnob->setSteps(bitsTo - bitsFrom + 1);
        bitsKnob->setDefaultValue(model->getBitsDefaultValue());
        bitsKnob->setValue(model->bits());
        auto [rateFrom, rateTo] = model->getRateRange();
        rateKnob->setRange(rateFrom, rateTo);
        rateKnob->setRangeType(Knob::RangeType::Logarithmic);
        rateKnob->setDefaultValue(model->getRateDefaultValue());
        rateKnob->setValue(model->rate());
        auto [chaosFrom, chaosTo] = model->getChaosRange();
        chaosKnob->setRange(chaosFrom, chaosTo);
        chaosKnob->setDefaultValue(model->getChaosDefaultValue());
        chaosKnob->setValue(model->chaos());
        auto [gainFrom, gainTo] = model->getGainRange();
        gainKnob->setRange(gainFrom, gainTo);
        gainKnob->setRangeType(Knob::RangeType::Logarithmic);
        gainKnob->setDefaultValue(model->getGainDefaultValue());
        gainKnob->setValue(model->gain());
        auto [mixFrom, mixTo] = model->getMixRange();
        mixKnob->setRange(mixFrom, mixTo);
        mixKnob->setDefaultValue(model->getMixDefaultValue());
        mixKnob->setValue(model->mix());
}

void BitcrasherWidget::bindModel()
{
        auto model = static_cast<BitcrasherModel *>(getModel());
        if (!model)
                return;

        RK_ACT_BIND(enableButton,
                    toggled,
                    RK_ACT_ARGS(bool value),
                    model,
                    enable(value));
        RK_ACT_BIND(bitsKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setBits(value));
        RK_ACT_BIND(rateKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setRate(value));
        RK_ACT_BIND(chaosKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setChaos(value));
        RK_ACT_BIND(gainKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setGain(value));
        RK_ACT_BIND(mixKnob,
                    valueUpdated,
                    RK_ACT_ARGS(double value),
                    model,
                    setMix(value));
        RK_ACT_BIND(model,
                    modelUpdated,
                    RK_ACT_ARGS(),
                    this,
                    updateView());
        RK_ACT_BIND(model,
                    enabled,
                    RK_ACT_ARGS(bool value),
                    enableButton,
                    setPressed(value));
        RK_ACT_BIND(model,
                    bitsUpdated,
                    RK_ACT_ARGS(int value),
                    bitsKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    rateUpdated,
                    RK_ACT_ARGS(double value),
                    rateKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    chaosUpdated,
                    RK_ACT_ARGS(double value),
                    chaosKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    gainUpdated,
                    RK_ACT_ARGS(double value),
                    gainKnob,
                    setValue(value));
        RK_ACT_BIND(model,
                    mixUpdated,
                    RK_ACT_ARGS(double value),
                    mixKnob,
                    setValue(value));
}

void BitcrasherWidget::unbindModel()
{
        auto model = getModel();
        unbindObject(model);
        enableButton->unbindObject(model);
        bitsKnob->unbindObject(model);
        rateKnob->unbindObject(model);
        chaosKnob->unbindObject(model);
        gainKnob->unbindObject(model);
        mixKnob->unbindObject(model);
}
