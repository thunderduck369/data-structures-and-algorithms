#include "PathSearch.h"
#include <unordered_set>
#include <iostream>
#include <math.h>
#include <time.h>

namespace ufl_cap4053
{
	namespace searches
	{
		// CLASS DEFINITION GOES HERE
		bool isGreaterThan(PathSearch::PlannerNode* const& node1, PathSearch::PlannerNode* const& node2) {
			return (node1->fCost > node2->fCost);
		}
		PathSearch::PathSearch() : search_queue(PriorityQueue(&isGreaterThan)) {};
		PathSearch::~PathSearch() {
			unload();
		}
		void PathSearch::load(TileMap* _tileMap) {
			std::vector<std::vector<int>> odd = {{0, -1}, {1, -1}, {-1, 0}, {1, 0}, {0, 1}, {1, 1}};
			std::vector<std::vector<int>> even = { {-1, -1}, {0, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1} };

			int n = _tileMap->getRowCount();
			int m = _tileMap->getColumnCount();
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					Tile* t = _tileMap->getTile(i, j);
					search_graph[t] = new Node(t);

					// edges for odd rows
					if (i % 2 == 1) {
						for (std::vector<int> digits : odd) {
							if (i + digits[1] < n && j + digits[0] < m && i + digits[1] >= 0 && j + digits[0] >= 0) {
								Tile* adj = _tileMap->getTile(i + digits[1], j + digits[0]);
								search_graph[t]->edges.push_back(new Edge(adj, 2 * _tileMap->getTileRadius() * adj->getWeight()));
							}
						}
					}

					// edges for even rows
					if (i % 2 == 0) {
						for (std::vector<int> digits : even) {
							if (i + digits[1] < n && j + digits[0] < m && i + digits[1] >= 0 && j + digits[0] >= 0) {
								Tile* adj = _tileMap->getTile(i + digits[1], j + digits[0]);
								search_graph[t]->edges.push_back(new Edge(adj, 2 * _tileMap->getTileRadius() * adj->getWeight()));
							}
						}
					}
				}
			}
		}
		void PathSearch::initialize(int startRow, int startCol, int goalRow, int goalCol) {
			for (auto tile_node : search_graph) {
				// add start node to queue
				if (tile_node.first->getRow() == startRow && tile_node.first->getColumn() == startCol) {
					search_queue.push(new PlannerNode(tile_node.first, nullptr, 0, 0));
				}
				// update goal node
				if (tile_node.first->getRow() == goalRow && tile_node.first->getColumn() == goalCol) {
					//search_graph[tile_node.first]->goal = true;
					goal_tile = tile_node.first;
				}
			}
			Tile* start = search_queue.front()->vertex;
			visited[start] = search_queue.front();
			visited[start]->hCost = sqrt(pow(goal_tile->getXCoordinate() - start->getXCoordinate(), 2) + pow(goal_tile->getXCoordinate() - start->getYCoordinate(), 2));
			planner_col.push_back(search_queue.front());
			solution_path.clear();
			done = false;
		}
		void PathSearch::update(long timeslice) {
			clock_t start_t = clock();
			long total_t = 0;
			if (timeslice == 0) {
				while (!search_queue.empty()) {
					// get front and pop
					PlannerNode* curr = search_queue.front();
					if (curr->vertex == goal_tile) {
						// get path
						std::cout << "Cost: " << curr->fCost << std::endl;
						while (curr != nullptr) {
							solution_path.push_back(curr->vertex);
							curr = curr->parent;
						}
						done = true;
						break;
					}
					search_queue.pop();

					// add edges to queue
					for (Edge* edge : search_graph.at(curr->vertex)->edges) {
						double newCost = curr->gCost + edge->cost;
						Tile* successor = edge->endpoint;
						successor->setFill(0xFF000000);
						if (!visited[successor] && successor->getWeight() != 0) {
							double newEst = sqrt(pow(goal_tile->getXCoordinate() - successor->getXCoordinate(), 2) + pow(goal_tile->getYCoordinate() - successor->getYCoordinate(), 2));
							PlannerNode* newNode = new PlannerNode(successor, curr, newEst, newCost);
							// add planner node to collection to be deleted on cleanup
							planner_col.push_back(newNode);
							visited[successor] = newNode;
							search_queue.push(newNode);
						}
						else if (successor->getWeight() != 0) {
							if (newCost < visited[successor]->gCost) {
								visited[successor]->gCost = newCost;
								visited[successor]->fCost = visited[successor]->hCost + newCost;
								visited[successor]->parent = curr;

								bool found = false;
								std::vector<PlannerNode*> temp;
								while (!search_queue.empty()) {
									if (search_queue.front() == visited[successor]) {
										found = true;
									}
									temp.push_back(search_queue.front());
									search_queue.pop();
								}
								if (!found) {
									search_queue.push(visited[successor]);
								}
								else {
									for (PlannerNode* node : temp) {
										search_queue.push(node);
									}
								}
							}
						}
					}
				}
			}
			else {
				while (!search_queue.empty() && total_t < timeslice) {
					// get front and pop
					PlannerNode* curr = search_queue.front();
					if (curr->vertex == goal_tile) {
						// get path
						std::cout << "Cost: " << curr->fCost << std::endl;
						while (curr != nullptr) {
							solution_path.push_back(curr->vertex);
							curr = curr->parent;
						}
						done = true;
						break;
					}
					search_queue.pop();

					// add edges to queue
					for (Edge* edge : search_graph.at(curr->vertex)->edges) {
						double newCost = curr->gCost + edge->cost;
						Tile* successor = edge->endpoint;
						successor->setFill(0xFF000000);
						if (!visited[successor] && successor->getWeight() != 0) {
							double newEst = sqrt(pow(goal_tile->getXCoordinate() - successor->getXCoordinate(), 2) + pow(goal_tile->getYCoordinate() - successor->getYCoordinate(), 2));
							PlannerNode* newNode = new PlannerNode(successor, curr, newEst, newCost);
							// add planner node to collection to be deleted on cleanup
							planner_col.push_back(newNode);
							visited[successor] = newNode;
							search_queue.push(newNode);
						}
						else if (successor->getWeight() != 0) {
							if (newCost < visited[successor]->gCost) {
								visited[successor]->gCost = newCost;
								visited[successor]->fCost = visited[successor]->hCost + newCost;
								visited[successor]->parent = curr;

								bool found = false;
								std::vector<PlannerNode*> temp;
								while (!search_queue.empty()) {
									if (search_queue.front() == visited[successor]) {
										found = true;
									}
									temp.push_back(search_queue.front());
									search_queue.pop();
								}
								if (!found) {
									search_queue.push(visited[successor]);
								}
								else {
									for (PlannerNode* node : temp) {
										search_queue.push(node);
									}
								}
							}
						}
					}
					clock_t end_t = clock();
					total_t = (long)(end_t - start_t) * 1000/ CLOCKS_PER_SEC;
				}
			}
		}
		void PathSearch::shutdown() {
			while (!planner_col.empty()) {
				delete planner_col.front();
				planner_col.erase(planner_col.begin());
			}
			planner_col.clear();
			visited.clear();
			search_queue.clear();
		}
		void PathSearch::unload() {
			shutdown();
			solution_path.clear();
			done = false;
			while (!search_graph.empty()) {
				Node* item = search_graph.begin()->second;
				while (!item->edges.empty()) {
					delete item->edges.front();
					item->edges.erase(item->edges.begin());
				}
				item->edges.clear();
				delete item;
				search_graph.erase(search_graph.begin());
			}
			search_graph.clear();
		}
		bool PathSearch::isDone() const {
			return done;
		}
		std::vector<Tile const*> const PathSearch::getSolution() const{
			return solution_path;
		}
	}
}  // close namespace ufl_cap4053::searches
