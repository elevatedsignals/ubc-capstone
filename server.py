#!/usr/bin/env python
# By Rosa Mohammadi (2018)

from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler
import requests

class RequestHandler(BaseHTTPRequestHandler):

    def do_POST(self):

        print (self.path)

        print("\n----- Request to Elevated Signals Start ----->\n")

        content_length = int(self.headers['Content-Length']) # <--- Gets the size of data
        post_data = self.rfile.read(content_length) # <--- Gets the data itself
        print (post_data)

        r = requests.post("https://dashboard.elevatedsignals.com" + self.path,  params = post_data)
        print (r.status_code)
        print (r.headers)
        print (r.content)

        print("\n----- Request to Elevated Signals End ----->\n")

        # respond back to client with success/fail
        self.send_response(r.status_code)


def main():
    # listen on port 8080 (no ssl) for arduino client
    port = 8080
    print('Listening on localhost:%s' % port)
    server = HTTPServer(('', port), RequestHandler)
    server.serve_forever()


if __name__ == "__main__":
    main()
