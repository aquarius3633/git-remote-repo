import threading

# shared buffer
buffer = []

# lock to ensure mutual exclusion
lock = threading.Lock()

# function to be executed by producer thread
def producer():
    while True:
        # read one character at a time from keyboard
        c = input("Enter a character: ")
        
        # acquire the lock before accessing the shared buffer
        lock.acquire()
        
        # add the character to the buffer
        buffer.append(c)
        
        # release the lock after modifying the buffer
        lock.release()

# function to be executed by consumer thread
def consumer():
    while True:
        # acquire the lock before accessing the shared buffer
        lock.acquire()
        
        # check if buffer is not empty
        if len(buffer) > 0:
            # remove the character from the buffer
            c = buffer.pop(0)
            
            # get the number of remaining characters in the buffer
            remaining = len(buffer)
            
            # release the lock after modifying the buffer
            lock.release()
            
            # write the character and the remaining count to console
            print("[{}:{}]".format(c, remaining))
        else:
            # release the lock if the buffer is empty
            lock.release()

# create and start the producer and consumer threads
t1 = threading.Thread(target=producer)
t2 = threading.Thread(target=consumer)
t1.start()
t2.start()