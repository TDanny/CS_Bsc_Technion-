//
// request.c: Does the bulk of the work for the web server.
// 

#include "segel.h"
#include "request.h"

// requestError(      fd,    filename,        "404",    "Not found", "OS-HW3 Server could not find this file");
void requestError(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg , struct timeval arrival,
                  struct timeval differnce , int thread_index, int* static_threads, int* dynamic_threads,
                  int* total_threads) 
{
   char buf[MAXLINE], body[MAXBUF];

   // Create the body of the error message
   sprintf(body, "<html><title>OS-HW3 Error</title>");
   sprintf(body, "%s<body bgcolor=""fffff"">\r\n", body);
   sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
   sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
   sprintf(body, "%s<hr>OS-HW3 Web Server\r\n", body);

   // Write out the header information for this response
   sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
   Rio_writen(fd, buf, strlen(buf));
   //printf("%s", buf);

   sprintf(buf, "Content-Type: text/html\r\n");
   Rio_writen(fd, buf, strlen(buf));
   //printf("%s", buf);

   sprintf(buf, "Content-Length: %lu\r\n", strlen(body));
   //Rio_writen(fd, buf, strlen(buf));
   //printf("%s", buf);

   sprintf(buf, "%sStat-Req-Arrival:: %lu.%06lu\r\n", buf, arrival.tv_sec, arrival.tv_usec);
   sprintf(buf, "%sStat-Req-Dispatch:: %lu.%06lu\r\n", buf, differnce.tv_sec, differnce.tv_usec);
   sprintf(buf, "%sStat-Thread-Id:: %d\r\n", buf, thread_index);
   sprintf(buf, "%sStat-Thread-Count:: %d\r\n", buf, total_threads[thread_index]);
   sprintf(buf, "%sStat-Thread-Static:: %d\r\n", buf, static_threads[thread_index]);
   sprintf(buf, "%sStat-Thread-Dynamic:: %d\r\n\r\n", buf, dynamic_threads[thread_index]);
   Rio_writen(fd, buf, strlen(buf));
   // Write out the content
   Rio_writen(fd, body, strlen(body));
   printf("%s", body);

}


//
// Reads and discards everything up to an empty text line
//
void requestReadhdrs(rio_t *rp)
{
   char buf[MAXLINE];

   Rio_readlineb(rp, buf, MAXLINE);
   while (strcmp(buf, "\r\n")) {
      Rio_readlineb(rp, buf, MAXLINE);
   }
   return;
}

//
// Return 1 if static, 0 if dynamic content
// Calculates filename (and cgiargs, for dynamic) from uri
//
int requestParseURI(char *uri, char *filename, char *cgiargs) 
{
   char *ptr;

   if (strstr(uri, "..")) {
      sprintf(filename, "./public/home.html");
      return 1;
   }

   if (!strstr(uri, "cgi")) {
      // static
      strcpy(cgiargs, "");
      sprintf(filename, "./public/%s", uri);
      if (uri[strlen(uri)-1] == '/') {
         strcat(filename, "home.html");
      }
      return 1;
   } else {
      // dynamic
      ptr = index(uri, '?');
      if (ptr) {
         strcpy(cgiargs, ptr+1);
         *ptr = '\0';
      } else {
         strcpy(cgiargs, "");
      }
      sprintf(filename, "./public/%s", uri);
      return 0;
   }
}

//
// Fills in the filetype given the filename
//
void requestGetFiletype(char *filename, char *filetype)
{
   if (strstr(filename, ".html")) 
      strcpy(filetype, "text/html");
   else if (strstr(filename, ".gif")) 
      strcpy(filetype, "image/gif");
   else if (strstr(filename, ".jpg")) 
      strcpy(filetype, "image/jpeg");
   else 
      strcpy(filetype, "text/plain");
}

void requestServeDynamic(int fd, char *filename, char *cgiargs, struct timeval arrival , struct timeval differnce, int thread_index ,int* static_threads, int* dynamic_threads,
                  int* total_threads )
{
   char buf[MAXLINE], *emptylist[] = {NULL};

   // The server does only a little bit of the header.  
   // The CGI script has to finish writing out the header.
   sprintf(buf, "HTTP/1.0 200 OK\r\n");
   sprintf(buf, "%sServer: OS-HW3 Web Server\r\n", buf);
   sprintf(buf, "%sStat-Req-Arrival:: %lu.%06lu\r\n", buf, arrival.tv_sec, arrival.tv_usec);
   sprintf(buf, "%sStat-Req-Dispatch:: %lu.%06lu\r\n", buf, differnce.tv_sec, differnce.tv_usec);
   sprintf(buf, "%sStat-Thread-Id:: %d\r\n", buf, thread_index);
   sprintf(buf, "%sStat-Thread-Count:: %d\r\n", buf, total_threads[thread_index]);
   sprintf(buf, "%sStat-Thread-Static:: %d\r\n", buf, static_threads[thread_index]);
   sprintf(buf, "%sStat-Thread-Dynamic:: %d\r\n", buf, dynamic_threads[thread_index]);
   Rio_writen(fd, buf, strlen(buf));

   pid_t pid = Fork();
    if (pid == 0) {
      /* Child process */
      Setenv("QUERY_STRING", cgiargs, 1);
      /* When the CGI process writes to stdout, it will instead go to the socket */
      Dup2(fd, STDOUT_FILENO);
      Execve(filename, emptylist, environ);
   }
    WaitPid(pid,NULL,0);

}


