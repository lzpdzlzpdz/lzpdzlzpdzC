Linux C数据结构理论基础学习和实践
道生一，一生二，二生三，三生万物。
——《道德经》

一、Linux C数据结构理论篇
先打好Linux C数据结构基础，才能创造无限可能。
1.Linux C 数组简介

2.Linux C 链表简介
###典型应用
链表的应用非常广泛，绝大多数数据结构可以靠链表进行组织。

3.Linux C 哈希表简介
##hlist
###算法复杂度
添加、删除、查找的复杂度都为O(1)。
###实现
内核中哈希链表在list.h中实现，为了节省空间，单独设计了表头，表头中只有指向头结点没有指向尾节点的指针，能在海量的HASH表存储中减少一半的空间消耗，移植时，需要从include/linux/types.h中拷贝struct hlist_head的声明。
###接口API
```c
#define HLIST_HEAD_INIT { .first = NULL }
#define HLIST_HEAD(name) struct hlist_head name = {  .first = NULL }
#define INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)
static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h);
static inline void hlist_del(struct hlist_node *n);
```
需要注意的是，链表头和节点数据类型并不一致。


4.Linux C 队列简介

5.Linux C rbtree简介
红黑树是一种在插入或删除结点时都需要维持平衡的二叉查找树，并且每个结点都具有颜色属性：
    （1）、一个结点要么是红色的，要么是黑色的。
    （2）、根结点是黑色的。
    （3）、如果一个结点是红色的，那么它的子结点必须是黑色的，也就是说在沿着从根结点出发的任何路径上都不会出现两个连续的红色结点。
    （4）、从一个结点到一个NULL指针的每条路径上必须包含相同数目的黑色结点。

Linux内核中的红黑树实现代码在Linux-2.6.39 /include/linux/rbtree.h和/lib/rbtree.c文件中



##rbtree
###算法复杂度
rbtree的添加、删除、查找复杂度均为O(logn)。
###实现
需要拷贝出 include/linux/rbtree.h include/linux/rbtree_augmented.h lib/rbtree.c 文件。删除掉rbtree.c包含的头文件，增加stddef头文件，去掉EXPORT_SYMBOLE。[详解Linux内核红黑树算法的实现](http://blog.csdn.net/npy_lp/article/details/7420689)和[手把手实现红黑树](http://blog.csdn.net/chen19870707/article/details/39585277)两篇文章详细说明了其实现

6.比较
##三种结构性能对比
###数据结构的选择原则
- 遍历操作为主时，优先考虑链表；（没有数据结构能提供比线性算法复杂度更好的算法去遍历元素）
- 排除性能因素，当需要相对较少数据项时，优先考虑链表；
- 当需要与其它选择链表的代码交互时，优先考虑链表；
- 需要大小不明的数据集合，优先选择链表；
- 代码架构复合"生产者/消费者"模式，优先选择队列；
- 当需要一个定长的缓冲，选择队列；
- 如果需要映射一个UID到一个对象，选择映射；
- 如果需要存储大量数据，并且快速检索，选择红黑树；
二、Linux C数据结构实践篇——神奇书柜
设计各种各样的图书管理系统，每本书都要用一个书盒包装之后，再放到书柜上。
根据小明的不同需求，我为他设计了三个书柜，你需要哪个呢？

1.Linux C 数组的实践
1.1需求
小明开始喜欢收藏书籍，需要DIY一个“数组书柜（book cabinet）”。
（1）最多可以储存6本书
（2）可以新增图书
（3）可以借出图书
（4）可以置换破损图书
（5）可以查询图书

如图所示，数组书柜由6个书盒组成，每个书盒放置一本书。


1.2数据结构

typdef struct
{
	int useflag:1;
	int borrowflag:1;
	
	char bookname[60];
}array_book_cabinet_struct;

1.3主要功能函数
1.4编码
1.5 验证

2.Linux C 链表的实践
小东购买书籍的频次越来越高，原先购买的“数组书柜”已经不够用。同时由于资金有限，不能提前购买书盒，于是为其设计了一个“链表书柜”，最多可以存储120本书，但是事先不购买一个书盒，等到买书的时候再购买。


2.1需求分析
如图所示，每个书盒就是一个节点node。



2.2数据结构

list_head list_book_cabinet_head = {0};

typdef struct
{
	int useflag:1;
	int borrowflag:1;
	
	char bookname[60];
	list_node list;
}list_book_cabinet_struct;

2.3主要功能函数
2.4编码
2.5验证


3.Linux C 哈希表的实践
自从使用上“链表书柜之后”，小东可以存储的书变多了，但是他发现每次想查找一本书的实践可以变多了，需要从从左到右，一本一本的拆开书盒，看下里面的书面是不是自己想要的书籍，


书盒



3.1需求分析
设计一个“哈希书柜”，该哈希书柜，一共有6个槽，每个槽可以存放16本书。
查找一本书的时间，从之前最多查找120次，快速下降到最多查找20次。
该哈希书柜，采用了slot-cell模式。



3.2数据结构
#define hash_slot_max_num  6
#define cell_max_num  20
hlist_head hlist_book_cabinet_head[hash_slot_max_num] = {{0}};

typdef struct
{
	int useflag:1;
	int borrowflag:1;
	
	char bookname[60];
	hlist_node hlist;
}hlist_book_cabinet_struct;

3.3主要功能函数
3.4编码
3.5验证



4.Linux C 队列的实践
自从开始借出自己的书籍之后，来借书的人越来越多了，供不应求，书少人多，怎么办呢？
那只能先来后到了。
来借书的人，排队入场选书。

4.1需求分析
可以使用linux 链表来实现一个队列。
需要实现两个函数：
Push和pop




4.2数据结构
typedef struct
{
	int useflag;
	int borrowflag;
	
	char bookname[60];
	list_node book_queue_node;
}queue_book_struct;
4.3主要功能函数
4.4编码
4.5验证

5.Linux C rbtree的实践
现在小东觉得存放的书本杂乱无章，希望能整理下，根据书本的名称排个序。
建议使用rbtree

5.1需求分析
Rbtree，查找快，数据已经排序。



5.2数据结构
static struct rb_root BOOK_RB_ROOT =  { NULL, };
typedef struct
{
	int useflag;
	int borrowflag;
	
	char bookname[60];
	rbtree_node book_rbtree_node;
}rbtree_book_struct;

5.3主要功能函数
1. how to del a entry of rbtree
rb_erase(&pbook->book_rbtree_node,&BOOK_RB_ROOT);


2. how to add a entry
rbtree_insert(&BOOK_RB_ROOT,pbook);

3. how to compare
ret = rbtree_compare(pbookname, pbook->bookname);

5.4编码
验证
三、Linux C 商业篇
需求，分析，设计，开发，测试，商用。



四、Linux C 我问你答
1.Linux链表，如果知道一个书名，是否可以直接删除该节点？
答：不可以，需要根据书名这个关键字，查找到它在指定链表的位置，然后将该entry的节点从这个链表中删除，然后删除entry。

2.Linux 链表如果挂在两个链表头下。如何删除？
答：首先根据关键字找到entry。
如果上挂链表，直接删除调用list_del(&(pbook->book_list_node));
如果还上挂链表，继续调用list_del(&(pbook->fav_book_list_fnode));
如果下挂链表，则变量所有的子节点，一个个删除。
最后，等待删除entry的所有关系都删除之后，再将自己释放了！！！

3.如何变量一个rbtree，他是一个什么顺序？

4.Linux 内核链表 list_del_init 和 list_del 区别是什么？
