#ifndef STATS_H
#define STATS_H

#include <iostream>
#include <Stats/Graph.hpp>
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
#include <unordered_map>
#include <string> 

class Stats : public Drawable, public Updatable {
	
	private:
	
		std::unordered_map <int, std::string> titres;
		std::unordered_map <int, std::unique_ptr<Graph>> graphes; 
		int identifiant = -1;	//index du graphe actif (de la map de graphes)
		sf::Time compteurTemps;
		
	protected: 
		
		void updateTemps(sf::Time dt);
		
	public:
		
		Stats();
		void update (sf::Time dt) override; 
		void draw (sf::RenderTarget& target) const override; 
		void setActive(int index);			//mise a jour de l'identifiant a index
		void focusOn(std::string title);	//pour que identifiant actif soit celui du graphe de 'title'
		void reset();
		void addGraph(int id, std::string const& title, std::vector<std::string> const& series, double min, double max, Vec2d size);
 
}; 

#endif
