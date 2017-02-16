import base64
import sys
import requests


cookie = sys.argv[1]
cookie = base64.b64decode(cookie)
cookie_block = [cookie[i:i+16] for i in range(0, len(cookie), 16)]
plaintext = bytearray()

for index, cookie in enumerate(cookie_block[1:]):
	iv = bytearray(16)
	intermediary = bytearray(16)
	for i in range(15, -1, -1):
		code = 500
		while code != 200:
			iv[i] += 1
			guess_cookie = base64.b64encode(iv + cookie)
			resp = requests.get('http://localhost:4555', cookies={'user': guess_cookie})
			code = resp.status_code
			#print code

		intermediary[i] = iv[i] ^ (16-i)
		iv[i] = 0 ^ intermediary[i]

	text = bytearray(len(intermediary))
	for x in range(len(intermediary)):
		text[x] = bytearray(cookie_block[index])[x] ^ intermediary[x]
	plaintext += text

print''.join(chr(x) for x in plaintext)



