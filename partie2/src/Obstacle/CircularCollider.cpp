#include <iostream>
#include "CircularCollider.hpp"
#include <Application.hpp>


//METHODES GENERALES
CircularCollider::CircularCollider(Vec2d p, double r = 0.0)
	:position(p)
{
	if (r < 0.0){
		throw std::invalid_argument ("Le rayon est negatif."); 
	} else {
		rayon = r; 
	}
	
	clamping();		
}

//Constructeur de copie 
CircularCollider::CircularCollider (CircularCollider const& autre)
	:position(autre.position), rayon(autre.rayon)
{
	clamping();
}

//Methode pour l'algorithme de clamping pour un monde torique 
void CircularCollider::clamping ()
{	
	double worldSize = getAppConfig().simulation_world_size;
	auto width  = worldSize; // largeur
	auto height = worldSize; // hauteur
	
	if (position.x < 0.0)
	{
		position.x = width + position.x; 
	}else if (position.x > width)
	{
		position.x = position.x - width; 
	}else if (position.y < 0.0)
	{
		position.y = height + position.y; 
	}else if (position.y > height)
	{
		position.y = position.y - height; 
	}
	 
 }

//getters et setter
const Vec2d& CircularCollider::getPosition () const 
{
	return position; 
}

const double& CircularCollider::getRadius () const 
{
	return rayon; 
}

void CircularCollider::setPosition(const Vec2d& newPosition) 
{
	position=newPosition;
	clamping();
}

//operateur = interne de copie   
CircularCollider& CircularCollider::operator=(const CircularCollider& objet)
{
	position = objet.position; 
	rayon = objet.rayon; 
	return (*this); 
}


//GESTION DU DEPLACEMENT DANS UN MONDE TORIQUE
Vec2d CircularCollider::directionTo (const Vec2d& to) const
{
	double worldSize = getAppConfig().simulation_world_size;
	auto width  = worldSize; // largeur
	auto height = worldSize; // hauteur

	Vec2d direction; 
	direction.x = to.x - position.x; 
	direction.y = to.y - position.y; 
	
	double distance = DBL_MAX;   
	double tmp_dist; 
	
	std::array<double, 3> tabX = {to.x - position.x, to.x - width - position.x, to.x + width - position.x}; 
	std::array<double, 3> tabY = {to.y - position.y, to.y - height - position.y, to.y + height - position.y}; 
	
	Vec2d tmp_vec (direction);
	
	for (size_t i(0); i<tabX.size() ; ++i){
		for (size_t j(0); j<tabY.size() ; ++j){
			tmp_vec.x = tabX[i];
			tmp_vec.y = tabY[j]; 
			tmp_dist = tmp_vec.length();
			checkShortestDistance(distance, tmp_dist, direction, tmp_vec);	
		}
	}

	return direction; 
}

void CircularCollider::checkShortestDistance (double& distance, const double& tmp_dist, Vec2d& direction, const Vec2d& tmp_vec) const
{
	 if (distance > tmp_dist){
		 distance = tmp_dist; 
		 direction = tmp_vec; 
	} 
}

Vec2d CircularCollider:: directionTo (const CircularCollider& objet) const
{
	return directionTo(objet.position); 
}

double CircularCollider:: distanceTo (const Vec2d& to) const
{
	return directionTo(to).length(); 
}

double CircularCollider::distanceTo (const CircularCollider& objet) const
{
	return directionTo(objet).length(); 
}

//Permet au CircularCollider de bouger dans le monde torique
void CircularCollider::move(const Vec2d& dx) \
{
	
	position += dx;
	clamping();
}

CircularCollider& CircularCollider::operator+=(const Vec2d& dx)
{
	move(dx);
	return *this; 
}

//GESTION DES COLLISIONS
bool CircularCollider::isCircularColliderInside(const CircularCollider& other) const 
{
	double dif_rayons(rayon - other.rayon);						//calcul de la difference des rayons des 2 instances
	
	if ((dif_rayons>=0) and (dif_rayons>=distanceTo(other))) {  //conditions pour que other soit dans l'instance courante
		return true;
	} else {
		return false;
	}
}

//si instance other est en collision avec instance courante
bool CircularCollider::isColliding(const CircularCollider& other) const 
{
	if(distanceTo(other)<=(rayon+other.rayon)) {
		return true;
	} else {
		return false;
	}
}

//si point est a l'interieur de l'instance courante
bool CircularCollider::isPointInside(const Vec2d& point) const 
{	
	if(distanceTo(point)<=rayon) {
		return true;
	} else {
		return false;
	}
}

//si body est dans l'instance courante 
bool CircularCollider::operator>(const CircularCollider& body) const 
{
	if(isCircularColliderInside(body)) {
		return true;
	} else {
		return false;
	}
}

//si body est en collision avec l'instance courante
bool CircularCollider::operator|(const CircularCollider& body) const 
{	
	if(isColliding(body)) {
		return true;
	} else {
		return false;
	}
}

//si point est a l'interieur de l'instance courante
bool CircularCollider::operator>(const Vec2d& body) const 
{
	if(isPointInside(body)) {
		return true;
	} else {
		return false;
	}
}

//operateur affichant le rayon et la position de l'instance courante 
std::ostream& operator<<(std::ostream& sortie, const CircularCollider& objet) {
	return sortie<<"CircularCollider: position = " << objet.getPosition() <<"  radius = " << objet.getRadius();
}

