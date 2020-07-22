import os
import csv

headers = ["Instruction Counter", "Cache Misses", "Branch Misses", "Cache Reads", "Cache Writes", "LL1 data cache", "LL1 Instruction Cache", "Last Level Cache", "Data TLB", "Instruction Cache"]

l_1 = []
l_2 = []
l_3 = []
l_4 = []
l_5 = []
l_6 = []
l_7 = []
l_8 = []
l_9 = []
l_10 = []

# change to directory results are saved in
directory = '/Users/humzz/PycharmProjects/Perf'

for file in os.listdir(directory):
    if file.endswith(".txt"):
        with open("output1.txt", "r") as in_file:
            in_file.readline() # get rid of data collection line
            
            for line in in_file:
                stripped_line = line.strip()
                if stripped_line == 'HW Instruction Count:':
                    # Instruction count
                    for i in range(4):
                        reading = in_file.readline()
                        words = reading.split(' ')
                        l_1.append(int(words[2]))

                elif stripped_line == 'Hardware misses:':
                    # cache misses
                    for i in range(4):
                        reading = in_file.readline()
                        words = reading.split(' ')
                        l_2.append(int(words[1]))
                    
                    in_file.readline() # get the new line

                    # branch misses
                    for i in range(4):
                        reading = in_file.readline()
                        words = reading.split(' ')
                        l_3.append(int(words[1]))

                elif stripped_line == 'Cache Read/Write:':
                    # cache misses
                    for i in range(4):
                        reading = in_file.readline()
                        words = reading.split(' ')
                        l_4.append(int(words[1]))

                    in_file.readline() # get the new line

                    # branch misses
                    for i in range(4):
                        reading = in_file.readline()
                        words = reading.split(' ')
                        l_5.append(int(words[1]))

                elif stripped_line == 'Level 1 cache & TLB:':
                    # Level 1 Data Cache
                    for i in range(4):
                        reading = in_file.readline()
                        words = reading.split(' ')
                        l_6.append(int(words[1]))

                    in_file.readline() # get the new line

                    # Level 1 Instruction Cache
                    for i in range(4):
                        reading = in_file.readline()
                        words = reading.split(' ')
                        l_7.append(int(words[1]))

                    in_file.readline() # get the new line

                    # Last Level Cache
                    for i in range(4):
                        reading = in_file.readline()
                        words = reading.split(' ')
                        l_8.append(int(words[1]))

                    in_file.readline() # get the new line

                    # Data TLB
                    for i in range(4):
                        reading = in_file.readline()
                        words = reading.split(' ')
                        l_9.append(int(words[1]))

                    in_file.readline() # get the new line

                    # Instruction TLB
                    for i in range(4):
                        reading = in_file.readline()
                        words = reading.split(' ')
                        l_10.append(int(words[1]))

with open("array2.txt", "w") as out_file:
    cnt = 0
    
    #
    out_file.write(headers[0])
    out_file.write('\n')
    out_file.write('[')
    for item in l_1:
        if (cnt <= 1001):
            out_file.write(str(item))
            out_file.write(', ')
        else:
            out_file.write(str(item))
        
        cnt = cnt + 1
    out_file.write(']\n\n')
    cnt = 0

    #
    out_file.write(headers[1])
    out_file.write('\n')
    out_file.write('[')
    for item in l_2:
        if (cnt <= 1001):
            out_file.write(str(item))
            out_file.write(', ')
        else:
            out_file.write(str(item))
        
        cnt = cnt + 1
    out_file.write(']\n\n')
    cnt = 0

    out_file.write(headers[2])
    out_file.write('\n')
    out_file.write('[')
    for item in l_3:
        if (cnt <= 1001):
            out_file.write(str(item))
            out_file.write(', ')
        else:
            out_file.write(str(item))
        
        cnt = cnt + 1
    out_file.write(']\n\n')
    cnt = 0

    out_file.write(headers[3])
    out_file.write('\n')
    out_file.write('[')
    for item in l_4:
        if (cnt <= 1001):
            out_file.write(str(item))
            out_file.write(', ')
        else:
            out_file.write(str(item))
        
        cnt = cnt + 1
    out_file.write(']\n\n')
    cnt = 0

    out_file.write(headers[4])
    out_file.write('\n')
    out_file.write('[')
    for item in l_5:
        if (cnt <= 1001):
            out_file.write(str(item))
            out_file.write(', ')
        else:
            out_file.write(str(item))
        
        cnt = cnt + 1
    out_file.write(']\n\n')
    cnt = 0

    out_file.write(headers[5])
    out_file.write('\n')
    out_file.write('[')
    for item in l_6:
        if (cnt <= 1001):
            out_file.write(str(item))
            out_file.write(', ')
        else:
            out_file.write(str(item))
        
        cnt = cnt + 1
    out_file.write(']\n\n')
    cnt = 0

    out_file.write(headers[6])
    out_file.write('\n')
    out_file.write('[')
    for item in l_7:
        if (cnt <= 1001):
            out_file.write(str(item))
            out_file.write(', ')
        else:
            out_file.write(str(item))
        
        cnt = cnt + 1
    out_file.write(']\n\n')
    cnt = 0

    out_file.write(headers[7])
    out_file.write('\n')
    out_file.write('[')
    for item in l_8:
        if (cnt <= 1001):
            out_file.write(str(item))
            out_file.write(', ')
        else:
            out_file.write(str(item))
        
        cnt = cnt + 1
    out_file.write(']\n\n')
    cnt = 0

    out_file.write(headers[8])
    out_file.write('\n')
    out_file.write('[')
    for item in l_9:
        if (cnt <= 1001):
            out_file.write(str(item))
            out_file.write(', ')
        else:
            out_file.write(str(item))
        
        cnt = cnt + 1
    out_file.write(']\n\n')
    cnt = 0

    out_file.write(headers[9])
    out_file.write('\n')
    out_file.write('[')
    for item in l_10:
        if (cnt <= 1001):
            out_file.write(str(item))
            out_file.write(', ')
        else:
            out_file.write(str(item))
        
        cnt = cnt + 1
    out_file.write(']\n\n')
    cnt = 0

print("done")