#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
struct point2d {
    int x,y;
};
typedef struct
{
	int source;
	int dest;
	float weight;
}Edge;

typedef struct
{
	int V;
	int E;
	Edge* edges;
}Graph;
typedef struct
{
	int parent;
	int rank;
}Set;

int find(Set set[], int i)
{
	if(set[i].parent != i)
		set[i].parent = find(set, set[i].parent);

	return set[i].parent;
}
void Union(Set set[], int a, int b)
{
	int root_a = find(set, a);
	int root_b = find(set, b);

	if(set[root_a].rank < set[root_b].rank)
		set[root_a].parent = root_b;
	else if(set[root_a].rank > set[root_b].rank)
		set[root_b].parent = root_a;
	else
	{
		set[root_a].parent = root_b;
		set[root_b].rank++;
	}
}
int cmpX(const void* p1, const void* p2)
{
    struct point2d *a, *b;
    a = (struct point2d*) p1;
    b = (struct point2d*) p2;
    return (a->x - b->x);
}
int cmpY(const void* p1, const void* p2)
{
    struct point2d *a, *b;
    a = (struct point2d*) p1;
    b = (struct point2d*) p2;
    return (a->y - b->y);
}
float dist(struct point2d p1, struct point2d p2)
{
    return sqrt( (p1.x - p2.x)*(p1.x - p2.x) +
                 (p1.y - p2.y)*(p1.y - p2.y)
               );
}
int cmp_edge_weight(const void* a, const void *b)
{
	Edge *e1, *e2;
	e1 = (Edge *) a;
	e2 = (Edge *) b;
	return (e1->weight > e2->weight)? 1 : -1;
}



Graph* create_graph(int V)
{
	Graph* graph = (Graph *) malloc(sizeof(Graph));
	graph->V = V;
	graph-> E = 0;
	graph->edges = (Edge *) malloc(graph->V * graph->V * sizeof(Edge));
	return graph;
}
void add_edge(Graph *graph, int s, int t, float w)
{
	graph->edges[graph->E].source = s;
	graph->edges[graph->E].dest = t;
	graph->edges[graph->E].weight = w;
	graph->E++;
}
void Kruskal_MST(Graph *graph, Edge res_edges[])
{
	int V = graph->V;
	//Edge res_edges[V-1];

	qsort(graph->edges, graph->E, sizeof(graph->edges[0]), cmp_edge_weight);


	Set *sets = (Set *) malloc(V * sizeof(Set));

	for(int i = 0; i < V; i++)
	{
		sets[i].parent = i;
		sets[i].rank = 0;
	}

	int e_added = 0;
	int e_current = 0;
	while(e_added < V - 1)
	{
		Edge current_edge = graph->edges[e_current];
		int x = find(sets, current_edge.source);
		int y = find(sets, current_edge.dest);

		if(x != y)
		{
			res_edges[e_added++] = current_edge;
			Union(sets, x, y);
		}
		e_current++;
	}
	/*printf("Following are the edges in the constructed MST:\n");
    for (int i = 0; i < e_added; ++i)
    {
        printf("%d -- %d == %d\n", res_edges[i].source, res_edges[i].dest,
                                                   res_edges[i].weight);
    }*/
    return;
}
void euclideanMST(struct point2d **mst,struct point2d *points,int n)
{
	/* find out the Euclidean MST and store the MST in 2d-array mst.
	Each of the n-1 edges are store by storing it's two endpoints.
	*/
	
	//qsort(points, n, sizeof(struct point2d), cmpX);
	Edge res_edges[n-1];
	Graph *graph = create_graph(n);
	for(int i = 0 ; i < n; i++)
	{
		for(int j = i + 1; j < n; j++)
		{
			add_edge(graph, i, j, dist(points[i], points[j]));
		}
	}
	Kruskal_MST(graph, res_edges);
	printf("Following are the edges in the constructed MST:\n");
	for(int i = 0; i < n - 1; i++)
	{
		printf("(%d,%d) -- (%d,%d)   ==   %f\n", points[res_edges[i].source].x, points[res_edges[i].source].y, points[res_edges[i].dest].x, points[res_edges[i].dest].y, res_edges[i].weight);
	} 
}


int main()
{
    struct point2d *points,close2d[2],**mst;
    //struct point3d *plane,close3d[2];
    int i,n,m;
    scanf("%d",&n);
    points = (struct point2d *)malloc(n*sizeof(struct point2d));
    for(i=0;i<n;i++)
        scanf("%d%d",&(points[i].x),&(points[i].y));

    mst = (struct point2d **)malloc((n-1)*sizeof(struct point2d *));

    for(i=0;i<n;i++)
        mst[i]=(struct point2d *)malloc(3*sizeof(struct point2d));

    euclideanMST(mst,points,n);
    return 0;
}


