#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<float.h>

struct point2d {
  int x,y;
};

struct point3d {
  int x,y,z;
};

void closestpair2d(struct point2d *,struct point2d *, int);
void closestpair3d(struct point3d *,struct point3d *,int);
void euclideanMST(struct point2d **,struct point2d *,int);

int compareX(const void* a, const void* b)
{
    struct point2d *p1 = (struct point2d *)a,  *p2 = (struct point2d *)b;
    return (p1->x - p2->x);
}

int compareX3(const void* a, const void* b)
{
    struct point3d *p1 = (struct point3d *)a,  *p2 = (struct point3d *)b;
    return (p1->x - p2->x);
}

int compareY3(const void* a, const void* b)
{
    struct point3d *p1 = (struct point3d *)a,  *p2 = (struct point3d *)b;
    return (p1->y - p2->y);
}

int compareY(const void* a, const void* b)
{
    struct point2d *p1 = (struct point2d *)a,  *p2 = (struct point2d *)b;
    return (p1->y - p2->y);
}


void ysort3(struct point3d array[],int n)
{
	int i,j;
	struct point3d temp;
	for(i=0;i<n-1;i++)
	{
		for(j=0;j<n-i-1;j++)
		{
			if(array[j].y>array[j+1].y)
			{
				temp=array[j];
				array[j]=array[j+1];
				array[j+1]=temp;
				
			}
		}
	}
}




float dist(struct point2d a,struct point2d b)
{
	return sqrt(pow((a.x-b.x),2) + pow((a.y-b.y),2) );
}


float dist3(struct point3d a,struct point3d b)
{
	return sqrt(pow((a.x-b.x),2) + pow((a.y-b.y),2) + pow((a.z-b.z),2) );

}
void easy(struct point2d close2d[],struct point2d points[],int n)
{
	float min=FLT_MAX;
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(dist(points[i],points[j]) < min)
			{
				min=dist(points[i],points[j]);
				close2d[0]=points[i];
				close2d[1]=points[j];
			}
		}
	}
}


void easy3(struct point3d close2d[],struct point3d points[],int n)
{
	float min=FLT_MAX;
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(dist3(points[i],points[j]) < min)
			{
				min=dist3(points[i],points[j]);
				close2d[0]=points[i];
				close2d[1]=points[j];
			}
		}
	}


}







void stripclosest(struct point2d close2d[],struct point2d strip[],int n,int d )
{
int i,j;
	
	qsort(strip, n, sizeof(struct point2d), compareY);
	float min=d;
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n && abs(strip[j].y-strip[i].y) < min;j++)
		{
			if(dist(strip[j],strip[i]) < min)
			{
			
				min=dist(strip[j],strip[i]);
				close2d[0]=strip[i];
				close2d[1]=strip[j];
			}
		}
		
	}
	
}



void stripclosest3(struct point3d close2d[],struct point3d strip[],int n,int d )
{
int i,j;
	 qsort(strip, n, sizeof(struct point3d), compareY3);
	 /*for(i=0;i<n;i++)
    printf("%d %d %d\n",(strip[i].x),(strip[i].y),(strip[i].z));
	*/
	float min=d;
	for(i=0;i<n;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(dist3(strip[j],strip[i]) < min)
			{
			
				min=dist3(strip[j],strip[i]);
				close2d[0]=strip[i];
				close2d[1]=strip[j];
			}
		}
		
	}
	
}

void closestpair2d(struct point2d *close2d, struct point2d *points, int n)
{

  
  if(n<=3)
  {
  	easy(close2d,points,n);
  	return;
  	
  }
  struct point2d p1,p2;
  
  int mid=n/2;
  struct point2d temp=points[mid];
  float d;
  closestpair2d(close2d,points,mid);
  float dl=dist(close2d[0],close2d[1]);
  p1=close2d[0];
  p2=close2d[1];
  closestpair2d(close2d,points+mid,n-mid);
  float dr=dist(close2d[0],close2d[1]);
  if(dl<=dr)
  	d=dl;
   else
   	d=dr;
	
	if(d==dl)
	{
	
		close2d[0]=p1;
		close2d[1]=p2;
	}
	
int j=0,i;
struct point2d strip[n];
for(i=0;i<n;i++)
{
		if(abs(points[i].x-temp.x) < d)
		{
			strip[j]=points[i];
			j++;
		}
}

stripclosest(close2d,strip,j,d);


  
}

void closestpair3d(struct point3d *close2d, struct point3d *points, int n)
{


  if(n<=3)
  {
  	easy3(close2d,points,n);
  	return;
  	
  }
  struct point3d p1,p2;
  
  int mid=n/2;
  struct point3d temp=points[mid];
  float d;
  closestpair3d(close2d,points,mid);
  float dl=dist3(close2d[0],close2d[1]);
  p1=close2d[0];
  p2=close2d[1];
  closestpair3d(close2d,points+mid,n-mid);
  float dr=dist3(close2d[0],close2d[1]);
  if(dl<=dr)
  	d=dl;
   else
   	d=dr;
	
	if(d==dl)
	{
	
		close2d[0]=p1;
		close2d[1]=p2;
	}
int j=0,i;
struct point3d strip[n];
for(i=0;i<n;i++)
{
		if(abs(points[i].x-temp.x) < d)
		{
			strip[j]=points[i];
			j++;
		}
}

stripclosest3(close2d,strip,j,d);



  
  
  
  
}


int main()
{
  struct point2d *points,close2d[2],**mst;
  struct point3d *plane,close3d[2];
  int i,n,m;
 /* scanf("%d",&n);
  points = (struct point2d *)malloc(n*sizeof(struct point2d));
  for(i=0;i<n;i++)
    scanf("%d%d",&(points[i].x),&(points[i].y));
  */
  scanf("%d",&m);
  plane = (struct point3d *)malloc(m*sizeof(struct point3d));
  for(i=0;i<m;i++)
    scanf("%d%d%d",&(plane[i].x),&(plane[i].y),&(plane[i].z));
  
  /*qsort(points, n, sizeof(struct point2d), compareX);
 
  closestpair2d(close2d,points,n);
  printf("closest points are %d %d\n %d %d\n",close2d[0].x,close2d[0].y,close2d[1].x,close2d[1].y);
printf("closest distance is %f\n",dist(close2d[0],close2d[1]));
  */
  
   qsort(plane, m, sizeof(struct point3d), compareX3);
   /*for(i=0;i<m;i++)
    printf("%d %d %d\n",(plane[i].x),(plane[i].y),(plane[i].z));
  */
  closestpair3d(close3d,plane,m);
  
  printf("closest points are %d %d %d\n %d %d %d\n",close3d[0].x,close3d[0].y,close3d[0].z,close3d[1].x,close3d[1].y,close3d[1].z);
printf("closest distance is %f\n",dist3(close3d[0],close3d[1]));
  
  /*mst = (struct point2d **)mallloc((n-1)*sizeof(struct point2d *));
  for(i=0;i<n;i++)
    mst[i]=(struct point2d *)mallloc(2*sizeof(struct point2d));
  euclideanMST(mst,points,n);
  */
  return 0;
}


