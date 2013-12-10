#include "queue.h"

typedef struct _queue_node_t queue_node_t;

struct _queue_node_t {
    queue_node_t* previous;
    queue_node_t* next;
    void* value;
};

struct _queue_t {
    queue_node_t* head;
    queue_node_t* tail;
    size_t size;
    bool autofree;
};


static queue_node_t* queue_node_new(void* value){
    assert(value);

    queue_node_t* node = (queue_node_t*) malloc(sizeof(queue_node_t));
    if(NULL == node) {
        return NULL;
    }

    node->previous = NULL;
    node->next = NULL;
    node->value = value;
    return node;
}

static void queue_node_destroy(queue_node_t** node, bool autofree){
    assert(node);
    // queue doesn't own elements, so we need to deallocate only internal nodes
    if(*node) {
        if(autofree)
            free(*node->value);
        free(*node);
        *node = NULL;
    }
}


queue_t* queue_new(void) {
    queue_t* self = (queue_t*) malloc(sizeof(queue_t));
    if(!self)
        return NULL;

    self->head = NULL;
    self->tail = NULL;
    self->size = 0;
    self->autofree = false;

    return self;
}


void queue_destroy(queue_t** queue_ptr) {
    assert(queue_ptr);

    queue_t* self = *queue_ptr;
    if(self) {
        node = self->tail;
        while(node) {
            next=node->next;
            queue_node_destroy(&node, self->autofree);
            node = next;
        }
        *queue_ptr = NULL;
    }
}

bool queue_is_empty(queue_t* self) {
    return (0 == queue_size(self));
}

void queue_push(queue_t* self, void* element) {
    assert(self);

    queue_node_t* node = queue_node_new(element);
    if(!node) {
        return; // TODO can we do better?
    }

    if(queue_is_empty(self)) {
        self->head = node;
        self->tail = node;
        self->size++;
    }
    else {
        node->next = self->tail;
        self->tail->previous = node;
        self->tail = node;
        self->size++;
    }
}


void queue_autofree(queue_t* self) {
    assert(self);

    self->autofree = true;
}

