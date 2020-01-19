#include "Sensor.hpp"
#include <Application.hpp> 
#include <Environment/Environment.hpp> 
#include <array>
#include <Animal/NeuronalScorpion/NeuronalScorpion.hpp>

Sensor::Sensor (Vec2d position, State etat, NeuronalScorpion* scorpion, size_t i)
: location(position), state(etat), score(0), inhibitor(0), scorpionHote(scorpion), index(i)
{}

Sensor::Sensor (double x, double y, NeuronalScorpion* scorpion, size_t i)
: state(PAS_ACTIF), score (0), inhibitor(0), scorpionHote(scorpion), index(i)
{
	location.x = x; 
	location.y = y; 
}

Vec2d Sensor::getLocationSensor() const
{
	return location; 
}

double Sensor::getScore() const
{
	return score;
}

State Sensor::getState() const
{
	return state;
}

double Sensor::getInhibitor() const
{
	return inhibitor;
}

void Sensor::setInhibitor(double inhibition)
{
	inhibitor = inhibition;
}

void Sensor::update (sf::Time dt)
{	
	updateState(); 
	updateInhibitor();	
	this->updateScore(); 
}

void Sensor::updateState()
{
	if (getAppEnv().getIntensityForSensor(scorpionHote, index) > getAppConfig().sensor_intensity_threshold){
		state = ACTIF; 
	} else {
		state = PAS_ACTIF; 
	}
}

void Sensor::updateInhibitor ()
{
	//inhibition des trois autres sensors
	if (state == ACTIF) {
		std::array<std::pair<double,Sensor*>, 8> tabSenseurs(scorpionHote->getSensors());
		for(size_t i(3); i<=5; ++i) {
			tabSenseurs[(i+index)%8].second->setInhibitor(++score*getAppConfig().sensor_inhibition_factor); 
		}
	} 
	
	//condition pour rester dans bon interval
	double max (getAppConfig().sensor_inhibition_max); 
	if (inhibitor<0) {
		inhibitor = 0;
	} else if (inhibitor > max) {
		inhibitor = max;
	}
}

void Sensor::updateScore ()
{
	if (state == ACTIF) {
		score += 2.0 * (1.0 - inhibitor);
	} 
}

void Sensor::reInitialisation() 
{
	score = 0;
	inhibitor = 0;
	state = PAS_ACTIF;
}




