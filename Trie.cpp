#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#define MAX_CHAR 256
using namespace std;
vector<long long> v1;
int count_trie = 0;
class SuffixTrieNode
{
private:
	SuffixTrieNode *children[MAX_CHAR];
	list<int> *indexes;

public:
	SuffixTrieNode()
	{
		indexes = new list<int>;
		for (int i = 0; i < MAX_CHAR; i++)
			children[i] = NULL;
	}
	void insertSuffix(string suffix, int index);
	list<int> *search(string pat);
};
class SuffixTrie
{
private:
	SuffixTrieNode root;

public:
	SuffixTrie(string &txt)
	{
		for (int i = 0; i < txt.length(); i++)
			root.insertSuffix(txt.substr(i), i);
	}
	void search(string pat);
};
void SuffixTrieNode::insertSuffix(string s, int index)
{
	indexes->push_back(index);
	if (s.length() > 0)
	{
		char cIndex = s.at(0);
		if (children[cIndex] == NULL)
			children[cIndex] = new SuffixTrieNode();
		children[cIndex]->insertSuffix(s.substr(1), index + 1);
	}
}
list<int> *SuffixTrieNode::search(string s)
{
	if (s.length() == 0)
		return indexes;
	if (children[s.at(0)] != NULL)
		return (children[s.at(0)])->search(s.substr(1));
	else
		return NULL;
}

void SuffixTrie::search(string pat)
{
	list<int> *result = root.search(pat);
	if (result == NULL)
		;
	else
	{
		list<int>::iterator i;
		int patLen = pat.length();
		for (i = result->begin(); i != result->end(); ++i)
		{
			long long it = lower_bound(v1.begin(), v1.end(), *i - patLen) - v1.begin();
			int c;
			if (it == 0)
			{
				c = *i - patLen;
			}
			else
			{
				c = *i - patLen - v1[it - 1];
			}
			if (it == 0)
			{
				c++;
			}
			count_trie++;
			cout << "Found at line: " << it + 1 << " and position " << c << endl;
		}
	}
}
int main()
{
	ifstream newfile;
	newfile.open("sherlock2.txt");
	string s1;
	if (newfile.is_open())
	{
		string tp;
		while (getline(newfile, tp))
		{
			s1 += tp;
			s1 += ' ';
			if (v1.size() == 0)
			{
				v1.push_back(tp.size());
			}
			else
			{
				v1.push_back(v1.back() + tp.size() + 1);
			}
		}
		newfile.close();
	}
	SuffixTrie S(s1);
	string s2;
	getline(cin, s2);
	S.search(s2);
	cout << "Number of occurence " << count_trie;

	return 0;
}
