# https://www.techbeamers.com/python-tutorial-write-multithreaded-python-server/
# https://www.adamsmith.haus/python/answers/how-to-set-a-timeout-on-a-socket-receiving-data-in-python
import copy
import socket
import threading
import sys
import select

server_dict = {
    "0": 23019,
    "1": 32037,
    "2": 18789,
    "3": 16443,
    "4": 18189
}

client_dict = {
    "0": 32037,
    "1": 29295,
    "2": 13603,
    "3": 29533,
    "4": 21952
}

LOCALHOST = "127.0.0.1"
PORT = 1234

SERVER_KEY_REQUEST = "107 KEY REQUEST\a\b"
SERVER_OK = "200 OK\a\b"
SERVER_LOGIN_FAILED = "300 LOGIN FAILED\a\b"
SERVER_SYNTAX_ERROR = "301 SYNTAX ERROR\a\b"
SERVER_LOGIC_ERROR = "302 LOGIC ERROR\a\b"
SERVER_KEY_OUT_OF_RANGE_ERROR = "303 KEY OUT OF RANGE\a\b"
SERVER_MOVE = "102 MOVE\a\b"
SERVER_TURN_LEFT = "103 TURN LEFT\a\b"
SERVER_TURN_RIGHT = "104 TURN RIGHT\a\b"
SERVER_PICK_UP = "105 GET MESSAGE\a\b"
SERVER_LOGOUT = "106 LOGOUT\a\b"

CLIENT_RECHARGING = "RECHARGING\a\b"
CLIENT_FULL_POWER = "FULL POWER\a\b"

MAX_LEN_CLIENT_USERNAME = 20
MAX_LEN_CLIENT_KEY_ID = 5
MAX_LEN_CLIENT_CONFIRMATION = 7
MAX_LEN_CLIENT_OK = 12
MAX_LEN_CLIENT_RECHARGING = 12
MAX_LEN_CLIENT_FULL_POWER = 12
MAX_LEN_CLIENT_MESSAGE = 100

TIMEOUT = 1.0
TIMEOUT_RECHARGING = 5.0

SUFFIX = "\a\b"


def contains_suffix(_str):
    previous_ch = ''
    for i in range(len(_str)):
        current_ch = _str[i]
        if previous_ch + current_ch == SUFFIX:
            return True
        previous_ch = current_ch
    return False


# msg is expected to have no suffix "\a\b"
# for the function to work as expected
def is_valid_message_length(msg, max_len):
    print("MAX_LEN: ", max_len)
    print("GOT_LEN: ", len(msg))
    print("MSG: ", msg)
    # case when 21-chars long message might have '\a' at the end
    if len(msg) + 1 == max_len:
        if msg[-1:] == SUFFIX[0]:
            return True

    return len(msg) <= max_len


def hash_name(name, key_id, hash_type):
    _sum = 0
    for ch in name:
        _sum += ord(ch)
    name_hash = (_sum * 1000) % 65536
    if hash_type == 's':
        name_hash += server_dict[key_id]
    elif hash_type == 'c':
        name_hash += client_dict[key_id]
    name_hash %= 65536
    return name_hash


def are_strings_equal(_str1, _str2):
    _str1 = str(_str1)
    _str2 = str(_str2)
    if len(_str1) != len(_str2):
        return False
    for i in range((len(_str1))):
        if ord(_str1[i]) != ord(_str2[i]):
            return False
    return True


def is_a_number(_str):
    minuses = 0

    for i in range(len(_str)):
        if minuses > 1:
            return False
        if _str[i] == '-':
            minuses += 1
            continue
        if _str[i].isnumeric() is not True:
            return False
    if minuses == 1 and _str[0] != '-':
        return False
    return True


class Coordinate:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __str__(self):
        return "[ " + str(self.x) + " ; " + str(self.y) + " ]"

    def get_x(self):
        return self.x

    def get_y(self):
        return self.y


