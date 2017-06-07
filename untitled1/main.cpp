#include <stdlib.h>
#include <string>
#include <fcgio.h>
#include <fcgi_stdio.h>
#include <zconf.h>

#define THREAD_COUNT 3

using namespace std;

static int counts[THREAD_COUNT];

int main2() {
    int count = 0;
    while (FCGI_Accept() >= 0) {
        printf("Content-Type:gnss/data\r\n\r\n");
        printf("Ntrip-Version: Ntrip/2.0\n");
        printf("Server: NTRIP Caster 2.0\n");
        printf("Date: 25/Nov/2007:18:30:45: GMT\n");
        printf("Ntrip-Version: Ntrip/2.0\n");

//        printf( "Content-type:text/html\r\n\r\n" );
        printf( "<p> Hello FastCGI !  </ p>" );
        printf( "<br /> Request number = [%d]", ++count );
        printf( "<br /> Process ID: %d ", getpid() );
    }

    return 0;
}


static void *doit(void *a)
{
    int rc;
    FCGX_Request request;
    FCGX_InitRequest(&request, 0, 0);
    int count = 0;
    for (;;)
    {
        rc = FCGX_Accept_r(&request);

        if (rc < 0)
            break;

        FCGX_FPrintF(request.out,
                     "Content-Type:gnss/data\r\n\r\n"
                             "Ntrip-Version: Ntrip/2.0\n"
                             "Server: NTRIP Caster 2.0\n"
                             "Date: 25/Nov/2007:18:30:45: GMT\n"
                             "Ntrip-Version: Ntrip/2.0\n"
                             "<br /> Request number = [%d]", ++count);

        FCGX_Finish_r(&request);
    }
    return NULL;
}

int main3(void)
{
//    int i;
//    pthread_t id[THREAD_COUNT];
//
//    FCGX_Init();
//
//    for (i = 1; i < THREAD_COUNT; i++)
//        pthread_create(&id[i], NULL, doit, (void*)i);
//
//    doit(0);
    return 0;
}
