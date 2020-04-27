#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

/*
  int perf_event_open(struct perf_event_attr *attr, 
  pid_t pid, int cpu, int group_fd,unsigned long flags);
*/

// Prototypes
void turnon_measuring(int fd);
void turnoff_measuring(int fd);


static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
    int cpu, int group_fd, unsigned long flags)
{
  int ret;

  ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
      group_fd, flags);
  return ret;
}

int main(int argc, char **argv)
{
  struct perf_event_attr pe;
  long long val1, val2, val3, val4;
  int fd1, fd2, fd3, fd4;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_INSTRUCTIONS;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  // have to create
  fd1 = perf_event_open(&pe, -1, 0, -1, 0);
  fd2 = perf_event_open(&pe, -1, 1, -1, 0);
  fd3 = perf_event_open(&pe, -1, 2, -1, 0);
  fd4 = perf_event_open(&pe, -1, 3, -1, 0);

  if (fd1 == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
  }
  if (fd2 == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
  }
  if (fd3 == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
  }
  if (fd4 == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
  }

  // Begin testing
  turnon_measuring(fd1);
  turnon_measuring(fd2);
  turnon_measuring(fd3);
  turnon_measuring(fd4);
  
  sleep(5);

  turnoff_measuring(fd1);
  turnoff_measuring(fd2);
  turnoff_measuring(fd3);
  turnoff_measuring(fd4);
  
  read(fd1, &val1, sizeof(long long));
  read(fd2, &val2, sizeof(long long));
  read(fd3, &val3, sizeof(long long));
  read(fd3, &val4, sizeof(long long));

  printf("Used %lld instructions\n", val1);
  printf("Used %lld instructions\n", val2);
  printf("Used %lld instructions\n", val3);
  printf("Used %lld instructions\n", val4);
  
  close(fd1);
  close(fd2);
  close(fd3);
  close(fd4);
}

void turnon_measuring(int fd) {
  ioctl(fd, PERF_EVENT_IOC_RESET, 0);
  ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
}

void turnoff_measuring(int fd) {
  ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
}