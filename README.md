# Readme


### Description


### Installation

Ce projet requiert une compilation avant de pouvoir être utilisé. Il y a deux compilations à réaliser :
* une pour le shell en lui même
* une pour la commande "ls", qui est indépendant du shell

Les commandes suivantes sont à effectuer à la racine du répertoire :

```sh
$ make ls
$ make shell
```

### Utilisation
Une fois la compilation réalisée, on peut utiliser le terminal :

```sh
$ cd bin/
$ ./shell
```

On peut également exécuter le terminal en lui passant des scripts :

```sh
$ cd bin/
$ ./shell ../script
```

License
----

MIT
