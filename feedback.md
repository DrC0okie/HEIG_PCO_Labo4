Etudiants :
Mangold Aubry
Van Hove Timothée


Conception                  :  1.56 / 1.75
Exécution et fonctionnement :  0.75 / 0.75
Codage                      :  0.75 / 0.75
Documentation               :  1.25 / 1.25
Commentaires et entêtes     :   0.5 / 0.5
Bonus / Malus               :   0.0 / 5.0

Note finale                 : 5.8


Remarque assistant: 
Rapport : 
Ok pour le rapport.

Par contre pour les tests, vous donnez divers parcours, pourquoi pas, mais vous ne validez pas les tests basiques ou bien les cas limites.
Ce qui est attendu pour les tests est de montrer que certains comportements spécifiques sont validés. 
Par exemple : 
- Attente de 5 secondes en gare → ok
- LocoA arrive en gare puis locoB arrive, attente, locoB prioritaire → ok
- LocoAen attente d’entrer dans la section critique, arrêt d’urgence demandé, locoB passe le capteur de sortie de la section critique, les locos sont correctement arrêtées → ok
- Loco A et B en gare, locoA effectue tout le parcours, locoB est arrêtée et démarre son comportement une fois que locoA est en gare, pas de soucis/décalage → ok
Si vous proposez 5-6 (ou plus si besoin) tests de ce genre, c’est suffisant, mais l’objectif est de montrer que votre code est robuste dans diverses situations.



Code :
Concernant le code, je comprends votre objectif, mais du coup il va un peu à l’encontre de l’énoncé. Lorsque l’on demande que la loco qui arrive en dernier à la gare qui a la priorité sur le section critique, si vous avez 2 sections critiques, ça rend le comportement un peu étrange si on commence à arrêter et redémarrer les loco.

En fait c’est dommage parce que je comprends globalement ce que vous avez voulu faire, mais vous vous éloignez du cahier des charges.

Ceci dit, concernant la synchro et la gestion de la concurrence, pas grand chose à redire, vous avez bien compris les concepts.