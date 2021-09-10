import re
import copy
def solution(expression):
    answer = 0
    oper = list()
    ans = list()
    for i in range(len(expression)):
        if not 48 <= ord(expression[i]) <= 57:
            oper.append(expression[i])
    expression = re.split("[-*+]",expression)        
    for i in range(len(expression)):
        expression[i] = int(expression[i])
    ans.append(stop(expression,oper,1))
    ans.append(stop(expression,oper,2))
    ans.append(stop(expression,oper,3))
    ans.append(stop(expression,oper,4))
    ans.append(stop(expression,oper,5))
    ans.append(stop(expression,oper,6))
    answer = max(abs(max(ans)),abs(min(ans)))
    return answer
def stop(arr,oper,key):
    if key == 1:
        arr, oper = mul(arr,oper)
        arr, oper = add(arr,oper)
        arr, oper = sub(arr,oper)
    elif key == 2:
        arr, oper = mul(arr,oper)
        arr, oper = sub(arr,oper)
        arr, oper = add(arr,oper)
    elif key == 3:
        arr, oper = add(arr,oper)
        arr, oper = mul(arr,oper)
        arr, oper = sub(arr,oper)
    elif key == 4:
        arr, oper = add(arr,oper)
        arr, oper = sub(arr,oper)
        arr, oper = mul(arr,oper)
    elif key == 5:
        arr, oper = sub(arr,oper)
        arr, oper = add(arr,oper)
        arr, oper = mul(arr,oper)
    elif key == 6:
        arr, oper = sub(arr,oper)
        arr, oper = mul(arr,oper)
        arr, oper = add(arr,oper)
    return arr[0]
    

def mul(arr,oper):
    oprd = copy.deepcopy(arr)
    op = copy.deepcopy(oper)
    i = 0
    while i <= len(op)-1:
        if op[i] == '*':
            oprd[i] = oprd[i] * oprd[i+1]
            del oprd[i+1]
            del op[i]
        else:
            i+=1
    return oprd,op

def add(arr,oper):
    oprd = copy.deepcopy(arr)
    op = copy.deepcopy(oper)
    i = 0
    while i <= len(op)-1:
        if op[i] == '+':
            oprd[i] = oprd[i] + oprd[i+1]
            del oprd[i+1]
            del op[i]
        else:
            i+=1
    return oprd,op

def sub(arr,oper):
    oprd = copy.deepcopy(arr)
    op = copy.deepcopy(oper)
    i = 0
    while i <= len(op)-1:
        if op[i] == '-':
            oprd[i] = oprd[i] - oprd[i+1]
            del oprd[i+1]
            del op[i]
        else:
            i+=1
    return oprd,op
            
    
