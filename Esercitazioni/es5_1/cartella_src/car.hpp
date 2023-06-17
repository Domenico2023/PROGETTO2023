#ifndef __CAR_H
#define __CAR_H

#include <iostream>

using namespace std;

namespace CarLib {
    class Car
    {
    private:
        //attributes
        string producer;
        string model;
        string color;
        //methods (none)

    public:
        //constructor
        Car(const string& producer, const string& model, const string& color);
        //attributes (none)
        //methods
        string show() {return model + " (" + producer + ") " + color;}
    };

    enum struct CarProducer {UNKNOWN = 'U', FORD = 'F', TOYOTA = 'T', VOLKSWAGEN = 'V', AUDI = 'A', MERCEDES = 'M', BUGATTI = 'B'};

    class CarFactory
    {
    private:
        static CarProducer producer;
    public:
        static void startProduction(const CarProducer& producer);
        static Car* Create(const string& color);
    };
}
#endif
// __CAR_H
