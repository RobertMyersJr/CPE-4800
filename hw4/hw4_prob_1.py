#!/usr/bin/python3
from Crypto.Cipher import DES
import binascii

key = b'12345678'
plaintext = b'HiRobert' # Exactly 8 bytes

print(f"Original: {plaintext.decode()}")

cipher = DES.new(key, DES.MODE_ECB)

single_des = cipher.decrypt(plaintext)

# Proposed DED: Decrypt -> Encrypt -> Decrypt
des_enc = cipher.encrypt(single_des)
des_enc_des = cipher.decrypt(des_enc)

print(f"DES Decrypt Hex: {binascii.hexlify(single_des).decode()}")
print(f"DED Final Hex:   {binascii.hexlify(ded_final).decode()}")

