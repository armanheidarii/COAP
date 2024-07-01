from coapthon.server.coap import CoAP
from resource import BasicResource


class CoAPServer(CoAP):
    def __init__(self, host, port):
        CoAP.__init__(self, (host, port))
        self.add_resource("basic/", BasicResource())


def main():
    hostname = "127.0.0.1"
    port = 5683
    server = CoAPServer(hostname, port)
    try:
        print(f"COAP server is listening in {hostname}:{port}")
        server.listen(10)
    except KeyboardInterrupt:
        print("Server Shutdown")
        server.close()
        print("Exiting...")


if __name__ == "__main__":
    main()
