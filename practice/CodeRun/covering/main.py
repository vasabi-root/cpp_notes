def check_l(num, target):
    return num < target

def bin_search(l: int, r: int, points: [], k: int):
    while l < r:
        m = (l+r) // 2
        if check_coverage(m, points, k):
            r = m
        else:
            l = m+1
    return l

def check_coverage(l: int, points: [], k: int):
    last = 0
    for seg_i in range(k):
        for i, p in enumerate(points[last:]):
            if p - points[last] > l:
                last += i
                break
        else: # если не брэйкнулись на последней итерации
            if seg_i == k-1:
                return True # то покрыли все точки
    return False # иначе не покрыли

def coverage_bin_search(l: int, r: int, points: [], k: int):
    while l < r:
        m = (l+r) // 2
        
        flag = 0        
        last = 0
        for seg_i in range(k):
            for i, p in enumerate(points[last:]):
                if p - points[last] > m:
                    last += i
                    break
            else: # если не брэйкнулись на последней итерации
                if seg_i == k-1:
                    flag = True
                    break
            flag = False
            
        if flag:
            r = m
        else:
            l = m+1
    return l
                

def main():
    n, k = map(int, input().split())
    points = list(map(int, input().split()))
    # if k >= len(points):
    #     print(0)
    #     return 0

    points = sorted(points)

    maxl = round((points[-1] - points[0]) / k)
    
    # answer = bin_search(0, maxl, points=points, k=k)
    answer = coverage_bin_search(0, maxl, points=points, k=k)
    print(answer)
    return answer


if __name__ == '__main__':
    main()
    