class ClientThread(threading.Thread):
    # def __init__(self, client_address, server_sock, client_socket):
    server_sock = ''
    csocket = ''

    def __init__(self, client_address, client_sock):
        threading.Thread.__init__(self)
        self.csocket = client_sock
        self.client_address = client_address
        print("New connection", client_address)

    def close_connection(self):
        self.csocket.close()
        sys.exit(1)

    def server_error(self, error_msg):
        self.send_message(error_msg)
        self.close_connection()
        sys.exit(1)
        # raise Exception(error_msg)

    def validate_key(self, key_id):
        if key_id.isnumeric() is not True:
            self.server_error(SERVER_SYNTAX_ERROR)
        if key_id not in server_dict.keys():
            self.server_error(SERVER_KEY_OUT_OF_RANGE_ERROR)

    def validate_hash(self, received_client_hash, name, key_id):
        referent_client_hash = hash_name(name, key_id, 'c')

        if received_client_hash.isnumeric() is not True:
            self.server_error(SERVER_SYNTAX_ERROR)

        if are_strings_equal(referent_client_hash, received_client_hash) is not True:
            print("Different hashes:")
            print("\t-- reference: _" + str(referent_client_hash) + "_")
            print("\t-- received:  _" + str(received_client_hash) + "_")
            self.server_error(SERVER_LOGIN_FAILED)
            return

        self.send_message(SERVER_OK)

    # max_len parameters indicates the maximum length of the expected message
    def receive_message(self, max_len):
        msg = ""
        is_suffix_received = False

        self.csocket.settimeout(TIMEOUT)
        while is_suffix_received is not True:
            try:
                print("message:", msg)
                if len(msg) >= max_len:
                    if are_strings_equal("RECHARGING", msg):
                        print("HOLD UP! RECHARGING.")
                    print("A way too long message...")
                    self.server_error(SERVER_SYNTAX_ERROR)
                byte = self.csocket.recv(1).decode('unicode_escape')
                if msg[-1:] == '\a':
                    if byte == '\b':
                        is_suffix_received = True
                msg += byte
            except socket.timeout:
                print("READING: TIMEOUT ERROR")
                self.close_connection()
                # sys.exit(1)
                pass
        print("---> _" + msg + "_   #####################################")
        # print("Returned message is: >" + msg[:-2] + "<")
        return msg[:-2]

    def send_message(self, msg):
        print("<--- _" + msg + "_")
        self.csocket.sendall(msg.encode('utf-8'))

    def authenticate(self):
        # -> CLIENT_USERNAME
        name = self.receive_message(MAX_LEN_CLIENT_USERNAME)
        # TODO: self.validate_name(name)
        # <- SERVER_KEY_REQUEST
        self.send_message(SERVER_KEY_REQUEST)
        # -> CLIENT_KEY_ID -- TODO: may be interrupted by RECHARGING
        key_id = self.receive_message(MAX_LEN_CLIENT_KEY_ID)
        self.validate_key(key_id)
        # <- SERVER_CONFIRMATION
        server_hash = hash_name(name, key_id, 's')
        server_confirmation = str(server_hash) + SUFFIX
        self.send_message(server_confirmation)
        # -> CLIENT_CONFIRMATION -- TODO: may be interrupted by RECHARGING
        client_hash = self.receive_message(MAX_LEN_CLIENT_CONFIRMATION)
        self.validate_hash(client_hash, name, key_id)

    def parse_and_validate_ok_message(self, msg):
        # cutting off "OK "
        if len(msg) < 6:
            self.server_error(SERVER_SYNTAX_ERROR)

        msg_without_ok = msg[3:]
        if ' ' not in msg_without_ok:
            self.server_error(SERVER_SYNTAX_ERROR)

        first_num = second_num = ""
        for i in range(len(msg_without_ok)):
            if msg_without_ok[i] == ' ':
                for j in range(i + 1, len(msg_without_ok)):
                    second_num += msg_without_ok[j]
                break
            first_num += msg_without_ok[i]
        if is_a_number(first_num) and is_a_number(second_num) is not True:
            self.server_error(SERVER_SYNTAX_ERROR)
        return first_num, second_num

    def parse_coord(self, msg):
        first_num, second_num = self.parse_and_validate_ok_message(msg)
        # print("x: _" + first_num + "_")
        # print("y: _" + second_num + "_")
        x = int(first_num)
        y = int(second_num)
        coord = Coordinate(x, y)
        return coord

    # TODO: may be interrupted by RECHARGING
    def rotate_up(self, curr_dir):
        if curr_dir == "UP":
            return "UP"

        if curr_dir == "DOWN":
            self.send_message(SERVER_TURN_RIGHT)
            self.parse_coord(self.receive_message(MAX_LEN_CLIENT_OK))
            self.send_message(SERVER_TURN_RIGHT)
        elif curr_dir == "LEFT":
            self.send_message(SERVER_TURN_RIGHT)
        elif curr_dir == "RIGHT":
            self.send_message(SERVER_TURN_LEFT)

        self.parse_coord(self.receive_message(MAX_LEN_CLIENT_OK))

        return "UP"

    def rotate_down(self, curr_dir):
        if curr_dir == "DOWN":
            return "DOWN"

        if curr_dir == "UP":
            self.send_message(SERVER_TURN_RIGHT)
            self.parse_coord(self.receive_message(MAX_LEN_CLIENT_OK))
            self.send_message(SERVER_TURN_RIGHT)
        elif curr_dir == "LEFT":
            self.send_message(SERVER_TURN_LEFT)
        elif curr_dir == "RIGHT":
            self.send_message(SERVER_TURN_RIGHT)

        self.parse_coord(self.receive_message(MAX_LEN_CLIENT_OK))

        return "DOWN"

    def rotate_left(self, curr_dir):
        if curr_dir == "LEFT":
            return "LEFT"

        if curr_dir == "UP":
            self.send_message(SERVER_TURN_LEFT)
        elif curr_dir == "DOWN":
            self.send_message(SERVER_TURN_RIGHT)
        elif curr_dir == "RIGHT":
            self.send_message(SERVER_TURN_RIGHT)
            self.parse_coord(self.receive_message(MAX_LEN_CLIENT_OK))
            self.send_message(SERVER_TURN_RIGHT)

        self.parse_coord(self.receive_message(MAX_LEN_CLIENT_OK))

        return "LEFT"

    def rotate_right(self, curr_dir):
        if curr_dir == "RIGHT":
            return "RIGHT"

        if curr_dir == "UP":
            self.send_message(SERVER_TURN_RIGHT)
        elif curr_dir == "DOWN":
            self.send_message(SERVER_TURN_LEFT)
        elif curr_dir == "LEFT":
            self.send_message(SERVER_TURN_RIGHT)
            self.parse_coord(self.receive_message(MAX_LEN_CLIENT_OK))
            self.send_message(SERVER_TURN_RIGHT)

        self.parse_coord(self.receive_message(MAX_LEN_CLIENT_OK))

        return "RIGHT"

    def find_direction(self):
        # STEP 1
        # <- SERVER_MOVE
        self.send_message(SERVER_MOVE)
        # -> CLIENT_OK
        msg = self.receive_message(MAX_LEN_CLIENT_OK)
        coord1 = self.parse_coord(msg)

        # STEP 2
        # <- SERVER_MOVE
        self.send_message(SERVER_MOVE)
        # -> CLIENT_OK
        msg = self.receive_message(MAX_LEN_CLIENT_OK)
        coord2 = self.parse_coord(msg)

        # if pos did not change, turn left and move once again
        if coord1 == coord2:
            # <- SERVER_TURN_LEFT
            self.send_message(SERVER_TURN_LEFT)
            # -> CLIENT_OK
            msg = self.receive_message(MAX_LEN_CLIENT_OK)
            coord1 = self.parse_coord(msg)

            # <- SERVER_MOVE
            self.send_message(SERVER_MOVE)
            # -> CLIENT_OK
            msg = self.receive_message(MAX_LEN_CLIENT_OK)
            coord2 = self.parse_coord(msg)

        direction = ""

        if coord1.get_x() < coord2.get_x():
            direction = "RIGHT"
        elif coord1.get_x() > coord2.get_x():
            direction = "LEFT"
        elif coord1.get_y() < coord2.get_y():
            direction = "UP"
        elif coord1.get_y() > coord2.get_y():
            direction = "DOWN"

        return coord2, direction

    def move_x(self, curr_coord, direction):
        print("--- move_x:")
        if curr_coord.get_x() == 0:
            return direction

        if curr_coord.get_x() < 0:
            direction = self.rotate_right(direction)
        elif curr_coord.get_x() > 0:
            direction = self.rotate_left(direction)
        self.send_message(SERVER_MOVE)
        print("--- move_x")

        return direction

    def move_y(self, curr_coord, direction):
        print("--- move_y:")
        if curr_coord.get_y() == 0:
            return direction
        if curr_coord.get_y() < 0:
            direction = self.rotate_up(direction)
        elif curr_coord.get_y() > 0:
            direction = self.rotate_down(direction)
        self.send_message(SERVER_MOVE)
        print("--- move_y")

        return direction

    def cross_obstacle(self, coord, direction):
        print("Crossing...")
        print(coord, direction)
        # coord = None
        if direction == "RIGHT":
            print("Crossing... RIGHT")

            direction = self.rotate_down(direction)
            # msg = self.receive_message(MAX_LEN_CLIENT_OK)
            # coord = self.parse_coord(msg)
            self.send_message(SERVER_MOVE)
            msg = self.receive_message(MAX_LEN_CLIENT_OK)
            coord = self.parse_coord(msg)

            direction = self.rotate_right(direction)
            # msg = self.receive_message(MAX_LEN_CLIENT_OK)
            # coord = self.parse_coord(msg)
            self.send_message(SERVER_MOVE)
            msg = self.receive_message(MAX_LEN_CLIENT_OK)
            coord = self.parse_coord(msg)
        elif direction == "LEFT":
            print("Crossing... LEFT")

            direction = self.rotate_down(direction)
            # msg = self.receive_message(MAX_LEN_CLIENT_OK)
            # coord = self.parse_coord(msg)
            self.send_message(SERVER_MOVE)
            msg = self.receive_message(MAX_LEN_CLIENT_OK)
            coord = self.parse_coord(msg)

            direction = self.rotate_left(direction)
            # msg = self.receive_message(MAX_LEN_CLIENT_OK)
            # coord = self.parse_coord(msg)
            self.send_message(SERVER_MOVE)
            msg = self.receive_message(MAX_LEN_CLIENT_OK)
            coord = self.parse_coord(msg)
        elif direction == "UP":
            print("Crossing... UP")

            direction = self.rotate_left(direction)
            print("dead 0")

            # msg = self.receive_message(MAX_LEN_CLIENT_OK)
            # print("dead 1")

            # coord = self.parse_coord(msg)
            # print("dead 2")

            self.send_message(SERVER_MOVE)
            msg = self.receive_message(MAX_LEN_CLIENT_OK)
            coord = self.parse_coord(msg)

            direction = self.rotate_up(direction)
            # msg = self.receive_message(MAX_LEN_CLIENT_OK)
            # coord = self.parse_coord(msg)

            self.send_message(SERVER_MOVE)
            msg = self.receive_message(MAX_LEN_CLIENT_OK)
            coord = self.parse_coord(msg)
        elif direction == "DOWN":
            print("Crossing... DOWN")

            direction = self.rotate_right(direction)

            self.send_message(SERVER_MOVE)
            msg = self.receive_message(MAX_LEN_CLIENT_OK)
            coord = self.parse_coord(msg)

            direction = self.rotate_down(direction)

            self.send_message(SERVER_MOVE)
            msg = self.receive_message(MAX_LEN_CLIENT_OK)
            coord = self.parse_coord(msg)
        print("After crossing: ", coord)
        return coord, direction

    def navigate_robot(self, curr_coord, direction):
        print("DIRECTION IS: ", direction)
        print("CURR_CORD IS: ", curr_coord)

        while True:
            print("CURR_DIR: ", direction)
            if curr_coord == Coordinate(0, 0):
                print("FINISH 1")
                break
            pred_coord = curr_coord
            if curr_coord.get_x() != 0:
                direction = self.move_x(curr_coord, direction)
                msg = self.receive_message(MAX_LEN_CLIENT_OK)
                curr_coord = self.parse_coord(msg)
            if pred_coord.get_x() == curr_coord.get_x() and curr_coord.get_y() == 0:
                pred_coord = curr_coord
                curr_coord, direction = self.cross_obstacle(curr_coord, direction)

            if curr_coord == Coordinate(0, 0):
                print("FINISH 2")
                break

            pred_coord = curr_coord
            if curr_coord.get_y() != 0:
                direction = self.move_y(curr_coord, direction)
                msg = self.receive_message(MAX_LEN_CLIENT_OK)
                curr_coord = self.parse_coord(msg)
            if pred_coord.get_y() == curr_coord.get_y() and curr_coord.get_x() == 0:
                pred_coord = curr_coord
                curr_coord, direction = self.cross_obstacle(curr_coord, direction)

            if curr_coord == Coordinate(0, 0):
                print("FINISH 3")
                break

            print(curr_coord)

    def find_path(self):
        curr_coord, direction = self.find_direction()

        if curr_coord.get_x() > 0:
            direction = self.rotate_left(direction)
        elif curr_coord.get_x() < 0:
            direction = self.rotate_right(direction)
        elif curr_coord.get_y() > 0:
            direction = self.rotate_down(direction)
        elif curr_coord.get_y() < 0:
            direction = self.rotate_up(direction)

        self.navigate_robot(curr_coord, direction)

    def pick_up(self):
        self.send_message(SERVER_PICK_UP)
        msg = self.receive_message(MAX_LEN_CLIENT_MESSAGE)
        self.send_message(SERVER_LOGOUT)

    def run(self):
        try:
            self.authenticate()
            self.find_path()
            self.pick_up()
            self.close_connection()
        except Exception as e:
            print("----- TODO: ERROR TYPE ----")
            print("----- " + str(e) + " -----")
        '''
        self.authenticate()
        self.find_path()
        self.close_connection()
        '''


if __name__ == "__main__":
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((LOCALHOST, PORT))
    print("Server started")
    print("Waiting for client request..")

    threads = []
    robot_number = 1

    server.listen()

    while True:
        client_sock, client_address = server.accept()
        print("\n\n\n\n")
        print(robot_number)
        new_thread = ClientThread(client_address, client_sock)
        new_thread.start()
        threads.append(new_thread)
        robot_number += 1
