#include <stdio.h>
#include <stdlib.h>

#define ll long long

int cnt, head[60008], dfn[60008], low[60008], num, in[60008];

int f[60008];

struct Stack {
    int val[60008];
    int top;
};

typedef struct Stack* stk;

stk new_st()
{
    stk x = (stk)malloc(sizeof(struct Stack));
    x->top = 0;
    return x;
}

stk ele;

struct edge {
    int id, nex;
} ed[200005];

int min(int x, int y)
{
    if (x < y)
        return x;
    return y;
}

void adde(int x, int y)
{
    ed[++cnt] = (struct edge) { y, head[x] }, head[x] = cnt;
}

int find(int x)
{
    if (f[x] == x)
        return x;
    return f[x] = find(f[x]);
}

void match(int x, int y)
{
    x = find(x), y = find(y);
    if (x < y)
        f[y] = x;
    else
        f[x] = y;
}

int top(stk st)
{
    return st->val[st->top - 1];
}

void pop(stk st)
{
    st->top--;
}

void push(stk st, int x)
{
    st->val[st->top++] = x;
}

int read()
{
    int s = 0, f = 1;
    char ch = getchar();
    for (; ch < '0' || ch > '9'; ch = getchar())
        if (ch == '-')
            f = -1;
    for (; ch >= '0' && ch <= '9'; ch = getchar())
        s = (s << 3) + (s << 1) + (ch & 15);
    return s * f;
}

void dfs(int x)
{
    dfn[x] = low[x] = ++num;
    push(ele, x), in[x] = 1;
    for (int i = head[x]; i; i = ed[i].nex) {
        int v = ed[i].id;
        if (!dfn[v]) {
            dfs(v);
            low[x] = min(low[x], low[v]);
        } else if (in[v])
            low[x] = min(low[x], dfn[v]);
    }
    if (dfn[x] == low[x]) {
        int v = 0;
        while (v != x) {
            v = top(ele);
            pop(ele);
            in[v] = 0;
            match(x, v);
        }
    }
}

int main()
{
    int n = read(), m = read(), ans = 0;
    for (int i = 1; i <= m; i++) {
        int x = read(), y = read();
        adde(x, y);
    }
    for (int i = 1; i <= n; i++)
        f[i] = i;
    ele = new_st();
    for (int i = 1; i <= n; i++)
        if (!dfn[i])
            dfs(i);
    for (int i = 1; i <= n; i++)
        if (f[i] == i)
            ans++;
    printf("%d\n", ans);

//    for (int i = 1; i <= n; i++) {
//        if (f[i] == i) {
//            for (int j = i; j <= n; j++) {
//                if (find(j) == i)
//                    printf("%d ", j);
//            }
//            printf("\n");
//        }
//    }
    return 0;
}