from Crypto.Cipher import DES
import binascii

# DES requires an 8-byte key and 8-byte blocks
key = b'8ByteKey'
plaintext = b'Standard' # Exactly 8 bytes

def des_op(data, key, mode):
    # ECB mode is the simplest for demonstrating the math
    cipher = DES.new(key, DES.MODE_ECB)
    if mode == "encrypt":
        return cipher.encrypt(data)
    else:
        return cipher.decrypt(data)

print(f"Original: {plaintext.decode()}")

# Target: Single DES Decrypt
single_des = des_op(plaintext, key, "decrypt")

# Proposed DED: Decrypt -> Encrypt -> Decrypt
step1 = des_op(plaintext, key, "decrypt")
step2 = des_op(step1, key, "encrypt")
ded_final = des_op(step2, key, "decrypt")

print(f"DES Decrypt Hex: {binascii.hexlify(single_des).decode()}")
print(f"DED Final Hex:   {binascii.hexlify(ded_final).decode()}")

if single_des == ded_final:
    print("\nSUCCESS: DED(K,K,K) is identical to a single DES decryption.")
