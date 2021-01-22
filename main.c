#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define ARRAY_SIZE 18626
#define MAX_BUFFER 256 


struct Game {

  int StopId;
  char Name[MAX_BUFFER];
  float Latitude,Longitude;
	};


 struct Edge{
  int start;
  int end;
  int weight;
};

typedef struct {
    int vertex;
    int weight;
} edge_t;
 
typedef struct {
    edge_t **edges;
    int edges_len;
    int edges_size;
    int dist;
    int prev;
    int visited;
    struct Game now;
} vertex_t;
 
typedef struct {
    vertex_t **vertices;
    int vertices_len;
    int vertices_size;
} graph_t;

int edge;
 
typedef struct {
    int *data;
    int *prio;
    int *index;
    int len;
    int size;
} heap_t;
 
void add_vertex (graph_t *g, int i) {

    if (g->vertices_size < i + 1) {
        int size = g->vertices_size * 2 > i ? g->vertices_size * 2 : i + 4;
        g->vertices = realloc(g->vertices, size * sizeof (vertex_t *));
        for (int j = g->vertices_size; j < size; j++)
            g->vertices[j] = NULL;
        g->vertices_size = size;
    }
    if (!g->vertices[i]) {
        g->vertices[i] = calloc(1, sizeof (vertex_t));
        g->vertices_len++;
    }
}
 
void add_edge (graph_t * g, struct Edge* YEE) {
    int a = YEE->start;
    int b = YEE->end;
    int w = YEE->weight;
  
    // a = a - 'a';
    // b = b - 'a';
    add_vertex(g, a);
    add_vertex(g, b);
    vertex_t *v = g->vertices[a];

    if (v->edges_len >= v->edges_size) {
        v->edges_size = v->edges_size ? v->edges_size * 2 : 4;
        v->edges = realloc(v->edges, v->edges_size * sizeof (edge_t *));
    }
   
    edge_t *e = calloc(1, sizeof (edge_t));
    e->vertex = b;
    e->weight = w;
    v->edges[v->edges_len++] = e;

    vertex_t *bv = g->vertices[b];

    if (bv->edges_len >= bv->edges_size) {
        bv->edges_size = bv->edges_size ? bv->edges_size * 2 : 4;
        bv->edges = realloc(bv->edges, bv->edges_size * sizeof (edge_t *));
    }
   
    edge_t *be = calloc(1, sizeof (edge_t));
    be->vertex = a;
    be->weight = w;
    bv->edges[bv->edges_len++] = be;

    edge++;
}

void assignStop(graph_t * g, struct Game stop ){

    vertex_t *v = g->vertices[stop.StopId];
   // printf("%s\t%f\t%f\n", stop.Name,stop.Longitude,stop.Latitude);
    v->now = stop;
    g->vertices[stop.StopId] = v;
}
 
heap_t *create_heap (int n) {
    heap_t *h = calloc(1, sizeof (heap_t));
    h->data = calloc(n + 1, sizeof (int));
    h->prio = calloc(n + 1, sizeof (int));
    h->index = calloc(n, sizeof (int));
    return h;
}
 
void push_heap (heap_t *h, int v, int p) {
    int i = h->index[v] == 0 ? ++h->len : h->index[v];
    int j = i / 2;
    while (i > 1) {
        if (h->prio[j] < p)
            break;
        h->data[i] = h->data[j];
        h->prio[i] = h->prio[j];
        h->index[h->data[i]] = i;
        i = j;
        j = j / 2;
    }
    h->data[i] = v;
    h->prio[i] = p;
    h->index[v] = i;
}
 
int min (heap_t *h, int i, int j, int k) {
    int m = i;
    if (j <= h->len && h->prio[j] < h->prio[m])
        m = j;
    if (k <= h->len && h->prio[k] < h->prio[m])
        m = k;
    return m;
}
 
