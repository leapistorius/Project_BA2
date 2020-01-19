#include "WaveGerbil.hpp"
#include <Application.hpp>
#include <Environment/Environment.hpp>

//constructeurs
WaveGerbil::WaveGerbil(const Vec2d& positionInitiale, double energie, bool sexe)
    : Gerbil(positionInitiale, energie, sexe), compteur(sf::Time::Zero)
{}

WaveGerbil::WaveGerbil(const Vec2d& positionInitiale)
    : Gerbil(positionInitiale)
{}

void WaveGerbil::update(sf::Time dt)
{
	Gerbil::update(dt); 
	compteur+=dt;
	
	if (compteur.asSeconds()>= (1.0/getAppConfig().wave_gerbil_frequency)) {	//des que compteur depasse fq 
		getAppEnv().addWave(newWave());								// -> dessiner une onde
		compteur = sf::Time::Zero;									// -> remettre compteur a zero
	}
}

Wave* WaveGerbil::newWave()
{	
	return new Wave (this->getPosition(),getAppConfig().wave_default_energy,getAppConfig().wave_default_radius,getAppConfig().wave_default_mu,getAppConfig().wave_default_speed);
}

