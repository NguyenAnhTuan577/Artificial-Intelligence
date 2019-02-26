female(queen_elizabeth).
female(princess_diana).
female(camilla_parker_bowles).
female(princess_anne).
female(sarah_ferguson).
female(sophie_rhys-jones).
female(kate_middleton).
female(autumn_kelly).
female(zara_phillips).
female(princess_beatrice).
female(princess_eugenie).
female(lady_louise_Mountbatten-windsor).
female(princess_charlotte).
female(savannah_phillips).
female(isla_phillips).
female(mia_grace_tindall).

male(prince_philip).
male(prince_charles).
male(captain_mark_phillips).
male(timothy_laurence).
male(prince_andrew).
male(prince_edward).
male(prince_william).
male(prince_harry).
male(peter_phillips).
male(mike_tindall).
male(james_Viscount_severn).
male(prince_george).

married(queen_elizabeth,prince_philip).
married(prince_philip,queen_elizabeth).
married(camilla_parker_bowles,prince_charles).
married(prince_charles,camilla_parker_bowles).
married(princess_anne,timothy_laurence).
married(timothy_laurence,princess_anne).
married(sophie_rhys-jones,prince_edward).
married(prince_edward,sophie_rhys-jones).
married(kate_middleton,prince_william).
married(prince_william,kate_middleton).
married(autumn_kelly,peter_phillips).
married(peter_phillips,autumn_kelly).
married(zara_phillips,mike_tindall).
married(mike_tindall,zara_phillips).

divorced(sarah_ferguson,prince_andrew).
divorced(prince_andrew,sarah_ferguson).
divorced(princess_diana,prince_charles).
divorced(prince_charles,princess_diana).
divorced(princess_anne,captain_mark_phillips).
divorced(captain_mark_phillips,princess_anne).

parent(queen_elizabeth,prince_charles).
parent(prince_philip,prince_charles).
parent(queen_elizabeth,princess_anne).
parent(prince_philip,princess_anne).
parent(queen_elizabeth,prince_andrew).
parent(prince_philip,prince_andrew).
parent(queen_elizabeth,prince_edward).
parent(prince_philip,prince_edward).
parent(princess_diana,prince_william).
parent(prince_charles,prince_william).
parent(princess_diana,prince_harry).
parent(prince_charles,prince_harry).
parent(captain_mark_phillips,peter_phillips).
parent(princess_anne,peter_phillips).
parent(captain_mark_phillips,zara_phillips).
parent(princess_anne,zara_phillips).
parent(sarah_ferguson,princess_beatrice).
parent(prince_andrew,princess_beatrice).
parent(sarah_ferguson,princess_eugenie).
parent(prince_andrew,princess_eugenie).
parent(sophie_rhys-jones,james_Viscount_severn).
parent(prince_edward,james_Viscount_severn).
parent(sophie_rhys-jones,lady_louise_Mountbatten-windsor).
parent(prince_edward,lady_louise_Mountbatten-windsor).
parent(prince_william,prince_george).
parent(kate_midleton,prince_george).
parent(prince_william,princess_charlotte).
parent(kate_midleton,princess_charlotte).
parent(autumn_kelly,savannah_phillips).
parent(peter_phillips,savannah_phillips).
parent(autumn_kelly,isla_phillips).
parent(peter_phillips,isla_phillips).
parent(zara_phillips,mia_grace_tindall).
parent(mike_tindall,mia_grace_tindall).

husband(X,Y) :- married(Y,X),male(X),female(Y).
wife(X,Y):- married(X,Y),male(Y),female(X).
father(X,Y):- parent(X,Y),male(X).
mother(X,Y):-parent(X,Y),female(X).
child(X,Y):-parent(Y,X).
son(X,Y):-child(X,Y),male(X).
daughter(X,Y):-child(X,Y),female(X).
grandparent(X,Y):-parent(X,Z),parent(Z,Y).
grandmother(X,Y):-grandparent(X,Y),female(X).
grandfather(X,Y):-grandparent(X,Y),male(X).
grandchild(X,Y):-parent(Z,X),parent(Y,Z).
grandson(X,Y):-grandchild(X,Y),male(X).
granddaughter(X,Y):-grandchild(X,Y),female(X).
sibling(X,Y):-mother(Z,X),father(T,X),mother(Z,Y),father(T,Y),X\=Y.
brother(X,Y):-sibling(X,Y),male(X).
sister(X,Y):-sibling(X,Y),female(X).
aunt(X,Y):-sister(X,Z),parent(Z,Y).
aunt(X,Y):-wife(X,Z),sibling(Z,T),parent(T,Y).
uncle(X,Y):-brother(X,Z),parent(Z,Y).
uncle(X,Y):-husband(X,Z),sibling(Z,T),parent(T,Y).
niece(X,Y):-aunt(Y,X),female(X).
niece(X,Y):-uncle(Y,X),female(X).
nephew(X,Y):-aunt(Y,X),male(X).
nephew(X,Y):-uncle(Y,X),male(X).
divorced(X,prince_charles).