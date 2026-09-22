#ifndef DSP_BITCRASHER_PROXY_VST_H
#define DSP_BITCRASHER_PROXY_VST_H

#include "DspBitcrasherProxy.h"
#include "EntVstParameters.h"

class EntVstController;

class DspBitcrasherProxyVst : public DspBitcrasherProxy
{
public:
        explicit DspBitcrasherProxyVst(RkObject *parent,
                                       EntVstController *controller);
        ~DspBitcrasherProxyVst() override;
        bool enable(bool value = true) override;
        bool isEnabled() const override;
        bool setBits(int value) override;
        int bits() const override;
        bool setRate(double value) override;
        double rate() const override;
        bool setChaos(double value) override;
        double chaos() const override;
        bool setFold(double value) override;
        double fold() const override;
        bool setMix(double value) override;
        double mix() const override;

protected:
        void onParameterChanged(EntVst::ParameterId id,
                                Steinberg::Vst::ParamValue value);

private:
        EntVstController *vstController;
};

#endif // DSP_BITCRASHER_PROXY_VST_H
