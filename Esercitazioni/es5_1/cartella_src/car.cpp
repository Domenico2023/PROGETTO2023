#include "car.hpp"

namespace CarLib {
    Car::Car(const string& producer, const string& model, const string& color):
        producer(producer), model(model), color(color) {}

    CarProducer CarFactory::producer = CarProducer::UNKNOWN;   //default

    void CarFactory::startProduction(const CarProducer& producer)
    {CarFactory::producer = producer;}

    Car* CarFactory::Create(const string& color)
    {
        switch(producer){
        case CarProducer::FORD:
            return new Car("Ford", "Mustang", color);
        case CarProducer::TOYOTA:
            return new Car("Toyota", "Prius", color);
        case CarProducer::VOLKSWAGEN:
            return new Car("Volkswagen", "Golf", color);
        default: throw runtime_error("Invalid producer");
        }
    }
}
