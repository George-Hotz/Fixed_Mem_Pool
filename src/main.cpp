/*                                                              
   Author: George-Hotz(雪花)                                  
   Github：https://github.com/George-Hotz/Fixed_Mem_Pool          
*/

#include "Fixed_Mem_Pool.h"


struct TreeNode // 一个树结构的节点，等会申请空间的时候就用这个树节点来申请
{
	int _val;
	TreeNode* _left;
	TreeNode* _right;

	TreeNode()
		:_val(0)
		, _left(nullptr)
		, _right(nullptr)
	{}
};

void TestObjectPool() // malloc和当前定长内存池性能对比
{
	// 申请释放的轮次
	const size_t Rounds = 128;

	// 每轮申请释放多少次
	const size_t N = 1000000;
	
	// 这里总共申请和释放的次数就是Rounds * N次，测试这么些次谁更快

	std::vector<TreeNode*> v1;
	v1.reserve(N);
	
	// 测试malloc的性能
	size_t begin1 = clock();
	for (size_t j = 0; j < Rounds; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			v1.push_back(new TreeNode); // 这里虽然用的是new，但是new底层用的也是malloc
		}
		for (int i = 0; i < N; ++i)
		{
			delete v1[i]; // 同样的，delete底层也是free
		}
		v1.clear(); // 这里clear作用就是将vector中的内容清空，size置零，
		// 但capacity保持不变，这样才能循环上去重新push_back
	}
	size_t end1 = clock();
	
		
	std::vector<TreeNode*> v2;
	v2.reserve(N);
	
	// 定长内存池，其中申请和释放的T类型就是树节点
	Fixed_Mem_Pool<TreeNode> TNPool(256 * 1024);
	size_t begin2 = clock();
	for (size_t j = 0; j < Rounds; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			v2.push_back(TNPool.palloc()); // 定长内存池中的申请空间
		}
		for (int i = 0; i < N; ++i)
		{
			TNPool.pfree(v2[i]); // 定长内存池中的回收空间
		}
		v2.clear();// 这里clear作用就是将vector中的内容清空，size置零，
		// 但capacity保持不变，这样才能循环上去重新push_back
	}
	size_t end2 = clock();

	double cpu_time_used1 = ((double) (end1 - begin1)) / CLOCKS_PER_SEC;
	double cpu_time_used2 = ((double) (end2 - begin2)) / CLOCKS_PER_SEC;

	std::cout << "malloc cost time:" << cpu_time_used1 << "s" << std::endl; 
	std::cout << "mem_pool cost time:" << cpu_time_used2 << "s" << std::endl;
}


int main(){
    TestObjectPool();
    return 0;
}