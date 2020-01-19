#ifndef WAVE_GERBIL_H
#define WAVE_GERBIL_H

#include <iostream>
#include <Animal/Gerbil.hpp>
#include <Environment/Wave.hpp>

class WaveGerbil: public Gerbil {
	
	public:
	
		//constructeurs et destructeur
		WaveGerbil(const Vec2d& positionInitiale, double energie, bool sexe);
		WaveGerbil(const Vec2d& positionInitiale);
		~WaveGerbil() = default;
		
		//pour ajouter des ondes dans l'environnement a une frequence donnee 
		void update(sf::Time dt);
		Wave* newWave();	
		
	private:	
	
		sf::Time compteur;
	
};

#endif
