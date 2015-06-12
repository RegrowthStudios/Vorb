template<typename T>
vcore::ThreadPool<T>::~ThreadPool() {
    destroy();
}

template<typename T>
void vcore::ThreadPool<T>::clearTasks() {
    #define BATCH_SIZE 50
    IThreadPoolTask<T>* tasks[BATCH_SIZE];
    int size;
    // Grab tasks in batches
    while ((size = m_tasks.try_dequeue_bulk(tasks, BATCH_SIZE)));
}

template<typename T>
void vcore::ThreadPool<T>::init(ui32 size) {
    // Check if its already initialized
    if (m_isInitialized) return;
    m_isInitialized = true;

    /// Allocate all threads
    m_workers.resize(size);
    for (ui32 i = 0; i < size; i++) {
        m_workers[i] = new WorkerThread(&ThreadPool::workerThreadFunc, this);
    }
}

template<typename T>
void vcore::ThreadPool<T>::destroy() {
    if (!m_isInitialized) return;

    // Tell threads to quit
    std::vector<QuitThreadPoolTask> quitTasks(m_workers.size());
    for (size_t i = 0; i < m_workers.size(); i++) {
        m_workers[i]->data.stop = true;
        m_tasks.enqueue(&quitTasks[i]);
    }
    
    // Join all threads
    for (size_t i = 0; i < m_workers.size(); i++) {
        m_workers[i]->join();
        delete m_workers[i];
    }

    // Free memory
    std::vector<WorkerThread*>().swap(m_workers);

    // Clear all tasks
    clearTasks();

    // We are no longer initialized
    m_isInitialized = false;
}

template<typename T>
void vcore::ThreadPool<T>::workerThreadFunc(T* data) {
    data->stop = false;
    IThreadPoolTask<T>* task;

    while (true) {
        // Check for exit
        if (data->stop) return;

        m_tasks.wait_dequeue(task);
        task->execute(data);
        task->setIsFinished(true);
        // Store result if needed
        if (task->shouldAddToFinishedTasks()) {
            m_finishedTasks.enqueue(task);
        }
    }
}
