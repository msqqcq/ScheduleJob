/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#include <log.h>
#include <http_socket.h>
#include <estring.h>

#if DEBUG_MODE
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#endif

int socket_create(const char *host, uint16_t port)
{
	int sid, d, k;

	struct sockaddr_in addr;

	sid = socket(AF_INET, SOCK_STREAM, 0);
	if ( sid == -1 )
	{
		logger(LOG_ERROR, "Create socket failed; " ERROR_CODE_FMT, ERROR_CODE_STR);
	}
	assert( sid != -1 );

	k = 1;
	setsockopt(sid, SOL_SOCKET, SO_KEEPALIVE, &k, sizeof( int ));
	setsockopt(sid, SOL_SOCKET, SO_REUSEADDR, &k, sizeof( int ));
	setsockopt(sid, SOL_SOCKET, SO_REUSEPORT, &k, sizeof( int ));

	/* if host is NULL */
	if ( host )
	{
		inet_pton(AF_INET, host, &addr.sin_addr);
	}
	else
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	addr.sin_port   = htons(port);
	addr.sin_family = AF_INET;

	d = bind(sid, (struct sockaddr *)&addr, sizeof(addr));
	if ( d == -1 )
	{
		logger(LOG_ERROR, "Bind to socket: %d failed; " ERROR_CODE_FMT, sid, ERROR_CODE_STR);
	}
	assert(d != -1);
	
	return sid;
}

int socket_listen(int fd, int bl)
{
    int r;
    if ( !fd )
    {
        logger(LOG_ERROR, "Listen socket: %d wrong; fd is:%d\n", fd);
        assert( fd );
    }
    
    r = listen(fd, bl);
    if ( r == -1 )
    {
        logger(LOG_ERROR, "Listen socket: %d faild; " ERROR_CODE_FMT, fd, ERROR_CODE_STR);
        assert( r != -1 );
    }
    return 1;
}

int socket_connect(int _server_fd, const char *host, uint16_t port)
{
	int _result;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(host);

	_result = connect(_server_fd, (struct sockaddr *)&addr, sizeof(addr));
	if ( _result )
	{
		logger(LOG_ERROR, "Connect: %d error; " ERROR_CODE_FMT, _server_fd, ERROR_CODE_STR);
	}
	return _result;
}

int socket_nonblock(int fd)
{
	int result, flags = fcntl(fd, F_GETFL);
	result = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	if ( result == -1 )
	{
	    logger(LOG_ERROR, "Set fd: %d nonblocking failed; " ERROR_CODE_FMT, fd, ERROR_CODE_STR);
	}
	return result;
}

int socket_send_fd(int fd, int fd_to_send)
{
    char buff[2];
    memset(buff, 0, sizeof(buff));
    
    struct iovec io_vec[1];
    memset(io_vec, 0, sizeof(struct iovec));
    io_vec->iov_len = 2;
    io_vec->iov_base = buff;
    
    struct msghdr msg_hdr;
    memset(&msg_hdr, 0, sizeof(struct msghdr));
    msg_hdr.msg_iov     = io_vec;
    msg_hdr.msg_iovlen  = 1;
    msg_hdr.msg_name    = NULL;
    msg_hdr.msg_namelen = 0;
    
    struct cmsghdr cmptr;
    memset(&cmptr, 0, sizeof(struct cmsghdr));
    
    if ( fd_to_send < 0 )
    {
        msg_hdr.msg_control    = NULL;
        msg_hdr.msg_controllen = 0;
        buff[1] = ~fd_to_send;
        if ( buff[1] == 0 )
            buff[1] = 1; /* nonzero means failure */
    }
    else
    {
        cmptr.cmsg_level = SOL_SOCKET;
        cmptr.cmsg_type  = SCM_RIGHTS;
        cmptr.cmsg_len   = SOCKET_FD_LEN;
        
        msg_hdr.msg_control = &cmptr;
        msg_hdr.msg_controllen = SOCKET_FD_LEN;
        *(int *)CMSG_DATA(&cmptr) = fd_to_send;
        buff[1] = 0; /* zero means ok */
    }
    
    /* first char is 0 */
    buff[0] = 0;
    
    if ( sendmsg(fd, &msg_hdr, 0) != 2 )
        return (-1);
    
    return (0);
}

int socket_recv_fd(int fd)
{
    char  buff[256];
    struct cmsghdr cmptr;
    memset(&cmptr, 0, sizeof(struct cmsghdr));
    struct iovec io_vec[1];
    memset(io_vec, 0, sizeof(struct iovec));
    struct msghdr  msg_hdr;
    memset(&msg_hdr, 0, sizeof(struct msghdr));
    
    io_vec[0].iov_base  = buff;
    io_vec[0].iov_len   = sizeof(buff);
    msg_hdr.msg_iov     = io_vec;
    msg_hdr.msg_iovlen  = 1;
    msg_hdr.msg_name    = NULL;
    msg_hdr.msg_namelen = 0;
    
    msg_hdr.msg_control    = &cmptr;
    msg_hdr.msg_controllen = SOCKET_FD_LEN;
    
    recvmsg(fd, &msg_hdr, 0);
    
    return *(int *)CMSG_DATA(&cmptr);
}

char *socket_read(int fd, long *len)
{
/**
 * @brief Introduction
 * This function will read buff data from socket more as possible
 * when read data length less than the expected, return the current reading buff string
 */
    ssize_t t;
    char   *res, *v;
    char    buff[BUFFER_SIZE];
    
    res = NULL;
    
    while (true)
    {
        e_memzero(buff, BUFFER_SIZE * sizeof(char));
        t = read( fd, buff, sizeof( char ) * BUFFER_SIZE );
        if ( t == 0 )
        {
            *len = -1;
            return NULL;
        }
        else if ( t == -1 )
        {
            if ( errno == EAGAIN || errno == EWOULDBLOCK )
                continue;
            logger(LOG_ERROR, "Read data from fd: %d failed; " ERROR_CODE_FMT, fd, ERROR_CODE_STR);
            break;
        }
        v = realloc(res, sizeof(char) * (*len + t));
        if ( v == NULL )
        {
            logger(LOG_ERROR, "Memory alloc failed, please check the free memory size.", NULL);
            e_memfree(res);
            return NULL;
        }
        e_copymem(v + *len, buff, t * sizeof(char));
        
       *len += t;
        res  = v;
        if ( t < BUFFER_SIZE )
        {
            break;
        }
    }
    
    return res;
}

inline
long socket_write(int fd, char *res, unsigned long res_len)
{
    if (!res || fd <= 0) return 0;
    return write(fd, res, sizeof(char) * res_len);
}

#if DEBUG_MODE
#pragma clang diagnostic pop
#endif