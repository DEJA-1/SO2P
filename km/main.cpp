#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>
#include <random>

#include "util/PhilosopherState.h"

class DiningPhilosophers {
    int numPhilosophers;
    std::vector<std::mutex> forks;
    std::atomic<bool> running;
    std::mutex outputMutex;

    std::mt19937 rng;
    std::uniform_int_distribution<int> thinkingTime;
    std::uniform_int_distribution<int> eatingTime;

    std::vector<PhilosopherState> states;

public:
    DiningPhilosophers(int n) {
        numPhilosophers = n;
        forks = std::vector<std::mutex>(n);
        running = true;

        std::random_device rd;
        rng = std::mt19937(rd());
        thinkingTime = std::uniform_int_distribution<int>(2000, 4000);
        eatingTime = std::uniform_int_distribution<int>(2000, 3000);

        states = std::vector<PhilosopherState>(n, PhilosopherState::THINKING);
    }

    void philosopher(int id) {
        while (running) {
            think(id);
            eat(id);
        }
    }

    void run(int seconds) {
        std::vector<std::thread> philosophers;
        runPhilosophersOnThreads(philosophers);
        finishSimulationAfterTimeout(seconds, philosophers);
    }

private:
    void think(int id) {
        updateState(id, PhilosopherState::THINKING);
        std::this_thread::sleep_for(std::chrono::milliseconds(thinkingTime(rng)));
    }

    void eat(int id) {
        updateState(id, PhilosopherState::HUNGRY);

        int leftFork;
        int rightFork;
        pickUpForks(id, leftFork, rightFork);

        updateState(id, PhilosopherState::EATING);
        std::this_thread::sleep_for(std::chrono::milliseconds(eatingTime(rng)));

        putDownForks(leftFork, rightFork);
    }

    void pickUpForks(int id, int &leftFork, int &rightFork) {
        leftFork = id;
        rightFork = (id + 1) % numPhilosophers;

        if (leftFork < rightFork) {
            forks[leftFork].lock();
            forks[rightFork].lock();
        } else {
            forks[rightFork].lock();
            forks[leftFork].lock();
        }
    }

    void putDownForks(int leftFork, int rightFork) {
        forks[leftFork].unlock();
        forks[rightFork].unlock();
    }

    void printTable() {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "\r[ ";
        for (int i = 0; i < numPhilosophers; i++) {
            std::cout << i << ": " << std::setw(8) << std::left << stateToString(states[i]) << " | ";
        }
        std::cout << "]  " << std::flush;
    }

    void updateState(int id, PhilosopherState state) {
        if (!running) return;

        states[id] = state;
        printTable();
    }

    void runPhilosophersOnThreads(std::vector<std::thread> &philosophers) {
        for (int i = 0; i < numPhilosophers; i++) {
            philosophers.emplace_back(&DiningPhilosophers::philosopher, this, i);
        }
    }

    void finishSimulationAfterTimeout(const int seconds, std::vector<std::thread> &philosophers) {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
        running = false;

        clearThreads(philosophers);
    }

    static void clearThreads(std::vector<std::thread> &philosophers) {
        printClearingThreads();

        for (auto &philosopher: philosophers) {
            if (philosopher.joinable()) {
                philosopher.join();
            }
        }
    }

    static void printClearingThreads() {
        std::cout << "\n------------------------------------------------" << std::endl;
        std::cout << "simulation finishied, clearing threads.." << std::endl;
        std::cout << "------------------------------------------------" << std::endl;
    }
};

void printSimulationEnd() {
    std::cout << "\n------------------------------------------------" << std::endl;
    std::cout << "simulation completed" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
}

void printSimulationStart(int numPhilosophers) {
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "starting simulation with " << numPhilosophers << " philosophers" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_philosophers>" << std::endl;
        return 1;
    }

    int numPhilosophers = std::stoi(argv[1]);
    if (numPhilosophers < 2) {
        std::cerr << "Error: Number of philosophers must be at least 2" << std::endl;
        return 1;
    }

    printSimulationStart(numPhilosophers);

    DiningPhilosophers dp(numPhilosophers);
    dp.run(20);

    printSimulationEnd();
    return 0;
}