int pop_heap (heap_t *h) {
    int v = h->data[1];
    int i = 1;
    while (1) {
        int j = min(h, h->len, 2 * i, 2 * i + 1);
        if (j == h->len)
            break;
        h->data[i] = h->data[j];
        h->prio[i] = h->prio[j];
        h->index[h->data[i]] = i;
        i = j;
    }
    h->data[i] = h->data[h->len];
    h->prio[i] = h->prio[h->len];
    h->index[h->data[i]] = i;
    h->len--;
    return v;
}
 
void dijkstra (graph_t *g, int a, int b) {
    int i, j;
    // a = a - 'a';
    // b = b - 'a';
    if(!g->vertices[a]||!g->vertices[b]){
      printf("Not valid Stop\n");
        return;
    }

    for (i = 0; i < g->vertices_len; i++) {
        vertex_t *v = g->vertices[i];
        if (v) {
            v->dist = INT_MAX;
            v->prev = 0;
           v->visited = 0;
        }
    }
    vertex_t *v = g->vertices[a];
    v->dist = 0;
    heap_t *h = create_heap(g->vertices_len);
    push_heap(h, a, v->dist);
    while (h->len) {
        i = pop_heap(h);
        if (i == b)
            break;
        v = g->vertices[i];
        v->visited = 1;
        for (j = 0; j < v->edges_len; j++) {
            edge_t *e = v->edges[j];
            vertex_t *u = g->vertices[e->vertex];
            if (!u->visited && v->dist + e->weight <= u->dist) {
                u->prev = i;
                u->dist = v->dist + e->weight;
                push_heap(h, e->vertex, u->dist);
            }
        }
    }
   
    //printf("IAMSEXY\n");
}
 
void print_path (graph_t *g, int i) {
    int n, j;
    vertex_t *v, *u, *x;
    struct Game gge;
    struct Game ggne;
    struct Game rout[n];
    
    if(!g->vertices[i]){
      printf("no path\n");
        return;
    }

     v = g->vertices[i];
    // gge = v->now;
    
    //printf("\n%s ", gge.Name);
    if (v->dist == INT_MAX) {
        gge = v->now;
        //printf("\n%s ", gge.Name);
        printf("no path\n");
        return;
    }
    v = g->vertices[i];
    gge = v->now;
    //printf("->%i", i);
    for (n = 1, u = v; u->dist; u = g->vertices[u->prev], n++){
      x = g->vertices[u->prev];
      rout[n] = x->now;
      // gge = x->now;
      // printf("\n%s ", gge.Name);
      // printf("->%i", u->prev);
    }
    printf("\n");
    char *path = malloc(n);
    path[n - 1] =  i;
    for (j = 0, u = v; u->dist; u = g->vertices[u->prev], j++){
        path[n - j - 2] = u->prev;
        
    }
    for (n=n-1; n>0; n--){
      // rout[n] = x->now;
      gge = rout[n];
      printf("\n%i: %s\t%f\t%f\n", gge.StopId, gge.Name,gge.Latitude,gge.Longitude);
      //printf("", u->prev);
    }
    v = g->vertices[i];
    gge = v->now;
    printf("\n%i: %s\t%f\t%f\n", gge.StopId, gge.Name,gge.Latitude,gge.Longitude);
    
    printf("\nDistance Covered: %d \n", v->dist);
}


int
next_field( FILE *f, char *buf, int max ) {
	int i=0, end=0, quoted=0;
	
	for(;;) {
		// fetch the next character from file		
		buf[i] = fgetc(f);
		// if we encounter quotes then flip our state and immediately fetch next char
		if(buf[i]=='"') { quoted=!quoted; buf[i] = fgetc(f); }
		// end of field on comma if we're not inside quotes
		if(buf[i]==',' && !quoted) { break; }
		// end record on newline or end of file
		if(feof(f) || buf[i]=='\n') { end=1; break; } 
		// truncate fields that would overflow the buffer
		if( i<max-1 ) { ++i; } 
	}

	buf[i] = 0; // null terminate the string
	return end; // flag stating whether or not this is end of the line
}

