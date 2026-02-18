#!/bin/python3
"""
@file prob_2.py 
@author Robert Myers Jr.
@version V1.0
@brief Script used to use show off xor'ing a message to distribute the probability to be more uniform
"""

# used the following to generate text https://www.lipsum.com/feed/html
from typing import Counter
from scipy.stats import chisquare
import random
import matplotlib.pyplot as plt


text = """
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis porttitor massa eleifend, vehicula sem nec, blandit nisi. Donec commodo lacinia nunc, sed sollicitudin dolor tincidunt sed. Praesent ac massa non neque posuere lobortis. Praesent a cursus nibh. Sed id lectus nisl. Morbi feugiat, quam eu mattis ultricies, risus arcu pretium justo, a hendrerit urna tellus sit amet nulla. Nullam non tellus turpis. In pharetra tempus nunc in accumsan. Donec vitae nunc erat. Aliquam lacinia ornare velit, ac pharetra massa eleifend quis. Sed fringilla massa dui, at interdum nisi imperdiet ut. Donec egestas, mauris suscipit rutrum vestibulum, elit metus commodo ipsum, eget finibus sem orci nec ex.
Cras purus urna, suscipit ac nisl id, porttitor ullamcorper justo. Suspendisse vel magna at lacus blandit rhoncus. Nam eros elit, bibendum eu hendrerit convallis, eleifend ut metus. Sed eros nisl, consequat nec vehicula ac, ultrices non ex. Aliquam sed consequat eros, consequat condimentum ante. Sed lacinia mi eget felis condimentum dignissim. Vivamus in aliquet mi. Nam at condimentum orci, ut iaculis mauris.
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum eu ante tempor, efficitur nulla vel, faucibus turpis. Mauris congue ipsum sed purus ultrices varius eu vitae velit. Aenean ac quam dolor. Integer euismod ipsum sit amet lectus sagittis pretium. Cras dignissim leo risus, vitae imperdiet neque efficitur sit amet. Vivamus aliquet enim et nunc volutpat efficitur.
Pellentesque commodo arcu ac purus varius, id porttitor lectus egestas. Sed ultrices neque sollicitudin ex feugiat blandit. Integer fringilla, ligula a blandit facilisis, nunc augue convallis nulla, vitae venenatis ante lorem vel lorem. Sed consectetur fringilla felis nec ornare. Aenean tempor commodo leo, vitae suscipit ligula gravida id. Sed id vestibulum nulla, a luctus felis. Proin porttitor elit ut enim mattis interdum. Donec velit dolor, finibus quis enim sed, iaculis congue erat. Mauris ullamcorper velit orci, at efficitur tortor scelerisque ut.
Nam ac blandit purus. Mauris iaculis tortor ut quam consectetur auctor. Quisque lobortis sit amet neque vitae venenatis. Mauris euismod ultrices elementum. Cras commodo ornare blandit. Aenean eget erat non nisl tempus ultrices id vitae augue. Fusce semper convallis ex, eu fringilla mi efficitur et. In consectetur luctus mauris sed molestie. Donec aliquam volutpat magna, in tincidunt odio varius vel. Mauris eget odio eu turpis blandit cursus.
""".lower()


def plot_frequencies(frequencies, title):
    sorted_items = sorted(frequencies.items())
    chars, counts = zip(*sorted_items) if sorted_items else ([], [])
    
    plt.figure(figsize=(12, 5))
    plt.bar(chars, counts, color='skyblue')
    plt.title(title)
    plt.xlabel('Character')
    plt.ylabel('Frequency')
    plt.show()

def print_top_ten(frequency_of_characters):
    top_ten = frequency_of_characters.most_common(10)
    print("Top ten characters and frequencies")
    for character_frequency in top_ten:
        print(f"'{character_frequency[0]}' : {character_frequency[1]}")


frequency_of_characters_of_plain_text = Counter(text)
print_top_ten(frequency_of_characters_of_plain_text)


xor_string = "".join(chr(ord(char_in_text) ^ random.randint(0, 255)) for char_in_text in text)

frequency_of_characters_of_xor_text = Counter(xor_string)
print_top_ten(frequency_of_characters_of_xor_text)

plot_frequencies(frequency_of_characters_of_plain_text, "Plain Text Distrubtion")
plot_frequencies(frequency_of_characters_of_xor_text, "XOR Distrubtion")
