
from des import DES
import binascii

#des - odszyfrowanie = odwrocenie kluczy

# key = bytes(b'\x3b\x38\x98\x37\x15\x20\xf7\x5e')
# data = bytes(b'\x11\x22\x33\x44\x55\x66\x77\x88')
# d = DES(key)
# enc = d.encrypt(data)
# dec = d.decrypt(enc)

# print("key: ", binascii.hexlify(key))
# print("data: ", binascii.hexlify(data))
# print("encrypted: ", binascii.hexlify(enc))
# print("decrypted: ", binascii.hexlify(dec))
# print("data == decrypted", data == dec)

DES.test()


#DES.test()