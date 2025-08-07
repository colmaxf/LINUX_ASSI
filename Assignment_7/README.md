# Building a Priority-Based Task Processing System (POSIX Message Queue)

POSIX Message Queue is a more modern and flexible version than System V Message Queue. Instead of using a `key`, it uses a name (e.g., `/my_queue`) for identification, similar to a file.

The greatest strength of POSIX Message Queue is its support for **priority** for each message. When a process receives a message, the queue will always return the message with the highest priority first, regardless of when it was sent.

This exercise requires you to build a system consisting of a "dispatcher" process and a "worker" process, where the worker will always choose the most important job to do first.

## Exercise: Build Two C Programs

### 1. `task_worker`

A process that receives and processes tasks from the queue in the correct priority order. This is the "listening" process, always running to handle work.

**Initialization:**
*   Define the queue name, for example: `const char* QUEUE_NAME = "/my_task_queue";`.
*   Use `mq_open()` with the `O_CREAT | O_RDONLY` flags to create or open a message queue. This function will return a descriptor for the queue.
*   You may need to define attributes for the queue (e.g., maximum message size) using an `mq_attr` struct.

**Main Loop:**
*   Run an infinite loop.
*   Use `mq_receive()` to wait for and receive a task. This function will automatically retrieve the task with the highest priority waiting in the queue.
*   Print the task content and its priority to the screen. For example: `Processing task (Priority: <prio>): <task_content>`.
*   Simulate task processing by using `sleep(1)`.

**Cleanup:**
*   Set up a signal handler for `SIGINT` (Ctrl+C).
*   Upon receiving the signal, the program must use `mq_close()` to close the descriptor and `mq_unlink()` to completely remove the message queue from the system before exiting.

### 2. `task_dispatcher`

This is the client program for sending work requests.

**Initialization:**
*   Use the same `QUEUE_NAME` as in `task_worker`.
*   Use `mq_open()` with the `O_WRONLY` flag to open the existing queue.

**Sending Tasks:**
*   The program will take 2 command-line arguments: a priority (an integer) and the task content (a string). Example: `./task_dispatcher <priority> "<task_description>"`
*   Use `mq_send()` to send the task to the queue. This function allows you to pass in the task content and its priority.
After sending, print a confirmation message and then exit.

## Compilation and Testing Guide

> **Note:** POSIX Message Queue is part of the real-time library, so you must add the `-lrt` flag when compiling.

### 1. Compilation
```bash
gcc task_worker.c -o task_worker -lrt
gcc task_dispatcher.c -o task_dispatcher -lrt
```

### 2. Run Program
Open **Terminal 1**, run `task_worker`:
```bash
./task_worker
```
Open **Terminal 2**, Submit tasks with unordered priority:
```bash
./task_dispatcher 10 "Update weekly report"
./task_dispatcher 30 "CRITICAL: Restart web server"
./task_dispatcher 20 "Backup user database"
```

### 3. Results
In **Terminal 1**, you will see `task_worker` processing tasks in priority order, not the order they were submitted:
```
Processing task (Priority: 30): CRITICAL: Restart web server
Processing task (Priority: 20): Backup user database
Processing task (Priority: 10): Update weekly report
```
