#ifndef CIRCULAR_COLLIDER_H
#define CIRCULAR_COLLIDER_H

#include <iostream>
#include <fstream>
#include <array>
#include <limits>
#include <cfloat>
#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>
#include <Interface/Drawable.hpp>

class CircularCollider : public Drawable
{
	private:

		Vec2d position;
		double rayon;

		bool isPointInside(const Vec2d& point) const;
		void checkShortestDistance (double& distance, const double& tmp_dist, Vec2d& direction, const Vec2d& tmp_vec) const;
		bool isCircularColliderInside(const CircularCollider& other) const;


	protected:

		void clamping();	//assurer un monde torique accessible aux sous-classes


	public:

		//Constructeurs/destructeur
		CircularCollider (Vec2d p, double r);
		CircularCollider (CircularCollider const& autre);	//Constructeur de copie
		virtual ~CircularCollider () = default;

		//getters/setters pour la position et le rayon
		const Vec2d& getPosition() const;
		const double& getRadius() const;
		void setPosition(const Vec2d& newPosition);
		void setRadius (double r); 

		//operateur= interne de copie
		CircularCollider& operator=(const CircularCollider& objet);

		//GESTION DU DEPLACEMENT
		Vec2d directionTo(const Vec2d& to) const;					
		Vec2d directionTo (const CircularCollider& objet)const;
		double distanceTo (const Vec2d& to)const;
		double distanceTo (const CircularCollider& objet)const;
		void move(const Vec2d& dx);									//Permet au CircularCollider de bouger dans le monde torique
		CircularCollider& operator+=(const Vec2d& dx);

		//GESTION DES COLLISIONS
		bool operator>(const CircularCollider& body)const;		//si body est dans l'instance courante
		bool operator|(const CircularCollider& body)const;		//si body est en collision avec l'instance courante
		bool operator>(const Vec2d& body)const;					//si point est a l'interieur de l'instance courante
		bool isColliding(const CircularCollider& other) const;	

};

std::ostream& operator<<(std::ostream& sortie, const CircularCollider& objet);	//operateur affichant le rayon et la position de l'instance courante

#endif
