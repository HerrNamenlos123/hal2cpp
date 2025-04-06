
G0 X0
F3000

#1 = 0
o101 while [#1 LT 10000]
    G1 X10
    G4 P1
    G1 X0
    G4 P1
    #1 = [#1 + 1]
o101 endwhile

M2