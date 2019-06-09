#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#define DELIM " ,.\n\"\'\t"


void dfs(treeNode *root, int n);
void bfs(treeNode *root);
treeNode* createNode(char *word);
treeNode* addNode(treeNode* root, char *word);
treeNode* createTreeFromTextFile(char *filename);
void cutLineAfterCertainSymbol(char *line, char symbol);
int treeSize(treeNode *root);
void deleteTree(treeNode *root);
void createTextFile();



DblLinkedList* createDblLinkedList() {
    DblLinkedList *tmp = (DblLinkedList*) malloc(sizeof(DblLinkedList));
    tmp->size = 0;
    tmp->head = tmp->tail = NULL;
 
    return tmp;
}

void deleteDblLinkedList(DblLinkedList **list) {
    Node *tmp = (*list)->head;
    Node *next = NULL;
    while (tmp) {
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
    free(*list);
    (*list) = NULL;
}


void pushFront(DblLinkedList *list, void *data) {
    Node *tmp = (Node*) malloc(sizeof(Node));
    if (tmp == NULL) {
        exit(1);
    }
    tmp->value = data;
    tmp->next = list->head;
    tmp->prev = NULL;
    if (list->head) {
        list->head->prev = tmp;
    }
    list->head = tmp;
 
    if (list->tail == NULL) {
        list->tail = tmp;
    }
    list->size++;
}


void* popFront(DblLinkedList *list) {
    Node *prev;
    void *tmp;
    if (list->head == NULL) {
        exit(2);
    }
 
    prev = list->head;
    list->head = list->head->next;
    if (list->head) {
        list->head->prev = NULL;
    }
    if (prev == list->tail) {
        list->tail = NULL;
    }
    tmp = prev->value;
    free(prev);
 
    list->size--;
    return tmp;
}

void pushBack(DblLinkedList *list, void *value) {
    Node *tmp = (Node*) malloc(sizeof(Node));
    if (tmp == NULL) {
        exit(3);
    }
    tmp->value = value;
    tmp->next = NULL;
    tmp->prev = list->tail;
    if (list->tail) {
        list->tail->next = tmp;
    }
    list->tail = tmp;
 
    if (list->head == NULL) {
        list->head = tmp;
    }
    list->size++;
}
 
void* popBack(DblLinkedList *list) {
    Node *next;
    void *tmp;
    if (list->tail == NULL) {
        exit(4);
    }
 
    next = list->tail;
    list->tail = list->tail->prev;
    if (list->tail) {
        list->tail->next = NULL;
    }
    if (next == list->head) {
        list->head = NULL;
    }
    tmp = next->value;
    free(next);
 
    list->size--;
    return tmp;
}



treeNode* createTreeFromTextFile(char *filename) {

	FILE *f = fopen(filename, "r");
	if (!f) {
		printf("Cannot open file %s\nTry another file\n", filename); 
		exit(1);
	}

	char line[MAX_LINE_LEN];
	char *word = (char *)malloc(sizeof(char)*MAX_WORD_LEN);

	treeNode *root = NULL;
	while (!feof(f)) {
		fgets(line, MAX_LINE_LEN-1, f);
		word = strtok(line, DELIM);
		while (word) {
			root = addNode(root, word);
			word = strtok(NULL, DELIM);
		}
	}
	free(word); 	

	return root;
}

void createTextFile() {
	char filename[MAX_WORD_LEN], line[MAX_LINE_LEN];
	printf("Enter text file name to create: ");
	fgets(filename, MAX_WORD_LEN-1, stdin);
	cutLineAfterCertainSymbol(filename, '\n');

	if (!filename[0] || strlen(filename) < 5)
		strncpy(filename, "default.txt", 12);

	FILE *f = fopen(filename, "w");
	if (!f) {
		printf("Cannot open file %s\nTry another file\n", filename);
	}
	printf("Enter your lines ($end - is a sign of end of input): \n");
	printf("> ");
	fgets(line, MAX_LINE_LEN-1, stdin);

	while (strcmp(line, "$end\n") != 0) {
		size_t i = fwrite(line, sizeof(char), strlen(line), f);
		printf("(%ld)\n", i);
		printf("> ");
		fgets(line, MAX_LINE_LEN-1, stdin);
	}

	fseek(f, 0L, SEEK_END);
	long size = ftell(f);
	fseek(f, 0L, SEEK_SET);

	if (size <= 5) {
		printf("File size is too small\nTry again\n");
		remove(filename);
		exit(1);
	}

	fclose(f);

}


treeNode* createNode(char *word) {
	treeNode* node = (treeNode*)malloc(sizeof(treeNode));
	strncpy(node->word, word, MAX_WORD_LEN);
	node->left  = NULL;
	node->right = NULL;
	node->count = 1;
	return node;
}

treeNode* addNode(treeNode* root, char *word) {
	int cmp;
	if (!root) //if there is no node or tree is empty
		root = createNode(word);
	else if ((cmp = strcmp(root->word, word)) == 0)
		root->count++;
		else if (cmp > 0) //if our word smaller than existed in root node
			root->left  = addNode(root->left, word);
		else //if our word bigger than existed in root node
			root->right = addNode(root->right, word);
	return root;
}

void cutLineAfterCertainSymbol(char *line, char symbol) {

	int i = 0;

	while (line[i]) {
		if (line[i] == symbol) {
			line[i] =  '\0';
			break;
		}
		++i;
	}
}

void dfs(treeNode *root, int n) {
	
	if (root) {
		dfs(root->left, n+1);
		for (int i = 0; i < n; ++i)
			printf("  ");
		printf("%s, %d\n", root->word, root->count);
		dfs(root->right, n+1);
	}
}

void bfs(treeNode *root) {
	
	DblLinkedList *queue = createDblLinkedList();
	pushBack(queue, root);

	while (queue->size) {
		treeNode *tmp = (treeNode*)popFront(queue);
		printf("%s\n", tmp->word);

		if (tmp->left)
			pushBack(queue, tmp->left);
		if (tmp->right)
			pushBack(queue, tmp->right);
	}
	deleteDblLinkedList(&queue);
}

int treeSize(treeNode *root) {
	int size = 0;
	if (root) 
		size = 1 + treeSize(root->left) + treeSize(root->right);
	return size;
}

void deleteTree(treeNode *root) {
	if (root) {
		deleteTree(root->left);
		free(root);
		deleteTree(root->right);
	}
}



int main() {

	createTextFile();

	char filename[MAX_WORD_LEN];
	printf("Enter your file name: ");
	fgets(filename, MAX_WORD_LEN-1, stdin);
	cutLineAfterCertainSymbol(filename, '\n');
	treeNode *root = createTreeFromTextFile(filename);
	printf("\nTHIS IS A Depth-First Search\n");
	dfs(root, 0);
	printf("\nTHIS IS A Breadth-first search\n");
	bfs(root);
	deleteTree(root);

	return 0;
}