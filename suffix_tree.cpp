#ifndef _suffix_tree_def
#define _suffix_tree_def
#include <suffix_tree.h>
// including the required libraries
Suffix_Tree::~Suffix_Tree()
{
    freeSuffixTreeByPostOrder(root);
}
// Function to create a new node in the suffix tree
SNode *Suffix_Tree::newNode(int start, int *end)
{
    SNode *STSNode = (SNode *)malloc(sizeof(SNode));
    int r = 0;
    while (r < MAX)
        STSNode->children[r++] = NULL;
    STSNode->suffixLink = root;
    STSNode->start = start;
    STSNode->end = end;
    STSNode->suffixIndex = -1;
    return STSNode;
}

// function to return the edge length
int Suffix_Tree::edgeLength(SNode *A)
{
    if (A == root)
        return 0;
    long int e = *(A->end);
    long int s = A->start;
    return (e - s + 1);
}

// function to walk down along the tree If activeLength is greater 
//  than current edge length, set next internal node as 
//  activeNode and adjust activeEdge and activeLength 
//  accordingly to represent same activePoint
int Suffix_Tree::walkDown(SNode *A)
{
    if (activeLength >= edgeLength(A))
    {
        activeNode = A;
        activeLength -= edgeLength(A);
        activeEdge += edgeLength(A);
        return 1;
    }
    return 0;
}


void Suffix_Tree::extendSuffixTree(int pos)
{
    // exteniding all the leaves
    END = pos;
    /*Increment remainingSuffixCount indicating that a 
    new suffix added to the list of suffixes yet to be 
    added in tree*/
    remainingSuffixCount++;
    lastNewNode = NULL;
    while (remainingSuffixCount)
    {
        if (activeLength == 0)
            activeEdge = pos;
        // determining the edge from active node to active edge
        if (activeNode->children[test[activeEdge]] == NULL)
        {
            activeNode->children[test[activeEdge]] = newNode(pos, &END);
            if (lastNewNode != NULL)
            {
                lastNewNode->suffixLink = activeNode;
                lastNewNode = NULL;
            }
        }
        // if there is no edge between active node and acitve edge
        else
        {
            // go to the next node 
            SNode *next = activeNode->children[test[activeEdge]];
            if (walkDown(next))
            {
                continue;
            }
            if (test[next->start + activeLength] == test[pos])
            {
                if (lastNewNode != NULL && activeNode != root)
                {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = NULL;
                }
                activeLength++;
                break;
            }
            splitEnd = (int *)malloc(sizeof(int));
            *splitEnd = next->start + activeLength - 1;
            SNode *split = newNode(next->start, splitEnd);
            activeNode->children[test[activeEdge]] = split;
            split->children[test[pos]] = newNode(pos, &END);
            next->start += activeLength;
            split->children[test[next->start]] = next;
            if (lastNewNode != NULL)
            {
                lastNewNode->suffixLink = split;
            }
            lastNewNode = split;
        }
        remainingSuffixCount--;
        if (activeNode == root && activeLength > 0)
        {
            activeLength--;
            activeEdge = pos - remainingSuffixCount + 1;
        }
        else if (activeNode != root)
        {
            activeNode = activeNode->suffixLink;
        }
    }
}

// funtion to print hte suffix tree
void Suffix_Tree::print(int i, int j)
{
    int k;
    for (k = i; k <= j; k++)
        printf("%c", test[k]);
}

// funtion to set the suffix index
void Suffix_Tree::setSuffixIndex(SNode *n, int labelHeight)
{
    if (n == NULL)
        return;
    int leaf = 1;
    int i;
    for (i = 0; i < MAX; i++)
    {
        if (n->children[i] != NULL)
        {
            leaf = 0;
            setSuffixIndex(n->children[i], labelHeight +
                                               edgeLength(n->children[i]));
        }
    }
    if (leaf == 1)
    {
        n->suffixIndex = size - labelHeight;
    }
}

// function for freeing memory 
void Suffix_Tree::freeSuffixTreeByPostOrder(SNode *n)
{
    if (n == NULL)
        return;
    int i;
    for (i = 0; i < MAX; i++)
    {
        if (n->children[i] != NULL)
        {
            freeSuffixTreeByPostOrder(n->children[i]);
        }
    }
    if (n->suffixIndex == -1)
        free(n->end);
    free(n);
}

// function to build suffix tree
void Suffix_Tree::buildSuffixTree(string &text)
{
    test = text;
    size = test.size();
    rootEnd = (int *)malloc(sizeof(int));
    *rootEnd = -1;
    root = newNode(-1, rootEnd);
    activeNode = root;
    int j = 0;
    while (j < size)
        extendSuffixTree(j++);
    int labelHeight = 0;
    setSuffixIndex(root, labelHeight);
}
SNode *Suffix_Tree::returnRoot()
{
    return root;
}

// funtion to traverse the edges
int Suffix_Tree::traverseEdge(string &str, int idx, int start, int end, string &text)
{
    int k = 0;
    for (k = start; k <= end && str[idx] != '\0'; k++, idx++)
    {
        if (text[k] != str[idx])
            return -1;
    }
    if (str[idx] == '\0')
        return 1;
    return 0;
}

int Suffix_Tree::doTraversalToCountLeaf(SNode *n)
{
    if (n == NULL)
        return 0;
    if (n->suffixIndex > -1)
    {
        long long it = lower_bound(v1.begin(), v1.end(), n->suffixIndex) - v1.begin();

        int c;
        if (it == 0)
        {
            c = n->suffixIndex;
        }
        else
        {
            c = n->suffixIndex - v1[it - 1];
        }
        if (it == 0)
        {
            c++;
        }
        cout << "Found at line: " << it + 1 << " position: " << c << endl;
        ;

        return 1;
    }
    int count = 0;
    int i = 0;
    for (i = 0; i < MAX; i++)
    {
        if (n->children[i] != NULL)
        {
            count += doTraversalToCountLeaf(n->children[i]);
        }
    }
    return count;
}

// funtion to count the leaves
int Suffix_Tree::countLeaf(SNode *n)
{
    if (n == NULL)
        return 0;
    return doTraversalToCountLeaf(n);
}

// funtion to traverse the tree
int Suffix_Tree::doTraversal(SNode *n, string &str, int idx, string &text)
{
    if (n == NULL)
    {
        return -1;
    }
    int res = -1;
    if (n->start != -1)
    {
        res = traverseEdge(str, idx, n->start, *(n->end), text);
        if (res == -1)
            return -1;
        if (res == 1)
        {
            if (n->suffixIndex > -1)
            {
                long long it = lower_bound(v1.begin(), v1.end(), n->suffixIndex) - v1.begin();

                int c;
                if (it == 0)
                {
                    c = n->suffixIndex;
                }
                else
                {
                    c = n->suffixIndex - v1[it - 1];
                }
                if (it == 0)
                {
                    c++;
                }
                cout << "Found at line: " << it + 1 << " position: " << c << endl;
            }
            else
            {
                int count = countLeaf(n);
                cout << "Number of Occurences: " << count << endl;
            }
            return 1;
        }
    }
    idx = idx + edgeLength(n);
    if (n->children[str[idx]] != NULL)
        return doTraversal(n->children[str[idx]], str, idx, text);
    else
        return -1;
}

// funtion to check the string in the suffix tree 
void Suffix_Tree::checkForSubString(string &str, SNode *root, string &text)
{

    int res = doTraversal(root, str, 0, text);

    if (res == 1)
    {
        ind = 0;
    }
    else
    {
        printf("Number of Occurences: 0", str);
    }
}
#endif
