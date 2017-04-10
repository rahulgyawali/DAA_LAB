#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct point2d {
  int x,y;
};

struct point3d {
  int x,y,z;
};

void closestpair2d(struct point2d *,struct point2d *, int);
float distance_2d(struct point2d P1, struct point2d P2);
void merge_sort_2d(struct point2d points[], int p, int r, char dim);
void merge_2d(struct point2d points[], int p, int q, int r, char dim);
void bruteForceClosest_2d(struct point2d *close, struct point2d P[], int n);
void closestpair2dUtil(struct point2d close2d[], struct point2d Px[], struct point2d Py[], int n);
void stripClosestPair2d(struct point2d strip_close[], struct point2d strip[], int size, float min_d);
float min(float x, float y);

void closestpair3d(struct point3d *,struct point3d *,int);
float distance_3d(struct point3d P1, struct point3d P2);
void merge_sort_3d(struct point3d plane[], int p, int r, char dim);
void merge_3d(struct point3d plane[], int p, int q, int r, char dim);
void bruteForceClosest_3d(struct point3d *close, struct point3d P[], int n);
void closestpair3dUtil(struct point3d close3d[], struct point3d Px[], struct point3d Py[], int n);
void stripClosestPair3d(struct point3d strip_close[], struct point3d strip[], int size, float min_d);

void euclideanMST(struct point2d **,struct point2d *,int);
int findset(int parent[], int i);

int main()
{
  struct point2d *points,close2d[2],**mst;
  struct point3d *plane,close3d[2];
  int i,n,m;
  scanf("%d",&n);
  points = (struct point2d *)malloc(n*sizeof(struct point2d));
  for(i=0;i<n;i++)
    scanf("%d%d",&(points[i].x),&(points[i].y));
  scanf("%d",&m);
  plane = (struct point3d *)malloc(m*sizeof(struct point3d));
  for(i=0;i<m;i++)
    scanf("%d%d%d",&(plane[i].x),&(plane[i].y),&(plane[i].z));
  closestpair2d(close2d,points,n);
  closestpair3d(close3d,plane,m);
  mst = (struct point2d **)malloc((n-1)*sizeof(struct point2d *));
  for(i=0;i<n;i++)
    mst[i]=(struct point2d *)malloc(2*sizeof(struct point2d));
  euclideanMST(mst,points,n);
  return 0;
}

void closestpair2d(struct point2d *close2d, struct point2d *points, int n)
{
  /* find out the closest pair of points among the points in "points" and store them in the array close2d */
  struct point2d Px[n];
  struct point2d Py[n];
  int i;

  for (i = 0; i < n; i++) {
    Px[i] = points[i];
    Py[i] = points[i];
  }

  merge_sort_2d(Px, 0, n - 1, 'x');
  merge_sort_2d(Py, 0, n - 1, 'y');

  closestpair2dUtil(close2d, Px, Py, n);

  printf("%d,%d and %d,%d\n", close2d[0].x, close2d[0].y, close2d[1].x, close2d[1].y);
}

void closestpair2dUtil(struct point2d close2d[], struct point2d Px[], struct point2d Py[], int n)
{
  struct point2d mid_point;
  struct point2d PyL[n];
  struct point2d PyR[n];
  struct point2d close2d_l[2];
  struct point2d close2d_r[2];
  struct point2d strip_close[2];
  struct point2d strip[n];
  struct point2d min_pair[2];
  int i, j;
  int li, ri;
  int mid;
  float min_d;

  if (n <= 3) {
    bruteForceClosest_2d(close2d, Px, n);
  } else {
    mid = n / 2;
    mid_point = Px[mid];

    li = 0;
    ri = 0;

    for (i = 0; i < n; i++) {
      if (Py[i].x <= mid_point.x) {
        PyL[i] = Py[i];
        li++;
      } else {
        PyR[i] = Py[i];
        ri++;
      }
    }

    closestpair2dUtil(close2d_l, Px, PyL, mid);
    closestpair2dUtil(close2d_r, Px + mid, PyR, n - mid); 

    if (distance_2d(close2d_l[0], close2d_l[1]) < distance_2d(close2d_r[0], close2d_r[1])) {
      min_d = distance_2d(close2d_l[0], close2d_l[1]);
      min_pair[0] = close2d_l[0];
      min_pair[1] = close2d_l[1]; 
    } else {
      min_d = distance_2d(close2d_r[0], close2d_r[1]);
      min_pair[0] = close2d_r[0];
      min_pair[1] = close2d_r[1];
    }

    j = 0;

    for (i = 0; i < n; i++) {
      if (abs(Py[i].x - mid_point.x) < min_d) {
        strip[j] = Py[i];
        j++;
      }
    }

    stripClosestPair2d(strip_close, strip, j, min_d);

    if (min_d < distance_2d(strip_close[0], strip_close[1])) {
      close2d[0] = min_pair[0];
      close2d[1] = min_pair[1];
    } else {
      close2d[0] = strip_close[0];
      close2d[1] = strip_close[1];
    }
  }
}

