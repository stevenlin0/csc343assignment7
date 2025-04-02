#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>

using namespace std;

//This is a lock
mutex cout_mutex;

// This is to show what the philosophers do
void philosopher(int id, mutex& left_fork, mutex& right_fork) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.5, 1.5);

    // This is to repeat the think and eat
    for (int i = 0; i < 5; ++i) {
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Philosopher " << id << " is thinking..." << endl;
        }
        this_thread::sleep_for(chrono::duration<double>(dis(gen)));

        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Philosopher " << id << " is hungry and trying to eat..." << endl;
        }

        if (id == 0) {
            lock_guard<mutex> first(left_fork);
            lock_guard<mutex> second(right_fork);

            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "Philosopher " << id << " is eating..." << endl;
            }
            this_thread::sleep_for(chrono::duration<double>(dis(gen)));
        } else {
            lock_guard<mutex> first(right_fork);
            lock_guard<mutex> second(left_fork);

            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "Philosopher " << id << " is eating..." << endl;
            }
            this_thread::sleep_for(chrono::duration<double>(dis(gen)));
        }

        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Philosopher " << id << " finished eating." << endl;
        }
    }
}

int main() {
    // This is to create forks
    mutex fork0, fork1;

    // This is to create philosopher threads
    thread phil0(philosopher, 0, ref(fork0), ref(fork1));
    thread phil1(philosopher, 1, ref(fork0), ref(fork1));

    // This is to wait for philosophers to finish
    phil0.join();
    phil1.join();

    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "All philosophers have finished." << endl;
    }

    return 0;
}
