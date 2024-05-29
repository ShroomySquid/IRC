#!/bin/bash

SERVER_PID=""
LOG_FILE="irc_commands.log"
SERVER_LOG_FILE="server.log"
PORT=6667
PASSWORD="correct_password"

log_message() {
    echo "$(date +'%Y-%m-%d %H:%M:%S') - $1"
}

if [ -e "$LOG_FILE" ]; then
    rm "$LOG_FILE"
    log_message "Previous command log file deleted."
fi

if [ -e "$SERVER_LOG_FILE" ]; then
    rm "$SERVER_LOG_FILE"
    log_message "Previous server log file deleted."
fi

check_and_kill_port() {
    local port=$1
    local pids
    pids=$(lsof -i :$port -t)

    if [ -n "$pids" ]; then
        log_message "Port $port is in use by PIDs: $pids"
        for pid in $pids; do
            log_message "Killing process $pid"
            kill -9 $pid
        done
    else
        log_message "Port $port is not in use."
    fi
}

start_server() {
    local port=$1
    local password=$2
    log_message "Starting IRC server on port $port..."
    ./ircserv $port $password > "$SERVER_LOG_FILE" 2>&1 &
    SERVER_PID=$!
    log_message "IRC server started with PID $SERVER_PID"
}

stop_server() {
    if [ -z "$SERVER_PID" ]; then
        log_message "Error: No IRC server running."
        return 1
    fi
    log_message "Stopping IRC server..."
    kill $SERVER_PID
    log_message "IRC server stopped."
}


send_irc_commands() {
    local port=$1

    if ! kill -0 $SERVER_PID &> /dev/null; then
        echo "Error: IRC server is not running."
        return 1
    fi
    log_message "Sending IRC commands..."
    local commands=(
        # Before PASS error cases
        "NICK" # You must be authentified to use this command
        "USER" # You must be authentified to use this command

        # PASS error cases
        "PASS" # Not enough parameters ERR_NEEDMOREPARAMS (461)
        "PASS wrong_password" # Password incorrect ERR_PASSWDMISMATCH (464)
        "PASS correct_password" # Client is not authentified
        "PASS correct_password" # You may not reregister ERR_ALREADYREGISTERED (462)

        # NICK error cases
        "NICK" # No nickname given ERR_NONICKNAMEGIVEN (431)
        "NICK #Joe" # ERR_ERRONEUSNICKNAME (432)
        "NICK :Joe" # ERR_ERRONEUSNICKNAME (432)
        "NICK more_than_20_characters" # ERR_ERRONEUSNICKNAME (432)
        "NICK Joe" # Nickname set to Joe

        # USER error cases
        "USER" # Not enough parameters ERR_NEEDMOREPARAMS (461)
        "USER #George" # ERR_ERRONEUSNICKNAME (432)
        "USER :George" # ERR_ERRONEUSNICKNAME (432)
        "USER George"

        # "NICK Joe" # Nickname set to Joe
        # "NICK Joe" # Nickname already in use
        # "USER George" # User registered as George
        # "USER George" # Already registered
        # "JOIN #testchannel" # Join the channel #testchannel
        # "PRIVMSG #testchannel Hello world!" # Send a message to the channel
        # "QUIT" # Quit the server
    )

    {
        for command in "${commands[@]}"; do
            printf "%s\r\n" "$command"
        done
    } | nc localhost $port >> "$LOG_FILE" 2>&1

    log_message "IRC commands sent. Command log file saved to: $LOG_FILE"
    log_message "Server log file saved to: $SERVER_LOG_FILE"
}


# --- Main Script Execution ---

check_and_kill_port $PORT
start_server $PORT $PASSWORD
sleep 2
send_irc_commands $PORT
stop_server
