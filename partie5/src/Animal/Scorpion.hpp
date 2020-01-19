#ifndef SCORPION_H
#define SCORPION_H

#include <iostream>
#include <Animal/Animal.hpp>

class Scorpion: public Animal
{

	private: 

		 static size_t compteurScorpion; //pour statistiques 

	public:
		
		//constructeurs et destructeur
		Scorpion(const Vec2d& positionInitiale, double energie, bool sexe);
		Scorpion(const Vec2d& positionInitiale);
		~Scorpion();

		//getters
		double getStandardMaxSpeed() const override;
		double getMass() const override;
		double getViewRange()const override;
		double getViewDistance()const override;
		double getRandomWalkRadius() const override;
		double getRandomWalkDistance() const override;
		double getRandomWalkJitter() const override;
		double getEnergieInitial() const override;
		double getSize() const override;
		sf::Texture& getTexture() const override;
		sf::Time getLongetivity() const override;
		double getPerteEnergie() const override;
		double getEnergyLossMatingFemale () const; 
		double getEnergyLossMatingMale() const; 
		size_t getMinBabies () const; 
		size_t getMaxBabies () const;
		double getGestationTime () const override; 
		static size_t getCompteurScorpion (); 
		
		void givingBirth() const override;
		void meet() override;

		//methodes eatable
		bool eatable(OrganicEntity const* entity) const override;
		bool eatableBy(Gerbil const* gerbil) const override;
		bool eatableBy(Food const* food) const override;
		bool eatableBy(Scorpion const* scorpion) const override;
		
		//methodes mate 
		bool matable(OrganicEntity const* other) const override;
		bool canMate(Scorpion const* scorpion) const override;
		bool canMate(Gerbil const* gerbil) const override;
		bool canMate(Food const* food) const override;

};

#endif

