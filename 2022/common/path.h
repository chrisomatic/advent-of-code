#pragma once

typedef struct
{
    int x,y;
} PathPos;

typedef struct PathNode PathNode;
struct PathNode
{
    int x,y;
    int g_score,h_score;
    int height;
    struct PathNode* parent;
};

#define PATH_MAX_QUEUE_NODES 1024*10

typedef struct PathNodeQueue PathNodeQueue;
struct PathNodeQueue
{
    PathNode nodes[PATH_MAX_QUEUE_NODES];
    int node_count;
};

int* path_map;
int path_map_width;
int path_map_height;

void path_print_queue(PathNodeQueue* q)
{
    printf("Queue (size: %d)\n",q->node_count);
    for(int i = 0; i < q->node_count; ++i)
    {
        printf("%c [%p -> %p]",q->nodes[i].height,&q->nodes[i],q->nodes[i].parent);
    }
    printf("\n");
}

bool path_node_enqueue(PathNodeQueue* q, PathNode* node)
{
    if(q->node_count >= PATH_MAX_QUEUE_NODES)
    {
        printf("Path Node Queue is full!\n");
        return false;
    }

    memcpy(&q->nodes[q->node_count],node,sizeof(PathNode));
    q->node_count++;
    path_print_queue(q);
    return true;
}

PathNode* path_node_dequeue(PathNodeQueue* q, bool use_priority)
{
    if(q->node_count == 0)
        return NULL;

    PathNode* min = &q->nodes[0];

    int lowest_index = 0;

    if(use_priority)
    {
        for(int i = 1; i < q->node_count; ++i)
        {
            int f_score = q->nodes[i].g_score + q->nodes[i].h_score;
            int f_score_min = min->g_score + min->h_score;
            
            if(f_score < f_score_min || (f_score == f_score_min && q->nodes[i].h_score < min->h_score))
            {
                min = &q->nodes[i];
                lowest_index = i;
            }
        }
    }

    // shift nodes down
    int remaining_nodes = q->node_count - lowest_index -1;
    if(remaining_nodes > 0)
    {
        memcpy(min,min+1,remaining_nodes*sizeof(PathNode));
    }

    q->node_count--;

    return min;
}

bool path_node_queue_contains(PathNodeQueue* q,PathNode* n)
{
    for(int i = 0; i < q->node_count; ++i)
    {
        if(q->nodes[i].x == n->x && q->nodes[i].y == n->y)
        {
            return true;
        }
    }
    return false;
}

int path_node_get_manhatten_distance(PathNode* a, PathNode* b)
{
    int dx = ABS(a->x - b->x);
    int dy = ABS(a->y - b->y);
    return 10*(dx+dy);
}

int path_get_neighbors(PathNode* n, PathNode* neighbors, int direction_count)
{
    int x = n->x;
    int y = n->y;

    PathPos up    = {.x = x, .y = y - 1};
    PathPos down  = {.x = x, .y = y + 1};
    PathPos left  = {.x = x - 1, .y = y};
    PathPos right = {.x = x + 1, .y = y};

    PathPos dirs[] = {up,down,left,right};

    int neighbor_count = 0;

    for(int i = 0; i < 4; ++i)
    {
        PathPos* dir = &dirs[i];

        if(dir->x >= 0 && dir->x < path_map_width)
        {
            if(dir->y >= 0 && dir->y < path_map_height)
            {
                // is neighbor walkable?
                int c = path_map[(dir->y * path_map_width) + dir->x];

                if(c <= n->height + 1)
                {
                    PathNode* b = &neighbors[neighbor_count++];

                    b->x = dir->x;
                    b->y = dir->y;
                    b->height = c;
                }
            }
        }
    }
    return neighbor_count;
}


PathNodeQueue path_open   = {0};
PathNodeQueue path_closed = {0};

void path_reconstruct(PathNode* start, PathNode* end)
{
    PathNodeQueue path;
    PathNode* current = end;

    while(current && current != start)
    {
        path_node_enqueue(&path, current);
        printf("[%p]\n",current);
        if(current->parent == current)
            break;
        
        current = current->parent;
    }

    printf("Path Count: %d\n",path.node_count);
    
    while(path.node_count > 0)
    {
        PathNode* n = path_node_dequeue(&path,false);
        printf("%c [%d,%d]\n",n->height,n->x,n->y);
    }
}

