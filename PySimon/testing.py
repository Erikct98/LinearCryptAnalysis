for i in range(-32, 0):
    print(f'{i}|'.rjust(5), end=' ') 
    for j in range(-1, 11):
        print(f'{i ^ j}'.rjust(3), end=" ") 
    print()
print('-' * 50)
print('      ', end='')
for j in range(-1, 11):
    print( f'{j}'.rjust(3), end=" ") 
 