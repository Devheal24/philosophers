*This project has been created as part of the 42 curriculum by mgarnier.*

![Logo](philo/images/philosophers.jpg)
# <h1 align="center"><span style="color:red">📖​PHILOSOPHERS🍝</span></h1>

## <span style="color:green">1️⃣​Description</span>

The purpose of this project is to manage multiple threads that share the same information.
The use of mutexes is essential for correct operation.

In this context, the threads are represented by philosophers, and the mutexes are represented by forks. There is an equal number of philosophers and forks.

Philosophers must perform a series of actions in a specific order, but to do so, each philosopher needs to acquire two forks, which are shared among all the philosophers.

In the bonus part, the mutexes are replaced by semaphore and threads of forks by child process.

## <span style="color:green">2️⃣​Instructions</span>

philosophers:

`cd philo/`

`make`

`./philo [nb_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [optional_number_of_times_each_philosopher_must_eat]`

philosophers bonus:

`cd philo_bonus/`

`make`

`./philo_bonus [nb_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [optional_number_of_times_each_philosopher_must_eat]`

Exemple:
![exemple](images/exemple_philo.png)

## <span style="color:green">3️⃣​Resources</span>

I used this [website](https://www.codequoi.com/threads-mutex-et-programmation-concurrente-en-c/#attention-aux-deadlock) to better understand thread, mutex and semaphore.

I asked Chatgpt for exercices on these new concepts and practiced by writting small programs.

I developed my own testing script using Bash.