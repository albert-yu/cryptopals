import base64
import binascii
from typing import List, Set, Dict, Tuple, Optional


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


def unscramble(cipher: str, encoding: str) -> Tuple[str, str]:
    """
    Unscrambles the cipher with the given encoding
    and returns the key (a char) and the plain text
    as a tuple
    """
    max_score = -1
    best_plaintext = None
    key = None
    for i in range(0, 256):
        key_repeated = (chr(i) * len(cipher)).encode(encoding)
        key_byte_array = bytearray(key_repeated)
        xor_d = xor(key_byte_array, cipher)
        plaintext = bytes(xor_d)
        plaintext = plaintext.decode(encoding)
        curr_score = score(plaintext)
        if curr_score > max_score or not max_score:
            max_score = curr_score
            best_plaintext = plaintext
            key = chr(i)
    return (key, best_plaintext)


def prob_3_test():
    scrambled = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"
    scrambled_bytes = bytearray.fromhex(scrambled)
    encoding = "utf-8"
    (key, unscrambled) = unscramble(scrambled_bytes, encoding)
    print("Key: " + key)
    print("Unscrambled: " + unscrambled)
    expected = "Cooking MC's like a pound of bacon"
    if (unscrambled == expected):
        print("Problem 3 test passed.")
    else:
        print("Problem 3 test failed!")
        print("Expected: " + expected)
        print("Actual:   " + unscrambled)

#----------------------------------------------------------

def file_string(filename: str):
    """
    Reads the file as a string, stripping newlines
    """
    f = open(filename, "r")
    data = f.read().replace("\n", "") 
    f.close()
    return data


def unscramble_all(filename):
    # store best candidates
    best_plaintext = None
    key = None
    max_score = -1

    f = open(filename, "r")
    for line in f.readlines():
        # remove newline chars
        line = line.replace('\n', '')
        if (len(line) != 60):
            continue
        temp_key = None
        temp_str = None
        # need to operate on byte array
        as_bytes = bytearray.fromhex(line)
        (temp_key, temp_str) = unscramble(as_bytes, "ISO-8859-1")
        curr_score = score(temp_str)
        if (curr_score > max_score):
            max_score = curr_score
            best_plaintext = temp_str
            key = temp_key

    f.close()

    return (key, best_plaintext)


def prob_4_test():
    filename = "../set_1/data/4.txt"
    (key, unscrambled) = unscramble_all(filename)
    print("Key: " + key)
    print("Unscrambled: " + unscrambled)  # newline at end!
    expected = "Now that the party is jumping\n"
    if (unscrambled == expected):
        print("Problem 4 test passed.")
    else:
        print("Problem 4 test failed!")
        print("Expected: " + expected)
        print("Actual:   " + unscrambled)

#----------------------------------------------------------

def hamming_distance(b1: bytes, b2: bytes):
    xord = [a ^ b for (a,b) in zip(b1, b2)] 
    count = 0
    for ch in xord:
        count += bin(ch).count("1")
    return count


def hamming_distance_test():
    s1 = b"this is a test"
    s2 = b"wokka wokka!!!"
    dist = hamming_distance(s1, s2)
    expected = 37
    if dist == expected:
        print("Hamming test passed.")
    else:
        print("Hamming test failed!")
        print("Expected: {0}".format(expected))
        print("Actual: {0}".format(dist))


def best_keysizes(encoded: bytes, num_keysizes=3, min_keysize=2, max_keysize=40) -> List[int]:
    keysize_to_hamming = dict()
    for keysize in range(min_keysize, max_keysize + 1):
        first_n = encoded[:keysize]
        second_n = encoded[keysize:keysize*2]
        hamming = hamming_distance(first_n, second_n)
        normalized = hamming / keysize
        keysize_to_hamming[keysize] = normalized
    
    smallest_to_largest_hamming = dict(sorted(keysize_to_hamming.items(), key=lambda item: item[1]))
    return list(smallest_to_largest_hamming.keys())[:num_keysizes]


def partition(encoded: bytes, partition_size: int) -> List[bytes]:
    result = []
    chunk = bytearray(b'')
    for i in range(len(encoded)):
        if len(chunk) > 0 and i % partition_size == 0:
            result.append(bytes(chunk))
            chunk = bytearray(b'')
        chunk.append(encoded[i])
    
    # append any remaining
    if len(chunk) > 0:
        result.append(chunk)

    return result


def prob_6_test():
    hamming_distance_test()
    filename = "../set_1/data/6.txt"
    b64 = file_string(filename)
    as_bytes = base64.b64decode(b64)
    print(as_bytes[:20])
    print(best_keysizes(as_bytes))
    print(partition(as_bytes, 5)[:4])
    

#----------------------------------------------------------

def main():
    prob_1_test()
    print("-------------")
    prob_2_test()
    print("-------------")
    prob_3_test()
    print("-------------")
    prob_4_test()
    print("-------------")
    prob_6_test()

if __name__ == "__main__":
    main()