void stripClosestPair2d(struct point2d strip_close[], struct point2d strip[], int size, float min_d)
{
  float min = min_d;
  struct point2d min_pair[2];
  int i, j;

  min_pair[0] = strip[0];
  min_pair[1] = strip[1];

  for (i = 0; i < size; i++) {
    for (j = i + 1; (j < size) && (strip[j].y - strip[i].y < min); j++) {
      if (distance_2d(strip[i], strip[j]) < min) {
        min = distance_2d(strip[i], strip[j]);
        min_pair[0] = strip[i];
        min_pair[1] = strip[j];
      }
    }
  }

  strip_close[0] = min_pair[0];
  strip_close[1] = min_pair[1];
}

void closestpair3d(struct point3d *close3d, struct point3d *plane,int m)
{
  /* find out the closest pair of points among the points in "plane" and store them in the array close3d */
  struct point3d Px[m];
  struct point3d Py[m];
  int i;

  for (i = 0; i < m; i++) {
    Px[i] = plane[i];
    Py[i] = plane[i];
  }

  merge_sort_3d(Px, 0, m - 1, 'x');
  merge_sort_3d(Py, 0, m - 1, 'y');

  closestpair3dUtil(close3d, Px, Py, m);

  printf("%d,%d,%d and %d,%d,%d\n", close3d[0].x, close3d[0].y, close3d[0].z, close3d[1].x, close3d[1].y, close3d[1].z);

}

void closestpair3dUtil(struct point3d close3d[], struct point3d Px[], struct point3d Py[], int n)
{
  struct point3d mid_point;
  struct point3d PyL[n];
  struct point3d PyR[n];
  struct point3d close3d_l[2];
  struct point3d close3d_r[2];
  struct point3d strip_close[2];
  struct point3d strip[n];
  struct point3d min_pair[2];
  int i, j;
  int li, ri;
  int mid;
  float min_d;

  if (n <= 3) {
    bruteForceClosest_3d(close3d, Px, n);
  } else {
    mid = n / 2;
    mid_point = Px[mid];

    li = 0;
    ri = 0;

    for (i = 0; i < n; i++) {
      if (Py[i].x <= mid_point.x) {
        PyL[i] = Py[i];
        li++;
      } else {
        PyR[i] = Py[i];
        ri++;
      }
    }

    closestpair3dUtil(close3d_l, Px, PyL, mid);
    closestpair3dUtil(close3d_r, Px + mid, PyR, n - mid); 

    if (distance_3d(close3d_l[0], close3d_l[1]) < distance_3d(close3d_r[0], close3d_r[1])) {
      min_d = distance_3d(close3d_l[0], close3d_l[1]);
      min_pair[0] = close3d_l[0];
      min_pair[1] = close3d_l[1]; 
    } else {
      min_d = distance_3d(close3d_r[0], close3d_r[1]);
      min_pair[0] = close3d_r[0];
      min_pair[1] = close3d_r[1];
    }

    j = 0;

    for (i = 0; i < n; i++) {
      if (abs(Py[i].x - mid_point.x) < min_d) {
        strip[j] = Py[i];
        j++;
      }
    }

    stripClosestPair3d(strip_close, strip, j, min_d);

    if (min_d < distance_3d(strip_close[0], strip_close[1])) {
      close3d[0] = min_pair[0];
      close3d[1] = min_pair[1];
    } else {
      close3d[0] = strip_close[0];
      close3d[1] = strip_close[1];
    }
  }
}

