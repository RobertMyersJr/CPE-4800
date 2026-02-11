#/bin/sh
openssl genpkey -algorithm RSA -out client.pem
openssl rsa -pubout -in client.pem -out client.pem.pub
openssl genpkey -algorithm RSA -out server.pem
openssl rsa -pubout -in server.pem -out server.pem.pub
