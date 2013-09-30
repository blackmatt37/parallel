#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

typedef struct TreeNode
{
	int freq;
	char key;

	struct TreeNode* left;
	struct TreeNode* right;

} Node;

int compnode(const void*, const void*);
void walk(Node* , int , char* , char[][30]);

int main(int argc, char const *argv[])
{
	// printf("Encode\n");

	char* message = "HELLOWORLD";
	Node* nodes[256];
	int freq[256];
	bzero(freq, 256*sizeof(int));
	for (int i = 0; i < strlen(message); ++i)
	{
		freq[(int)message[i]]++;
	}
	int n = 0;
	for (int i = 0; i < 256; ++i)
	{
		if (freq[i] > 0)
		{
			nodes[n] = (Node*)malloc(sizeof(Node));
			nodes[n]->freq = freq[i];
			nodes[n]->key = (char)i;
			nodes[n]->left = NULL;
			nodes[n]->right = NULL;
			n++;
		}
	}
	while(n > 1)
	{
		qsort(nodes, n, sizeof(Node*), compnode);
		Node* temp  = (Node*)malloc(sizeof(Node));
		temp->freq  = nodes[n-1]->freq + nodes[n-2]->freq;
		temp->key   = '*';
		temp->left  = nodes[n-1];
		temp->right = nodes[n-2];
		nodes[n-2] = temp;
		nodes[n-1] = NULL;
		n--;
	}
	char* p = malloc(30);
	char e[256][30];
	walk(nodes[0], 0, p, e);
	return 0;
}
void walk(Node* current, int depth, char* p, char e[][30])
{
	if (current->left == NULL && current->right == NULL)
	{
		// printf("%s\n", p);
		p[depth] = '\0';
		printf("%c %d %s\n", current->key, current->freq, p);
		return;
	}
	p[depth] = '0';
	walk(current->left, depth+1, p, e);
	p[depth] = '1';
	walk(current->right, depth+1, p, e);
}
int compnode(const void* a, const void* b)
{
	const Node* na = *((Node**)a);
	const Node* nb = *((Node**)b);
	return (nb->freq - na->freq);
}