void stripClosestPair3d(struct point3d strip_close[], struct point3d strip[], int size, float min_d)
{
  float min = min_d;
  struct point3d min_pair[2];
  int i, j;

  min_pair[0] = strip[0];
  min_pair[1] = strip[1];

  for (i = 0; i < size; i++) {
    for (j = i + 1; (j < size) && (strip[j].y - strip[i].y < min); j++) {
      if (distance_3d(strip[i], strip[j]) < min) {
        min = distance_3d(strip[i], strip[j]);
        min_pair[0] = strip[i];
        min_pair[1] = strip[j];
      }
    }
  }

  strip_close[0] = min_pair[0];
  strip_close[1] = min_pair[1];
}

float distance_2d(struct point2d P1, struct point2d P2)
{
  return sqrt(((P1.x - P2.x)*(P1.x - P2.x)) + ((P1.y - P2.y)*(P1.y - P2.y)));
}

float distance_3d(struct point3d P1, struct point3d P2)
{
  return sqrt(((P1.x - P2.x)*(P1.x - P2.x)) + ((P1.y - P2.y)*(P1.y - P2.y)) + ((P1.z - P2.z)*(P1.z - P2.z)));
}

void bruteForceClosest_2d(struct point2d *close, struct point2d P[], int n)
{
    float min = 9999.00;
    struct point2d min_pt[2];
    int i, j;

    for (i = 0; i < n; ++i) {
      for (j = i+1; j < n; ++j) {
        if (distance_2d(P[i], P[j]) < min) {
          min = distance_2d(P[i], P[j]);

          min_pt[0].x = P[i].x;
          min_pt[1].x = P[j].x;
          min_pt[0].y = P[i].y;
          min_pt[1].y = P[j].y;
        }
      }
    }

    close[0] = min_pt[0];
    close[1] = min_pt[1];
}

void bruteForceClosest_3d(struct point3d *close, struct point3d P[], int n)
{
    float min = 9999.00;
    struct point3d min_pt[2];
    int i, j;

    for (i = 0; i < n; ++i) {
      for (j = i+1; j < n; ++j) {
        if (distance_3d(P[i], P[j]) < min) {
          min = distance_3d(P[i], P[j]);

          min_pt[0].x = P[i].x;
          min_pt[1].x = P[j].x;
          min_pt[0].y = P[i].y;
          min_pt[1].y = P[j].y;
          min_pt[0].z = P[i].z;
          min_pt[1].z = P[j].z;
        }
      }
    }

    close[0] = min_pt[0];
    close[1] = min_pt[1];
}

float min(float x, float y)
{
  return (x < y)? x : y;
}

void merge_sort_2d(struct point2d points[], int p, int r, char dim)
{
  int q;

  if ((p < r) && dim == 'x') {
    q = ((p + r) / 2);
    merge_sort_2d(points, p, q, 'x');
    merge_sort_2d(points, (q + 1), r, 'x');
    merge_2d(points, p, q, r, 'x');
  } else if ((p < r) && dim == 'y') {
    q = ((p + r) / 2);
    merge_sort_2d(points, p, q, 'y');
    merge_sort_2d(points, (q + 1), r, 'y');
    merge_2d(points, p, q, r, 'y');
  }
}

void merge_2d(struct point2d points[], int p, int q, int r, char dim)
{ 
  struct point2d L[1000];
  struct point2d R[1000];
  int nL; 
  int nR;
  int i;
  int j;
  int k;

  nL = q - p + 1;
  nR = r - q;
  i = 0;
  j = 0;

  for (i = p, j = 0; j < nL; i++, j++) {
    L[j] = points[i];
  }
  
  for (i = (q + 1), j = 0; j < nR; i++, j++) {
    R[j] = points[i];
  }

  if (dim == 'x') {
    for (i = 0, j = 0, k = p; (k < r) && (i < nL) && (j < nR); k++) {
      if (L[i].x <= R[j].x) {
        points[k] = L[i];
        i++;
      } else {
        points[k] = R[j];
        j++;
      }
    } 

    if (i < nL) {
      for (; i < nL; i++, k++) {
        points[k] = L[i];
      }
    } else {
      for (; j < nR; j++, k++) {
        points[k] = R[j];
      }
    }
  } else {
    for (i = 0, j = 0, k = p; (k < r) && (i < nL) && (j < nR); k++) {
      if (L[i].y <= R[j].y) {
        points[k] = L[i];
        i++;
      } else {
        points[k] = R[j];
        j++;
      }
    } 

    if (i < nL) {
      for (; i < nL; i++, k++) {
        points[k] = L[i];
      }
    } else {
      for (; j < nR; j++, k++) {
        points[k] = R[j];
      }
    }
  }
}

