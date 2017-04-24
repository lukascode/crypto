from collections import deque
import binascii
 
IP = [

58, 50, 42, 34, 26, 18, 10, 2,
60, 52, 44, 36, 28, 20, 12, 4,
62, 54, 46, 38, 30, 22, 14, 6,
64, 56, 48, 40, 32, 24, 16, 8,
57, 49, 41, 33, 25, 17, 9,  1,
59, 51, 43, 35, 27, 19, 11, 3,
61, 53, 45, 37, 29, 21, 13, 5,
63, 55, 47, 39, 31, 23, 15, 7

] 

IP_FINAL = [

40, 8, 48, 16, 56, 24, 64, 32,
39, 7, 47, 15, 55, 23, 63, 31,
38, 6, 46, 14, 54, 22, 62, 30,
37, 5, 45, 13, 53, 21, 61, 29,
36, 4, 44, 12, 52, 20, 60, 28,
35, 3, 43, 11, 51, 19, 59, 27,
34, 2, 42, 10, 50, 18, 58, 26,
33, 1, 41, 9,  49, 17, 57, 25 

]

EXPANSION = [

32, 1, 2, 3, 4, 5,
4, 5, 6, 7, 8, 9,
8, 9, 10, 11, 12, 13,
12, 13, 14, 15, 16, 17,
16, 17, 18, 19, 20, 21,
20, 21, 22, 23, 24, 25,
24, 25, 26, 27, 28, 29,
28, 29, 30, 31, 32, 1 

]

SBOXES = [

#1
[
    [14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 ],
    [0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8 ],
    [4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0 ],
    [15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 ]
],

#2
[
    [15, 1 ,8 ,14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10],
    [3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5],
    [0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15],
    [13, 8 ,10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9]
],

#3
[
    [10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8],
    [13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1],
    [13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7],
    [1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12] 
],

#4
[
    [7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15],
    [13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9],
    [10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4],
    [3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14]
],

#5
[
    [2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9],
    [14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6],
    [4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14],
    [11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3]
],

#6
[
    [12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11],
    [10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8],
    [9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6],
    [4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13]
],

#7
[
    [4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1],
    [13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6],
    [1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2],
    [6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12]
],

#8
[
    [13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7],
    [1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2],
    [7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8],
    [2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11]
]

]

P_PERMUTATION = [

16, 7, 20, 21,
29, 12, 28, 17,
1, 15, 23, 26,
5, 18, 31, 10,
2, 8, 24, 14,
32, 27, 3, 9,
19, 13, 30, 6,
22, 11, 4, 25 

]

PC_1 = [

57, 49, 41, 33, 25, 17, 9,
1, 58, 50, 42, 34, 26, 18,
10, 2, 59, 51, 43, 35, 27,
19, 11, 3, 60, 52, 44, 36,
63, 55, 47, 39, 31, 23, 15,
7, 62, 54, 46, 38, 30, 22,
14, 6, 61, 53, 45, 37, 29,
21, 13, 5, 28, 20, 12, 4 

]

PC_2 = [

14, 17, 11, 24, 1, 5,
3, 28, 15, 6, 21, 10,
23, 19, 12, 4, 26, 8,
16, 7, 27, 20, 13, 2,
41, 52, 31, 37, 47, 55,
30, 40, 51, 45, 33, 48,
44, 49, 39, 56, 34, 53, 
46, 42, 50, 36, 29, 32 

]


LEFT_SHIFTS = [1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1]


