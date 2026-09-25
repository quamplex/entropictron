/**
 * File name: BitcrasherWidget.h
 * Project: Entropictron (A texture synthesizer)
 */

#ifndef ENT_BITCRASHER_WIDGET_H
#define ENT_BITCRASHER_WIDGET_H

#include "EntAbstractView.h"

class BitcrasherModel;
class Knob;
class RkButton;
class RkContainer;

class BitcrasherWidget : public EntAbstractView
{
public:
        explicit BitcrasherWidget(EntWidget *parent, BitcrasherModel *model);
        ~BitcrasherWidget() override = default;
        void createView() override;
        void updateView() override;

protected:
        void bindModel() override;
        void unbindModel() override;

private:
        RkButton *enableButton;
        Knob *bitsKnob;
        Knob *rateKnob;
        Knob *chaosKnob;
        Knob *gainKnob;
        Knob *mixKnob;
};

#endif // ENT_BITCRASHER_WIDGET_H
