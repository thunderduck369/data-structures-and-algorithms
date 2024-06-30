
// LinkedList class should go in the "ufl_cap4053::fundamentals" namespace!
namespace ufl_cap4053 {
	namespace fundamentals {

		template <typename T> class LinkedList {
		public:
			struct Node {
				T val;	
				Node* next = nullptr;
				Node* prev = nullptr;
			};
			struct Iterator {
				Node* curr;
				Iterator() {
					curr = nullptr;
				}
				T operator*() const {
					return curr->val;
				}
				Iterator& operator++() {
					if (curr != nullptr) {
						curr = curr->next;
					}
					return *this;
				}
				bool operator==(Iterator const& rhs) {
					if (rhs == curr) return true;
					else return false;
				}
				bool operator!=(Iterator const& rhs) {
					if (rhs.curr == curr) return false;
					else return true;
				}
			};
			LinkedList<T>() {
				head = nullptr;
				tail = nullptr;
			}
			Iterator begin() const {
				Iterator it;
				it.curr = head;
				return it;
			}
			Iterator end() const {
				Iterator it;
				if (tail == nullptr) {
					it.curr = nullptr;
				}
				else {
					it.curr = tail->next;
				}
				return it;
			}
			bool isEmpty() const {
				if (head == nullptr) return true;
				return false;
			}
			T getFront() const {
				if (head == nullptr) return nullptr;
				return head->val;
			}
			T getBack() const {
				if (head == nullptr) return nullptr;
				return tail->val;
			}
			void enqueue(T element) {
				Node* node = new Node;
				node->val = element;
				if (head == nullptr) {
					head = node;
					tail = node;
				}
				else {
					tail->next = node;
					node->prev = tail;
					tail = node;
				}
			}
			void dequeue() {
				if (head == nullptr) return;
				if (head->next == nullptr) {
					delete head;
					head = nullptr;
					tail = nullptr;
				}
				else {
					head->next->prev = nullptr;
					Node* temp = head->next;
					delete head;
					head = temp;
				}
			}
			void pop() {
				if (head == nullptr) return;
				if (tail->prev == nullptr) {
					delete tail;
					head = nullptr;
					tail = nullptr;
				}
				else {
					tail->prev->next = nullptr;
					Node* temp = tail->prev;
					delete tail;
					tail = temp;
				}
			}
			void clear() {
				while (head != nullptr) {
					this->dequeue();
				}
				head = nullptr;
				tail = nullptr;
			}
			bool contains(T element) const {
				Node* temp = head;
				while (temp != nullptr) {
					if (temp->val == element) return true;
					temp = temp->next;
				}
				return false;
			}
			void remove(T element) {
				Node* temp = head;
				while (temp != nullptr) {
					if (temp->val == element) {
						if (temp->prev == nullptr) {
							if (head->next == nullptr) {
								delete head;
								head = nullptr;
								tail = nullptr;
							}
							else {
								head->next->prev = nullptr;
								Node* temp2 = head->next;
								delete head;
								head = temp2;
							}
							return;
						}
						else if (temp->next == nullptr) {
							if (tail->prev == nullptr) {
								delete tail;
								head = nullptr;
								tail = nullptr;
							}
							else {
								tail->prev->next = nullptr;
								Node* temp2 = tail->prev;
								delete tail;
								tail = temp2;
							}
							return;
						}
						else {
							temp->prev->next = temp->next;
							temp->next->prev = temp->prev;
							delete temp;
							return;
						}
					}
					temp = temp->next;
				}
				return;
			}
			~LinkedList<T>() {
				this->clear();
			}
		private:
			Node* head;
			Node* tail;
		};
	}
}  // namespace ufl_cap4053::fundamentals
