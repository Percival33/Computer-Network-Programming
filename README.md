# Computer-Network-Programming

---

- [Computer-Network-Programming](#computer-network-programming)
  - [Team:](#team)
  - [Project overview](#project-overview)
  - [Installation and Running](#installation-and-running)
  - [Architecture](#architecture)
  - [Functional and Non-Functional Requirements](#functional-and-non-functional-requirements)
    - [Functional Requirements](#functional-requirements)
    - [Non-Functional Requirements](#non-functional-requirements)
  - [Use Cases and Scenarios](#use-cases-and-scenarios)
    - [Client Application for Sending Advertisements](#client-application-for-sending-advertisements)
    - [Bulletin Board](#bulletin-board)
  - [Labs Overview](#labs-overview)
    - [Lab 1: Network Programming Basics](#lab-1-network-programming-basics)
    - [Lab 2: Advanced Networking Concepts](#lab-2-advanced-networking-concepts)

---

## Team:

- Marcin Jarczewski
- Bartosz Jasiński
- Kacper Murygin
- Michał Sobiech

## Project overview

OleOgłoszenia is a university project designed for the "Programowanie Sieciowe" (Computer Network Programming) course, focusing on the development of a real-time advertising bulletin board system. It includes a server and two client applications: one for sending and another for displaying advertisements, utilizing the WebSocket protocol to ensure real-time communication.

## Installation and Running

```bash
# In the “project” folder:
python3 -m pip install -r requirements.txt
python3 main.py
# Visit the following for interaction:
# To register a bulletin board: localhost:8000/board
# To send an advertisement: localhost:8000/send
# For API endpoints: localhost:8000/docs
```

## Architecture

The system is structured into three components: two client programs and a server, enabling real-time communication and advertisement management across various bulletin boards.

## Functional and Non-Functional Requirements

### Functional Requirements

- Real-time support for multiple advertising boards.
- Concurrent handling of multiple clients.
- Asynchronous operation with configuration in a single file (`config.py`).

### Non-Functional Requirements

- Clear error messages for clients and error logging for system improvement.
- User-friendly client application.

## Use Cases and Scenarios

### Client Application for Sending Advertisements

- Adding an advertisement to a new or existing category.

### Bulletin Board

- Connecting to a category.
- Receiving new messages.

## Labs Overview

### Lab 1: Network Programming Basics

- **Lab 1.1:** Implementation of a simple iterative server-client model using C, focusing on UDP datagram transmission.
- **Lab 1.2:** Enhancement of the previous task with acknowledgement packets to mimic a TCP-like communication reliability.
- **Lab 1.3:** Experimentation with UDP datagram sizes, exploring the limits and behaviors of UDP transmissions.

### Lab 2: Advanced Networking Concepts

- **Lab 2.1:** Data serialization and transmission over TCP, exploring efficient data structures and their handling over the network.
- **Lab 2.2:** Development of a dual-stack (IPv4/IPv6) TCP communication model, ensuring compatibility and flexibility in network programming.
- **Lab 2.3a:** Investigation of buffer sizes and their impact on transmission efficiency, observing the behavior of TCP sockets under different conditions.
