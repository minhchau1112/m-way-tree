// Mway Tree
#include<iostream>
#define M 5
using namespace std;

struct Node {
	int count; // số lượng khóa trên mỗi nút
	int value[M - 1]; // mảng các giá trị của các khóa trên nút
	Node* child[M]; // địa chỉ các con của nút
};

struct MwayTree {
	Node* root;
};

struct QNode {
	int data;
	QNode* next;
};

struct Queue {
	QNode* front;
	QNode* rear;
};


void Init(Queue& q);
QNode* CreateNewNode(int value);
void Enqueue(Queue& q, int value);
void Dequeue(Queue& q);
void Enqueue_Node(Queue& q, Node* p);
void BFT(Node* cur);
int findNode(Node* cur, int value, int& pos);
Node* search(Node* cur, int value);
Node* findPos(Node* cur, Node* parent, int value, int& pos, bool& flag);
void QuickSort(int* a, int l, int r);
void Allocated(Node*& p);
Node* Create(int value);
void InsertNode(Node*& root, int value);
void clear(Node*& del, int pos);
void DeleteNode(Node*& root, int value);

void Init(Queue& q) {
	q.front = NULL;
	q.rear = NULL;
}

QNode* CreateNewNode(int value) {
	QNode* p = new QNode;
	if (p == NULL)return NULL;
	p->data = value;
	p->next = NULL;
	return p;
}

void Enqueue(Queue& q, int value) {
	QNode* temp = CreateNewNode(value);
	if (q.rear == NULL) {
		q.rear = temp;
		q.front = temp;
		return;
	}
	q.rear->next = temp;
	q.rear = temp;
}

void Dequeue(Queue& q) {
	if (q.front == NULL) return;
	QNode* temp = q.front;
	q.front = q.front->next;
	if (q.front == NULL) {
		q.rear = NULL;
	}
	delete temp;

}

void Enqueue_Node(Queue &q, Node* p) {
	if (p != NULL) {
		for (int i = 0; i < p->count; i++) {
			Enqueue(q, p->value[i]);
		}
	}
}

void Initialize(MwayTree& t) {
	t.root = NULL;
}

// Duyệt theo chiều rộng
void BFT(Node* cur) {
	Queue q;
	Init(q);
	Node* t = cur;
	Enqueue_Node(q, t); // Lần luợt thêm các khóa của nút gốc vào queue;
	// Lặp lại vòng lặp khi t khác NULL.
	while(t) {
		int i = 0;
		while (i<t->count) {
			cout << q.front->data << " "; // In ra giá trị đầu trong queue
			Enqueue_Node(q, t->child[i]); // Lần lượt thêm các khóa của nút con thứ i của t vào queue 
			i++;
			Dequeue(q); // Lấy giá trị đầu trong queue ra
		}
		Enqueue_Node(q, t->child[i]); // Thêm khóa của nút con cuối cùng của t vào queue
		// Nếu vẫn còn giá trị trong queue thì tìm kiếm nút chứa giá trị đấy trong cây, nếu đã hết giá trị thì kết thúc.
		if (q.front != NULL) { 
			t = search(cur, q.front->data);
		}
		else return;
	} 
}

// Duyệt theo chiều sâu
void DFT(Node* cur) {
	if (cur == NULL)return;
	// Lần lượt in ra các khóa trong nút
	for (int i = 0; i < cur->count; i++) {
		cout << cur->value[i] << " ";
	}
	for (int j = 0; j <= cur->count; j++) {
		DFT(cur->child[j]); // Lần lượt thăm các nút con.
	}
}


int findNode(Node* cur, int value, int& pos) {
	if (value < cur->value[0]) { // Nếu giá trị cần tìm nhỏ hơn khóa đầu tiên thì gán pos = 0 rồi return 0
		pos = 0;
		return 0;
	} // Nếu lớn hơn khóa đầu tiên thì
	else {
		pos = cur->count - 1; // gán pos = vị trí của khóa cuối cùng
		while (value < cur->value[pos] && pos>0)pos--; // tìm vị trí nhỏ nhất mà tại đó giá trị cần tìm nhỏ hơn khóa bên phải
		pos++; // vị trí của nút con thích hợp để chèn node vào nếu cần
		// nếu giá trị cần tìm = với giá trị ở vị trí nhỏ nhất mà tại đó nhỏ hơn khóa bên phải thì return 1, khác thì return 0
		if (value == cur->value[pos-1])return 1; 
		else return 0;
	}
}

Node* search(Node* cur, int value) {
	int pos;
	if (cur == NULL) return NULL;
	/* nếu tìm thấy giá trị đấy trong cây thì trả về địa chỉ của nút đó, còn vẫn chưa tìm thấy thì tiếp
	tục tìm ở nút con thích hợp.
	*/
	if (findNode(cur, value, pos))return cur;
	else return search(cur->child[pos], value);
}

Node* findPos(Node* cur,Node* parent, int value,int& pos,bool &flag) {
	// Nếu đã tìm thấy vị trí thích hợp thì return parent
	if (cur == NULL) return parent;
	// Nếu giá trị cần thêm đã tồn tại thì gán glag = true (đánh dấu đã có trong cây)
	if (findNode(cur, value, pos)) {
		cout << "Exits" << endl;
		flag = true;
		return cur;
	} else { 
		// Nếu giá trị cần thêm chưa có trong cây thì gán parent = cur, và tiếp tục tìm kiếm ở nút con thích hợp
		parent = cur;
		findPos(cur->child[pos],parent, value, pos, flag);
	}
}

