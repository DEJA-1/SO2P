# SO2P Project

First project implements the *Dining Philosophers Problem*. The simulation visualizes the state of each philosopher in real-time.

---

##  How to build

```bash
clone repository
mkdir build && cd build
cmake -G Ninja ..
ninja
```

---

## Running the program

```bash
./dining_philosophers <number_of_philosophers>
```

- `number_of_philosophers` – minimum: 2 (e.g. `5`)

---

## Problem description

The **Dining Philosophers Problem** is a classic concurrency challenge:
- N philosophers sit around a circular table.
- Each philosopher alternates between **thinking** and **eating**. The philosopher gets hungry after thinking.
- To eat, a philosopher needs **both the left and right forks**.
- Forks are shared between neighboring philosophers.

### Goals:
- Avoid **deadlock** *(situation where all philosophers are waiting forever and none can proceed)*
- Prevent **starvation** *(situation where a philosopher waits for long time while others continue to eat)*

---

## How does it work?

Each philosopher runs in a separate **thread**, where each of them follows this loop: 

    1. Think
    2. Become hungry (wait for forks)
    3. Eat
    4. Release forks

---

## Critical sections and synchronization

### What are the critical sections?
- The moments when a philosopher **picks up or puts down forks**.
- Since forks are shared between neighbors, simultaneous access between neighbour philosophers must be prevented.

###  How is it solved?

Each fork is represented as a `std::mutex`, so only one philosopher can use it at a time.

Before eating, a philosopher needs to lock both forks — the one on the left and the one on the right.

To avoid deadlock, the program uses a simple trick:

Instead of locking the left fork first (which could cause a cycle), each philosopher **always locks the lower-numbered fork first**, and then the higher one. For example:

```cpp
if (leftFork < rightFork) {
    forks[leftFork].lock();
    forks[rightFork].lock();
} else {
    forks[rightFork].lock();
    forks[leftFork].lock();
}
```

This way, all philosophers follow the same rule and try to lock forks in the same order.  
Because of this, they don’t end up waiting for each other in a circle.

After eating, the philosopher simply unlocks both forks.

It’s a small change, but it makes the whole system safe and allows all philosophers to eat without getting stuck.
