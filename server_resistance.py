import socket
import threading
import time

TARGET_HOST = '127.0.0.1'
TARGET_PORT = 6667
NUM_CONNECTIONS = 10000
CONCURRENCY_LIMIT = 12
THROTTLE_DELAY = 0.1

request_count = 0
bytes_transferred = 0
total_response_time = 0
error_count = 0
start_time = None

def connect_to_server(user_id):
    global request_count, bytes_transferred, total_response_time, error_count
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TARGET_HOST, TARGET_PORT))

        s.send(b'PASS patate\r\n')
        s.send(f'NICK user{user_id}\r\n'.encode())
        s.send(b'USER user 0 * :user\r\n')
        s.send(b'JOIN #test\r\n')
        s.send(b'PRIVMSG #test :Hello from client\r\n')

        start = time.time()
        while True:
            response = s.recv(4096)
            if not response:
                break
            total_response_time += time.time() - start
            request_count += 1
            bytes_transferred += len(response)  # Track the number of bytes transferred
        s.close()
    except Exception as e:
        print(f"Error connecting to server: {e}")
        error_count += 1

def main():
    global start_time
    start_time = time.time()
    print(f"Creating {NUM_CONNECTIONS} connections to {TARGET_HOST}:{TARGET_PORT}")
    threads = []
    for i in range(1, NUM_CONNECTIONS + 1):
        if i % CONCURRENCY_LIMIT == 0:
            time.sleep(THROTTLE_DELAY)  # Throttle the connection rate
        t = threading.Thread(target=connect_to_server, args=(i,))
        t.start()
        threads.append(t)
    # Disconnect from server
    for i in range(1, NUM_CONNECTIONS + 1):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TARGET_HOST, TARGET_PORT))
        s.send(f'QUIT user{i}\r\n'.encode())
        s.close()

    for t in threads:
        t.join()

    duration = time.time() - start_time

    requests_per_second = request_count / duration
    bytes_per_second = bytes_transferred / duration
    avg_response_time = total_response_time / request_count if request_count > 0 else 0
    error_rate = error_count / NUM_CONNECTIONS

    print(f"Throughput: {requests_per_second} requests per second")
    print(f"Bandwidth: {bytes_per_second} bytes per second")
    print(f"Average Response Time: {avg_response_time} seconds")
    print(f"Error Rate: {error_rate}")

if __name__ == "__main__":
    main()
