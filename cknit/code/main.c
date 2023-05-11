/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#include <ck_task.h>
#include <estring.h>
#include <log.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
    int   opt;
    int   oi;
    long  pid, re;
    char *bff, *os = "r:n::";

    oi = 0;
    static struct option lo[] =
    {
        {"k",     1,   NULL, 'r'},
        {"help",  0,   NULL, 'n'},
        {NULL,    0,   NULL,  0 },
    };

    if ( argc == 1 )
    {
        goto print_help;
    }

    while ( ( opt =
        getopt_long_only( argc, argv, os, lo, &oi ) ) != -1 )
    {
        switch (opt)
        {
            case 'r':
                if ( e_memcmp(optarg, E_STRL("start")) == 0 )
                {
                    main_thread();
                }
                elif ( e_memcmp(optarg, E_STRL("restart")) == 0 )
                {
                    bff = e_data_from_file(PIDFILE, NULL);
                    if ( bff ) {
                        errno = 0;
                        pid    = strtol( bff, NULL, 10);
                        if ( pid != 0 )
                        {
                            re = kill( (int)pid, 0);
                            if ( re == 0 || ( re != -1 || errno == EPERM ) )
                            {
                                kill( (int)pid, SIGUSR2);
                            }
                        }
                        sleep(1);
                        main_thread();
                    } else {
                        main_thread();
                    }
                }
                elif ( e_memcmp(optarg, E_STRL("stop")) == 0 )
                {
                    bff = e_data_from_file(PIDFILE, NULL);
                    if ( bff ) {
                        errno = 0;
                        pid    = strtol( bff, NULL, 10);
                        if ( pid != 0 ) {
                            re = kill( (int)pid, 0);
                            if ( re == 0 || ( re != -1 || errno == EPERM ) )
                            {
                                kill((int)strtol(bff, NULL, 10), SIGUSR2);
                            } else {
                                fprintf(stdout, "cknit is not running.\n");
                            }
                        } else {
                            fprintf(stdout, "cknit is not running.\n");
                        }
                    } else {
                        fprintf(stdout, "cknit is not running.\n");
                    }
                    e_write_to_file(PIDFILE, "");
                }
                else
                {
                    fprintf(stderr, "cknit --k [restart|start|stop]\n");
                }
                break;
            case 'n':
                goto print_help;
            default:
                break;
        }
    }
    return 0;

print_help:

    fprintf(
        stdout,
        "Usage: cknit [OPTION]... \n"
        "A Task tool written in C and manage by APIs "
        "\n\n"
        "OPTIONs: \n"
        "  -h, --help                        Display this help and exit\n"
        "  -k, --k  [restart|start|stop]     Restart|Start|Stop cknit\n"
    );
}