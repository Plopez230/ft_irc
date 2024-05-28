import re
import socket

class Client:

    def __init__(self, address, port):
        self.address = address
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.settimeout(3)
        self.socket.connect((self.address, self.port))
        self.receive_queue = []

    def __del__(self):
        self.socket.close()

    def send(self, msg):
        self.socket.sendall((msg + "\n").encode('utf-8'))

    def receive(self):
        if len(self.receive_queue) == 0:
            msg = self.socket.recv(10000).decode('utf-8')
            self.receive_queue.extend(
                [s for s in msg.split("\n") if not s == ""])
        try:
            return self.receive_queue.pop(0)
        except:
            return None

class Playback:

    def __init__(self, filename, ip, port):
        self.filename = filename
        self.lines = []
        self.ip = ip
        self.port = port
        self.read_file()
        
    def read_file(self):
        with open(self.filename) as file:
            for line in file:
                self.lines.append(line)

    def read_line(self, line):
        reg = re.search("([<>x])([0-9])[:](.*)", line)

        return {
            "action": reg[1],
            "client": reg[2],
            "message": reg[3]
        }
    
    def run(self, validate_responses=True):
        clients = {}

        for l in self.lines:
            line = self.read_line(l)
            
            if not line["client"] in clients:
                clients[line["client"]] = Client(self.ip, self.port)

            if line["action"] == "<": #receive
                print (l)
                response = clients[line["client"]].receive()
                if validate_responses:
                    assert line["message"] in response, \
                        line["message"] + " not in " + response

            if line["action"] == ">": #send
                print (l)
                clients[line["client"]].send(line["message"])

            if line["action"] == "x": #close connection
                print (l)
                del clients[line["client"]]
