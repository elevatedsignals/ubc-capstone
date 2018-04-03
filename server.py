#!/usr/bin/env python
# Reflects the requests from HTTP methods GET, POST, PUT, and DELETE
# Written by Nathan Hamiel (2010)
# Modified by Rosa Mohammadi (2018)

from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler

class RequestHandler(BaseHTTPRequestHandler):

    def do_GET(self):

        request_path = self.path

        print("\n----- Request Start ----->\n")
        print(request_path)
        print(self.headers)
        print("<----- Request End -----\n")

        self.send_response(200)
        self.send_header("Set-Cookie", "foo=bar")

    def do_POST(self):

        client_path = self.path
        client = self

        print("\n----- Request Start ----->\n")
        print(client_path)

        # use same headers for client server

        # modify content length

        # from payload formatting parse API key

        # send to server path with api key endpoint, without API key

        # get success from Server

        # respond back to arduino client with success
        client.send_response(200)

def main():
    # listen on port 8080 (no ssl) for arduino client
    port = 8080
    print('Listening on localhost:%s' % port)
    server = HTTPServer(('', port), RequestHandler)
    server.serve_forever()


if __name__ == "__main__":
    main()
