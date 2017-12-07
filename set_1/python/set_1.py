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


def hex_xor(hex_1, hex_2):
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


def main():
	prob_1_test()
	print("-------------")
	prob_2_test()

if __name__ == "__main__":
	main()