class DES(object):
    def __init__(self, key):
        #key must be 64bit byte type
        if (type(key) is not bytes) and (type(key) is not bytearray) or (len(key) != 8):
            raise ValueError("key should be 8 bytes value")
        self.__key = DES.__getBitsList(key)


    def encrypt(self, data):
        if (type(data) is not bytes) and (type(data) is not bytearray):
            raise ValueError("data should be bytes value type")
        if len(data) == 0:
            return data
        data = DES.__padding(data)
        blocks_64 = [data[x:x+8] for x in range(0, len(data), 8)] 
        result = bytearray()
        for block in blocks_64:
            result += self.__run(block)
        return result

    def decrypt(self, data):
        if (type(data) is not bytes) and (type(data) is not bytearray):
            raise ValueError("data should be bytes value type")
        if len(data) == 0:
            return data
        blocks_64 = [data[x:x+8] for x in range(0, len(data), 8)]
        result = bytearray()
        for block in blocks_64:
            result += self.__run(block, "decrypt")
        result = DES.__unpadding(result)
        return result

    @staticmethod
    def __padding(data):
        left = len(data)%8
        return data + bytes([8-left]*(8-left))

    @staticmethod
    def __unpadding(data):
        l = len(data)
        num = data[l-1]
        return data[:l-num]

    def __run(self, data, action='encrypt'):
        assert action in ('encrypt', 'decrypt')
        data = DES.__getBitsList(data)
        data = DES.__mapFromSource(data, IP) #initial permutation IP
        keys = DES.__generate_keys(self.__key)
        if action == 'decrypt':
            keys = keys[::-1]
        # run 16 rounds
        for key in keys:
            data = DES.__round(data, key)
        data = data[32:]+data[:32]
        data = DES.__mapFromSource(data, IP_FINAL) #final permutation
        return DES.__getBytesFrom(data)

    #one des round
    @staticmethod
    def __round(data, key):
        assert len(data) == 64
        assert len(key) == 48
        left, right = data[:32], data[32:]
        f_result = DES.__F(right, key)
        return right + DES.__xor(left, f_result)


    #left - 28bits, right - 28bits, shift - 1 or 2 (LEFT_SHIFTS)
    @staticmethod
    def __generate_keys(key):
        key = DES.__mapFromSource(key, PC_1) #now this is a 56bit key
        keys = []
        C, D = key[:28], key[28:] 
        for shift in LEFT_SHIFTS:
            c_items, d_items = deque(C), deque(D)
            c_items.rotate(-shift)
            d_items.rotate(-shift)
            C, D = list(c_items), list(d_items)
            keys.append(DES.__mapFromSource(C+D, PC_2))
        return keys


    #F function
    @staticmethod
    def __F(data, key):
        assert len(data) == 32
        data = DES.__mapFromSource(data, EXPANSION) #EXPANSION, 48bits here
        data = DES.__xor(data, key)
        #6bits chunks
        chunks = [data[x:x+6] for x in range(0, len(data), 6)]
        result = []
        #collect 4bits sbox function output
        for i, chunk in enumerate(chunks):
            result += DES.__sbox(chunk, SBOXES[i])
        return DES.__mapFromSource(result, P_PERMUTATION)

    #sbox - 6bits in, 4bits out
    @staticmethod
    def __sbox(data, box):
        assert len(data) == 6
        data_str = ''.join(map(str, data))
        row = int(data_str[0]+data_str[5], 2)
        col = int(data_str[1:5], 2)
        val = box[row][col]
        # return 4 bits sequence
        return [int(i) for i in bin(val)[2:].rjust(4, '0')]

    @staticmethod
    def __xor(a, b):
        assert len(a) == len(b)
        return [ a[i]^b[i] for i in range(0, len(a)) ]

    @staticmethod
    def __getBytesFrom(list):
        chunks = [list[x:x+8] for x in range(0, len(list), 8)]
        ints = []
        for i in chunks:
            b_str = ''.join(map(str, i))
            ints.append(int(b_str, 2))
        return bytes(ints)


    @staticmethod
    def __getBitsList(bytes_):
        bitsstr = DES.__getBinaryStr(bytes_)
        return [int(i) for i in bitsstr]

    @staticmethod
    def __getBinaryStr(bytes_):
        assert (type(bytes_) is bytes) or (type(bytes_) is bytearray)
        result = ''
        for byte in bytes_:
            result += bin(byte)[2:].rjust(8, '0')
        return result


    @staticmethod
    def __mapFromSource(chunk, source):
        result = [0]*len(source)
        for idx, value in enumerate(source):
            result[idx] = chunk[source[idx]-1]
        return result
