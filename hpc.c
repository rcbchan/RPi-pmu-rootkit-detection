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

// The following are prototypes for each HPC 
void m_counthwinstruct();
void m_hwmisses();

/* Wrapper for perf_event_open */
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
  m_counthwinstruct();
  m_hwmisses();
}




/* Measuring functions */
void turnon_measuring(int fd) {
  ioctl(fd, PERF_EVENT_IOC_RESET, 0);
  ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
}

void turnoff_measuring(int fd) {
  ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
}




/* Functions for HPC */
void m_counthwinstruct() {
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

  // have to create a file descriptor for each core 
  // Raspberry Pi 3B+
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

  printf("HW Instruction Count:\n");
  printf("\tCPU0: Used %lld instructions\n", val1);
  printf("\tCPU1: Used %lld instructions\n", val2);
  printf("\tCPU2: Used %lld instructions\n", val3);
  printf("\tCPU3: Used %lld instructions\n", val4);
  
  close(fd1);
  close(fd2);
  close(fd3);
  close(fd4);
}

void m_hwmisses() {
  struct perf_event_attr pe;
  long long val1_1, val1_2, val1_3, val1_4;
  int fd1_1, fd1_2, fd1_3, fd1_4;

  /** Cache misses **/
  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CACHE_MISSES;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  // have to create a file descriptor for each core 
  // Raspberry Pi 3B+
  fd1_1 = perf_event_open(&pe, -1, 0, -1, 0);
  fd1_2 = perf_event_open(&pe, -1, 1, -1, 0);
  fd1_3 = perf_event_open(&pe, -1, 2, -1, 0);
  fd1_4 = perf_event_open(&pe, -1, 3, -1, 0);
  
  if (fd1_1 == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
  }
  if (fd1_2 == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
  }
  if (fd1_3 == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
  }
  if (fd1_4 == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
  }

  /** Branch misses **/
  long long val2_1, val2_2, val2_3, val2_4;
  int fd2_1, fd2_2, fd2_3, fd2_4;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CACHE_MISSES;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  // have to create a file descriptor for each core 
  // Raspberry Pi 3B+
  fd2_1 = perf_event_open(&pe, -1, 0, fd1_1, 0);
  fd2_2 = perf_event_open(&pe, -1, 1, fd1_2, 0);
  fd2_3 = perf_event_open(&pe, -1, 2, fd1_3, 0);
  fd2_4 = perf_event_open(&pe, -1, 3, fd1_4, 0);


  // Begin testing
  turnon_measuring(fd1_1);
  turnon_measuring(fd1_2);
  turnon_measuring(fd1_3);
  turnon_measuring(fd1_4);
  
  sleep(5);

  turnoff_measuring(fd1_1);
  turnoff_measuring(fd1_2);
  turnoff_measuring(fd1_3);
  turnoff_measuring(fd1_4);
  
  read(fd1_1, &val1_1, sizeof(long long));
  read(fd1_2, &val1_2, sizeof(long long));
  read(fd1_3, &val1_3, sizeof(long long));
  read(fd1_3, &val1_4, sizeof(long long));
  
  read(fd2_3, &val2_4, sizeof(long long));
  read(fd2_3, &val2_4, sizeof(long long));
  read(fd2_3, &val2_4, sizeof(long long));
  read(fd2_3, &val2_4, sizeof(long long));

  printf("\nHardware misses:\n");
  printf("\tCPU0: %lld cache misses\n", val1_1);
  printf("\tCPU1: %lld cache misses\n", val1_2);
  printf("\tCPU2: %lld cache misses\n", val1_3);
  printf("\tCPU3: %lld cache misses\n\n", val1_4);

  printf("\tCPU0: %lld branch misses\n", val1_4);
  printf("\tCPU1: %lld branch misses\n", val1_4);
  printf("\tCPU2: %lld branch misses\n", val1_4);
  printf("\tCPU3: %lld branch misses\n", val1_4);
  
  close(fd1_1);
  close(fd1_2);
  close(fd1_3);
  close(fd1_4);

  close(fd2_1);
  close(fd2_2);
  close(fd2_3);
  close(fd2_4);
}
