a)
Apenas os processos que não tenham sido tratados é que estão identificados, 
ou seja, só o primeiro processo filho que já tinha dado exit é que aparece como
processo defunct e o segundo processo filho não aparece sequer, apesar de 
já lhe ter sido feito o exit, pois o pais está à espera dele com o waitpid.


b)
Um dos filhos aparece como defunct pois já terminou e o pai não se encontra
à espera de nenhum resultado dele.

c)
killed