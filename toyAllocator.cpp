#include<iostream>
using namespace std;
#define MAX 1024//The heap has 1024 bytes total capacity.
//Each index in heap[] corresponds to 1 byte of usable memory.
//The bookkeeping array used[HEAP_SIZE] also has 1024 entries, each storing 0 (free) or 1 (allocated).


struct memory_allocator{
    static unsigned char heap[MAX];
    static unsigned char used [MAX];//bookkeeping array
};

//If you allocate 10 bytes, you find 10 consecutive zeros in used[], mark them as ones, and return a pointer to the corresponding heap[] index.
//If you free 10 bytes, you flip those entries back to zero.
unsigned char memory_allocator :: heap[MAX];//defining outside struct
unsigned char memory_allocator :: used[MAX];//bookkeeping array

void initial_allocator(){
    for (int i=0;i<MAX;i++){
        memory_allocator :: used [i]=0;//mark everthing free
    }
}

//for double loop
/*unsigned char *allocate(int required_size){
     for (int i=0;i<MAX-required_size;i++){
        bool free_block=true;
        for (int j=0;i<required_size;j++){
        if(used[i+j]==1){
         bool free_block=false;
         break;
        }
        }
        if (free_block){
            for (int j=0;i<required_size;j++){
            used[i+j]=1;//mark block as used
            }
            return &heap[i];//for the caller no the internal program it knows after we convert 0 int 1
            
        }
     }
     return NULL; 
}
*/

//single loop
unsigned char *allocate ( int required_size){
    if (required_size>MAX || required_size<=0) {
        return nullptr;
    }
    int start =-1;
    int count=0;
    for(int i=0;i<MAX;i++){
        if (memory_allocator::used[i]==0) {
            if (count==0) {
                start=i;
            }
            count++;
            if (count==required_size){
                for (int j=start;j<start+required_size;j++){//if start at 3 then 3+req say 6 then index 3 to 8
                    memory_allocator::used[j]=1;//used
                }   
                return &memory_allocator:: heap[start];//Without this pointer, the caller could not use the heap at all. The allocator would be silently reserving memory, but the program would never know where to put its data.
            }
        } else {
            start= -1;
            count=0;
        }
    }
    return nullptr;//if whole heap scanned and no block found
}

/*unsigned char deallocate(int start ,int size){
    for (int i =start ;i<start+size; i++){
   memory_allocator:: used [i]=0;
    }
   
}*/
void deallocate(unsigned char *ptr,int size ){
    if (ptr==nullptr || size<=0) return;
    int start_index = ptr - memory_allocator::heap;//ptr we decalre in main function which will ofc give the pointer we got in allocator..and heap used 0 index 
    if (start_index<0|| start_index>=MAX) return;
    for(int i=start_index;i<start_index+size;i++){
        memory_allocator::used[i]=0;
    }
}

int main(){
    initial_allocator();
    int size1;
    cout <<"enter the number of bytes you wanna allocate"<<endl;
    cin>>size1;
    unsigned char *ptr = allocate(size1);
    int start_index=ptr-memory_allocator::heap;    //we know the the value of ptr wil be pointr ie heap[start] as allocate returns it
    if(ptr!=nullptr){//if null it wil not run
        cout <<"allocated bytes "<<size1<<"and index are"<<endl;
        for (int i=start_index;i<start_index+size1;i++){
            cout<<i<<endl;
        }
        cout<<endl;
    }
    else {
        cout <<"allocation failed"<<endl;
        return 1;
    }
    
    deallocate(ptr,size1);
    cout <<"deallocated bytes "<<size1<<endl;//and in this case we start and end
    return 0;
}
/*for testing 
     unsigned char *ptr1 = allocate(10);
    unsigned char *ptr2 = allocate(20);
    unsigned char *ptr3 = allocate(5);

    cout << "ptr1 allocated at index: " << ptr1 - memory_allocator::heap << endl;
    cout << "ptr2 allocated at index: " << ptr2 - memory_allocator::heap << endl;
    cout << "ptr3 allocated at index: " << ptr3 - memory_allocator::heap << endl;

    deallocate(ptr2, 20);
    cout << "Deallocated ptr2 (20 bytes)" << endl;

    unsigned char *ptr4 = allocate(15); // should reuse freed space from ptr2
    cout << "ptr4 allocated at index: " << ptr4 - memory_allocator::heap << endl;
*/
/*output
ptr1 = allocate(10) → uses indices 0–9.

ptr2 = allocate(20) → uses indices 10–29.

ptr3 = allocate(5) → uses indices 30–34.

After deallocating ptr2 (20 bytes)
Yes, the remaining 5 bytes from ptr2  are still free bcz ptr 4 is taking 15 .
but if we add more 10 bytes it will start form 35 as we dont have the algorithm that will those 5 bytes and the next remaining bytes
There’s plenty of free space after index 34 as well.*/