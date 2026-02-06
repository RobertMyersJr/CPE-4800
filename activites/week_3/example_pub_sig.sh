#!/bin/sh
# run the .sh file to generate all of the keys, message
# and demonstrate digital signatures with public key
# note this example has to split signature into 2 parts due to the 256 key length of the hash
# Alice keys
openssl genpkey -algorithm RSA -out alice_private_key.pem -pkeyopt rsa_keygen_bits:2048
openssl rsa -pubout -in alice_private_key.pem -out alice_public_key.pem
# Bob keys
openssl genpkey -algorithm RSA -out bob_private_key.pem -pkeyopt rsa_keygen_bits:2048
openssl rsa -pubout -in bob_private_key.pem -out bob_public_key.pem
# make the message
echo -n "thanksforeverything" > message.txt
# Hash and sign the message with senders (Alice) key
openssl dgst -sha256 -sign alice_private_key.pem -out message.sig message.txt
# split into 2 128 bit signatures for RSA encrypt to work
split -b 128 message.sig message.sig.part
# Encrypt with receivers (Bob) public key
openssl pkeyutl -encrypt -pubin -inkey bob_public_key.pem -in message.sig.partaa -out encrypted_signature.sig.partaa
openssl pkeyutl -encrypt -pubin -inkey bob_public_key.pem -in message.sig.partab -out encrypted_signature.sig.partab
# Encrypt the message with receivers (Bob) public key
openssl pkeyutl -encrypt -pubin -inkey bob_public_key.pem -in message.txt -out message.enc
#
#
# Send encrypted message and encrypted signature in 2 parts
#
#
# decrypt each signature parts
openssl pkeyutl -decrypt -inkey bob_private_key.pem -in encrypted_signature.sig.partaa -out decrypted_signature.partaa
openssl pkeyutl -decrypt -inkey bob_private_key.pem -in encrypted_signature.sig.partab -out decrypted_signature.partab
# decrypt the message
openssl pkeyutl -decrypt -inkey bob_private_key.pem -in message.enc -out decrypted_message.txt
# verify the signature with Senders (alice) public key
cat decrypted_signature.partaa decrypted_signature.partab > decrypted_signature.sig
openssl dgst -sha256 -verify alice_public_key.pem -signature decrypted_signature.sig decrypted_message.txt
