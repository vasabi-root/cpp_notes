I know about [this](https://stackoverflow.com/questions/18164822/why-hastables-rehash-complexity-may-be-quadratic-in-worst-case) question, but mine is a bit different.

Why [`rehash`](https://en.cppreference.com/w/cpp/container/unordered_map/rehash) has quadratic complexity, but [`operator []`](https://en.cppreference.com/w/cpp/container/unordered_map/operator_at) (which can call `rehash`) has linear complexity in worst case?

Sorry, but I don't have 10 points for adding images, so keep the links:  
[`rehash` complexity screen](https://i.stack.imgur.com/oeC8D.png)  
[`operator[]` complexity screen](https://i.stack.imgur.com/dRocq.png)

`rehash` complexity screen  
![](https://i.stack.imgur.com/oeC8D.png)  

`operator[]` complexity screen  
![](https://i.stack.imgur.com/dRocq.png)

It looks like one of the statements is incorrect...

I know why rehash can has the quadratic complexity. However, it is not difficult to make it linear. Therefore, either statement can be true, but not both together (only if different sizes are meant, but then I don’t understand what can be taken as a size, except for the number of elements)

@Evg, there are several reasons why rehash() can be quadratic (sorry, many letters):  
1. Like you noticed before, all buckets form a big list. So, when you need to rehash AND you want to save references and pointers, you need to rearrange the elements of the list (instead of making new one).  
To implement this, you need to iterate through the list, extract current element and insert it to the correct place. And *correct place* is the place in the array of pointers to the nodes with the index equal to the new hash of the current element. This insertion can be implemented like insertion to the **end** of the bucket (and then we have $O(n^2)$) or to the **start** of the bucket (and then we have $O(n)$).

2. `strong exception-safety`  mechanisms, the implementation of which I do not know, may be the cause 

@Evg, I don't know why it *must* be quadratic, but I search for GCC implementations (like you advised before). I found [rehash](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/unordered_map.h#L1123) that calls [_M_rehash](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/hashtable.h#L2621) and it seems linear