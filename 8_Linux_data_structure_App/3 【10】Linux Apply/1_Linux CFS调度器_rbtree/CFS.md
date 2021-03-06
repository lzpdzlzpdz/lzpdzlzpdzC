linux内核分析——CFS（完全公平调度算法）

1.1 CFS原理
    cfs定义了一种新的模型，它给cfs_rq（cfs的run queue）中的每一个进程安排一个虚拟时钟，vruntime。如果一个进程得以执行，随着时间的增长（也就是一个个tick的到来），其vruntime将不断增大。没有得到执行的进程vruntime不变。
    而调度器总是选择vruntime跑得最慢的那个进程来执行。这就是所谓的“完全公平”。为了区别不同优先级的进程，优先级高的进程vruntime增长得慢，以至于它可能得到更多的运行机会。

1.2 CFS基本设计思路
CFS思路很简单，就是根据各个进程的权重分配运行时间(权重怎么来的后面再说)。
进程的运行时间计算公式为:
分配给进程的运行时间 = 调度周期 * 进程权重 / 所有进程权重之和   (公式1)
    调度周期很好理解，就是将所有处于TASK_RUNNING态进程都调度一遍的时间,差不多相当于O(1)调度算法中运行队列和过期队列切换一次的时间（对O(1)调度算法看得不是很熟，如有错误还望各位大虾指出）。举个例子，比如只有两个进程A, B，权重分别为1和2，调度周期设为30ms，那么分配给A的CPU时间为:30ms * (1/(1+2)) = 10ms；而B的CPU时间为：30ms * (2/(1+2)) = 20ms。那么在这30ms中A将运行10ms，B将运行20ms。
    公平怎么体现呢？它们的运行时间并不一样阿？
其实公平是体现在另外一个量上面，叫做virtual runtime(vruntime)，它记录着进程已经运行的时间，但是并不是直接记录，而是要根据进程的权重将运行时间放大或者缩小一个比例。
我们来看下从实际运行时间到vruntime的换算公式
vruntime = 实际运行时间 * 1024 / 进程权重 。 (公式2)
    为了不把大家搞晕，这里我直接写1024，实际上它等于nice为0的进程的权重，代码中是NICE_0_LOAD。也就是说，所有进程都以nice为0的进程的权重1024作为基准，计算自己的vruntime增加速度。还以上面AB两个进程为例，B的权重是A的2倍，那么B的vruntime增加速度只有A的一半。现在我们把公式2中的实际运行时间用公式1来替换，可以得到这么一个结果：
vruntime = (调度周期 * 进程权重 / 所有进程总权重) * 1024 / 进程权重 = 调度周期 * 1024 / 所有进程总权重 
看出什么眉目没有？没错，虽然进程的权重不同，但是它们的 vruntime增长速度应该是一样的 ，与权重无关。好，既然所有进程的vruntime增长速度宏观上看应该是同时推进的，
那么就可以用这个vruntime来选择运行的进程，谁的vruntime值较小就说明它以前占用cpu的时间较短，受到了“不公平”对待，因此下一个运行进程就是它。这样既能公平选择进程，又能保证高优先级进程获得较多的运行时间。这就是CFS的主要思想了。

或者可以这么理解：CFS的思想就是让每个调度实体（没有组调度的情形下就是进程，以后就说进程了）的vruntime互相追赶，而每个调度实体的vruntime增加速度不同，权重越大的增加的越慢，这样就能获得更多的cpu执行时间。

    再补充一下权重的来源，权重跟进程nice值之间有一一对应的关系，可以通过全局数组prio_to_weight来转换，nice值越大，权重越低。

1.3 CFS数据结构
介绍代码之前先介绍一下CFS相关的结构
第一个是调度实体sched_entity，它代表一个调度单位，在组调度关闭的时候可以把他等同为进程。每一个task_struct中都有一个sched_entity，进程的vruntime和权重都保存在这个结构中。那么所有的sched_entity怎么组织在一起呢？红黑树。所有的sched_entity以vruntime为key(实际上是以vruntime-min_vruntime为key，是为了防止溢出，反正结果是一样的)插入到红黑树中，同时缓存树的最左侧节点，也就是vruntime最小的节点，这样可以迅速选中vruntime最小的进程。
    注意只有等待CPU的就绪态进程在这棵树上，睡眠进程和正在运行的进程都不在树上。

 

\

 

 

1.4 Vruntime溢出问题
    之前说过红黑树中实际的作为key的不是vruntime而是vruntime-min_vruntime。min_vruntime是当前红黑树中最小的key。这是为什么呢，我们先看看vruntime的类型，是usigned long类型的，再看看key的类型，是signed long类型的，因为进程的虚拟时间是一个递增的正值，因此它不会是负数，但是它有它的上限，就是unsigned long所能表示的最大值，如果溢出了，那么它就会从0开始回滚，如果这样的话，结果会怎样？结果很严重啊，就是说会本末倒置的，比如以下例子，以unsigned char说明问题：

