#include "Envelope.hpp"

class ADSR : public Envelope
{
    Sample sample = 0;
    double phase = -1;
    bool gate = false;

public:
    double A = 0.02; // s
    double D = 0.1; // s
    double S = 1.0; // (0-1)
    double R = 0.1; // s

    double AC = 0.5;
    double DC = 0.5;
    double RC = 0.5;

    ADSR(double a, double d, double s, double r) : A(a), D(d), S(s), R(r) {};

    Sample NextSample() override;
    Sample GetSample() override;

    Sample Apply(Sample) override;
    
    void Trigger() override;
    void Gate(bool) override;
};