#include <stdio.h>

#define N 8

typedef struct {
    int x, y;
} Point;

//Oles oi epitreptes kiniseis
int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};

//entos skakieras
int inside(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}

//Ylopoish algorithmou BFS gia thn anazhthsh ths pio syntomhs diadromis
void knightShortestPath(Point start, Point end) {
    int visited[N][N] = {0};
    Point parent[N][N];
    Point q[N*N];
    int front = 0, rear = 0;

    q[rear++] = start;
    visited[start.x][start.y] = 1;

    while (front < rear) {
        Point p = q[front++];

        if (p.x == end.x && p.y == end.y) break;

	int i;
        for (i = 0; i < 8; i++) {
            int nx = p.x + dx[i], ny = p.y + dy[i];
            if (inside(nx, ny) && !visited[nx][ny]) {
                visited[nx][ny] = 1;
                parent[nx][ny] = p;
                q[rear++] = (Point){nx, ny};
            }
        }
    }

    Point path[N*N];
    int len = 0;
    Point p;
    for (p = end; !(p.x == start.x && p.y == start.y); p = parent[p.x][p.y])
    {
	path[len++] = p;
    }
    path[len++] = start;

    int i;
    for (i = len - 1; i >= 0; i--)
        printf("(%d, %d)\n", path[i].x, path[i].y);
}

int main() {
    Point start, end;
    printf("Eisagete arxh (x y): ");
    scanf("%d %d", &start.x, &start.y);
    printf("Eisagete (x y): ");
    scanf("%d %d", &end.x, &end.y);
    if(inside(start.x, start.y) && inside(end.x, end.y))
    {
	knightShortestPath(start, end);
    }
    else
    {
	printf("Oi times prepei na einai apo 0 0 mexri 7 7\n");
    }

}
