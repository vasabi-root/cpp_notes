import sys
import os

def main():
    fileName = r'D:\Users\vasab\Documents\CPP\practice\CodeRun\max_cost\\' + 'input.txt'
    with open(fileName, 'r') as f:
        m, n = map(int, f.readline().split())
        m += 1
        n += 1
        # table = [list(map(int, f.readline().split())) for i in range(m)]
        table = [[] for i in range(m)]
        table[0] = [0 for i in range(n)]
        for i in range(1, m):
            table[i] = [0, *list(map(int, f.readline().split()))]
        print(*table, sep='\n')
        
        dp = [[-1 for j in range(n)] for i in range(m)]
        dp[1][1] = table[1][1]
        
        for k in range(1, min(m, n)):
            for j in range(k if k != 1 else k+1, n):
                dp[k][j] = max(dp[k-1][j], dp[k][j-1]) + table[k][j]
            for i in range(k+1, m):
                dp[i][k] = max(dp[i-1][k], dp[i][k-1]) + table[i][k]
        
        way = ''
        i = m-1
        j = n-1        
        while i > 1 or j > 1:
            prev = max(dp[i-1][j], dp[i][j-1])
            if (prev == dp[i-1][j] and i-1 > 0):
                way += 'D '
                i -= 1
            elif j-1 > 0:
                way += 'R '
                j -= 1
        print(*dp, sep='\n')
        print(dp[m-1][n-1])
        way = way[-2::-1]
        
        print(way)
        
if __name__ == '__main__':
    main()

# def main():
#     m, n = map(int, input().split())
#     m += 1
#     n += 1

#     table = [[] for i in range(m)]
#     table[0] = [0 for i in range(n)]
#     for i in range(1, m):
#         table[i] = [0, *list(map(int, input().split()))]
    
#     dp = [[-1 for j in range(n)] for i in range(m)]
#     dp[1][1] = table[1][1]
    
#     for k in range(1, min(m, n)):
#         for j in range(k if k != 1 else k+1, n):
#             dp[k][j] = max(dp[k-1][j], dp[k][j-1]) + table[k][j]
#         for i in range(k+1, m):
#             dp[i][k] = max(dp[i-1][k], dp[i][k-1]) + table[i][k]
    
#     way = ''
#     i = m-1
#     j = n-1        
#     while i > 1 or j > 1:
#         prev = max(dp[i-1][j], dp[i][j-1])
#         if (prev == dp[i-1][j] and i-1 > 0):
#             way += 'D '
#             i -= 1
#         elif j-1 > 0:
#             way += 'R '
#             j -= 1
            
#     print(dp[m-1][n-1])
#     print(way)
        
# if __name__ == '__main__':
#     main()