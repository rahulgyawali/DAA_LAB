# include <stdio.h>
# include <stdlib.h>

struct node {
  struct node *left,*right;
  char data;
};


void decode(FILE *,FILE *,struct node *);
void encode(FILE *,FILE *,struct node *);
void constructHuffman(char *, int **, int , struct node **);
void buildMinHeap(char *, int **, int);
void collectFrequency(FILE *,int **,int);
int countDistinct(FILE *);

void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

void printCodes(struct node* root, int arr[], int top)
{
    if (root->left)
    {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
 
    if (root->right)
    {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
 
    if (root->left == NULL && root->right == NULL)
    {
        printf("%c: ", root->data);
        printArr(arr, top);
    }
}

int main()
{
    FILE *fip,*fop,*fdec;
    int **frequency,n,i;
    char *heap;
    struct node *huffman;
    
    fip = fopen("input.txt","r");
    fop = fopen("encode.txt","w");
    
    n = countDistinct(fip);
    
    frequency = (int **)malloc(n*sizeof(int *));
    heap = (char *)malloc(n*sizeof(char));
    for(i = 0; i < n; i++)
        frequency[i] = (int *)malloc(2 * sizeof(int));
    rewind(fip);
    collectFrequency(fip,frequency,n);
    rewind(fip);
    buildMinHeap(heap,frequency,n);
    constructHuffman(heap, frequency, n, &huffman);
    //printf("hello");
    rewind(fip);
    int arr[n];
    printCodes(huffman,arr,0);
    /*
    encode(fip,fop,huffman);
    fclose(fop);
    fop = fopen("encode","w");
    fdec = fopen("decode","r");
    decode(fop,fdec,huffman);
    fclose(fip);
    fclose(fop);
    fclose(fdec);*/
    return 0;
}

int countDistinct(FILE *fp)
{
    int n = 0;
    int i;
    int cnt[300];
    for(i = 0; i < 300; i++) {
        cnt[i] = 0;
    }
    char ch;
    while((ch = fgetc(fp)) != EOF) {
        cnt[ch]++;
    }
    for(i = 0; i < 300; i++) {
        if(cnt[i]) {
            n++;
        }
    }
    return n;
}

void collectFrequency(FILE *fp,int **frequency,int n)
{
    int i;
    int cnt[300] = {0};
    char ch;
    while((ch = fgetc(fp)) != EOF) {
        cnt[ch]++;
    }
    int j = 0;
    for(i = 0; i < 300; i++) {
        if(cnt[i]) {
            frequency[j][0] = i;
            frequency[j][1] = cnt[i];
            j++;
        }
    }
}

int gfreq_id(int **frequency, int val, int n) {
    int i;
    for(i = 0; i < n; i++) {
        if(frequency[i][0] == val) {
            return i;
        }
    }
}

int gfreq(int **frequency, int val, int n) {
    return frequency[gfreq_id(frequency, val, n)][1];
}

void heapify(char *heap, int **frequency, int n, int i)
{
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if(left < n && gfreq(frequency, heap[left], n) < gfreq(frequency, heap[smallest], n))
        left = smallest;
    if(right < n && gfreq(frequency, heap[right], n) < gfreq(frequency, heap[smallest], n))
        right = smallest;
    if(smallest != i) {
        char temp = heap[smallest];
        heap[smallest] = heap[i];
        heap[i] = temp;
        heapify(heap, frequency, n, smallest);
    }
}

void buildMinHeap(char *heap, int **frequency, int n)
{
    int i;
    for(i = 0; i < n; i++) {
        heap[i] = frequency[i][0];
    }
    for(i = n - 1; i >= 0; i--) {
        heapify(heap, frequency, n, i);
    }
}

char extract(char *heap, int **frequency, int *size, int n) 
{
    char temp = heap[0];
    (*size)--;
    heapify(heap, frequency, *size, 0);
    return temp;
}

void insert(char* heap, int** frequency, int* n, char ch)
{
    heap[*n] = ch;
    (*n)++;
    int child = (*n) - 1;
    int parent = (child - 1) / 2;
    while(parent >= 0 && gfreq(frequency, heap[parent], *n) > gfreq(frequency, heap[child], *n)) {
        char temp = heap[parent];
        heap[parent] = heap[child];
        heap[child] = temp;
        child = parent;
        parent = (child - 1) / 2;
    }
}

struct node *getnode(char data) 
{
    struct node *temp;
    temp = (struct node *)(malloc(sizeof(struct node)));
    temp -> left = NULL;
    temp -> right = NULL;
    temp -> data = data; 
    return temp;
}

void constructHuffman(char *heap, int **frequency, int n, struct node **huffman)
{
    int i;
    struct node *array[n];
    for(i = 0; i < n; i++) {
        array[i] = getnode(frequency[i][0]);
    }
    int size = n;
    while(size != 1) {
        char left = extract(heap, frequency, &size, n);
        char right = extract(heap, frequency, &size, n);
        int id1 = gfreq_id(frequency, heap[left], size);
        int id2 = gfreq_id(frequency, heap[right], size);
        struct node* temp = (struct node*)malloc(sizeof(struct node));
        temp -> left = array[id1];
        temp -> right = array[id2];
        temp -> data = array[id1] -> data;
        frequency[id1][1] += frequency[id2][1];
        insert(heap, frequency, &size, temp -> data);
        array[id1] = temp;
    }
    char ch = extract(heap, frequency, &size, n);
    int id = gfreq_id(frequency, ch, n);
    *huffman = array[id];
}


void encode(FILE *fip,FILE *fop,struct node *huffman)
{
  /*
    Encode fip using huffman code and write properly to fop.
  */
}

void decode(FILE *fip,FILE *fop,struct node *huffman)
{
  /*
    Decode fip using huffman code and write properly to fop.
  */
}