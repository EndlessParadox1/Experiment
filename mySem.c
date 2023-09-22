#include "mySem.h"

void sem_init(my_sem* s, int v) {
    s->value = v;
    pthread_mutex_init(&s->lock, NULL);
    pthread_cond_init(&s->cond, NULL);
}

void sem_wait(my_sem* s) {
    pthread_mutex_lock(&s->lock);
    while(s->value <= 0)
        pthread_cond_wait(&s->cond, &s->lock);
    s->value --;
    pthread_mutex_unlock(&s->lock);
}

void sem_post(my_sem* s) {
    pthread_mutex_lock(&s->lock);
    s->value ++;
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock(&s->lock);
}


void put(int v, queue* q) {
    *(q->buffer + q->tail) = v;
    q->tail = (q->tail + 1) % q->size;
    printf("put %d\n", v);
}

int get(queue* q) {
    int tmp = *(q->buffer + q->head);
    q->head = (q->head + 1) % q->size;
    printf("get %d\n", tmp);
    return tmp;
}

void* producer(void* args_) {
    args* tmp = (args*)args_;
    queue* q = tmp->q;
    for(int i = tmp->low; i <= tmp->high; i++) {
        sem_wait(&q->empty);
        sem_wait(&q->lock);
        put(i, q);
        sem_post(&q->lock);
        sem_post(&q->full);
    }
    return NULL;
}

void* consumer(void* args_) {
    args* tmp = (args*)args_;
    queue* q = tmp->q;
    for(int i = tmp->low; i <= tmp->high; i++) {
        sem_wait(&q->full);
        sem_wait(&q->lock);
        get(q);
        sem_post(&q->lock);
        sem_post(&q->empty);
    }
    return NULL;
}
