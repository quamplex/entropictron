
#include "ent_chaos.h"
#include <math.h>

void ent_chaos_init(struct ent_chaos *c, float sr)
{
        c->sample_rate = sr;
        c->phase = 0.0f;
        c->frequency = 220.0f;

        c->x = 0.5f;
        c->y = 0.3f;

        c->type = 0;
}

void ent_chaos_process(struct ent_chaos *c,
                       float **data,
                       size_t size)
{
        float phase = c->phase;
        float inc = 2.0f * M_PI * c->frequency / c->sample_rate;

        float x = c->x;
        float y = c->y;

        for (size_t i = 0; i < size; ++i) {

                float s = 0.0f;

                switch (c->type) {

                        // -------------------------
                        // 0: logistic chaos
                case 0:
                        x = 3.9f * x * (1.0f - x);
                        s = 2.0f * x - 1.0f;
                        break;

                        // -------------------------
                        // 1: chaotic sine feedback
                case 1:
                        x = sinf(phase + 5.0f * x);
                        s = x;
                        break;

                        // -------------------------
                        // 2: feedback FM chaos
                case 2:
                        x = sinf(phase + x * 4.0f);
                        s = sinf(phase + x * 2.0f);
                        break;

                        // -------------------------
                        // 3: broken nonlinear system
                case 3:
                        {
                                float t = sinf(phase);
                                x = t + 0.8f * x * x - 0.6f * x;
                                x = tanhf(x);
                                s = x;
                                break;
                        }

                        // -------------------------
                        // 4: lorenz-like
                case 4:
                        {
                                float dx = 10.0f * (y - x);
                                float dy = x * (28.0f - y) - y;

                                x += 0.001f * dx;
                                y += 0.001f * dy;

                                s = tanhf(x * 0.1f);
                                break;
                        }

                        // -------------------------
                        // 5: phase-chaos hybrid
                case 5:
                default:
                        {
                                float a = sinf(phase + x * 3.0f);
                                float b = cosf(phase * 0.7f + a);
                                x = a * b;
                                s = x;
                                break;
                        }

                }

                data[0][i] = s;
                data[1][i] = s;

                phase += inc;
                if (phase >= 2.0f * M_PI)
                        phase -= 2.0f * M_PI;
        }

        c->phase = phase;
        c->x = x;
        c->y = y;
}
