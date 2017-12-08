# python 3.6
import base64
import binascii


def hex_to_b64(hex_str):
    decoded = binascii.unhexlify(hex_str)
    print(decoded.decode("utf-8"))
    base64_str = base64.b64encode(decoded)
    return base64_str


def prob_1_test():
    hex_input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"
    base64_str = hex_to_b64(hex_input)
    expected = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"
    if (expected == base64_str.decode("utf-8")):
        print("Problem 1 test passed.")
    else:
        print("Problem 1 test failed!")
        print("Expected: " + expected)
        print("Actual:   " + base64_str.decode("utf-8"))

#----------------------------------------------------------

def hex_xor(hex_1, hex_2):
    """
    returns byte-by-byte XOR of two byte arrays in hex
    """
    if (len(hex_1) != len(hex_2)):
        return "";
    bytes_1 = binascii.unhexlify(hex_1)
    bytes_2 = binascii.unhexlify(hex_2)
    bytes_result = []
    for i in range(0, len(bytes_1)):
        xor_result = bytes_1[i] ^ bytes_2[i]
        bytes_result.append(chr(xor_result))

    result = "".join(bytes_result)
    print(result)
    return binascii.hexlify(bytes(result, "utf-8"))


def prob_2_test():
    input_1 = b"1c0111001f010100061a024b53535009181c"
    input_2 = b"686974207468652062756c6c277320657965"
    expected = b"746865206b696420646f6e277420706c6179"
    actual = hex_xor(input_1, input_2)
    if (actual == expected):
        print("Problem 2 test passed.")
    else:
        print("Problem 2 test failed!")
        print("Expected: " + expected.decode("utf-8"))
        print("Actual:   " + actual.decode("utf-8"))

#----------------------------------------------------------

def xor(byte_arr_1, byte_arr_2):
    length_1 = len(byte_arr_1)
    length_2 = len(byte_arr_2)
    retval = bytearray(length_1)
    if (length_1 != length_2):
        retval = b""
    else:
        for i in range(length_1):
            retval[i] = byte_arr_1[i] ^ byte_arr_2[i]
    return retval


def score(text):
    freq = {}
    freq[' '] = 700000000
    freq['e'] = 390395169
    freq['t'] = 282039486
    freq['a'] = 248362256
    freq['o'] = 235661502
    freq['i'] = 214822972
    freq['n'] = 214319386
    freq['s'] = 196844692
    freq['h'] = 193607737
    freq['r'] = 184990759
    freq['d'] = 134044565
    freq['l'] = 125951672
    freq['u'] = 88219598
    freq['c'] = 79962026
    freq['m'] = 79502870
    freq['f'] = 72967175
    freq['w'] = 69069021
    freq['g'] = 61549736
    freq['y'] = 59010696
    freq['p'] = 55746578
    freq['b'] = 47673928
    freq['v'] = 30476191
    freq['k'] = 22969448
    freq['x'] = 5574077
    freq['j'] = 4507165
    freq['q'] = 3649838
    freq['z'] = 2456495
    score = 0
    for c in text.lower():
        if c in freq:
            score += freq[c]
    return score


def unscramble(cipher):
    max_score = -1
    best_plaintext = None
    key = None
    for i in range(0, 256):
        key_repeated = (chr(i) * len(cipher)).encode("utf-8")
        key_byte_array = bytearray(key_repeated)
        xor_d = xor(key_byte_array, cipher)
        plaintext = bytes(xor_d)
        plaintext = plaintext.decode("utf-8")
        curr_score = score(plaintext)
        if curr_score > max_score or not max_score:
            max_score = curr_score
            best_plaintext = plaintext
            key = chr(i)
    return (key, best_plaintext)


def prob_3_test():
    scrambled = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"
    scrambled_bytes = bytearray.fromhex(scrambled)
    (key, unscrambled) = unscramble(scrambled_bytes)
    print("Key: " + key)
    print("Unscrambled: " + unscrambled)
    expected = "Cooking MC's like a pound of bacon"
    if (unscrambled == expected):
        print("Problem 3 test passed.")

#----------------------------------------------------------


def main():
    prob_1_test()
    print("-------------")
    prob_2_test()
    print("-------------")
    prob_3_test()


if __name__ == "__main__":
    main()

