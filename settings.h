#ifndef SETTINGS
#define SETTINGS

namespace weighttracker
{

enum WtUnits {Kilograms, Pounds};

template <WtUnits>
double convertTo(double value) { }

template<>
double convertTo<Kilograms>(double value)
{
    return value;
}

template<>
double convertTo<Pounds>(double value)
{
    return 2.2 * value;
}

}

#endif // SETTINGS

