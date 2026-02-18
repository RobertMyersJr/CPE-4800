#!/bin/python3
"""
@file lfsr.py 
@author Robert Myers Jr.
@version V1.0
@brief Script that performs a shrinking generator 
"""
import collections
import matplotlib.pyplot as plt

def plot_frequencies(frequencies, title):
    sorted_items = sorted(frequencies.items())
    chars, counts = zip(*sorted_items) if sorted_items else ([], [])
    
    plt.figure(figsize=(12, 5))
    plt.bar(chars, counts, color='skyblue')
    plt.title(title)
    plt.xlabel('Number')
    plt.ylabel('Frequency')
    plt.show()

def lfsr(seed, taps):
    """
    Linear-feedback shift register implementation based on information found here.
    https://en.wikipedia.org/wiki/Linear-feedback_shift_register
    """
    state = seed
    while True:
        feedback = 0
        # We xor each tap with the state to determine the feedback bit
        for tap in taps:
            set = (state & (1 << (tap - 1))) > 0
            feedback ^= set
        
        # Grab the bit that is going to be discarded
        output_bit = (state >> 15) & 1
        
        # Now we shift everything over in the register and shift in the feedback
        state = ((state << 1)) | feedback

        # Keep everything in 16 bits
        state = state & 0xFFFF

        yield output_bit

def shrinking_generator(seed1, taps1, seed2, taps2, num_bits):
    """
    A shrinking generator implementation based on the information found here
    https://en.wikipedia.org/wiki/Shrinking_generator
    """
    gen1 = lfsr(seed1, taps1)
    gen2 = lfsr(seed2, taps2)
    output_bits = []
    

    while len(output_bits) < num_bits:
        S_bit = next(gen1)
        A_bit = next(gen2)
        if S_bit == 1:
            output_bits.append(A_bit)
    return output_bits

def bits_to_16bit(bits):
    """
    Convert list of bits to a list of 16 bit numbers
    """
    values = []
    for i in range(len(bits)):
        two_bytes_val = 0
        for bit in bits[i:i+16]:
            byte_val = (two_bytes_val << 1) | bit
        values.append(two_bytes_val)
    return values

taps = (15, 14, 13, 8) 
bits = shrinking_generator(0xDEAD, taps, 0xBEEF, taps, 65536)
random_numbers = bits_to_16bit(bits)

counts = collections.Counter(random_numbers)

plot_frequencies(counts, "Random Numbers Generated")

