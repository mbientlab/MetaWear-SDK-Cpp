#include <mutex>
#include <queue>

template <class T>
class ConcurrentQueue {
public:
    ConcurrentQueue() = default;

    void push(const T& item);
    void push(T&& item);

    T& front();
    const T& front() const;

    void pop();
    void clear();

    bool empty() const;
    size_t size() const;
private:
    mutable std::mutex queue_mutex;
    std::queue<T> elements;
};

template <class T>
void ConcurrentQueue<T>::push(const T& item) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    elements.push(item);
}

template <class T>
void ConcurrentQueue<T>::push(T&& item) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    elements.push(item);
}

template <class T>
T& ConcurrentQueue<T>::front() {
    std::lock_guard<std::mutex> lock(queue_mutex);
    return elements.front();
}

template <class T>
const T& ConcurrentQueue<T>::front() const {
    std::lock_guard<std::mutex> lock(queue_mutex);
    return elements.front();
}

template <class T>
void ConcurrentQueue<T>::pop() {
    std::lock_guard<std::mutex> lock(queue_mutex);
    elements.pop();
}

template <class T>
void ConcurrentQueue<T>::clear() {
    std::lock_guard<std::mutex> lock(queue_mutex);
    while(!elements.empty()) {
        elements.pop();
    }
}

template <class T>
bool ConcurrentQueue<T>::empty() const {
    std::lock_guard<std::mutex> lock(queue_mutex);
    return elements.empty();
}

template <class T>
size_t ConcurrentQueue<T>::size() const {
    std::lock_guard<std::mutex> lock(queue_mutex);
    return elements.size();
}
