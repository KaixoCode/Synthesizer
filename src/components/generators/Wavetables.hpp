namespace Wavetables
{
    struct WaveTable 
    {
        virtual double Value(double) = 0;
    };

    struct Saw : public WaveTable 
    {
        double Value(double) override;
    };

    struct Sine : public WaveTable
    {
        double Value(double) override;
    };

    struct Square : public WaveTable
    {
        double Value(double) override;
    };

    struct Triangle : public WaveTable
    {
        double Value(double) override;
    };
}