#ifndef __REQUEST_H__

void requestHandle(int fd, struct timeval arrival, struct timeval differnce, int thread_index, int* static_threads,
                  int* dynamic_threads , int* total_threads);

#endif
