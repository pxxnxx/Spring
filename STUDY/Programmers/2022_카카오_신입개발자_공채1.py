def solution(id_list, report, k):
    answer = [0] * len(id_list)
    rpt = [set() for _ in range(len(id_list))]
    for x in report:
        y = x.split()
        y[0] = search(id_list,y[0])
        y[1] = search(id_list,y[1])
        rpt[y[1]].add(y[0])
    for x in rpt:
        if len(x) >= 2:
            for y in x:
                answer[y] += 1
    print(answer)
    return answer

def search(arr, member):
    for i in range(len(arr)):
        if arr[i] == member:
            return i
    
solution(["muzi","frodo","apeach","neo"],["muzi frodo","apeach frodo","frodo neo","muzi neo","apeach muzi"],2)
