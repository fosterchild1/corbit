<p align="center"><img src="https://github.com/fosterchild1/corbit/blob/master/resources/text.png" width="390" height="133"></p>

### <p align="center">TUI Simulation of orbits in C using ncurses.</p>

# Overview
This app uses [ncurses](https://en.wikipedia.org/wiki/Ncurses) to render planet, moons or even star orbits around a center point. The orbits are not 100% accurate, and are modified to look better on terminals.

# Installation
**Requirements: ncurses**
```bash
git clone https://github.com/fosterchild1/corbit.git
cd corbit
make install
```


# Usage
Simply run `corbit` to use it. There are also a couple of options you can add:
```
Usage: corbit [OPTIONS]...
CLI arguments:
-t --time [TIME]       advance TIME step each tick
-s --system [SYSTEM]   system to simulate
Available systems: sol,jov,sat,ura,nep,plu,ker
```
For example, to simulate jupiter and it's moons, run `corbit -s jov`.<br/>
The app also has built-in keybinds shown at the bottom of the screen that allows you to toy around with the simulation. You can hide them by pressing **F1**.</br>
**You can make your own systems** by configuring the config file inside `~/.config/corbit`.
