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
void m_cacherw();
void m_l1cache_tlb();

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
  m_cacherw();
  m_l1cache_tlb();
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
  fd2_1 = perf_event_open(&pe, -1, 0, -1, 0);
  fd2_2 = perf_event_open(&pe, -1, 1, -1, 0);
  fd2_3 = perf_event_open(&pe, -1, 2, -1, 0);
  fd2_4 = perf_event_open(&pe, -1, 3, -1, 0);


  // Begin testing
  turnon_measuring(fd1_1);
  turnon_measuring(fd1_2);
  turnon_measuring(fd1_3);
  turnon_measuring(fd1_4);

  turnon_measuring(fd2_1);
  turnon_measuring(fd2_2);
  turnon_measuring(fd2_3);
  turnon_measuring(fd2_4);
  
  sleep(5);

  turnoff_measuring(fd1_1);
  turnoff_measuring(fd1_2);
  turnoff_measuring(fd1_3);
  turnoff_measuring(fd1_4);

  turnoff_measuring(fd2_1);
  turnoff_measuring(fd2_2);
  turnoff_measuring(fd2_3);
  turnoff_measuring(fd2_4);
  
  read(fd1_1, &val1_1, sizeof(long long));
  read(fd1_2, &val1_2, sizeof(long long));
  read(fd1_3, &val1_3, sizeof(long long));
  read(fd1_3, &val1_4, sizeof(long long));
  
  read(fd2_1, &val2_1, sizeof(long long));
  read(fd2_2, &val2_2, sizeof(long long));
  read(fd2_3, &val2_3, sizeof(long long));
  read(fd2_4, &val2_4, sizeof(long long));

  printf("\nHardware misses:\n");
  printf("\tCPU0: %lld cache misses\n", val1_1);
  printf("\tCPU1: %lld cache misses\n", val1_2);
  printf("\tCPU2: %lld cache misses\n", val1_3);
  printf("\tCPU3: %lld cache misses\n\n", val1_4);

  printf("\tCPU0: %lld branch misses\n", val2_1);
  printf("\tCPU1: %lld branch misses\n", val2_2);
  printf("\tCPU2: %lld branch misses\n", val2_3);
  printf("\tCPU3: %lld branch misses\n", val2_4);
  
  close(fd1_1);
  close(fd1_2);
  close(fd1_3);
  close(fd1_4);

  close(fd2_1);
  close(fd2_2);
  close(fd2_3);
  close(fd2_4);
}

void m_cacherw(){
  struct perf_event_attr pe;
  long long val1_1, val1_2, val1_3, val1_4;
  int fd1_1, fd1_2, fd1_3, fd1_4;

  /** Cache Read/Writes **/
  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HW_CACHE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CACHE_OP_READ;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

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
  pe.type = PERF_TYPE_HW_CACHE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CACHE_OP_WRITE;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  // have to create a file descriptor for each core 
  // Raspberry Pi 3B+
  fd2_1 = perf_event_open(&pe, -1, 0, -1, 0);
  fd2_2 = perf_event_open(&pe, -1, 1, -1, 0);
  fd2_3 = perf_event_open(&pe, -1, 2, -1, 0);
  fd2_4 = perf_event_open(&pe, -1, 3, -1, 0);


  // Begin testing
  turnon_measuring(fd1_1);
  turnon_measuring(fd1_2);
  turnon_measuring(fd1_3);
  turnon_measuring(fd1_4);

  turnon_measuring(fd2_1);
  turnon_measuring(fd2_2);
  turnon_measuring(fd2_3);
  turnon_measuring(fd2_4);
  
  sleep(5);

  turnoff_measuring(fd1_1);
  turnoff_measuring(fd1_2);
  turnoff_measuring(fd1_3);
  turnoff_measuring(fd1_4);

  turnoff_measuring(fd2_1);
  turnoff_measuring(fd2_2);
  turnoff_measuring(fd2_3);
  turnoff_measuring(fd2_4);
  
  read(fd1_1, &val1_1, sizeof(long long));
  read(fd1_2, &val1_2, sizeof(long long));
  read(fd1_3, &val1_3, sizeof(long long));
  read(fd1_4, &val1_4, sizeof(long long));
  
  read(fd2_1, &val2_1, sizeof(long long));
  read(fd2_2, &val2_2, sizeof(long long));
  read(fd2_3, &val2_3, sizeof(long long));
  read(fd2_4, &val2_4, sizeof(long long));

  printf("\nCache Read/Write:\n");
  printf("\tCPU0: %lld cache reads\n", val1_1);
  printf("\tCPU1: %lld cache reads\n", val1_2);
  printf("\tCPU2: %lld cache reads\n", val1_3);
  printf("\tCPU3: %lld cache reads\n\n", val1_4);

  printf("\tCPU0: %lld cache writes\n", val2_1);
  printf("\tCPU1: %lld cache writes\n", val2_2);
  printf("\tCPU2: %lld cache writes\n", val2_3);
  printf("\tCPU3: %lld cache writes\n", val2_4);
  
  close(fd1_1);
  close(fd1_2);
  close(fd1_3);
  close(fd1_4);

  close(fd2_1);
  close(fd2_2);
  close(fd2_3);
  close(fd2_4);
}

