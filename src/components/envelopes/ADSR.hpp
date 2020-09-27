#include "Envelope.hpp"

class ADSR : public Envelope
{
    double sample = 0;
    double phase = -1;
    bool gate = false;

public:
    double A = 0.02; // s
    double D = 0.1; // s
    double S = 1.0; // (0-1)
    double R = 0.1; // s

    double AC = 1.0; // curve
    double DC = 1.0; // curve
    double RC = 1.0; // curve

    double NextSample() override;
    double Sample() override;
    
    void Trigger() override;
    void Gate(bool) override;
};