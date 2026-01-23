# Paletizer-control-on-STM32
# 🏗️ Palettiseur Industriel Temps Réel (FreeRTOS)

![Platform](https://img.shields.io/badge/Platform-STM32F072-blue.svg)
![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS-green.svg)

Ce projet consiste à concevoir et implémenter le contrôle d'un palettiseur industriel automatisé sous **FreeRTOS**. L'objectif est de piloter une scène complexe dans **Factory I/O** via une liaison série (UART), tout en optimisant la réactivité et la consommation énergétique du microcontrôleur STM32.

---

## 📺 Démonstration Vidéo

https://github.com/user-attachments/assets/a4fbecfc-b240-44e8-9584-d9e2b4afbc21


---

## 🎯 Objectifs du Projet
* **Multitâche :** Décomposition du système en 6 tâches parallèles indépendantes.
* **Synchronisation :** Usage de sémaphores, files (Queues) et notifications de tâches.
* **Optimisation :** Réduction drastique de la charge CPU et de la consommation énergétique.
* **Analyse :** Validation de l'ordonnancement temps réel via **Tracealyzer**.

---

## 🏗️ Architecture Logicielle
Le projet repose sur un modèle **Publish/Subscribe** pour la gestion des E/S :

* **`vTask_Pub`** : Scrutateur central des capteurs qui "réveille" les tâches métier uniquement en cas d'événement (toutes les 50ms).
* **`vTask_Write`** : Gestionnaire d'écriture centralisé utilisant le **DMA** pour libérer le processeur lors des envois UART.
* **Tâches Métier** : Automates (MEF) pilotant chaque section mécanique (Tapis, Ascenseur, Palette).


---

## ⚡ Optimisations & Performances
Pour maximiser l'efficacité énergétique, les techniques suivantes ont été appliquées :

1. **Clock Scaling** : Passage de la fréquence système de **48 MHz** à **8 MHz**.
2. **Tick Rate Tuning** : Optimisation du tick FreeRTOS à **100 Hz** pour limiter les réveils inutiles.
3. **Mode Sleep** : Implémentation de l'instruction `WFI` (Wait For Interrupt) dans l'**Idle Hook** pour couper l'horloge CPU dès que possible.

### Résultats chiffrés :
* **Charge CPU** : Réduite à moins de **2%** en régime permanent.
* **RAM Used** : **79.98%** (12.8 KB / 16 KB).
* **Flash Used** : **20.92%** (26.78 KB / 128 KB).

---

## 💻 Stack Technique
* **C / Bare Metal** (CMSIS)
* **FreeRTOS** (Kernel)
* **Hardware** : STM32F072RB (Nucleo-64)
* **Logiciels** : STM32CubeIDE, Factory I/O, Tracealyzer (Percepio)

---

## 🚀 Installation & Utilisation
1. Clonez le dépôt : `git clone https://github.com/ton-pseudo/palletizer-rtos.git`
2. Importez le projet dans **STM32CubeIDE**.
3. Configurez le **Bridge UART** pour relier Factory I/O à votre carte STM32.
4. Lancez la simulation Factory I/O (scène "Palletizer").
5. Compilez et flashez la carte.

---
*Projet réalisé dans le cadre du module OSTR (Systèmes Temps Réel).*
