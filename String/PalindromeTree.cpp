#include <bits/stdc++.h>
using namespace std;

/*
* Complexity: O(N)
* Problems:
* 1. http://codeforces.com/gym/100548/status/G
*/

struct PalindromeTree {
	static const int MAXN = 100000 + 10;
	static const int MAXC = 26;
	int next[MAXN][MAXC], fail[MAXN], cnt[MAXN];
	int num[MAXN], len[MAXN], s[MAXN];
	int last, n, p;
	int newnode(int l) {
		for(int i = 0; i < MAXC; i++) next[p][i] = 0;
		cnt[p] = num[p] = 0;
		len[p] = l;
		return p++;
	}
	void init() {
		p = 0;
		newnode(0); newnode(-1);
		n = last = 0;
		s[n] = -1; fail[0] = 1;
	}
	int getfail(int x) {
		while (s[n - len[x] - 1] != s[n]) x = fail[x];
		return x;
	}
	void add(char c) {
		c -= 'a';
		s[++n] = c;
		int cur = getfail(last);
		if(next[cur][c] == 0) {
			int now = newnode(len[cur] + 2);
			fail[now] = next[getfail(fail[cur])][c];
			next[cur][c] = now;
			num[now] = num[fail[now]] + 1;
		}
		last = next[cur][c];
		cnt[last]++;
	}
	void count() {
		for (int i = p - 1; i >= 0; i--) cnt[fail[i]] += cnt[i];
	}
} pdt;

int main() {
	return 0;
}
