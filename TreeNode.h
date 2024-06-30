#include <queue>
using namespace std;

// TreeNode class should go in the "ufl_cap4053::fundamentals" namespace!
namespace ufl_cap4053 {
	namespace fundamentals {
		template <typename T> class TreeNode {
		public:
			TreeNode<T>() {}
			TreeNode<T>(T element) {
				val = element;
			}
			const T& getData() const {
				return val;
			}
			size_t getChildCount() const {
				return children.size();
			}
			TreeNode<T>* getChild(size_t index) {
				return children.at(index);
			}
			TreeNode<T>* getChild(size_t index) const {
				return children.at(index);
			}
			void addChild(TreeNode<T>* child) {
				children.push_back(child);
			}
			TreeNode<T>* removeChild(size_t index) {
				TreeNode<T>* node = children.at(index);
				children.erase(children.begin()+index);
				return node;
			}
			void breadthFirstTraverse(void (*dataFunction)(const T)) const {
				queue<const TreeNode<T>*> bfs_q;
				bfs_q.push(this);

				while (!bfs_q.empty()) {
					const TreeNode<T>* curr_node = bfs_q.front();
					dataFunction(curr_node->val);
					bfs_q.pop();
					for (TreeNode<T>* node : curr_node->children) {
						bfs_q.push(node);
					}
				}
			}
			void preOrderTraverse(void (*dataFunction)(const T)) const {
				dataFunction(this->val);

				for (TreeNode<T>* node : this->children) {
					node->preOrderTraverse(dataFunction);
				}
			}
			void postOrderTraverse(void (*dataFunction)(const T)) const {
				for (TreeNode<T>* node : this->children) {
					node->postOrderTraverse(dataFunction);
				}

				dataFunction(this->val);
			}
		private:
			T val;
			vector<TreeNode<T>*> children;
		};
	}
}  // namespace ufl_cap4053::fundamentals
