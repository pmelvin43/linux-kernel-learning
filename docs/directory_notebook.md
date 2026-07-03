# Most Important Kernel Directories (so far)

## /proc
### "What is the kernel doing?"

- virtual filesystem, nothing exists on disk
- /proc/cpuinfo, meminfo, modules, devices, 1234
- anything interesting that the kernel has to report
- current kernel / process information
- Good uses: statistics, debug info, current kernel state, process information (what it was originally only for)

## /sys
### "What kernel objects exist, and how are they configured?"

- unlike proc, represents the kernels object module
- ex: /sys/class, module, devices, block
- kernel objects and configuration
- Good uses: device attriubutes, module parameters, hardware info, configuration

## /dev
### "How can userspace communicate with a device or driver?"

- interface to devices, device files, between userspace and drivers
- files that the userspace opens
- /dev/chardev

## /lib/modules
### "where kernel modules are stored"

- contains kernel modules, dependency information, module metadata
- when you do `modprobe`, linux searches here