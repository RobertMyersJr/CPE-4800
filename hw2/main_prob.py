#!/bin/python3
"""
@file main_prob.py 
@author Robert Myers Jr.
@version V1.0
@brief Script used to use probability to decrpyted a shift cipher
"""

from typing import Counter


encrpyted_message = """Puv. Udfkho Obqgh olyhg mxvw zkhuh wkh Dyrqohd pdlq urdg glsshg grzq lqwr d olwwoh kroorz, iulqjhg zlwk doghuv dqg odglhv' hdugursv dqg wudyhuvhg eb d eurrn wkdw kdg lwv vrxufh dzdb edfn lq wkh zrrgv ri wkh rog Fxwkehuw sodfh; lw zdv uhsxwhg wr eh dq lqwulfdwh, khdgorqj eurrn lq lwv hduolhu frxuvh wkurxjk wkrvh zrrgv, zlwk gdun vhfuhwv ri srro dqg fdvfdgh; exw eb wkh wlph lw uhdfkhg Obqgh'v Kroorz lw zdv d txlhw, zhoo-frqgxfwhg olwwoh vwuhdp, iru qrw hyhq d eurrn frxog uxq sdvw Puv. Udfkho Obqgh'v grru zlwkrxw gxh uhjdug iru ghfhqfb dqg ghfruxp; lw suredeob zdv frqvflrxv wkdw Puv. Udfkho zdv vlwwlqj dw khu zlqgrz, nhhslqj d vkdus hbh rq hyhubwklqj wkdw sdvvhg, iurp eurrnv dqg fkloguhq xs, dqg wkdw li vkh qrwlfhg dqbwklqj rgg ru rxw ri sodfh vkh zrxog qhyhu uhvw xqwlo vkh kdg ihuuhwhg rxw wkh zkbv dqg zkhuhiruhv wkhuhri.
"""

decrpyted_message = """Mrs. Rachel Lynde lived just where the Avonlea main road dipped down into a little hollow, fringed with alders and ladies' eardrops and traversed by a brook that had its source away back in the woods of the old Cuthbert place; it was reputed to be an intricate, headlong brook in its earlier course through those woods, with dark secrets of pool and cascade; but by the time it reached Lynde's Hollow it was a quiet, well-conducted little stream, for not even a brook could run past Mrs. Rachel Lynde's door without due regard for decency and decorum; it probably was conscious that Mrs. Rachel was sitting at her window, keeping a sharp eye on everything that passed, from brooks and children up, and that if she noticed anything odd or out of place she would never rest until she had ferreted out the whys and wherefores thereof.
"""
list_of_characters_to_omit = [",", ".", " ", "\n", ";","'", "-"]

def character_frequency(message: str) -> dict[str, int]:
    # Code was derived from here.
    # https://www.geeksforgeeks.org/python/python-frequency-of-each-character-in-string/
    frequency_of_characters = Counter(message.lower())
    
    for char in list_of_characters_to_omit:
        frequency_of_characters.pop(char)

    
    return frequency_of_characters

def main():
    encrpyted_message_lower = encrpyted_message.lower()
    decrpyted_message_lower = decrpyted_message.lower()
    char_freqs = character_frequency(encrpyted_message_lower)


    largest_key = max(char_freqs, key=char_freqs.get)

    sorted_letters_by_frequency = ["e", "t", "a", "o", "i", "n", "s", 
                                   "h", "r", "d", "l", "c", "u", "m",
                                   "w", "f", "g", "y", "p", "b", "v",
                                   "k", "j", "x", "q", "z"]

    for shift_char in sorted_letters_by_frequency :
        print(f"Shifting more frequent letter to {shift_char}")

        distance = abs(ord(largest_key) - ord(shift_char))
        print(f"Shifting everything to {distance} to the left")

        result = ""
        for char in encrpyted_message_lower:
            if(char in list_of_characters_to_omit):
                result += char;
                continue
            ## Use ord to get the unicode and use the unicode to do shifting
            offset = ord('a')
            new_pos = (ord(char) - offset - distance) % 26
            result += chr(new_pos + offset)
        if(result == decrpyted_message_lower):
            print(result + "\n")
            exit(0)
    print("failed to decrpyt message")

        

if __name__ == "__main__":
    main()