void m_l1cache_tlb() {
  struct perf_event_attr pe;
  long long val1_1, val1_2, val1_3, val1_4;
  int fd1_1, fd1_2, fd1_3, fd1_4;

  /** Level 1 data cache **/
  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HW_CACHE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CACHE_L1D;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  fd1_1 = perf_event_open(&pe, -1, 0, -1, 0);
  fd1_2 = perf_event_open(&pe, -1, 1, -1, 0);
  fd1_3 = perf_event_open(&pe, -1, 2, -1, 0);
  fd1_4 = perf_event_open(&pe, -1, 3, -1, 0);

  /** L1 Instruction cache **/
  long long val2_1, val2_2, val2_3, val2_4;
  int fd2_1, fd2_2, fd2_3, fd2_4;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HW_CACHE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CACHE_L1I;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  fd2_1 = perf_event_open(&pe, -1, 0, -1, 0);
  fd2_2 = perf_event_open(&pe, -1, 1, -1, 0);
  fd2_3 = perf_event_open(&pe, -1, 2, -1, 0);
  fd2_4 = perf_event_open(&pe, -1, 3, -1, 0);

  /** Last level cache **/
  long long val3_1, val3_2, val3_3, val3_4;
  int fd3_1, fd3_2, fd3_3, fd3_4;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HW_CACHE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CACHE_LL;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  fd3_1 = perf_event_open(&pe, -1, 0, -1, 0);
  fd3_2 = perf_event_open(&pe, -1, 1, -1, 0);
  fd3_3 = perf_event_open(&pe, -1, 2, -1, 0);
  fd3_4 = perf_event_open(&pe, -1, 3, -1, 0);

  /** Last level cache **/
  long long val4_1, val4_2, val4_3, val4_4;
  int fd4_1, fd4_2, fd4_3, fd4_4;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HW_CACHE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CACHE_DTLB;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  fd4_1 = perf_event_open(&pe, -1, 0, -1, 0);
  fd4_2 = perf_event_open(&pe, -1, 1, -1, 0);
  fd4_3 = perf_event_open(&pe, -1, 2, -1, 0);
  fd4_4 = perf_event_open(&pe, -1, 3, -1, 0);

  /** Last level cache **/
  long long val5_1, val5_2, val5_3, val5_4;
  int fd5_1, fd5_2, fd5_3, fd5_4;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HW_CACHE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CACHE_ITLB;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  // have to create a file descriptor for each core 
  // Raspberry Pi 3B+
  fd5_1 = perf_event_open(&pe, -1, 0, -1, 0);
  fd5_2 = perf_event_open(&pe, -1, 1, -1, 0);
  fd5_3 = perf_event_open(&pe, -1, 2, -1, 0);
  fd5_4 = perf_event_open(&pe, -1, 3, -1, 0);

  // Begin testing
  turnon_measuring(fd1_1);
  turnon_measuring(fd1_2);
  turnon_measuring(fd1_3);
  turnon_measuring(fd1_4);

  turnon_measuring(fd2_1);
  turnon_measuring(fd2_2);
  turnon_measuring(fd2_3);
  turnon_measuring(fd2_4);

  turnon_measuring(fd3_1);
  turnon_measuring(fd3_2);
  turnon_measuring(fd3_3);
  turnon_measuring(fd3_4);

  turnon_measuring(fd4_1);
  turnon_measuring(fd4_2);
  turnon_measuring(fd4_3);
  turnon_measuring(fd4_4);

  turnon_measuring(fd5_1);
  turnon_measuring(fd5_2);
  turnon_measuring(fd5_3);
  turnon_measuring(fd5_4);
  
  sleep(5);

  turnoff_measuring(fd1_1);
  turnoff_measuring(fd1_2);
  turnoff_measuring(fd1_3);
  turnoff_measuring(fd1_4);

  turnoff_measuring(fd2_1);
  turnoff_measuring(fd2_2);
  turnoff_measuring(fd2_3);
  turnoff_measuring(fd2_4);

  turnoff_measuring(fd3_1);
  turnoff_measuring(fd3_2);
  turnoff_measuring(fd3_3);
  turnoff_measuring(fd3_4);

  turnoff_measuring(fd4_1);
  turnoff_measuring(fd4_2);
  turnoff_measuring(fd4_3);
  turnoff_measuring(fd4_4);

  turnoff_measuring(fd5_1);
  turnoff_measuring(fd5_2);
  turnoff_measuring(fd5_3);
  turnoff_measuring(fd5_4);
  
  read(fd1_1, &val1_1, sizeof(long long));
  read(fd1_2, &val1_2, sizeof(long long));
  read(fd1_3, &val1_3, sizeof(long long));
  read(fd1_4, &val1_4, sizeof(long long));
  
  read(fd2_1, &val2_1, sizeof(long long));
  read(fd2_2, &val2_2, sizeof(long long));
  read(fd2_3, &val2_3, sizeof(long long));
  read(fd2_4, &val2_4, sizeof(long long));

  read(fd3_1, &val3_1, sizeof(long long));
  read(fd3_2, &val3_2, sizeof(long long));
  read(fd3_3, &val3_3, sizeof(long long));
  read(fd3_4, &val3_4, sizeof(long long));

  read(fd4_1, &val4_1, sizeof(long long));
  read(fd4_2, &val4_2, sizeof(long long));
  read(fd4_3, &val4_3, sizeof(long long));
  read(fd4_4, &val4_4, sizeof(long long));

  read(fd5_1, &val5_1, sizeof(long long));
  read(fd5_2, &val5_2, sizeof(long long));
  read(fd5_3, &val5_3, sizeof(long long));
  read(fd5_4, &val5_4, sizeof(long long));

  printf("\nLevel 1 cache & TLB:\n");
  printf("\tCPU0: %lld level 1 data cache\n", val1_1);
  printf("\tCPU1: %lld level 1 data cache\n", val1_2);
  printf("\tCPU2: %lld level 1 data cache\n", val1_3);
  printf("\tCPU3: %lld level 1 data cache\n\n", val1_4);

  printf("\tCPU0: %lld level 1 instruct cache\n", val2_1);
  printf("\tCPU1: %lld level 1 instruct cache\n", val2_2);
  printf("\tCPU2: %lld level 1 instruct cache\n", val2_3);
  printf("\tCPU3: %lld level 1 instruct cache\n\n", val2_4);

  printf("\tCPU0: %lld last level cache\n", val3_1);
  printf("\tCPU1: %lld last level cache\n", val3_2);
  printf("\tCPU2: %lld last level cache\n", val3_3);
  printf("\tCPU3: %lld last level cache\n\n", val3_4);

  printf("\tCPU0: %lld data TLB\n", val4_1);
  printf("\tCPU1: %lld data TLB\n", val4_2);
  printf("\tCPU2: %lld data TLB\n", val4_3);
  printf("\tCPU3: %lld data TLB\n\n", val4_4);

  printf("\tCPU0: %lld instruct TLB\n", val5_1);
  printf("\tCPU1: %lld instruct TLB\n", val5_2);
  printf("\tCPU2: %lld instruct TLB\n", val5_3);
  printf("\tCPU3: %lld instruct TLB\n", val5_4);
  
  close(fd1_1);
  close(fd1_2);
  close(fd1_3);
  close(fd1_4);

  close(fd2_1);
  close(fd2_2);
  close(fd2_3);
  close(fd2_4);

  close(fd3_1);
  close(fd3_2);
  close(fd3_3);
  close(fd3_4);

  close(fd4_1);
  close(fd4_2);
  close(fd4_3);
  close(fd4_4);

  close(fd5_1);
  close(fd5_2);
  close(fd5_3);
  close(fd5_4);
}