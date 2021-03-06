#ifndef FOOD_H
#define FOOD_H

#include <iostream>
#include <Environment/OrganicEntity.hpp>
#include <Environment/Banana.hpp>
#include <Utility/Utility.hpp>

class Food: public OrganicEntity {
	
	private: 
	
		static size_t compteurFood; //pour statistiques 
	
	public:
 
		//constructeur/destructeur
		Food(const Vec2d& position_);
		Food (const Vec2d& position_, double size, double energie); 
		~Food();

		//Getters
		sf::Texture& getTexture() const override;
		double getSize() const override; 
		double getEnergieInitial() const override; 
		sf::Time getLongetivity() const;
		sf::Time getTempsEnceinte() const override;
		double getPerteEnergie() const override;
		double getEnergieConsumption() const override; 
		double getVitesse() const override; 
		static size_t getCompteurFood(); 

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
		bool eatableBy(Banana const* banana) const override; 
		
		//methodes matable
		bool matable(OrganicEntity const* other) const override;
		bool canMate(Scorpion const* scorpion) const override;
		bool canMate(Gerbil const* gerbil) const override;
		bool canMate(Food const* food) const override;
		bool canMate(Banana const* banana) const override;
		
		//methodes objets 
		bool upgradable (OrganicEntity const* other) const override; 
		bool canUpgrade(Scorpion const* scorpion) const override;
		bool canUpgrade(Gerbil const* gerbil) const override;
		bool canUpgrade(Food const* food) const override;
		bool canUpgrade(Banana const* banana) const override; 

		Food(const Food&) = default; 	//eviter copie d'entites mal-faites
}; 

#endif

