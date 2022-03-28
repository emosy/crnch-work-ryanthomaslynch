#diff of multiple cc files

import os


list = os.listdir("src/")
# print(list)

for i in list:
    os.system("diff -w src/umd-memsys.cc src/" + i + " >> diffs/" + i + ".diff")

os.system("find diffs/ -empty -type f -delete")