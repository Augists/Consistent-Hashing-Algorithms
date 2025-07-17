# Google's Maglev Consistent Hash Implementation

This repository provides implementations of Google's Maglev consistent hashing algorithm in both C and Go.

Maglev was introduced by Google in a 2016 paper titled ["Maglev: A Fast and Reliable Software Network Load Balancer"](https://static.googleusercontent.com/media/research.google.com/en//pubs/archive/44824.pdf). It's designed to provide high-performance, consistent, and resilient load balancing by minimizing disruptions when the set of backends changes.

## Repository Structure

This project is organized into two main directories:

- **/c**: Contains the C language implementation of the Maglev algorithm. This version focuses on performance and manual memory management. It includes the core algorithm, a demonstration program, and scripts to reproduce the k-failure experiment from the paper.

- **/go**: Contains the Go language implementation. This version showcases a more modern, memory-safe approach with built-in concurrency features. It also includes the core algorithm, a demo, and the k-failure experiment.

Each directory is self-contained and includes its own `README.md` with specific instructions for building and running the code.