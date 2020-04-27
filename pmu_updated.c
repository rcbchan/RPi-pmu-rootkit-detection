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
  char buf[4096];
  struct read_format* rf = (struct read_format*) buf;
  int i;

  memset(&pe, 0, sizeof(struct perf_event_attr));

  // CPU Cycles
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CPU_CYCLES;
  pe.disabled = 1;
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
  pe.exclude_hv = 1; // this excludes the events that happen in the hypervisor - not running a VM
  pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd2 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);

  ioctl(fd2, PERF_EVENT_IOC_ID, &id2);

  // HW Cache Misses
  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CACHE_MISSES;
  pe.disabled = 1;
  pe.exclude_hv = 1;
  pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd3 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);

  ioctl(fd3, PERF_EVENT_IOC_ID, &id3);
/*
  // HW_CACHE_references
  int fd4;
  uint64_t id4;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CACHE_REFERENCES;
  pe.disabled = 1;
  pe.exclude_hv = 1;
  pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd4 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);
  
  ioctl(fd4, PERF_EVENT_IOC_ID, &id4);

  // Branch Misses 
  int fd5;
  uint64_t id5;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_BRANCH_MISSES;
  pe.disabled = 1;
  pe.exclude_hv = 1;
  pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd5 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);
  
  ioctl(fd5, PERF_EVENT_IOC_ID, &id5);

 // BUS Cycles 
  int fd6;
  uint64_t id6;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_BUS_CYCLES;
  pe.disabled = 1;
  pe.exclude_hv = 1;
  pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd6 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);
  
  ioctl(fd6, PERF_EVENT_IOC_ID, &id6);

 // Branch Stalled Cycles Frontend 
  int fd7;
  uint64_t id7;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_STALLED_CYCLES_FRONTEND;
  pe.disabled = 1;
  pe.exclude_hv = 1;
  pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd7 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);
  
  ioctl(fd7, PERF_EVENT_IOC_ID, &id7);

 // Stalled Cycles Backend
  int fd8;
  uint64_t id8;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_STALLED_CYCLES_BACKEND;
  pe.disabled = 1;
  pe.exclude_hv = 1;
  pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd8 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);
  
  ioctl(fd8, PERF_EVENT_IOC_ID, &id8);

 // Total cycles 
  int fd9;
  uint64_t id9;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_REF_CPU_CYCLES;
  pe.disabled = 1;
  pe.exclude_hv = 1;
  pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd9 = syscall(__NR_perf_event_open, &pe, 0, -1, fd1, 0);
  
  ioctl(fd9, PERF_EVENT_IOC_ID, &id9);
*/

  //PMU test
  ioctl(fd1, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
  ioctl(fd1, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
  
  printf("Data Collection Started");
  
  ioctl(fd1, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);


  uint64_t val1, val2, val3;// val4, val5, val6, val7, val8, val9;

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
/*    else if (rf->values[i].id == id4) {
      val4 = rf->values[i].value; 
    }
    else if (rf->values[i].id == id5) {
      val5 = rf->values[i].value; 
    }
    else if (rf->values[i].id == id6) {
      val6 = rf->values[i].value; 
    }
    else if (rf->values[i].id == id7) {
      val7 = rf->values[i].value; 
    }
    else if (rf->values[i].id == id8) {
      val8 = rf->values[i].value; 
    }
    else if (rf->values[i].id == id9) {
      val9 = rf->values[i].value; 
    }
    */
  }

  
  printf("cpu cycles: %"PRIu64"\n", val1);
  printf("page faults: %"PRIu64"\n", val2);
  printf("cache misses: %"PRIu64"\n", val3);
/*  printf("cache references: %"PRIu64"\n", val4);
  printf("branch misses: %"PRIu64"\n", val5);
  printf("bus cycles: %"PRIu64"\n", val6);
  printf("frontend stalled cycles: %"PRIu64"\n", val7);
  printf("backend stalled cycles: %"PRIu64"\n", val8);
  printf("total cpu cycles: %"PRIu64"\n", val9);
*/

  return(0);
}

do_stuffs() {


}



