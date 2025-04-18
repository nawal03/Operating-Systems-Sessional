# Operating Systems Sessional 

This repository contains my sessional work for the Operating Systems course, focusing on hands-on kernel-level programming using the xv6 operating system. The goal was to develop a deeper understanding of OS concepts by working directly with a simple, Unix-like teaching OS.

## 📦 Prerequisites

Before running or modifying `xv6`, make sure to set up the required tools:

🔗 [xv6 Toolchain Setup Instructions](https://pdos.csail.mit.edu/6.828/2022/tools.html)

## 🧱 Base Code

This work is based on the official xv6 codebase:

🔗 [MIT xv6 RISC-V GitHub Repo](https://github.com/mit-pdos/xv6-riscv)

To apply any changes from this repository to the original xv6 codebase, use the following command:

```bash
git apply /path/solution.patch
```

## 📂 Contents

### 1. Shell Scripts
Basic shell scripts to test and reinforce foundational scripting concepts.

### 2. Pthread Synchronization
Implementations of thread synchronization problems including:
- Master-Worker model
- Reader-Writer problem
- Semaphore 

### 3. xv6 System Calls
Added custom system calls to xv6 to understand syscall creation and kernel-user interactions.

### 4. xv6 Lottery Scheduling
Replaced default scheduler with a lottery-based scheduler where processes are selected probabilistically based on assigned ticket counts.

### 5. xv6 Memory Management
Extended xv6's memory system by implementing:
- Copy-on-Write (COW)
- Paging support
