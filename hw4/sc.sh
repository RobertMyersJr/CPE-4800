#!/bin/bash

# 1. Setup variables
# DES blocks are 8 bytes. "Standard" is 8 chars.
PLAINTEXT="Standard" 
KEY_HEX="0123456789ABCDEF"  
PROV="-provider legacy -provider default"

echo "Original Plaintext: $PLAINTEXT"
echo "--------------------------------------------------------"

# 2. Perform Standard DES Decryption (Single Stage)
# Using -nopad to ensure we get an output even if the block isn't 'valid' ciphertext
DES_SINGLE=$(echo -n "$PLAINTEXT" | openssl enc -des-ecb -d -K "$KEY_HEX" -nosalt -nopad -base64 $PROV 2>/dev/null)

# 3. Perform the proposed DED (Decrypt -> Encrypt -> Decrypt)
# Step 1: Decrypt
STEP1=$(echo -n "$PLAINTEXT" | openssl enc -des-ecb -d -K "$KEY_HEX" -nosalt -nopad $PROV 2>/dev/null)

# Step 2: Encrypt (result of Step 1)
STEP2=$(echo -n "$STEP1" | openssl enc -des-ecb -e -K "$KEY_HEX" -nosalt -nopad $PROV 2>/dev/null)

# Step 3: Decrypt (result of Step 2)
DED_FINAL=$(echo -n "$STEP2" | openssl enc -des-ecb -d -K "$KEY_HEX" -nosalt -nopad -base64 $PROV 2>/dev/null)

# 4. Check for empty strings
if [[ -z "$DES_SINGLE" || -z "$DED_FINAL" ]]; then
    echo "ERROR: OpenSSL returned an empty string. Check if 'legacy' provider is available."
    openssl list -providers
    exit 1
fi

# 5. Print Results
echo "Standard DES (Decryption) Result: $DES_SINGLE"
echo "Proposed DED Algorithm Result:    $DED_FINAL"
echo "--------------------------------------------------------"

# 6. Verification Logic
if [ "$DES_SINGLE" == "$DED_FINAL" ]; then
    echo "VERIFICATION: SUCCESS"
    echo "The strings match. DED(K,K,K) is equivalent to a single DES(K) operation."
else
    echo "VERIFICATION: FAILURE - The strings do not match."
fi
