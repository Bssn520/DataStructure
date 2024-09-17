/**
 * 列表类
 * 
 * 包含: 数组, 列表容量，列表大小，列表每次扩容的倍数
 */
typedef struct
{
    int* arr;       // 数组，存储列表元素
    int capacity;   // 列表的最大容量
    int size;       // 列表中的元素数量
    int extendRadio; // 每次扩容的倍数
} MyList;


MyList* newMyList();
void destoryMyList(MyList* list);
int size(MyList* list);
int capacity(MyList* list);
void extendCapacity(MyList* list);
int getElement(MyList* list, int index);
int setElement(MyList* list, int index, int val);
void pushElement(MyList* list, int val);
void insertElement(MyList* list, int index, int val);
int delElement(MyList* list, int index);
int* toArray(MyList* list);
void arrPrint(int* arr, int size);