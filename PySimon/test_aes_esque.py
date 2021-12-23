import logging
import threading
import time
from AES_ESQUE import AES_ESQUE

def compute_pairs(start, end):
    cipher = AES_ESQUE()
    cpts = [cipher.encrypt(i) for i in range(start, end)]
    
    with open(f'{start}-{end}.txt', 'w') as f:
        f.write(cpts)

if __name__ == "__main__":
    format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=format, level=logging.INFO,
                        datefmt="%H:%M:%S")
    threads = list()
    for i in range(1<<7):
        logging.info("Main    : create and start thread %d.", i)
        x = threading.Thread(target=compute_pairs, args=(i << 20, (i + 1) << 20))
        threads.append(x)
        x.start()

    for idx, thread in enumerate(threads):
        logging.info("Main    : before joining thread %d.", idx)
        thread.join()
        logging.info("Main    : thread %d done", idx)
