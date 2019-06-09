#define MAX_WORD_LEN 20
#define MAX_LINE_LEN 200 /* in characters */

typedef struct treeNode
{
	struct treeNode *left;
	struct treeNode *right;
	char word[MAX_WORD_LEN];
	int count;	
} treeNode;


typedef struct _DblLinkedList {
    size_t size;
    struct Node *head;
    struct Node *tail;
} DblLinkedList;


typedef struct Node {
    void *value;
    struct Node *next;
    struct Node *prev; 
} Node;




