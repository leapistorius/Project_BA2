#include "Wave.hpp"
#include <Utility/Utility.hpp>
#include <Utility/Constants.hpp>
#include <Application.hpp>

Wave::Wave(Vec2d depart, double energie, double rayon, double constante, double vitesse)
:CircularCollider(depart, rayon), temps (sf::Time::Zero), niveauEnergieInitial(energie), rayonInitial(rayon), constOnde(constante), vitessePropagation(vitesse)
{
	ensembleArcs.push_back(new Arcs (std::make_pair(-PI, PI))); 
}

double Wave::getIntensity() const 
{
	return intensity; 
}

std::vector<Arcs*> Wave::getEnsembleArcs() const
{
	return ensembleArcs;
}

void Wave::draw(sf::RenderTarget& targetWindow) const
{
	for (auto& curve: ensembleArcs)
	{
		Arc arc(buildArc(curve->first/DEG_TO_RAD, curve->second/DEG_TO_RAD, getRadius(), getPosition(), sf::Color::Black, 0.0, intensity * getAppConfig().wave_intensity_thickness_ratio));
		targetWindow.draw(arc);
	}

}

void Wave::update(sf::Time dt)
{
	updateTime(dt); 
	updateRayon(); 
	updateEnergie(); 
	updateIntensity(); 
	updateArcs();
}

void Wave::updateTime(sf::Time dt)
{
	temps += dt; 
}

void Wave::updateRayon()
{
	setRadius(vitessePropagation* temps.asSeconds() + rayonInitial); 
}

void Wave::updateEnergie()
{
	energie = niveauEnergieInitial * exp(-getRadius()/constOnde); 
}

void Wave::updateIntensity()
{
	intensity = energie/(2*PI* getRadius()); 
}

void Wave::updateArcs()
{
	std::vector<CircularCollider*> obstacles (getAppEnv().Environment::getObstacles());
	
	//iteration sur les arcs d'une onde
	for (size_t i(0); i <ensembleArcs.size();++i){
		
		double arcRadius(this->getRadius());
		double firstAngle(ensembleArcs[i]->first); 
		
		//iteration sur les possibles obstacles
		for (auto& obstacle: obstacles){
			
			double obstacleRadius (obstacle->getRadius()); 
			double condition ((obstacle->getPosition() - this->getPosition()).angle()); 
			
			//si un arc est en collision avec un obstacle
			if (condition >= ensembleArcs[i]->first and condition <= ensembleArcs[i]->second and *this|*obstacle){
				
				double alpha (2* std::atan2(obstacleRadius, arcRadius + obstacleRadius)); 
				double secondAngle(condition - alpha/2);
				
				ensembleArcs[i]->first = condition + alpha/2; 
				ensembleArcs.push_back(new Arcs (std::make_pair(firstAngle, secondAngle)));
			}
		}
	}
	
}

