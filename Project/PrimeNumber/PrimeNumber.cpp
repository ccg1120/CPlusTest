#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <windows.h> 


using namespace std;


const int Max_Count = 150000;
const int Max_ThreadNumber = 4;



bool IsPrimeNumber(int start);
void ShowPrimeNumber(const vector<int>& primes);

int main()
{

	chrono::system_clock::time_point start_time = chrono::system_clock::now();

	mutex m_LockNum;
	recursive_mutex m_Lock_guardNum;
	recursive_mutex m_Lock_guardVectors;
	//lock_guard<recursive_mutex>lock(m_Lock_guardNum);

	mutex m_LockVector;
	CRITICAL_SECTION m_LockNumCritical;
	CRITICAL_SECTION m_LockVectorCritical;
	
	InitializeCriticalSection(&m_LockNumCritical);
	InitializeCriticalSection(&m_LockVectorCritical);
	int num = 1;
	vector<int> primes;
	
	
	vector<shared_ptr<thread>> threads;

	for (size_t i = 0; i < Max_ThreadNumber; i++)
	{
		shared_ptr<thread> thread(new thread(
			[&](){
				while (true)
				{
					int n;
					{
						lock_guard<recursive_mutex>lock(m_Lock_guardNum);
						//m_LockNum.lock();
						//EnterCriticalSection(&m_LockNumCritical);
						
						n = num;
						num++;
					}
					//LeaveCriticalSection(&m_LockNumCritical);
					//m_LockNum.unlock();

					if (n >= Max_Count)
					{
						//m_LockNum.unlock();
						break;
					}

					if (IsPrimeNumber(n))
					{
						//EnterCriticalSection(&m_LockVectorCritical);
						//m_LockVector.lock();
						{
							lock_guard<recursive_mutex>lock(m_Lock_guardVectors);
							primes.push_back(n);
						}
						//m_LockVector.unlock();
						//LeaveCriticalSection(&m_LockVectorCritical);
					}
					//m_LockNum.unlock();
				}
			}
		));

		threads.push_back(thread);
	}

	for (size_t i = 0; i < threads.size(); i++)
	{
		threads[i]->join();
	}

	DeleteCriticalSection(&m_LockNumCritical);
	DeleteCriticalSection(&m_LockVectorCritical);

	chrono::system_clock::time_point end_time = chrono::system_clock::now();

	auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

	cout << "Took " << duration << " Miliseconds ." << endl;
	cout << "prime Count :" << primes.size();

	//ShowPrimeNumber(primes);

	
	return 0;
}

bool IsPrimeNumber(int num)
{
	if (num == 1)
	{
		return false;
	}
	else if (num == 2 || num == 3)
	{
		return true;
	}

	int len = num - 1;

	for (int i = 2; i < len; i++)
	{
		if ((num % i) == 0)
		{
			return false;
		}
	}

}

void ShowPrimeNumber(const vector<int>& primes)
{
	for (size_t i = 0; i < primes.size(); i++)
	{
		cout << "prime " << (i + 1) << " : " << primes[i] << endl;
	}
}
