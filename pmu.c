#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <asm/unistd.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>

// func prototypes
void do_something();

// Struct def
struct read_format {
  uint64_t nr;
  struct {
    uint64_t value;
    uint64_t id;
  } values[];
};



int main(int agrc, char **argv) {

  struct perf_event_attr pe;
  int fd1, fd2, fd3;
  uint64_t id1, id2, id3;
  uint64_t val1, val2, val3;
  char buf[4096];
  struct read_format* rf = (struct read_format*) buf;
  int i;

  memset(&pe, 0, sizeof(struct perf_event_attr));

  // CPU Cycles
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CPU_CYCLES;
  pe.disabled = 1;
  //	pe.exclude_kernel = 1; /* This will exclude events that happen in the kernel  */
  pe.exclude_hv = 1; // This excludes the events in the hypervisor - not running a VM
  pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd1 = syscall(__NR_perf_event_open, &pe, 0, -1, -1, 0);

  ioctl(fd1, PERF_EVENT_IOC_ID, &id1); // returns the perf_event id value for the given event file
  // the result is saved inside of id*	

  // Software Page Faults
  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_SOFTWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_SW_PAGE_FAULTS;
  pe.disabled = 1;
  //	pe.exclude_kernel = 1; /* This will execute events that happen in the kernel  */
  pe.exclude_hv = 1; // this excludes the events that happen in the hypervisor - not running a VM
  pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd2 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);

  ioctl(fd2, PERF_EVENT_IOC_ID, &id2);

  // Something
  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CACHE_MISSES;
  pe.disabled = 1;
  //      pe.exclude_kernel = 1;
  pe.exclude_hv = 1;
  pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd3 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);

  ioctl(fd3, PERF_EVENT_IOC_ID, &id3);

  //sleep(20);

  // PMU test
  ioctl(fd1, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
  ioctl(fd1, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
  sleep(10);
  do_something();
  ioctl(fd1, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

  // Read from file descriptor
  read(fd1, buf, sizeof(buf));
  for (i = 0; i < rf->nr; i++) {
    if (rf->values[i].id == id1) {
      val1 = rf->values[i].value;
    } 
    else if (rf->values[i].id == id2) {
      val2 = rf->values[i].value;
    }
    else if (rf->values[i].id == id3) {
      val3 = rf->values[i].value; 
    }

  }

  printf("cpu cycles: %"PRIu64"\n", val1);
  printf("page faults: %"PRIu64"\n", val2);
  printf("cache misses: %"PRIu64"\n", val3);


  return(0);
}

void do_something() {
  int i;
  char* ptr;

  ptr = malloc(10*1024*1024);
  for (i = 0; i < 10*1024*1024; i++) {
    ptr[i] = (char) (i & 0xff); // pagefault
  }
  free(ptr);
}
