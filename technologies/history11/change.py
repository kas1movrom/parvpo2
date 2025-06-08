import sys
import string

from random import choice


def generate_rnd_str(num):
    return ''.join([choice(string.ascii_letters) for _ in range(num)] + ['\n'])


for i in range(int(sys.argv[1]), int(sys.argv[2])):
    with open(str(i) + 'file.txt', 'a', encoding='utf-8') as file:
        file.write(generate_rnd_str(10))