#!/bin/bash

# Define a 64-bit hex key (8 bytes) and 8 bytes of plaintext
KEY="0123456789ABCDEF"
PLAINTEXT="HelloWorld"

echo "Plaintext: $PLAINTEXT"
echo "Key: $KEY"
echo "---------------------------------------"

# 1. Simulate DED Sequence (D1 -> E1 -> D1)
# Step 1: Decrypt plaintext with K1
STEP1=$(echo -n "$PLAINTEXT" | openssl enc -des-ecb -d -K "$KEY" -nopad -nosalt | xxd -p)
# Step 2: Encrypt result with K1 (effectively K2)
STEP2=$(echo -n "$STEP1" | xxd -r -p | openssl enc -des-ecb -e -K "$KEY" -nopad -nosalt | xxd -p)
# Step 3: Decrypt result with K1 (effectively K3)
FINAL_DED=$(echo -n "$STEP2" | xxd -r -p | openssl enc -des-ecb -d -K "$KEY" -nopad -nosalt | xxd -p)

echo "DED Sequence Output (Hex): $FINAL_DED"

# 2. Perform Single DES Decryption
SINGLE_DES=$(echo -n "$PLAINTEXT" | openssl enc -des-ecb -d -K "$KEY" -nopad -nosalt | xxd -p)

echo "Single DES Decrypt (Hex):   $SINGLE_DES"
echo "---------------------------------------"

# 3. Verification
if [ "$FINAL_DED" == "$SINGLE_DES" ]; then
    echo "SUCCESS: DED with identical keys is equivalent to single DES decryption."
else
    echo "FAILURE: The outputs do not match."
fi

