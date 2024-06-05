#!/bin/bash

SERVER_PID=""
LOG_FILE="client.log"
SERVER_LOG_FILE="server.log"
PORT=6667
PASSWORD="correct_password"
PIPE_FILE="/tmp/irc_server_output.pipe"

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

    # Create a named pipe if it doesn't exist
    if [ ! -p "$PIPE_FILE" ]; then
        mkfifo "$PIPE_FILE"
    fi

    # Start the IRC server in the background, redirecting its output to the named pipe
    ./ircserv $port $password > "$PIPE_FILE" 2>&1 &
    SERVER_PID=$!

    # Use a background process to read from the named pipe, remove ANSI codes, and append to the log file
    {
        sed "s/\x1B\[[0-9;]*[mGK]//g" < "$PIPE_FILE" | tee -a "$SERVER_LOG_FILE"
    } &

    sleep 2  # Ensure the server has enough time to start
    if ps -p $SERVER_PID > /dev/null; then
        log_message "IRC server started with PID $SERVER_PID"
    else
        log_message "Failed to start IRC server."
        exit 1
    fi
}

stop_server() {
    if [ -z "$SERVER_PID" ]; then
        log_message "Error: No IRC server running."
        return 1
    fi
    log_message "Stopping IRC server..."
    kill $SERVER_PID
    if [ $? -eq 0 ]; then
        log_message "IRC server stopped."
    else
        log_message "Failed to stop IRC server."
    fi

    if [ -p "$PIPE_FILE" ]; then
        rm "$PIPE_FILE"
    fi
}

send_irc_commands() {
    local port=$1
    local commands=("${!2}")

    if ! kill -0 $SERVER_PID &> /dev/null; then
        log_message "Error: IRC server is not running."
        return 1
    fi

    {
        for command in "${commands[@]}"; do
            printf "%s\r\n" "$command"
            sleep 0.5  # Delay between commands
        done
    } | nc localhost $port >> "$LOG_FILE" 2>&1
}

testing_command_before_authentification() {
    log_message "Testing commands before authentication..."

    local commands=(
        "NICK" # You must be authenticated to use this command
        "USER" # You must be authenticated to use this command
    )

    send_irc_commands $PORT commands[@]
}

testing_pass_error_cases() {
    log_message "Testing PASS error cases..."

    local commands=(
        "PASS" # Not enough parameters ERR_NEEDMOREPARAMS (461)
        "PASS wrong_password" # Password incorrect ERR_PASSWDMISMATCH (464)
        "PASS correct_password" # Client is not authenticated
        "PASS correct_password" # You may not reregister ERR_ALREADYREGISTERED (462)
    )

    send_irc_commands $PORT commands[@]
}

testing_nick_error_cases() {
    log_message "Testing NICK error cases..."

    local commands=(
        "PASS correct_password"
        "NICK" # ERR_NONICKNAMEGIVEN (431)
        "NICK #Joe" # ERR_ERRONEUSNICKNAME (432)
        "NICK :Joe" # ERR_ERRONEUSNICKNAME (432)
        "NICK more_than_20_characters" # ERR_ERRONEUSNICKNAME (432)
        "NICK Joe" # Nickname set to Joe
    )

    send_irc_commands $PORT commands[@]
}

testing_user_error_cases() {
    log_message "Testing USER error cases..."

    local commands=(
        "PASS correct_password"
        "NICK Joe"
        "USER" # Not enough parameters ERR_NEEDMOREPARAMS (461)
        "USER #George" # ERR_ERRONEUSNICKNAME (432)
        "USER :George" # ERR_ERRONEUSNICKNAME (432)
        "USER George" # Correct user command
    )

    send_irc_commands $PORT commands[@]
}

testing_mode_command() {
    log_message "Testing MODE command..."

    local commands=(
        "PASS correct_password"
        "NICK Joe"
        "USER George"
        "JOIN #testchannel"
        "MODE #testchannel +k password"
        "MODE #testchannel +l 10"
        "MODE #testchannel +i"
        "MODE #testchannel"
    )

    send_irc_commands $PORT commands[@]
}

testing_user1() {
    log_message "Testing User 1..."

    local commands=(
        "PASS correct_password"
        "NICK nickname1"
        "USER username1"
        "JOIN"
        # "PRIVMSG #testchannel :Hello from User1"
    )

    send_irc_commands $PORT commands[@]
}

testing_user2() {
    log_message "Testing User 2..."

    local commands=(
        "PASS correct_password"
        "NICK nickname2"
        "USER username2 0 * realname2"
        "JOIN #testchannel1"
        "JOIN #testchannel2"
        "PRIVMSG #testchannel :Hello from User2"
    )

    send_irc_commands $PORT commands[@]
}

# --- Main Script Execution ---

check_and_kill_port $PORT
start_server $PORT $PASSWORD

# Sequential tests
# testing_command_before_authentification
# testing_pass_error_cases
# testing_nick_error_cases
# testing_user_error_cases
testing_mode_command

# Simulate two users connected to the server simultaneously
# testing_user1 &
# pid1=$!
# testing_user2 &
# pid2=$!

# Wait for both users to finish their commands
# wait $pid1
# wait $pid2

stop_server

# /buffer close #channel (close the buffer on the left side panel of channel)

