#include "../platform.h" // This file will make exporting DLL symbols simpler for students.
#include "../Framework/TileSystem/TileMap.h"
#include "../PriorityQueue.h"
#include <unordered_map>
#include <queue>

namespace ufl_cap4053
{
	namespace searches
	{
		
		
		class PathSearch
		{
		// CLASS DECLARATION GOES HERE
			struct Edge {
				Tile* endpoint;
				double cost;
				Edge(Tile* t, double _cost) {
					endpoint = t;
					cost = _cost;
				}
			};
			struct Node {
				bool goal = false;
				bool visited = false;
				std::vector<Edge*> edges;
				int cost;
				Node(Tile* t) {
					cost = t->getWeight();
				}
			};
				
			public:
				struct PlannerNode {
					Tile* vertex;
					PlannerNode* parent;
					double gCost;
					double hCost;
					double fCost;
					PlannerNode(Tile* v, PlannerNode* p, double _hCost, double _gCost) {
						vertex = v;
						parent = p;
						hCost = _hCost;
						gCost = _gCost;
						fCost = hCost + gCost;
					}
				};
				DLLEXPORT PathSearch(); // EX: DLLEXPORT required for public methods - see platform.h
				DLLEXPORT ~PathSearch();
				DLLEXPORT void load(TileMap* _tileMap);
				DLLEXPORT void initialize(int startRow, int startCol, int goalRow, int goalCol);
				DLLEXPORT void update(long timeslice);
				DLLEXPORT void shutdown();
				DLLEXPORT void unload();
				DLLEXPORT bool isDone() const;
				DLLEXPORT std::vector<Tile const*> const getSolution() const;
		private:
			std::unordered_map<Tile*, Node*> search_graph;
			//std::queue<PlannerNode*> search_queue;
			std::vector<PlannerNode*> planner_col;
			std::unordered_map<Tile*, PlannerNode*> visited = {};
			PriorityQueue<PlannerNode*> search_queue;
			std::vector<Tile const*> solution_path;
			Tile* goal_tile;
			bool done = false;
		};
	}
}  // close namespace ufl_cap4053::searches

