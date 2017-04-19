
import des

#des - odszyfrowanie = odwrocenie kluczy

key = bytes(b'\x00\x11\x22\x33\x44\x55\x66\x00');
key2 = "alamakota"
des = des.DES(key)

# print(des.__key)