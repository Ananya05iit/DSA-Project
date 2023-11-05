#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
vector<long long> v1;
int count_kmp = 0;
void computeLPSArray(string pat, int M, int *lps)
{
	int len = 0;

	lps[0] = 0;

	int i = 1;
	while (i < M)
	{
		if (pat[i] == pat[len])
		{
			len++;
			lps[i] = len;
			i++;
		}
		else
		{
			if (len != 0)
			{
				len = lps[len - 1];
			}
			else
			{
				lps[i] = 0;
				i++;
			}
		}
	}
}

void KMPSearch(string &pat, string &txt)
{
	int M = pat.length();
	int N = txt.length();
	int lps[M];

	computeLPSArray(pat, M, lps);

	int i = 0;
	int j = 0;
	while ((N - i) >= (M - j))
	{
		if (pat[j] == txt[i])
		{
			j++;
			i++;
		}

		if (j == M)
		{
			long long it = lower_bound(v1.begin(), v1.end(), i - j) - v1.begin();

			int c;
			if (it == 0)
			{
				c = i - j;
			}
			else
			{
				c = i - j - v1[it - 1];
			}
			if (it == 0)
			{
				c++;
			}
			count_kmp++;
			cout << "Found at line: " << it + 1 << " position: " << c << endl;

			j = lps[j - 1];
		}

		else if (i < N && pat[j] != txt[i])
		{
			if (j != 0)
				j = lps[j - 1];
			else
				i = i + 1;
		}
	}
}

int main()
{
	ifstream newfile;
	newfile.open("sherlock.txt");
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
	string s2;
	getline(cin, s2);
	KMPSearch(s2, s1);
	cout << "Number of Occurences: " << count_kmp;
}
