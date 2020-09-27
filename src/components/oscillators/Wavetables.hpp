namespace Wavetables
{
    struct WaveTable 
    {
        virtual double value(double) = 0;
    };

    struct Saw : public WaveTable 
    {
        double value(double) override;
    };

    struct Sine : public WaveTable
    {
        double value(double) override;
    };

    struct Square : public WaveTable
    {
        double value(double) override;
    };
}