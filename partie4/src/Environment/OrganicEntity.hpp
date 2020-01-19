#ifndef ORGANIC_ENTITY_H
#define ORGANIC_ENTITY_H

#include <iostream>
#include <Obstacle/CircularCollider.hpp>
#include <SFML/Graphics.hpp>
#include <Interface/Updatable.hpp>

class Gerbil;
class Scorpion;
class Food;

class OrganicEntity : public CircularCollider, public Updatable
{
		
	public:

		//constructeur et destructeur
		OrganicEntity(const Vec2d& position_, double taille, double energie);
		virtual ~OrganicEntity() = default;

		//methodes update
		virtual void update(sf::Time dt) override;
		void updateTime(sf::Time dt);
		void updateEnergie (sf::Time dt);
		
		virtual void draw(sf::RenderTarget& targetWindow) const = 0;
		virtual void meet() = 0;
		virtual void eating(double& energie) = 0;
		bool isDead();
		
		//getters
		virtual sf::Texture& getTexture() const = 0;
		virtual double getEnergieInitial() const = 0;
		virtual double getSize() const = 0;
		double getEnergie() const;
		virtual sf::Time getLongetivity() const;	//retourne temps en vie maximal
		virtual sf::Time getTempsEnceinte() const = 0;
		virtual double getPerteEnergie() const = 0;
		virtual double getEnergieConsumption() const = 0;
		virtual double getVitesse() const = 0;  

		//setter
		void setEnergie(double energie);

		//methodes eatable
		virtual bool eatable(OrganicEntity const* entity) const = 0;
		virtual bool eatableBy(Gerbil const* gerbil) const = 0;
		virtual bool eatableBy(Food const* food) const = 0;
		virtual bool eatableBy(Scorpion const* scorpion) const = 0;
		
		//methodes matable
		virtual bool matable(OrganicEntity const* other) const = 0;
		virtual bool canMate(Scorpion const* scorpion) const = 0;
		virtual bool canMate(Gerbil const* gerbil) const = 0;
		virtual bool canMate(Food const* food) const = 0;

	protected:

		double niveauEnergie;
		sf::Time tempsEnVie;	//temps en vie courant

};

#endif
