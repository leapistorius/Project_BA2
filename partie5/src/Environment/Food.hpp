#ifndef FOOD_H
#define FOOD_H

#include <iostream>
#include <Environment/OrganicEntity.hpp>
#include <Utility/Utility.hpp>

class Food: public OrganicEntity {
	
	private: 
	
		static size_t compteurFood; //pour statistiques 
	
	public:
 
		//constructeur/destructeur
		Food(const Vec2d& position_);
		~Food();

		//Getters
		double getEnergieInitial() const override;
		double getSize() const override;
		sf::Texture& getTexture() const override;
		sf::Time getLongetivity() const;
		sf::Time getTempsEnceinte() const override;
		double getPerteEnergie() const override;
		double getEnergieConsumption() const override; 
		double getVitesse() const override; 
		static size_t getCompteurFood(); 

		//TO DO: methode update : EVOLUTION DE LA SOURCE DE NOURRITURE A PROGRAMMER
		void update(sf::Time dt) override;
		void meet() override;
		void eating(double& energie) override;

		//Draw
		void draw(sf::RenderTarget& targetWindow) const override;

		//methodes eatable
		bool eatable(OrganicEntity const* entity) const override;
		bool eatableBy(Gerbil const* gerbil) const override;
		bool eatableBy(Food const* food) const override;
		bool eatableBy(Scorpion const* scorpion) const override;
		
		//methodes matable
		bool matable(OrganicEntity const* other) const override;
		bool canMate(Scorpion const* scorpion) const override;
		bool canMate(Gerbil const* gerbil) const override;
		bool canMate(Food const* food) const override;

	protected:

		Food(const Food&) = default; 	//eviter copie d'entites mal-faites
}; 

#endif

