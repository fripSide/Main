#include <cstdio>
#include <cstring>
#include <cmath>

// https://codingcompetitions.withgoogle.com/kickstart/round/0000000000050ee2/0000000000051136
const int maxn = 105;
char buf[maxn];
typedef long long LL;

struct Tire {
	LL cnt;
	int lev;
	bool isBan;
	Tire *child[2];
	Tire(): cnt(0), lev(0), isBan(false) {
		memset(child, 0, sizeof child);
	}
};

void build_tree(Tire *root, char *buf, int len) {
	Tire * cur = root;
	for (int i = 0; i < len; i++) {
		int idx = 0;
		if (buf[i] == 'R') {
			idx = 1;
		}
		if (cur->child[idx] == NULL) {
			cur->child[idx] = new Tire();
		}
		cur = cur->child[idx];
		cur->isBan = false;
		cur->lev = i;
	}
	cur->isBan = true;
}

LL Calc(Tire *root, int N) {
	LL ret = 0;
	for (int i = 0; i < 2; i++) {
		if (root->child[i] != NULL) {
			ret += Calc(root->child[i], N);
		} else {
			int lev = root->lev;
			ret += pow(2, N - lev - 1);
		}
	}
	return ret;
}

int main() {
	int T;
	scanf("%d", &T);
	for (int tCase = 1; tCase <= T; tCase++) {
		LL ans;
		Tire root;
		int N, P;
		scanf("%d %d", &N, &P);
		for (int i = 0; i < P; i++) {
			scanf("%s", buf);
			int len = strlen(buf);
			build_tree(&root, buf, len);
		}
		ans = Calc(&root, N);
		printf("Case #%d: %lld\n", tCase, ans);
	}
	return 0;
}

/* 解题报告：
题意：一个长度为N的R、B两个字符组成的串，有P个不能出现的前缀，问这样的字符串有多少种。
实现：前缀树。
*/