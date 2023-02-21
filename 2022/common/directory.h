
#define MAX_NODES 1024

struct FileNode
{
    bool dir;
    char name[32+1];
    struct FileNode* parent;
    struct FileNode* nodes[MAX_NODES];
    int node_count;
    int size;
};

typedef struct FileNode FileNode;

void dir_set_name(FileNode* d, const char* name)
{
    memset(d->name,0,32);
    strcpy(d->name,name);
}

FileNode* dir_add_node(FileNode* r, char* name, bool is_dir)
{
    if(!r)
        return NULL;

    if(r->node_count >= MAX_NODES)
    {
        printf("Node is full!\n");
        return NULL;
    }

    r->nodes[r->node_count] = calloc(1,sizeof(FileNode));

    FileNode* n = r->nodes[r->node_count];
    
    dir_set_name(n,name);
    n->dir = is_dir;
    n->parent = r;

    r->node_count++;

    return n;
    //printf("Added node %s to %s\n",name,r->name);
}

FileNode* dir_goto(FileNode* r, char* name)
{
    if(strcmp(r->name,name) == 0)
    {
        return r;
    }

    FileNode* n = NULL;

    for(int i = 0; i < r->node_count; ++i)
    {
        if(r->nodes[i] == NULL)
        {
            printf("node is null, wtf\n");
        }
        else
        {
            n = dir_goto(r->nodes[i],name);
            if(n)
            {
                break;
            }
        }
    }

    return n;
}

void dir_get_total_sizes(FileNode* r,int* sum)
{
    if(!r->dir)
    {
        *sum += r->size;
        return;
    }

    for(int i = 0; i < r->node_count; ++i)
    {
        dir_get_total_sizes(r->nodes[i],sum);
    }

    r->size = *sum;
}

void dir_print(FileNode* r, int level)
{
    for(int i = 0; i < level; ++i)
        printf("  ");

    if(r->dir)
    {
        printf("[DIR] %s (%d)\n",r->name, r->size);
    }
    else
    {
        printf("-%s (%d)\n",r->name,r->size);
    }
    
    for(int i = 0; i < r->node_count; ++i)
    {
        dir_print(r->nodes[i], level+1);
    }
}
