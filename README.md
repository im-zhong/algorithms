# algorithms
Introduction to Algorithms, Third Edition

# 要求
1. 所有的算法在头文件内实现，头文件最上头放上声明 然后在下面实现，算法的实现尽可能用c去实现
2. 测试文件如果用到了C++的内容，可以使用cpp后缀，如果没有用到就用c
3. 所有算法按照 container sort search graph string 等大的方向分隔开,同时也是一级文件夹，文件夹内部可以分情况划分出来 比如search/binary_search, btree, rbtree, binary_tree
4. 其他算法实现可能是使用container内部的容器，他们都是用头文件实现 所以别的文件只需要包含头文件即可，参考example的写法 如何使用container内部的头文件
5. todo: 实现一个类似C++ string的容器，这样我就可以全部改成C的代码了
6. todo: 实现一个类似C++ vector的容器
7. todo: 用list实现一个set，就可以代替cpp的set作为baseline，这样就可以用纯c代码实现整个项目了，毕竟只做正确性验证，对性能不做要求
8. todo: 列一下想要实现的数据结构吧，还挺多的，一时半会实现不完: avl, 伸展树（自底向上，简单）, 跳表, treap树, AA树就是红黑树的变种，没啥意思。 斐波那契堆和配对堆（pair Heap）. 图论算法大部分算法书都是讲的最小生成树，单源最短路，最短路和流，后两个不打算实现。目前没有实现的是稀疏图，有向图无向图的连通性，环路等等问题，还没有研究，总之研究的非常少，导致我并不是特别了解图。之后还是好好的整理一下。稀疏图稠密图的相关算法。然后还剩下的就是一些算法设计技巧及相关的问题。动态规划，贪心，分治等等。可以参考算法书上的一些例子学一学就可以了。
9. todo: 我又寻思了一下，不好，还是用纯cmake来管理项目更好，之前我不会make，老是忘，但是自从写了acpl2e之后，忘了我也可以快速参考一下如何写make文件，非常方便。我一直学不会cmake的原因就是没有一个大型的自己写出来的cmake项目，现在花点力气改造完成，以后哪怕忘了cmake也可以快速参考。参考别人的永远都记不住。拉一个新的分支做这件事情，refactor,这样现在写的makefile也可以作为参考。
10. todo: 学python，之后对有意思的算法做可视化
11. 暂时就这样吧 还差search没弄，不过大同小异，cmake的基础使用算是明白了一点点