void path_map_set(int* map, int width, int height)
{
    path_map = map;
    path_map_width = width;
    path_map_height = height;
}

void path_find(int start_x, int start_y, int end_x, int end_y)
{

    /*
    // @TEST
    PathNodeQueue q = {0};
    PathNode a,b,c;

    a.height = 'a'; a.g_score = 2;
    b.height = 'b'; b.g_score = 1;
    c.height = 'c'; c.g_score = 3;

    path_node_enqueue(&q,&a);
    path_node_enqueue(&q,&b);
    path_node_enqueue(&q,&c);

    path_print_queue(&q);

    path_node_dequeue(&q,false);
    path_print_queue(&q);

    path_node_dequeue(&q,false);
    path_print_queue(&q);

    return;
    */

    if(!path_map)
    {
        printf("Path Map is not set!\n");
        return;
    }

    int hs = path_map[start_y*path_map_width + start_x];

    PathNode start = {
        .x = start_x,
        .y = start_y,
        .height = hs,
    };

    int he = path_map[end_y*path_map_width + end_x];

    PathNode target = {
        .x = end_x,
        .y = end_y,
        .height = he,
    };
    
    path_node_enqueue(&path_open,&start);

    PathNode* begin = &path_open.nodes[0];

    while(path_open.node_count > 0)
    {
        PathNode* current = path_node_dequeue(&path_open,true);
        path_node_enqueue(&path_closed,current);

        if(current->x == target.x && current->y == target.y)
        {
            // Done.
            printf("Done!\n");
            path_reconstruct(begin,current);
            return;
        }

        PathNode neighbors[4] = {0};
        int neighbor_count = path_get_neighbors(current, neighbors, 4);

        printf("neighbors (%d)\n", neighbor_count);
        for(int i = 0; i < neighbor_count; ++i)
        {
            PathNode* neighbor = &neighbors[i];
            printf("  %d: %c\n",i, neighbors[i].height);
            if(neighbor->height > current->height+1 || path_node_queue_contains(&path_closed,neighbor))
            {
                // not traversible or in closed group
                continue;
            }

            int movement_cost_to_neighbor = current->g_score + path_node_get_manhatten_distance(current,neighbor);
            bool neighbor_in_open = path_node_queue_contains(&path_open,neighbor);
            if(movement_cost_to_neighbor < neighbor->g_score || !neighbor_in_open)
            {
                neighbor->g_score = movement_cost_to_neighbor;
                neighbor->h_score = path_node_get_manhatten_distance(neighbor,&target);
                neighbor->parent = current;

                if(!neighbor_in_open)
                {
                    path_node_enqueue(&path_open,neighbor);
                }

            }

            
        }
    }

    /*

    // The set of discovered nodes that may need to be (re-)expanded.
    // Initially, only the start node is known.
    // This is usually implemented as a min-heap or priority queue rather than a hash-set.
    openSet := {start}
    

    // For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from the start
    // to n currently known.
    cameFrom := an empty map

    // For node n, gScore[n] is the cost of the cheapest path from start to n currently known.
    gScore := map with default value of Infinity
    gScore[start] := 0

    // For node n, fScore[n] := gScore[n] + h(n). fScore[n] represents our current best guess as to
    // how cheap a path could be from start to finish if it goes through n.
    fScore := map with default value of Infinity
    fScore[start] := h(start)

    while openSet is not empty
        // This operation can occur in O(Log(N)) time if openSet is a min-heap or a priority queue
        current := the node in openSet having the lowest fScore[] value
        if current = goal
            return reconstruct_path(cameFrom, current)

        openSet.Remove(current)
        for each neighbor of current
            // d(current,neighbor) is the weight of the edge from current to neighbor
            // tentative_gScore is the distance from start to the neighbor through current
            tentative_gScore := gScore[current] + d(current, neighbor)
            if tentative_gScore < gScore[neighbor]
                // This path to neighbor is better than any previous one. Record it!
                cameFrom[neighbor] := current
                gScore[neighbor] := tentative_gScore
                fScore[neighbor] := tentative_gScore + h(neighbor)
                if neighbor not in openSet
                    openSet.add(neighbor)

    // Open set is empty but goal was never reached
    return failure
    */

}