unsigned char a = 251，b = 254;

b += 5;//到此判断a和b的大小

看看上面的例子，b回滚了，导致a远远大于b，其实真正的结果应该是b比a大8，怎么做到真正的结果呢？改为以下：

unsigned char a = 251，b = 254;

b += 5;

signed char c = a - 250,d = b - 250;//到此判断c和d的大小

结果正确了，要的就是这个效果，可是进程的vruntime怎么用unsigned long类型而不处理溢出问题呢？因为这个vruntime的作用就是推进虚拟时钟，并没有别的用处，它可以不在乎，然而在计算红黑树的key的时候就不能不在乎了，于是减去一个最小的vruntime将所有进程的key围绕在最小vruntime的周围，这样更加容易追踪。运行队列的min_vruntime的作用就是处理溢出问题的。

 

1.5 组调度
    关于组调度，详见：《linux组调度浅析 》。简单来说，引入组调度是为了实现做一件事的一组进程与做另一件事的另一组进程的隔离。每件“事情”各自有自己的权重，而不管它需要使用多少进程来完成。在cfs中，task_group和进程是同等对待的，task_group的优先级也由用户来控制（通过cgroup文件cpu.shares）。
实现上，task_group和进程都被抽象成schedule_entity（调度实体，以下简称se），上面说到的vruntime、load、等这些东西都被封装在se里面。而task_group除了有se之外，还有cfs_rq。属于这个task_group的进程就被装在它的cfs_rq中（“组”不仅是一个被调度的实体，也是一个容器）。组调度引入以后，一系列task_group的cfs_rq组成了一个树型结构。树根是cpu所对应的cfs_rq（也就是root group的cfs_rq）、树的中间节点是各个task_group的cfs_rq、叶子节点是各个进程。
在一个task_group的两头，是两个不同的世界，就像《盗梦空间》里不同层次的梦境一样。
\

以group-1为例，它所对应的se被加入到父组（cpu_rq）的cfs_rq中，接受调度。这个se有自己的load（由对应的cpu.shares文件来配置），不管group-1下面有多少个进程，这个load都是这个值。父组看不到、也不关心group-1下的进程。父组只会根据这个se的load和它执行的时间来更新其vruntime。当group-1被调度器选中后，会继续选择其下面的task-11或task-12来执行。这里又是一套独立的体系，task-11与task-12的vruntime、load、等这些东西只影响它们在group-1的cfs_rq中的调度情况。树型结构中的每一个cfs_rq都是独立完成自己的调度逻辑。不过，从cpu配额上看，task_group的配额会被其子孙层层瓜分。
    例如上图中的task-11，它所在的group-1对应se的load是8，而group-1下两个进程的load是9和3，task-11占其中的3/4。于是，在group-1所对应的cfs_rq内部看，task-11的load是9，而从全局来看，task-11的load是8*3/4=6。而task_group下的进程的时间片也是这样层层瓜分而来的，比如说group-1的cfs_rq下只有两个进程，计算得来的调度延迟是20ms。但是task-11并非占其中的3/4（15ms）。因为group-1的se的load占总额的8/(8+3+5)=1/2，所以task-11的load占总额的1/2*3/4=3/8，时间片是20ms*3/8=7.5ms。
这样的瓜分有可能使得task_group里面的进程分得很小的时间片，从而导致频繁re-schedule。不过好在这并不影响task_group外面的其他进程，并且也可以尽量让task_group里面的进程在每个调度延迟内都执行一次。
    cfs曾经有过时间片不层层瓜分的实现，比如上图中的task-11，时间片算出来是15ms就是15ms，不用再瓜分了。这样做的好处是不会有频繁的re-schedule。但是task_group里的进程可能会很久才被执行一次。瓜分与不瓜分两种方案的示例如下（还是继续上图的例子，深蓝色代表task-11、浅蓝色是task-12，空白是其他进程）：
\
     两种方案好像很难说清孰优孰劣，貌似cfs也在这两种方案间纠结了好几次。
在进程用完其时间片之前，有可能它所在的task_group的se先用完了时间片，而被其父组re-schedule掉。这种情况下，当这个task_group的se再一次被其父组选中时，上次得到执行、且尚未用完时间片的那个进程将继续运行，直到它用完时间片。（cfs_rq->last会记录下这个尚未用完时间片的进程。）

1.6 CFS小结
    CFS还有一个重要特点，即调度粒度小。CFS之前的调度器中，除了进程调用了某些阻塞函数而主动参与调度之外，每个进程都只有在用完了时间片或者属于自己的时间配额之后才被抢占。而CFS则在每次tick都进行检查，如果当前进程不再处于红黑树的左边，就被抢占。在高负载的服务器上，通过调整调度粒度能够获得更好的调度性能。

 
转至
https://www.cnblogs.com/tianguiyu/articles/6091378.html