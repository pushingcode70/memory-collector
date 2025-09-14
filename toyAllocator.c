#include <stdio.h>

#define MAX 1024

unsigned char heap[MAX];
unsigned char used[MAX];

void initial_allocator() {
    for (int i = 0; i < MAX; i++)
        used[i] = 0;
}

unsigned char* allocate(int required_size) {
    if (required_size <= 0 || required_size > MAX) return NULL;
    int start = -1, count = 0;
    for (int i = 0; i < MAX; i++) {
        if (used[i] == 0) {
            if (count == 0) start = i;
            count++;
            if (count == required_size) {
                for (int j = start; j < start + required_size; j++)
                    used[j] = 1;
                return &heap[start];
            }
        } else {
            start = -1;
            count = 0;
        }
    }
    return NULL;
}

void deallocate(unsigned char *ptr, int size) {
    if (ptr == NULL || size <= 0) return;
    int start_index = ptr - heap;
    if (start_index < 0 || start_index >= MAX) return;
    for (int i = start_index; i < start_index + size; i++)
        used[i] = 0;
}

int main() {
    initial_allocator();
    int size1;
    printf("enter the number of bytes you wanna allocate\n");
    scanf("%d", &size1);
    unsigned char *ptr = allocate(size1);
    int start_index = ptr - heap;
    if (ptr != NULL) {
        printf("allocated bytes %d and index are\n", size1);
        for (int i = start_index; i < start_index + size1; i++)
            printf("%d\n", i);
    } else {
        printf("allocation failed\n");
        return 1;
    }
    
    deallocate(ptr, size1);
    printf("deallocated bytes %d\n", size1);

    unsigned char *ptr1 = allocate(10);
    unsigned char *ptr2 = allocate(20);
    unsigned char *ptr3 = allocate(5);

    printf("ptr1 allocated at index: %d\n", ptr1 - heap);
    printf("ptr2 allocated at index: %d\n", ptr2 - heap);
    printf("ptr3 allocated at index: %d\n", ptr3 - heap);

    deallocate(ptr2, 20);
    printf("Deallocated ptr2 (20 bytes)\n");

    unsigned char *ptr4 = allocate(15);
    printf("ptr4 allocated at index: %d\n", ptr4 - heap);

    return 0;
}