void merge_sort_3d(struct point3d plane[], int p, int r, char dim)
{
  int q;

  if ((p < r) && dim == 'x') {
    q = ((p + r) / 2);
    merge_sort_3d(plane, p, q, 'x');
    merge_sort_3d(plane, (q + 1), r, 'x');
    merge_3d(plane, p, q, r, 'x');
  } else if ((p < r) && dim == 'y') {
    q = ((p + r) / 2);
    merge_sort_3d(plane, p, q, 'y');
    merge_sort_3d(plane, (q + 1), r, 'y');
    merge_3d(plane, p, q, r, 'y');
  }
}

void merge_3d(struct point3d plane[], int p, int q, int r, char dim)
{ 
  struct point3d L[1000];
  struct point3d R[1000];
  int nL; 
  int nR;
  int i;
  int j;
  int k;

  nL = q - p + 1;
  nR = r - q;
  i = 0;
  j = 0;

  for (i = p, j = 0; j < nL; i++, j++) {
    L[j] = plane[i];
  }
  
  for (i = (q + 1), j = 0; j < nR; i++, j++) {
    R[j] = plane[i];
  }

  if (dim == 'x') {
    for (i = 0, j = 0, k = p; (k < r) && (i < nL) && (j < nR); k++) {
      if (L[i].x <= R[j].x) {
        plane[k] = L[i];
        i++;
      } else {
        plane[k] = R[j];
        j++;
      }
    } 

    if (i < nL) {
      for (; i < nL; i++, k++) {
        plane[k] = L[i];
      }
    } else {
      for (; j < nR; j++, k++) {
        plane[k] = R[j];
      }
    }
  } else {
    for (i = 0, j = 0, k = p; (k < r) && (i < nL) && (j < nR); k++) {
      if (L[i].y <= R[j].y) {
        plane[k] = L[i];
        i++;
      } else {
        plane[k] = R[j];
        j++;
      }
    } 

    if (i < nL) {
      for (; i < nL; i++, k++) {
        plane[k] = L[i];
      }
    } else {
      for (; j < nR; j++, k++) {
        plane[k] = R[j];
      }
    }
  }
}

void euclideanMST(struct point2d **mst,struct point2d *points,int n)
{
  /* find out the Euclidean MST and store the MST in 2d-array mst.
     Each of the n-1 edges are store by storing it's two endpoints.
  */
  int par[n];
  int i, j;
  for(i=0;i<n;++i)
  {
    par[i] = i;
  }
  int t = n-1;
  int sz = 0;
  while(t--)
  {
    double mind = 1e8;
    int b,a, pi,pj;
    for(i=0;i<n;++i)
    {
      for(j=i+1;j<n;++j)
      {
        pi = findset(par, i);
        pj = findset(par, j);
        if(distance_2d(points[i], points[j]) < mind && pi != pj)
        {
          a = i;
          b = j;
          mind = distance_2d(points[i], points[j]);
        }
      }
    }
    int pa,pb;
    pa = findset(par,a);
    pb = findset(par,b);
    par[pa] = pb;
    mst[sz][0].x = points[a].x;
    mst[sz][0].y = points[a].y;
    mst[sz][1].x = points[b].x;
    mst[sz][1].y = points[b].y;
    sz++;
  }

  for(i = 0; i < n - 1; i++) {
    printf("(%d, %d) ---- (%d, %d)\n", mst[i][0].x, mst[i][0].y, mst[i][1].x, mst[i][1].y);
  }
}

int findset(int parent[], int i)
{
  while(i != parent[i]) {
    parent[i] = parent[parent[i]];
    i = parent[i];
  }

  return i;
}
