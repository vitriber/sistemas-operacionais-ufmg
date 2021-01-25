Então vão existir 3 níveis de prioridade:

1 - O primeiro é a prioridade de casal, 
2 - O segundo a prioridade de uma pessoa que está sozinha, mas que estava formando um casal na fila
3 - A prioridade da pessoa que está sozinha e não havia formado casal

DeadLock Raj:

A cada 5 segundos, confere se a fila circular tá cheia e libera um personagem aleatório

Eu guardo o ID de cada personagem, e salvo que ele tá esperando pra usar a fila ou não
Assim que eu sortear um número, eu coloco esperando[rand] = 0
manda um signal em uma variável condicional

Quando ocorre DeadLock:
- Sheldon, Leonard e Howard na Fila
- Amy, Bernadette e Penny na Fila
- Todos os casais na fila

- Fazer lista Aleatória de Execução
a ordem dos eventos acima deve variar entre execuções e implementações. 
Você não deve esperar ter exatamente o mesmo resultado

Checklist de Tarefas - Código:
- Foque no comportamento de um personagem e certifique-se de que ele funciona. - OK
- Depois dispare dois personagens apenas, para evitar que deadlocks aconteçam - OK
- Inclua o Raj e verifique se  deadlocks  são  detectados
- Verifique se os casais funcionam
- O mecanismo  de  prevenção da  inanição  funciona 
 (p.ex.,  use  apenas  dois  casais  de  altere  os  tempos  das 
 outras  atividades para  fazer  com  que  um  deles  (o  de  maior  prioridade)
esteja  sempre  querendo  usar  o  forno.
- Lista aleatória de execução
- Refatoração para uso de .h e .c e makefile

Checklist de Tarefas - Documentação
- Resumo
- Decisões de projeto
- Bugs