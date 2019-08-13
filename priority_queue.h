#include <vector>
using namespace std;
template <class T>
class PriorityQueue
{
private:
    int max_n_;
    int n_;
    vector<int> pq_;
    vector<int> qp_;
    vector<T> keys_;

public:
    PriorityQueue(int max_n)
        : n_(0),
          max_n_(max_n)
    {
        qp_.resize(max_n_ + 1);
        pq_.resize(max_n_ + 1);
        keys_.resize(max_n_ + 1);
        for (int i = 0; i <= max_n_; i++)
        {
            qp_[i] = -1;
        }
    }
    bool greater(int i, int j)
    {
        return keys_[pq_[i]] > keys_[pq_[j]];
    }
    void exch(int i, int j)
    {
        int swap = pq_[i];
        pq_[i] = pq_[j];
        pq_[j] = swap;
        qp_[pq_[i]] = i;
        qp_[pq_[j]] = j;
    }
    void swim(int k)
    {
        while (k > 1 && greater(k / 2, k))
        {
            exch(k, k / 2);
            k /= 2;
        }
    }

    void sink(int k)
    {
        while (2 * k <= n_)
        {
            int j = 2 * k;
            if (j < n_ && greater(j, j + 1))
                j++;
            if (!greater(k, j))
                break;
            exch(k, j);
            k = j;
        }
    }
    bool empty() { return n_ == 0; }
    int size() { return n_; }
    bool contains(int i) { return qp_[i] != -1; }
    void push(int i, T key)
    {
        if (contains(i))
        {
            return;
        }
        n_++;
        qp_[i] = n_;
        pq_[n_] = i;
        keys_[i] = key;
        swim(n_);
    }
    int pop()
    {
        int min = pq_[1];
        exch(1, n_--);
        sink(1);
        qp_[min] = -1;
        keys_[min] = T();
        pq_[n_ + 1] = -1;
        return min;
    }
    void decrease_key(int i, T key)
    {
        keys_[i] = key;
        swim(qp_[i]);
    }
};