#pragma once
#include <map>
#include <vector>


class Trie
{
	class TrieNode;

	typedef std::map<char, TrieNode*> TrieMap;
	typedef std::map<char, TrieNode*>::iterator TrieMapIter;

	// Internal TrieNode class - not relevant outside Trie
	class TrieNode
	{
	public:
		TrieNode(char c = '\0', bool cw = false, TrieNode* p = nullptr) :
			val(c), parent(p), completesWord(cw)
		{
			children.clear();
		}

		char val;
		TrieNode* parent;
		bool completesWord;
		TrieMap children;
	};



public:
	Trie()
	{
		root = new TrieNode('\0', true);
	}


	~Trie()
	{

	}

	
	//Inserts a string into the trie
	void insert(std::string str) const
	{
		TrieNode* cur = root;
		TrieMapIter res;
		for (char iChar : str)
		{
			if ((res = cur->children.find(iChar)) != cur->children.end())
			{
				cur = res->second;
			}
			else
			{
				cur->children[iChar] = new TrieNode(iChar,false,cur);
				if((res = cur->children.find(iChar)) != cur->children.end())
					cur = res->second;
			}
		}
		cur->completesWord = true;
	}

	//Searches the trie for the string str
	bool search(std::string str) const
	{
		if (str.empty())
		{
			return true;
		}
		TrieNode* cur = root;
		TrieMapIter res;

		for (char iChar : str)
		{
			if ((res = cur->children.find(iChar)) != cur->children.end())
			{
				cur = res->second;
			}
			else
			{
				return false;
			}
		}

		return cur->completesWord;
	}

	//Removes a string str from the trie if it exists
	void remove(std::string str) const
	{
		TrieNode* cur = root;
		TrieMapIter res;
		int i = 0; //index needed to delete the word
		for (; i < str.length(); i++)
		{
			if ((res = cur->children.find(str[i])) != cur->children.end())
			{
				cur = res->second;
			}
			else return;
		}
		if (cur->completesWord)
		{
			cur->completesWord = false;
			while (cur->completesWord == false && cur->children.empty())
			{
				cur->children.erase(str[i--]);
				cur = cur->parent;
			}
		}
	}

	//Find all strings that completes the string prefix and stores it in the vector strings
	void findAllWithPrefix(std::string prefix, std::vector<std::string>& strings) const
	{
		strings.clear();
		if (prefix.empty()) {
			strings.push_back(prefix);
		}
		TrieNode* cur = root;
		if (findPrefixEnd(prefix, cur) == prefix.length())
			recursiveFindWords(prefix, cur, strings);
	}



private:

	// findPrefixEnd: 
	// Sets the parameter end to the last node in the prefix of str and returns the length of the prefix
	unsigned int findPrefixEnd(std::string str, TrieNode*& end) const
	{
		TrieNode* cur = root;
		TrieMapIter res;
		unsigned int i = 0;

		// Find (part of) key already in trie
		for (; i < str.length(); i++)
		{
			if ((res = cur->children.find(str[i])) != cur->children.end())
			{
				cur = res->second;
			}
			else break;
		}

		end = cur;
		return i;
	}

	//Helper function for the findAllWithPrefix, that recursively find words with the string prefix.
	void recursiveFindWords(std::string prefix,TrieNode* prefixEndNode, std::vector<std::string>& strings) const
	{
		if (prefixEndNode->completesWord)
			strings.push_back(prefix);
		for (TrieMapIter i = prefixEndNode->children.begin(); i != prefixEndNode->children.end(); ++i)
		{
			recursiveFindWords(prefix + i->first,i->second, strings);
		}
	}


	TrieNode* root;
};
