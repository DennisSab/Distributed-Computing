# Distributed-Computing

## Airline Reservation System (Multithreaded - C with Pthreads)

## Overview
This project is a multithreaded airline reservation system implemented in C using Pthreads. It simulates booking, canceling, and managing flight reservations while ensuring synchronization between threads to avoid race conditions.

## Features
- **Multithreading with Pthreads**: Uses multiple threads to handle simultaneous booking requests.
- **Synchronization Mechanisms**: Implements mutexes and condition variables to ensure thread safety.
- **Flight Management**: Supports booking, cancellation, and checking seat availability.
- **Efficient Data Handling**: Uses data structures for managing seat allocation efficiently.

## Technologies Used
- **Language**: C
- **Libraries**: pthread.h, stdlib.h, stdio.h

## Usage
- The program allows multiple threads to request bookings concurrently.
- Each thread simulates a customer trying to book or cancel a seat.
- The system ensures proper synchronization to prevent inconsistent states.

## Synchronization Techniques
- **Mutexes**: Used to ensure that only one thread modifies the reservation system at a time.
- **Condition Variables**: Used for handling wait conditions efficiently.

## Future Enhancements
- Add a GUI for better user interaction.
- Implement a database for persistent storage.
- Improve performance using advanced thread management techniques.

## Contributing
Feel free to fork this repository, make improvements, and submit a pull request.

## License
This project is licensed under the MIT License.

---

### Author
[Ntenis Sampani]

For any queries, contact: [https://github.com/DennisSab]

