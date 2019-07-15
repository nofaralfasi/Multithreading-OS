Sleeping-barber problem

      Barbershop Requirements:
-     Barber sleeps if no customers waiting
-     Customers leave if no chairs for waiting
-     Waiting customers can't leave until haircut done.
       Solution: we use semaphores
-     Mutex = 1
-     counting: barber_sleeping, customer_queue, cut_done
