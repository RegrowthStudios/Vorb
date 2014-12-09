#include "stdafx.h"
#include "GLRPC.h"

void vg::GLRPCManager::invoke(GLRPC* so, bool blockUntilFinished /*= true*/) {
    m_queue.add(so, blockUntilFinished);
}
size_t vg::GLRPCManager::processRequests(size_t maxRequests /*= GLRPC_REQUESTS_PER_PROCESS*/) {
    if (maxRequests == 0) return 0;
    if (maxRequests > GLRPC_REQUESTS_PER_PROCESS) maxRequests = GLRPC_REQUESTS_PER_PROCESS;

    // Finish unfinished requests
    size_t n = maxRequests;
    while (m_unfinished.size() > 0 && n > 0) {
        GLRPC* rpc = m_unfinished.front();
        m_unfinished.pop();
        
        rpc->data.f->invoke(this, rpc->data.userData);
        m_queue.finish(rpc);
        n--;
    }
    if (n == 0) return maxRequests;

    // Obtain a bunch of requests
    GLRPC* arr[GLRPC_REQUESTS_PER_PROCESS];
    size_t c = m_queue.getObjects(arr);

    // Handle all requests
    size_t i = 0;
    for (; i < c && n > 0; i++) {
        GLRPCData& rpc = arr[i]->data;

        rpc.f->invoke(this, rpc.userData);
        m_queue.finish(arr[i]);
        n--;
    }

    // Add unfinished requests
    for (; i < c; i++) m_unfinished.push(arr[i]);

    return maxRequests - n;
}

