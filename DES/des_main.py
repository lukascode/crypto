
from des import DES
from optparse import OptionParser
import binascii
import sys

def main():
    parser = OptionParser()
    parser.add_option("-e", "--encrypt", action="store_true", dest="encrypt", default=False)
    parser.add_option("-d", "--decrypt", action="store_true", dest="decrypt", default=False)
    parser.add_option("-k", "--key", action="store", type="string", dest="key", default="3b3898371520f75e")
    parser.add_option("--no-hex", action="store_false", dest="keyhex", default=True)
    options, args = parser.parse_args()

    if parser.values.keyhex == True:
        key = bytearray.fromhex(parser.values.key)
    else:
        key = str.encode(parser.values.key)

    des = DES(key)
    if parser.values.decrypt == True:
        action = des.decrypt
    else:
        action = des.encrypt

    # or read eg 64bytes and write 72 (encryption)
    data = sys.stdin.buffer.read()
    sys.stdout.buffer.write(action(data))

main()