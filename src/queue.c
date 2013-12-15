#include <stdlib.h>
#include <assert.h>

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
};


static queue_node_t* queue_node_new(void* value){
    queue_node_t* node;

    assert(value);

    node = (queue_node_t*) malloc(sizeof(queue_node_t));
    if(NULL == node) {
        return NULL;
    }

    node->previous = NULL;
    node->next = NULL;
    node->value = value;
    return node;
}

static void queue_node_destroy(queue_node_t** node){
    assert(node);
    // queue doesn't own elements, so we need to deallocate only internal nodes
    if(*node) {
        free(*node);
        *node = NULL;
    }
}

aoc_queue_t* aoc_queue_new(void) {
    aoc_queue_t* self = (aoc_queue_t*) malloc(sizeof(aoc_queue_t));
    if(!self)
        return NULL;

    self->head = NULL;
    self->tail = NULL;
    self->size = 0;

    return self;
}


void aoc_queue_destroy(aoc_queue_t** queue_ptr) {
    queue_node_t* node;
    queue_node_t* next;
    aoc_queue_t* self;

    assert(queue_ptr);

    node = NULL;
    next = NULL;
    self = *queue_ptr;

    if(self) {
        node = self->tail;
        while(node) {
            next=node->next;
            queue_node_destroy(&node);
            node = next;
        }
        *queue_ptr = NULL;
    }
}

bool aoc_queue_is_empty(aoc_queue_t* self) {
    return (0 == aoc_queue_size(self));
}

void aoc_queue_push(aoc_queue_t* self, void* element) {
    queue_node_t* node;

    assert(self);

    node = queue_node_new(element);
    if(!node) {
        return; // TODO can we do better?
    }

    if(aoc_queue_is_empty(self)) {
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

void* aoc_queue_pull(aoc_queue_t* self) {
    void* value;
    queue_node_t* node;

    assert(self);

    value = NULL;

    if(aoc_queue_is_empty(self)) {
        return NULL;
    }

    node = self->head;
    if(self->head == self->tail) {
        self->tail = self->head = NULL;
    }
    else {
        self->head = node->previous;
        self->head->next = NULL;
    }

    value = node->value;
    queue_node_destroy(&node);
    self->size--;

    return value;
}

size_t aoc_queue_size(aoc_queue_t* self) {
    assert(self);
    return self->size;
}
