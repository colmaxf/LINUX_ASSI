# IPC Signal Operations

## Assignment 1: Basic Signal Handling

**Requirement:** Write a program that catches the `SIGINT` signal (sent by pressing `Ctrl+C`) and prints a message upon receiving it.
- Use the `signal()` function to register a handler for the `SIGINT` signal.
- In the handler, print the message "SIGINT received" each time the signal is caught.
- The program should continue running until it receives the `SIGINT` signal for the third time, at which point it should terminate.

**Hint:** Use a global counter variable to count the number of times the `SIGINT` signal is received and check this variable within the handler.

**Question:** If the `SIGINT` signal is ignored (e.g., using `SIG_IGN`), what happens when you press `Ctrl+C`?

---

## Assignment 2: Creating a Timer with SIGALRM

**Requirement:** Write a program that creates a timer using the `SIGALRM` signal.
- Use the `alarm()` function to trigger a `SIGALRM` signal every second.
- When a `SIGALRM` signal is received, the program should increment a counter and print "Timer: `<seconds>` seconds".
- The program should stop after counting to 10 seconds.

**Hint:** Use `signal()` to register a handler for `SIGALRM`, and call `alarm(1)` within the handler to make the timer repeat every second.

**Question:** What happens if you don't call `alarm(1)` again inside the signal handler?

---

## Assignment 3: Inter-Process Communication with Signals

**Requirement:** Write a program that creates two processes (parent and child) and uses the `SIGUSR1` signal for communication between them.
- Use `fork()` to create a child process from the parent.
- The parent process will send a `SIGUSR1` signal to the child process every 2 seconds.
- When the child process receives the `SIGUSR1` signal, it should print the message "Received signal from parent".
- The program should stop after the signal has been sent 5 times.

**Hint:** Use `kill()` to send the signal from the parent to the child, and use `signal()` in the child process to catch `SIGUSR1`.

**Question:** Why is it necessary to use `kill()` to send the signal in this assignment?

---

## Assignment 4: Handling Signals and User Input Simultaneously

**Requirement:** Write a program that can handle signals while also allowing user input from the keyboard.
- Use `select()` or `pselect()` to allow the program to process signals without being blocked while waiting for keyboard input.
- When a `SIGINT` signal is received, print the message "SIGINT received."
- If a `SIGTERM` signal is received, exit the program.
- When the user enters input and presses Enter, print the content to the console.