void
fetch_game (  FILE *csv, struct Game *p) {
	char buf[MAX_BUFFER];
  next_field( csv, buf, MAX_BUFFER );      // load id into buffer as string
	p->StopId = atoi(buf);
  next_field( csv, p->Name, MAX_BUFFER );
  
	next_field( csv, buf, MAX_BUFFER );      // load id into buffer as string
	p->Latitude = atof(buf);  
  next_field( csv, buf, MAX_BUFFER );      // load id into buffer as string
	p->Longitude = atof(buf);                       // then parse to integer
  //printf("%s ", p->Name);
  
}

void
fetch_edge (graph_t *g,  FILE *csv, struct Edge *p) {
	char buf[MAX_BUFFER];
  next_field( csv, buf, MAX_BUFFER );      // load id into buffer as string
	p->start = atoi(buf);
  next_field( csv, buf, MAX_BUFFER );      // load id into buffer as string
	p->end = atoi(buf);
  next_field( csv, buf, MAX_BUFFER );      // load id into buffer as string
	p->weight = atoi(buf);

  add_edge (g, p);
  //add_edge (g, p->start, p->end, p->weight);
}



int main(int argc, char *argv[]) {
  graph_t *g = calloc(1, sizeof(graph_t));
  FILE* f = (FILE*)malloc( sizeof( FILE ) ); 
  int nbGames = 0;
  int nbEdges = 0;
  //g = (struct graph_t*)malloc( sizeof( struct graph_t ) *ARRAY_SIZE );
  struct Game * pArray = (struct Game*)malloc( sizeof( struct Game ) *ARRAY_SIZE );
  struct Edge * eArray = (struct Edge*)malloc( sizeof( struct Edge ) *ARRAY_SIZE );
	//struct Game pArray[MAX_NB_GAMES];		
	struct Game p;
  struct Edge e;
  
	
	 // discard the header data in the first line
	
	

  f = fopen("edges.csv", "r");
	if(!f) { 
		printf("unable to open %s\n", argv[1]); 
		return EXIT_FAILURE; 
	}

  fetch_edge(g, f, &e);
  while(!feof(f)) {
		
		fetch_edge(g, f, &eArray[nbEdges] );
       
		nbEdges++;
	}
  
  f = fopen("vertices.csv", "r");
	if(!f) { 
		printf("unable to open %s\n", argv[1]); 
		return EXIT_FAILURE; 
	}
	
	fetch_game( f, &p );
  while(!feof(f)) {
		
		fetch_game( f, &pArray[nbGames] );
    assignStop(g, pArray[nbGames] );
		//print_pokemon( pArray[nbGames] );
		// printf("\n");
		nbGames++;
	}


  printf("----------------STATS----------------\n");
  printf("NO. STOPS:\t%i\n", (g->vertices_len-1));
  printf("NO. EDGES:\t%i\n", (edge-1));

  char c;
  int first = 0, second = 0;

   while(c != 'q'){
printf("\nWhere are you, And where would you like to go!!\n");
 scanf("%i%i", &first, &second);
if ((first > 7667) || (second > 7667)){
  printf("Those bus stops arent available!\nEnter another two stops less that 7667!\n");
  scanf("%i%i", &first, &second);
}
if ((first < 0) || (second < 0)){
  printf("Those bus stops arent available!\nEnter an ActuAl bus stop number!\n");
  scanf("%i%i", &first, &second);
}


   dijkstra(g, first, second);
   print_path (g, second);
   

printf("\nWould you like to continue? Press any key to continue (Type q to end)\n");
  scanf(" %c", &c);
  if(c == 'q'){
    return 0;
  }
   }


 

  // vertex_t *ne =  g->vertices[10];
  // struct Game gge = ne->now;
  // printf("\n%s , %i", gge.Name, gge.StopId);

  return 0;
}