void QuickSort(int* a, int l, int r) {
	int pivot = a[(l + r) / 2];
	int i = l, j = r;
	while (i <= j) {
		while (a[i] < pivot)i++;
		while (a[j] > pivot)j--;
		if (i <= j) {
			swap(a[i], a[j]);
			i++; j--;
		}
	}
	if (j > l)QuickSort(a, l, j);
	if (i < r)QuickSort(a, i, r);
}

// Cấp phát vùng nhớ cho các nút con
void Allocated(Node*& p) {
	*p->child = NULL;
	for (int i = 0; i < M; i++) {
		p->child[i] = NULL;
	}
}

// Khởi tạo nút mới.
Node* Create(int value) {
	Node * p = new Node;
	p->value[0] = value;
	p->count = 1;
	Allocated(p);
	return p;
}

void InsertNode(Node*& root,int value) {
	int pos;
	bool flag = false; // flag kiểm soát xem giá trị cần thêm đã có trong cây chưa
	// Tìm nút cha
	Node* parent = findPos(root,root, value, pos, flag);
	// Nếu giá trị cần thêm đã có trong cây thì return, còn chưa thì tiếp tục
	if (flag) return;
	// Nếu cây rỗng thì khởi tạo nút gốc
	if (root == NULL) {
		root = Create(value);
		return;
	}
	// Nếu nút cha vẫn còn chỗ thì thêm phần tử vào khóa tiếp theo và sắp xếp lại các khóa trên nút tăng dần
	if (parent->count < M - 1) {
		parent->value[parent->count] = value;
		parent->count++;
		QuickSort(parent->value,0,parent->count-1);
	}
	else { // nếu nút cha đã hết chỗ thì kiểm tra nút con thích hợp
		// nếu nút con thích hợp rỗng thì khởi tạo nút con đó
		if (parent->child[pos] == NULL) {
			parent->child[pos] = Create(value);
		} // nếu nút con thích hợp không rỗng thì thêm phần tử vào khóa tiếp theo của nút con đấy và sắp xếp lại.
		else {
			parent->child[pos]->value[parent->child[pos]->count] = value;
			parent->child[pos]->count++;
			QuickSort(parent->child[pos]->value, 0, parent->child[pos]->count - 1);
		}
	}
}

// Hàm xóa một khóa trong nút
void clear(Node*& del, int pos) {
	// dồn các giá trị đằng sau pos lên.
	for (int i = pos + 1; i<= del->count; i++) {
		del->value[i - 1] = del->value[i];
	}
	// nếu nút chỉ có đúng một khóa đấy thì gán số lượng khóa = 0, và cho địa chỉ của nút đấy = NULL
	if (del->count == 1) {
		del->count = 0;
		del = NULL;
	} // nếu nút có nhiều hơn 1 khóa thì giảm số lượng khóa đi một đơn vị
	else if (del->count > 1) {
		del->count--;
	}
}

void _delete(Node*& del, Node*& c,int pos,int p) {
	Node* tmp = c->child[c->count];
	while (tmp != NULL) { //Tìm nút con phải nhất 
		c = tmp;
		tmp = tmp->child[del->count];
	}
	// thế giá trị cần xóa = giá trị lớn nhất của nút con phải nhất
	del->value[pos] = c->value[c->count - 1];
	// sau đấy xóa khóa lớn nhất của nút con phải nhất.
	clear(c, c->count - 1);
}

void DeleteNode(Node*& root, int value) {
	Node* del = search(root, value); // tìm nút chứa khóa cần xóa
	if (del == NULL)return; // nếu nút chứa khóa đấy không tồn tại trong cây thì return
	int pos;
	if (findNode(del, value, pos) == 0) return;
	pos--; // tìm vị trí của khóa đấy trong nút
	Node* child_left = del->child[pos]; // địa chỉ nút con trái của khóa cần xóa
	Node* child_right = del->child[pos + 1]; // điạ chỉ nút con phải của khóa cần xóa
	if (child_left == NULL && child_right == NULL) { // nếu khóa cần xóa trống cả 2 nút con thì ta chỉ cần xóa khóa đấy
		clear(del, pos);
	}
	else {
		// Nếu nút con phải khác NULL thì 
		if (child_left != NULL) {
			Node* tmp = child_left->child[child_left->count];
			while (tmp != NULL) { //Tìm nút con phải nhất 
				child_left = tmp;
				tmp = tmp->child[tmp->count];
			}
			// thế giá trị cần xóa = giá trị lớn nhất của nút con phải nhất
			del->value[pos] = child_left->value[child_left->count - 1];
			// sau đấy xóa khóa lớn nhất của nút con phải nhất.
			clear(child_left, child_left->count - 1);
		}
		else { // Nếu nút con phải NULL, con trái khác NULL thì
			Node* tmp2 = child_right->child[0];
			while (tmp2 != NULL) { // tìm nút con trái nhất
				child_right = tmp2;
				tmp2 = tmp2->child[0];
			}
			// thế giá trị cần xóa = giá trị nhỏ nhất của nút con trái nhất
			del->value[pos] = child_right->value[0];
			// sau đấy xóa khóa nhỏ nhất của nút con trái nhất.
			clear(child_right, 0);
		}
	}
}


int main() {
	MwayTree t;
	Initialize(t);
	InsertNode(t.root, 18);
	InsertNode(t.root, 44);
	InsertNode(t.root, 76);
	InsertNode(t.root, 198);
	InsertNode(t.root, 7);
	InsertNode(t.root, 12);
	InsertNode(t.root, 80);
	InsertNode(t.root, 92);
	InsertNode(t.root, 141);
	InsertNode(t.root, 77);
	InsertNode(t.root, 148);
	InsertNode(t.root, 151);
	InsertNode(t.root, 172);
	InsertNode(t.root, 186);
	cout << "BFT : ";
	BFT(t.root);
	cout << endl << "DFT : ";
	DFT(t.root);
	return 0;
}