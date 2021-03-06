#include <bits/stdc++.h>
using namespace std;

#define double long double
const int MAXF = 1 << 17;
struct cp {
    double x, y;
    cp(double x = 0, double y = 0) : x(x), y(y) {}
    cp operator + (const cp& rhs) const {
        return cp(x + rhs.x, y + rhs.y);
    }
    cp operator - (const cp& rhs) const {
        return cp(x - rhs.x, y - rhs.y);
    }
    cp operator * (const cp& rhs) const {
        return cp(x * rhs.x - y * rhs.y, x * rhs.y + y * rhs.x);
    }
    cp operator !() const {
        return cp(x, -y);
    }
} rts[MAXF + 1];
cp fa[MAXF], fb[MAXF];
cp fc[MAXF], fd[MAXF];

int bitrev[MAXF];
void fftinit() {
    int k = 0; while ((1 << k) < MAXF) k++;
    bitrev[0] = 0;
    for (int i = 1; i < MAXF; i++) {
        bitrev[i] = bitrev[i >> 1] >> 1 | ((i & 1) << k - 1);
    }
    double PI = acos((double) -1.0);
    cp w = cp(cos(2 * PI / MAXF), sin(2 * PI / MAXF));
    rts[0] = rts[MAXF] = cp(1, 0);
    for (int i = 1; i + i <= MAXF; i++) {
        rts[i] = rts[i - 1] * w;
    }
    for (int i = MAXF / 2 + 1; i < MAXF; i++) {
        rts[i] = !rts[MAXF - i];
    }
}
void dft(cp a[], int n, int sign) {
    int d = 0; while ((1 << d) * n != MAXF) d++;
    for (int i = 0; i < n; i++) {
        if (i < (bitrev[i] >> d)) {
            swap(a[i], a[bitrev[i] >> d]);
        }
    }
    for (int len = 2; len <= n; len <<= 1) {
        int delta = MAXF / len * sign;
        for (int i = 0; i < n; i += len) {
            cp *x = a + i,*y = a + i + (len >> 1), *w = sign > 0 ? rts : rts + MAXF;
            for (int k = 0; k + k < len; k++) {
                cp z = *y * *w;
                *y = *x - z, *x = *x + z;
                x++, y++, w += delta;
            }
        }
    }
    if (sign < 0) {
        for (int i = 0; i < n; i++) {
            a[i].x /= n;
            a[i].y /= n;
        }
    }
}
void multiply(int a[], int b[], int na, int nb, long long c[]) {
    int n = na + nb - 1; while (n != (n & -n)) n += n & -n;
    for (int i = 0; i < n; i++) fa[i] = fb[i] = cp();
    for (int i = 0; i < na; i++) fa[i] = cp(a[i]);
    for (int i = 0; i < nb; i++) fb[i] = cp(b[i]);
    dft(fa, n, 1), dft(fb, n, 1);
    for (int i = 0; i < n; i++) fa[i] = fa[i] * fb[i];
    dft(fa, n, -1);
    for (int i = 0; i < n; i++) c[i] = (long long) floor(fa[i].x + 0.5);
}
void multiply(int a[], int b[], int na, int nb, int c[], int mod = (int) 1e9 + 7) {
    int n = na + nb - 1;
    while (n != (n & -n)) n += n & -n;
    for (int i = 0; i < n; i++) fa[i] = fb[i] = cp();
    static const int magic = 15;
    for (int i = 0; i < na; i++) fa[i] = cp(a[i] >> magic, a[i] & (1 << magic) - 1);
    for (int i = 0; i < nb; i++) fb[i] = cp(b[i] >> magic, b[i] & (1 << magic) - 1);
    dft(fa, n, 1), dft(fb, n, 1);
    for (int i = 0; i < n; i++) {
        int j = (n - i) % n;
        cp x = fa[i] + !fa[j];
        cp y = fb[i] + !fb[j];
        cp z = !fa[j] - fa[i];
        cp t = !fb[j] - fb[i];
        fc[i] = (x * t + y * z) * cp(0, 0.25);
        fd[i] = x * y * cp(0, 0.25) + z * t * cp(-0.25, 0);
    }
    dft(fc, n, -1), dft(fd, n, -1);
    for (int i = 0; i < n; i++) {
        long long u = ((long long) floor(fc[i].x + 0.5)) % mod;
        long long v = ((long long) floor(fd[i].x + 0.5)) % mod;
        long long w = ((long long) floor(fd[i].y + 0.5)) % mod;
        c[i] = ((u << 15) + v + (w << 30)) % mod;
    }
}
vector<int> multiply(vector<int> a, vector<int> b, int mod = (int) 1e9 + 7) {
    static int fa[MAXF], fb[MAXF], fc[MAXF];
    int na = a.size(), nb = b.size();
    for (int i = 0; i < na; i++) fa[i] = a[i];
    for (int i = 0; i < nb; i++) fb[i] = b[i];
    multiply(fa, fb, na, nb, fc, mod);
    int k = na + nb - 1;
    vector<int> res(k);
    for (int i = 0; i < k; i++) res[i] = fc[i];
    return res;
}

const int mod = (int) 1e9 + 7;
int n;
int a[MAXF];
int b[MAXF];
int c[MAXF];
int d[MAXF];

vector<int> divide(int L, int R) {
    if (L == R) {
        vector<int> res(L + 1, 0);
        res[0] = 1;
        res[L] = mod - 1;
        return res;
    }
    return multiply(divide(L, L + R >> 1), divide((L + R >> 1) + 1, R));
}

int main() {
    srand(time(NULL));
    fftinit();
    n = 1000;
    for (int i = 0; i < n; i++) {
        a[i] = rand();
        b[i] = rand();
    }
    multiply(a, b, n, n, c, mod);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            d[i + j] += (long long) a[i] * b[j] % mod;
            d[i + j] %= mod;
        }
    }
    for (int i = 0; i < n + n - 1; i++) {
        assert(c[i] == d[i]);
    }
    cerr << "Correct\n";
    cerr << "\nTime elapsed: " << 1000 * clock() / CLOCKS_PER_SEC << "ms\n";
    return 0;
}
