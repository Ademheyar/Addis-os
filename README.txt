Addis-os
========

Addis os is not about os but it is like os, ai agent, lunguge model and coding system it will read .ac or .oac codes to commentan and connect users and hardware deeply users can acsses all hardware with out limitation it gives unlimited resorse that if found in any pc

Overview
--------
Addis-os will be a hybrid platform that will combine low-level system software with model-driven automation. The project will explore how an operating environment will act both as a traditional kernel/runtime and as an intelligent interpreter for specialized source formats (.ac, .oac). Addis-os will enable users and automated agents to express intent, request hardware access, and orchestrate device behavior through auditable, policy-controlled requests.

Key Concepts
------------
- Kernel-as-interpreter: The kernel runtime will expose structured metadata and programmatic hooks so higher-level agents will be able to analyze, annotate, and transform code and device state.
- Model-driven hardware access: Agents will request hardware capabilities through explicit, logged policies; Addis-os will mediate and enforce those policies.
- Portable minimalism: The design will start small (x86_64 target) and will be intentionally simple so it will be auditable and adaptable to other architectures.

Deep Explanation (What This Project Will Be About)
--------------------------------------------------
At boot, Addis-os will bring a machine from firmware into a small, safe kernel runtime. The bootloader will initialize CPU state, set up a temporary stack, and will hand control to the kernel entry point. Early kernel setup will disable interrupts while essential architecture-specific state will be configured and the machine will transition into protected/long mode as required.

Memory management will be layered. A physical page allocator will manage frames; a minimal virtual memory subsystem will provide hierarchical page tables, simple mappings, and region-based isolation between kernel and userspace. The design will favor transparency and will include basic copy-on-write semantics to support fork-like behaviors.

The scheduler will start as a preemptive, round-robin policy with per-CPU runqueues. It will be designed so that new policies will be pluggable and so the scheduler will remain predictable and suitable for model-driven workflows.

Interrupts, consoles, and drivers will begin with essential components: a serial or VGA console for logging, a timer interrupt to enable preemption, and a minimal block driver for storage. The driver model will include clear probe and I/O interfaces; drivers will expose metadata and programmable hooks so agents will be able to assist with diagnostics and safe automation.

Userspace will be intentionally small. Addis-os will support loading ELF executables and will provide a tiny runtime with a focused syscall set (read, write, open, close, fork/exec, wait, exit, mmap). An init process will launch demo apps and manage system startup; IPC will start with pipes and signals and will later evolve toward capability- or message-based models.

Filesystems and storage will be incremental. The initial filesystem will be RAM-backed with a block device abstraction; on-disk options will be added later. Model-driven tooling will be able to index and document filesystem contents to improve developer workflows.

Security and correctness will be primary concerns. Addis-os will enforce privilege separation using hardware protections and will emphasize explicit access controls, auditing, and policy enforcement for any agent-driven operations.

Project Structure
-----------------
- /boot — boot code and artifacts.
- /kernel — arch, mm, sched, drivers, fs subsystems.
- /userland — small utilities and init program.
- /tests — QEMU/emulation scenarios and integration tests.

Todo List (All items will be written in future tense)
---------------------------------------------------
Core (short-term)
- I will implement a Multiboot- or UEFI-aware bootstrap path.
- I will implement an early serial or VGA console for kernel logging.
- I will implement a physical page allocator and a minimal virtual memory manager.
- I will implement a timer interrupt and a basic preemptive round-robin scheduler.

Devices & I/O (mid-term)
- I will implement a minimal block device driver (virtio/ATA) and a RAM disk.
- I will implement a simple console/keyboard input driver.

Userspace & Filesystems (mid-term)
- I will implement ELF executable loading and a minimal userspace runtime.
- I will implement a RAM-backed filesystem and later add a compact on-disk format.

Testing & Tooling (ongoing)
- I will add QEMU-based integration tests and automated boot verification.
- I will add a CI pipeline that will build and run smoke tests under emulation.

Long-term Extensions
- I will add multiprocessing support and per-CPU scheduling refinements.
- I will add basic network stack primitives or integrate a minimal network driver.
- I will add support for loadable kernel modules and a packageable userland.

Completed Tasks (stated in future form as requested)
---------------------------------------------------
- I will have created the repository layout and initial build scripts.
- I will have implemented an empty kernel entry point and a basic build that will produce a bootable image.
- I will have documented early design decisions in this README and in associated design notes.

Development Workflow
--------------------
- I will use iterative development: each subsystem will be developed on feature branches and will be merged when tests will pass.
- I will write small, focused commits with descriptive messages and will maintain a changelog for milestones.

How to Build and Run (Example)
------------------------------
Build commands will depend on toolchain and target. In general:
- I will install a cross-toolchain or use a system toolchain with explicit target flags.
- I will run the build script to produce a kernel image.
- I will run the image under QEMU for development and testing.

Contributing
------------
Contributions will be welcome. Contributors will be asked to open issues to discuss larger changes and to provide small, well-scoped pull requests that will include tests when appropriate.

License and Attribution
-----------------------
This project will adopt a permissive open source license (MIT or BSD-family by default) unless another license will be chosen explicitly. Third-party code will be attributed and will follow original license terms.

Contact and Further Documentation
---------------------------------
Design notes, architecture diagrams, and developer guides will be added over time. Issues and discussions will be used to coordinate development and will be linked from this README.

End
