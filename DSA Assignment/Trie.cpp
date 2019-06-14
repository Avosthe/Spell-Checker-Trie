
#include "Trie.h"

Trie::Trie() {
	this->eow = false;

	for (int i = 0; i < MAX_SIZE; i++)
		this->children[i] = NULL;
}

bool Trie::insert(Trie* rootNode, string word) {



}