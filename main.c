
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WIDTH   39
#define HEIGHT  21
#define WALL    0
#define FLOOR   1
#define WALL_C  "##"
#define FLOOR_C "  "
#define randint(max) (rand() % max)

typedef struct Point Point;
struct Point {
    int x;
    int y;
};

const Point DIRS[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

int     width   = WIDTH;
int     height  = HEIGHT;
long    seed    = -1;
int     *map    = 0;

void    init();
void    draw_map();
void    set_tile(int, int, int);
int     get_tile(int, int);
void    generate();
Point   random_neighbor(int, int);
int     has_neighbor(int, int);
void    *xmalloc(size_t);



int main(int argc, char **argv) {
    for(int i = 1; i < argc; i++) {
        if(!strcmp(argv[i], "-h")) {
            printf("usage: %s [-W <width>] [-H <height>] [--seed <seed>]\n", argv[0]);
            return 0;
        }
        if(!strcmp(argv[i], "-W") && i + 1 < argc) {
            sscanf(argv[++i], "%d", &width);
            if(width % 2 == 0 && --width < 5) width = 5;
        }
        if(!strcmp(argv[i], "-H") && i + 1 < argc) {
            sscanf(argv[++i], "%d", &height);
            if(height % 2 == 0 && --height < 5) height = 5;
        }
        if(!strcmp(argv[i], "--seed") && i + 1 < argc) {
            sscanf(argv[++i], "%ld", &seed);
        }
    }
    if(seed == -1) seed = time(0);
    srand(seed);
    init();
    generate();
    draw_map();
}



void init() {
    map = xmalloc(sizeof(int) * width * height);
    memset(map, WALL, width * height);
}

void draw_map() {
    if(!map) {
        printf("map has not been initialized!\n");
        exit(1);
    }
    printf("\n");
    for(int y = 0; y < height; y++) {
        char buffer[width * 2 + 1];
        memset(buffer, 0, width * 2 + 1);
        for(int x = 0; x < width; x++) {
            if(get_tile(x, y) == WALL) {
                strcpy(&buffer[x*2], WALL_C);
            }
            else if(get_tile(x, y) == FLOOR) {
                strcpy(&buffer[x*2], FLOOR_C);
            }
        }
        printf("%s\n", buffer);
    }
    printf("\n");
}

void set_tile(int x, int y, int tile) {
    map[x + y * width] = tile;
}

int get_tile(int x, int y) {
    return map[x + y * width];
}

void generate() {
    int nodes = (width - 1) * (height - 1) / 4 - 1;
    int x = 1, y = 1;
    set_tile(x, y, FLOOR);
    Point *visited = xmalloc(sizeof(Point) * nodes);
    int visitc = 0;
    visited[visitc++] = (Point) {x, y};
    while(nodes > 0) {
        if(!has_neighbor(x, y)) {
            Point p = visited[--visitc];
            x = p.x, y = p.y;
            continue;
        }
        Point next = random_neighbor(x, y);
        for(int i = 1; i < 3; i++) {
            set_tile(x + next.x * i, y + next.y * i, FLOOR);
        }
        nodes--;
        visited[visitc++] = (Point) {x, y};
        x += next.x * 2;
        y += next.y * 2;
    }
}

Point random_neighbor(int x, int y) {
    Point dir;
    int dx, dy;
    while(1) {
        dir = DIRS[randint(4)];
        dx = dir.x;
        dy = dir.y;
        if(x + dx * 2 < 0 || x + dx * 2 >= width) continue;
        if(y + dy * 2 < 0 || y + dy * 2 >= height) continue;
        if(get_tile(x + dx * 2, y + dy * 2) != FLOOR) break;
    }
    return dir;
}

int has_neighbor(int x, int y) {
    for(int i = 0; i < 4; i++) {
        Point p = DIRS[i];
        if(x + p.x * 2 < 0 || x + p.x * 2 >= width) continue;
        if(y + p.y * 2 < 0 || y + p.y * 2 >= height) continue;
        if(get_tile(x + p.x * 2, y + p.y * 2) == WALL) return 1;
    }
    return 0;
}

void *xmalloc(size_t size) {
    void *p = malloc(size);
    if(!p) {
        printf("failed to allocate %zu bytes of memory\n", size);
        exit(1);
    }
    return p;
}
