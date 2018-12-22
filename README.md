# CFLP

### 问题描述

Suppose there are n facilities and m customers. 

+ We wish to choose:
  + which of the n facilities to open
  + the assignment of customers to facilities
+ The objective is to minimize the sum of the opening cost and the assignment cost.
+  The total demand assigned to a facility must not exceed its capacity

### 解决算法

使用**模拟退火算法**与**局部搜索算法**解决CFLP问题

1. 随机挑选出一单元，并给他一个随机的位移$k$，求出系统因此而产生的能量变化$\Delta E_k$。
2. 若$\Delta E_k \leq0$，该位移可采纳，而变化后的系统状态可作为下次变化的起点；若$\Delta E_k \gt0$，位移后的状态可采纳的概率为

$$
  P_k = \frac{1}{(1+e^{-\Delta E_k/T})}
$$

  式中T为温度，然后从(0,1)区间均匀分布的随机数中挑选一个数R。若$R \lt P_k$，则将变化后的状态作为下次的起点；否则，将变化前的状态作为下次的起点。
​    3. 转第（1）步继续执行，直至达到平衡状态为止。