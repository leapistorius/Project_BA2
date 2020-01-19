#ifndef FOOD_GENERATOR_H
#define FOOD_GENERATOR_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Environment/Food.hpp>
#include <Environment/OrganicEntity.hpp>
#include <Interface/Updatable.hpp>

class FoodGenerator : public Updatable
{
	private:

		sf::Time time;

	public:

		//Constructeur et destructeur
		FoodGenerator();
		~FoodGenerator() = default;

		void update(sf::Time dt);				//compteur permettant de peupler l'environnement de Food a un certain pas de temps
		double getEnvironmentSize() const;
		Food* addFood() const;					//ajoute une source de nourriture aleatoirement dans l'environnement
};

#endif
