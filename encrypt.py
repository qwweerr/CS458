import base64
import sys
import requests


plaintext = sys.argv[1]

if len(bytearray(plaintext)) % 16 != 0:
	need = 16 - len(bytearray(plaintext)) % 16
	padding = [None] * need
	padding[0] = need
	for i in range(1,need,1):
		padding[i] = 0
	padding = bytearray(padding)
	plaintext_block = bytearray(len(bytearray(plaintext)) + len(padding))
	plaintext_block = bytearray(plaintext) + padding


plaintext_block = [bytearray(plaintext_block[i:i+16]) for i in range(0, len(plaintext_block), 16)]
cookies = bytearray(16 * (len(plaintext_block)+1))
cookie_block = [cookies[i:i+16] for i in range(0, len(cookies), 16)]


for index in range(len(plaintext_block)-1, -1, -1):
	iv = bytearray(16)
	intermediary = bytearray(16)
	for i in range(15, -1, -1):
		code = 500
		while code != 200:
			iv[i] += 1
			guess_cookie = base64.b64encode(iv + cookie_block[index+1])
			resp = requests.get('http://localhost:4555', cookies={'user': guess_cookie})
			code = resp.status_code

		intermediary[i] = iv[i] ^ (16-i)
		iv[i] = 0 ^ intermediary[i]
	text = bytearray(len(intermediary))
	for x in range(len(intermediary)):
		text[x] = plaintext_block[index][x] ^ intermediary[x]
	cookie_block[index] = text
# cyphertext = list(reversed(cyphertext))


encode = base64.b64encode(''.join([chr(x) for c in cookie_block for x in c]))

print encode

sys.exit(1)