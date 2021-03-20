# Operating Systems Project
This repository contains two applications regarding thread synchronization problems, which includes deadlocks and starvation.

## Consumer Producer
The consumer producer problem is an abstraction for thread synchronization problem with starvation. To understand it, keep in mind a farmer producing wheat, and a cow eating it in a constant rate of consumption and production. Additionally, only one of them is allowed to use the barn at a time. With these concepts, let's take look on the problems it might generate.
* If the cow keeps consuming the wheat at the barn all the time, then the producer will not be able to refill it, and the cow will be waiting until it dies. 
* On the other hand, if the producer keeps producing and refilling the barn forever, then the cow will never eat. 
In both scenarios, the cow dies. However, if we keep switching the barn occupancy for one of them at time, then the farmer will be able to feed the cow and the it won't die.

Take a look at the ConsumerProducer folder. It contains a solution for this problem. To use it, compile it with `make` and then run it adding the three following arguments `./consumer_producer <<buffer_size>> <<consumption_rate>> <<production_rate>>`.

## Dining Philosophers
This problem passes through a trouble similar to the last one. The dining philosophers problem is a representation for starvation and deadlocks. It consists into a group of k philosophers dining, but each of them need 2 chopsticks to eat, and there are only k chopsticks. The question is: how the philosophers are gonna eat, if there are no enough chopsticks for all? What if each of half of the philosopher tries to take his chopstick and the chopstick of his neighbor? Well, this could be a good solution, but this way we can have a deadlock because if a philosopher is fast enough to take the next neighbor chopstick, and the other one is not fast enough to take his own chopstick, then they will be waiting each other forever. Also, when solving the problem, we have to be sure to not restrict the use of chopsticks, since it can lead to starvation.

Take a look at the DiningPhilosophers folder. It contains a solution for this problem. To use it, compile it with `make` and then run it adding the three following arguments `./dining_philosophers <<philosophers_quantity>> <<thinking_time>> <<eating_time>>`.