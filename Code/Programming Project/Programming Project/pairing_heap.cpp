#include "huffman_coding.h"

static pairing_node* pairing_tree;			//min pairing heap tree

//meld two pairing trees, sibling pointers of input roots must be NULL
static pairing_node* meld(pairing_node* root1, pairing_node* root2) {
	pairing_node* parent;
	pairing_node* child;

	if (root1 == NULL) {
		return root2;
	}
	if (root2 == NULL) {
		return root1;
	}
	if (root1->key < root2->key) {
		parent = root1;
		child = root2;
	} else {
		parent = root2;
		child = root1;
	}
	child->left_sibling = parent;
	child->right_sibling = parent->child;
	if (parent->child != NULL) {
		parent->child->left_sibling = child;
	}
	parent->child = child;
	return parent;
}

//insert into pairing heap
static void insert(pairing_node* node) {
	pairing_tree = meld(pairing_tree, node);
}

//remove and get min from pairing heap
//return: pointer - success; NULL - heap is empty
static pairing_node* remove_min(void) {
	pairing_node* min;
	pairing_node* child;
	pairing_node* cumulated_tree;
	pairing_node* subtree1;
	pairing_node* subtree2;

	if (pairing_tree == NULL) {
		return NULL;
	}
	child = pairing_tree->child;
	if (child != NULL) {
		child->left_sibling = NULL;
	}
	min = pairing_tree;
	min->child = NULL;
	//go to right most child
	while (child != NULL && child->right_sibling != NULL) {
		child = child->right_sibling;
	}
	//meld subtrees
	cumulated_tree = NULL;
	while (child != NULL) {
		subtree1 = child->left_sibling;
		subtree2 = child;
		child = subtree1 == NULL ? NULL : subtree1->left_sibling;
		//change the two nodes to real roots by cutting their sibling connections
		if (subtree1 != NULL) {
			subtree1->left_sibling = NULL;
			subtree1->right_sibling = NULL;
		}
		subtree2->left_sibling = NULL;
		subtree2->right_sibling = NULL;
		//two pass meld
		cumulated_tree = meld(meld(subtree1, subtree2), cumulated_tree);
	}
	pairing_tree = cumulated_tree;
	return min;
}

//build huffman tree using binary heap
huffman_node* build_huffman_tree_using_pairing_heap(int* frequency_table, int size_of_frequency_table) {
	huffman_node* hnode;
	pairing_node* pnode1;
	pairing_node* pnode2;
	pairing_node* pairing_heap;

	if (frequency_table == NULL || size_of_frequency_table < 0) {
		return NULL;
	}
	//initialize pairing heap
	pairing_heap = new pairing_node[size_of_frequency_table];
	for (int i = 0; i < size_of_frequency_table; i++) {
		if (frequency_table[i] > 0) {
			hnode = new_huffman_node();
			hnode->numeric = i;
			pairing_heap[i].key = frequency_table[i];
			pairing_heap[i].value_pointer = hnode;
			pairing_heap[i].left_sibling = NULL;
			pairing_heap[i].right_sibling = NULL;
			pairing_heap[i].child = NULL;
			insert(pairing_heap + i);
		}
	}
	//build huffman tree using greedy algorithm
	pnode1 = remove_min();
	pnode2 = remove_min();
	while (pnode2 != NULL) {
		hnode = new_huffman_node();
		hnode->left_child = (huffman_node*)(pnode1->value_pointer);
		hnode->right_child = (huffman_node*)(pnode2->value_pointer);
		//existing node can be reused, no need to new
		pnode1->key += pnode2->key;
		pnode1->value_pointer = hnode;
		insert(pnode1);
		pnode1 = remove_min();
		pnode2 = remove_min();
	}
	hnode = pnode1 == NULL ? NULL : (huffman_node*)(pnode1->value_pointer);
	delete[] pairing_heap;
	pairing_heap = NULL;
	return hnode;
}
