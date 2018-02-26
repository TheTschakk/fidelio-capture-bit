
struct buffer {
    void *start;
    size_t length;
};

struct buffer *buffers;
static unsigned int n_buffers;


static int xioctl(int fd, int request, void *arg) {
    int r;

    do r = ioctl (fd, request, arg);
    while (-1 == r && EINTR == errno);

    return r;
}

int init_device(int fd) {

    struct v4l2_capability caps;

    if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &caps)){
        perror("Querying Capabilities");
        return 1;
    }

    if (!(caps.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        printf("dev is no video capture device\n");
    }

    if (!(caps.capabilities & V4L2_CAP_STREAMING)) {
        printf("dev does not support streaming\n");
    }

    struct v4l2_format fmt;
    CLEAR(fmt);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = WIDTH;
    fmt.fmt.pix.height = HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    fmt.fmt.pix.bytesperline = 0;

    if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt)){
        perror("Setting Pixel Format");
        return 1;
    }

    return 0;
}


int init_mmap(int fd) {

    struct v4l2_requestbuffers req;
    CLEAR(req);

    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
        perror("Requesting Buffer");
        return 1;
    }

    buffers = calloc(req.count, sizeof(*buffers));

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        struct v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;

        if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf)) {
            perror("Querying Buffer 1");
            return 1;
        }

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    }

    return 0;
}


int start_grabbing(void) {

    unsigned int i;
    enum v4l2_buf_type type;

    for (i = 0; i < n_buffers; ++i) {
        struct v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf)) {
            perror("Queue Buffer");
            return 1;
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == xioctl(fd, VIDIOC_STREAMON, &type)) {
        perror("Stream ON");
        return 1;
    }

    return 0;
}

int process_frame(unsigned char *yuyv) {
    int i;

    for (i=0; i<LENGTH; i++) {
        frm->data[i] = yuyv[2*i];
    }
    return 0;
}	

int read_frame(void) {

    struct v4l2_buffer buf;

    CLEAR(buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
        switch (errno) {
            case EAGAIN:
                printf("EAGAIN");
            case EIO:
                printf("EIO");
            default:
                printf("DQBUF");
        }
    }

    assert(buf.index < n_buffers);

    process_frame(buffers[buf.index].start);

    if (-1 == xioctl(fd, VIDIOC_QBUF, &buf)) {
        perror("Query Buffer 2");
        return 1;
    }
    return 1;
}

void wait_for_frame(void) {
    while (1) {	
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        struct timeval tv = {0};
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        int r = select(fd+1, &fds, NULL, NULL, &tv);

        if (-1 == r)
            perror("Waiting for Frame");

        if (read_frame())
            break;
    }
}

int stop_grabbing(void) {
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type)) {
        perror("Stop Capture");
        return 1;
    }
    return 0;
}

int uninit_device(void) {
    unsigned int i;

    for (i = 0; i < n_buffers; ++i)
        if (-1 == munmap(buffers[i].start, buffers[i].length)) {
            perror("munmap");
            return 1;
        }

    free(buffers);
    return 0;
}

