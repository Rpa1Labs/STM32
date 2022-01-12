# PROJET STM32

## Présentation du projet
Le but de ce miniprojet est de concevoir à l’aide d’une carte Nucleo STM32 un système embarqué permettant de faire des relevés de températures qui seront transmis via une fréquence porteuse de 433 MHz. Ces données devront être affichées sur n’importe quel smartphone.

## Choix des solutions techniques
Pour utiliser les différentes fonctionnalités, il a été décidé de séparer le projet en deux systèmes. 

Le premier est une partie capteur de température et émission 433 MHz et le second est la réception 433 MHz et affichage smartphone, puisque les smartphones ne peuvent pas recevoir directement le 433 MHz.

Certains choix de solutions techniques ont étés imposés. Il y a la carte STM32 “NUCLEO-F334R8”, le capteur de température “DHT22” ainsi que le module RF récepteur “CDR03A” et le module RF émetteur “CDT-88”.

Pour le premier système, toutes les solutions techniques imposées ont étés utilisés, c’est-à-dire le capteur de température, le module RF et la STM32. 
Le programme est situé dans le dossier STM32.

Pour le second système, puisqu’il est nécessaire d’avoir un système de réception 433 MHz qui puisse aussi communiquer avec un smartphone, la solution finale retenue a été un Raspberry Pi 3B+.
Le programme est situé dans le dossier Raspberry.
