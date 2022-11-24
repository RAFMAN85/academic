import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
from scipy.stats import binom

n=10
p=0.5
val=[5,10,100,1000,10000]

def Bern_gen(p):
    r=np.random.uniform(0.0,1.0)
    if(r<p):
        return 1
    else:
        return 0

def Binom_gen(n,p):
    s=0
    for i in range(0,n):
        s+=Bern_gen(p)
    return s #число которое имеет Bi

def quant_search(a1,a2):
    N=len(a2)
    K=int(a1 * (N-1))
    if K+1 < a1 * N:
        return a2[K+1]
    elif K+1 == a1*N:
        return (a2[K]+a2[K + 1]) / 2
    else:
        return a2[K]

edge = (1, 10)
edge_ran = range(edge[0], edge[1])
for i in [5, 10, 100, 1000, 10000]:
    f = []
    for letter in 'qwert':
        f.append(list(map(lambda x:float(x),open('C:/Users/Asus/Desktop/'+str(i)+letter+'.txt').read().split())))
    print('Длина выборки: ' + str(i))
    if i<=10:
        print(f)
    for k in range(5):
        f[k].sort()
    print('Вариационный ряд первого экземпляра выборки:\n', f[0])

    # Поиск ЭФР и ЭФВ
    imper_var_funcs=[]
    imper_distr_funcs=[]
    for k in range(5):
        imper_var_f= {}
        imper_distr_f={}
        for j in range(edge[0], edge[1]):
            imper_var_f[j] = list(map(lambda x:int(x>=j and x<j+1),f[k])).count(1) / i
            imper_distr_f[j] = round(imper_distr_f.get(j-1,0)+imper_var_f[j], [5,10,100,1000,10000].index(i)+1)
        imper_var_funcs.append(imper_var_f)
        imper_distr_funcs.append(imper_distr_f)

    # поиск точных верхних границ разности ЭФР
    max_divergation = np.zeros((6,6))
    max_divergation[:,0]=range(6)
    max_divergation[0,:]=range(6)
    for j in range(5):
        for k in range(5):
            if j != k:
                max_divergation[j+1,k+1]=max(list(map(lambda l:abs(round(imper_distr_funcs[j].get(l,0.)-imper_distr_funcs[k].get(l,0), [5,10,100,1000,10000].index(i)+1)),range(edge[0], edge[1]))))
                max_divergation[k+1,j+1]=max_divergation[j+1,k+1]
    print('ЭФР:')
    for j in range(5):
        print(imper_distr_funcs[j],end='\n')
    print('Максимальные разницы между ЭФР:')
    for j in range(6):
        for k in range(6):
            if j==0 and k==0:
                print(' ',end='\t')
            else:
                print(max_divergation[j,k],end='\t')
        print()

    # поиск квантилей
    print('Квантиль для 0.1: ',quant_search(0.1,f[0]))
    print('Квантиль для 0.5: ',quant_search(0.5,f[0]))
    print('Квантиль для 0.7: ',quant_search(0.7,f[0]))
    # графики
    fig, dist_plt = plt.subplots(nrows=1, ncols=1, figsize=(10, 10))
    dp1 = dist_plt.plot(edge_ran, list(map(lambda x: imper_distr_funcs[0].get(x-1, 0), edge_ran)), color='red',label='ЭФР первой выборки')
    dp2 = dist_plt.plot(edge_ran, list(map(lambda x: imper_distr_funcs[1].get(x-1, 0), edge_ran)), color='blue',label='ЭФР второй выборки')
    dp3 = dist_plt.plot(edge_ran, list(map(lambda x: imper_distr_funcs[2].get(x-1, 0), edge_ran)), color='brown',label='ЭФР третьей выборки')
    dp4 = dist_plt.plot(edge_ran, list(map(lambda x: imper_distr_funcs[3].get(x-1, 0), edge_ran)), color='green',label='ЭФР четвертой выборки')
    dp5 = dist_plt.plot(edge_ran, list(map(lambda x: imper_distr_funcs[4].get(x-1, 0), edge_ran)), color='orange',label='ЭФР пятой выборки')
    dist_plt.set_title('Эмпирические функции распределения')
    dist_plt.legend()
    plt.show()
    fig.savefig('C:/Users/Asus/Desktop/' + str(i) + 'func.jpg', format='jpg')

    fig, freqHist = plt.subplots(nrows=1,ncols=1,figsize=(10, 10))
    s1 = freqHist.bar(edge_ran, list(map(lambda x:imper_var_funcs[0][x], edge_ran)), color=colors.to_rgba('blue', 0.5),label="Emperical")
    s2 = freqHist.bar(edge_ran, list(map(lambda x:binom.pmf(x, n, p), edge_ran)),color=colors.to_rgba('red', 0.5),label='Theoretical')
    freqHist.set_title('Гистограмма частот первой выборки')
    freqHist.legend()
    plt.show()
    fig.savefig('C:/Users/Asus/Desktop/'+str(i)+'gist.jpg',format='jpg')

    fig, freqPoly = plt.subplots(nrows=1,ncols=1,figsize=(10, 10))
    p1 = freqPoly.plot(edge_ran,list(map(lambda x:imper_var_funcs[0][x],edge_ran)),label='Emperical',color='red')
    p2 = freqPoly.plot(edge_ran,list(map(lambda x:binom.pmf(x, n, p),edge_ran)),label='Theoreticl',color='black')
    freqPoly.set_title('Полигон частот для первой выборки')
    freqPoly.set_ylim(ymin=0)
    freqPoly.legend()
    plt.show()
    fig.savefig('C:/Users/Asus/Desktop/'+str(i)+'pol.jpg',format='jpg')