void requestServeStatic(int fd, char *filename, int filesize, struct timeval arrival , struct timeval differnce, int thread_index ,int* static_threads, int* dynamic_threads,
                  int* total_threads) 
{
   int srcfd;
   char *srcp, filetype[MAXLINE], buf[MAXBUF];

   requestGetFiletype(filename, filetype);

   srcfd = Open(filename, O_RDONLY, 0);

   // Rather than call read() to read the file into memory, 
   // which would require that we allocate a buffer, we memory-map the file
   srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
   Close(srcfd);

   // put together response
   sprintf(buf, "HTTP/1.0 200 OK\r\n");
   sprintf(buf, "%sServer: OS-HW3 Web Server\r\n", buf);
   sprintf(buf, "%sContent-Length: %d\r\n", buf, filesize);
   sprintf(buf, "%sContent-Type: %s\r\n", buf, filetype);
   sprintf(buf, "%sStat-Req-Arrival:: %lu.%06lu\r\n", buf, arrival.tv_sec, arrival.tv_usec);
   sprintf(buf, "%sStat-Req-Dispatch:: %lu.%06lu\r\n", buf, differnce.tv_sec, differnce.tv_usec);
   sprintf(buf, "%sStat-Thread-Id:: %d\r\n", buf, thread_index);
   sprintf(buf, "%sStat-Thread-Count:: %d\r\n", buf, total_threads[thread_index]);
   sprintf(buf, "%sStat-Thread-Static:: %d\r\n", buf, static_threads[thread_index]);
   sprintf(buf, "%sStat-Thread-Dynamic:: %d\r\n\r\n", buf, dynamic_threads[thread_index]);

   Rio_writen(fd, buf, strlen(buf));

   //  Writes out to the client socket the memory-mapped file 
   Rio_writen(fd, srcp, filesize);
   Munmap(srcp, filesize);

}

// handle a request
void requestHandle(int fd, struct timeval arrival, struct timeval differnce, int thread_index, int* static_threads,
                  int* dynamic_threads , int* total_threads)
{

   int is_static;
   struct stat sbuf;
   char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
   char filename[MAXLINE], cgiargs[MAXLINE];
   rio_t rio;

   Rio_readinitb(&rio, fd);
   Rio_readlineb(&rio, buf, MAXLINE);
   sscanf(buf, "%s %s %s", method, uri, version);

   printf("%s %s %s\n", method, uri, version);

   if (strcasecmp(method, "GET")) {
      total_threads[thread_index]++;
      requestError(fd, method, "501", "Not Implemented", "OS-HW3 Server does not implement this method",arrival, differnce, thread_index,
                  static_threads, dynamic_threads, total_threads);
      return;
   }
   requestReadhdrs(&rio);

   is_static = requestParseURI(uri, filename, cgiargs);
   if (stat(filename, &sbuf) < 0) {
      total_threads[thread_index]++;
      requestError(fd, filename, "404", "Not found", "OS-HW3 Server could not find this file",arrival, differnce, thread_index,
                  static_threads, dynamic_threads, total_threads);
      return;
   }

   if (is_static) {
      if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
         total_threads[thread_index]++;
         requestError(fd, filename, "403", "Forbidden", "OS-HW3 Server could not read this file",arrival, differnce, thread_index,
                  static_threads, dynamic_threads, total_threads);
         return;
      }
      total_threads[thread_index]++;
      static_threads[thread_index]++;      
      requestServeStatic(fd, filename, sbuf.st_size, arrival, differnce, thread_index, static_threads, dynamic_threads, total_threads);
   } else {
      if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
         total_threads[thread_index]++;
         requestError(fd, filename, "403", "Forbidden", "OS-HW3 Server could not run this CGI program",arrival, differnce, thread_index,
                  static_threads, dynamic_threads, total_threads);
         return;
      }
      total_threads[thread_index]++;
      dynamic_threads[thread_index]++;      
      requestServeDynamic(fd, filename, cgiargs, arrival, differnce, thread_index,  static_threads, dynamic_threads, total_threads);
   }
}


