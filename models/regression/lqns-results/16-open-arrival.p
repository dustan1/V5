# lqns 4.5
# lqns -p 16-open-arrival.in
V y
C 1.80739e-06
I 7
PP 1
NP 1
#!User:  0:00:00.00
#!Sys:   0:00:00.00
#!Real:  0:00:00.00
#!Solver:   14   0   0        100     7.1429     2.1665       3848     274.86     257.72  0:00:00.00  0:00:00.00  0:00:00.00 
B 2
t1          : e1          0.5         
t2          : e2          1           
              e3          1           
 -1

W 1
t1          : e1          e2          0.666667     -1 
 -1
 -1


X 3
t1          : e1          3.46665      -1 
 -1
t2          : e2          1            -1 
              e3          1            -1 
 -1
 -1


VAR 3
t1          : e1          15.4399      -1 
 -1
t2          : e2          1            -1 
              e3          1            -1 
 -1
 -1

FQ 2
t1          : e1          0.288463    1            -1 1           
 -1
t2          : e2          0.288463    0.288463     -1 0.288463    
              e3          0.4         0.4          -1 0.4         
 -1
                          0.688463    0.688463     -1 0.688463    
 -1



R 1
t2          e3          0.4         2.46795     
 -1

P p1 2
t1          1 0   1   e1          0.288463    0.799984     -1 
 -1
t2          2 0   1   e2          0.288463    0            -1 
                      e3          0.4         0            -1 
 -1
                                  0.688463    
 -1
                                0.976926    
 -1
 -1

