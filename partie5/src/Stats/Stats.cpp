#include <vector> 
#include "Stats.hpp"
#include <Application.hpp> 
#include <Environment/Environment.hpp>

Stats::Stats()
:compteurTemps(sf::Time::Zero)
{}

void Stats::focusOn(std::string title) 
{
	for (auto& label : titres){
		if (label.second == title) {
			setActive(label.first);	
			graphes[label.first] -> reset();	
		}
	}
}

void Stats::setActive(int index)
{
	identifiant = index;
}

void Stats::reset() 
{
	for(auto& graph : graphes) {
		graph.second->Graph::reset();
	}
}

void Stats::addGraph(int id, const std::string& title, const std::vector<std::string>& series, double min, double max, Vec2d size)
{
	graphes[id].reset(new Graph(series, size, min, max));	//nouveau graphe	
	titres[id] = title;		//titre associe 
	setActive(id);
}

void Stats::update(sf::Time dt) 
{	
	double temps (getAppConfig().stats_refresh_rate); //le pas de temps
	updateTemps(dt);
	
	if(compteurTemps.asSeconds()>=temps) {
		compteurTemps=sf::Time::Zero;
		for (size_t i(0); i<titres.size(); ++i){
			graphes[i]->updateData(sf::seconds(temps), (getAppEnv().fetchData(titres[i]))); 
		}
	}
}

void Stats::updateTemps(sf::Time dt)
{
	compteurTemps += dt;
}

void Stats::draw (sf::RenderTarget& target) const 
{
	for (auto& graph: graphes){
		if (graph.first == identifiant){ //regarde si c'est le graph qui est actif
			graph.second->draw(target);
		}
	} 

}

